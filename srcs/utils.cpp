#include "MemSubLoader.hpp"

// Find address containing audio ID
void Subtitles::findAddress(uintptr_t &address, int offset, HANDLE hProcess)
{
	SIZE_T bytesRead;
	if (ReadProcessMemory(hProcess, (LPCVOID)address, &address, 4, &bytesRead) )
		address += offset;
}

void Subtitles::search_memory(HANDLE hProcess)
{
	address_audio = bAddress_audio;
	address_play = bAddress_play;
	for(size_t i = 0; i < offset_audio.size(); i++)
		findAddress(address_audio,offset_audio[i],hProcess);
	for(size_t i = 0; i < offset_play.size(); i++)
		findAddress(address_play,offset_play[i],hProcess);
}

bool Subtitles::check_audio(HANDLE hProcess)
{
	SIZE_T bytesRead;
	if (!ReadProcessMemory(hProcess, (LPCVOID)address_play, &is_playing, 1, &bytesRead))
		std::cout<<"play error: "<<GetLastError()<<std::endl;
	if (!ReadProcessMemory(hProcess, (LPCVOID)address_audio, &AudioID, 4, &bytesRead))
		std::cout<<"audio error: "<<GetLastError()<<std::endl;
	if (is_playing)
	{
		if (AudioID != lastAudioID && AudioID > 0)
		{
			lastAudioID = AudioID;

			for(size_t i = 0; i < ID.size(); i++)
			{
				if (AudioID == ID[i])
				{
					textToDraw = Text[i].c_str();
					InvalidateRect(subtitlesHWND, NULL, FALSE);
				}
			}
		}
		return true;
	}
	else
		return false;
}

void Subtitles::file_memory(std::wifstream& file)
{
	int num;
	int offset;
	file >> bAddress_audio;

	file >> num;
	for(int i = 0; i < num; i++)
	{
		file >> offset;

		offset_audio.push_back(offset);
	}

	file >> bAddress_play;
	file >> num;

	for(int i = 0; i < num; i++)
	{
		file >> offset;

		offset_play.push_back(offset);
	}
}

void Subtitles::file_text(std::wifstream& file)
{
	std::wstring ws;
	while (getline(file, ws))
	{
		if (ws == L"END")
			break;

		int num;
		std::wstring text;
		std::wistringstream iss(ws);
		iss >> num;
		std::getline(iss, text);

		ID.push_back(num);
		Text.push_back(text);
	}
}

// Open File Explorer
bool openFileExplorer(HWND hwnd, wchar_t *filePath, int filePathSize, int button)
{
	OPENFILENAME ofn = {
		sizeof(OPENFILENAME)
	};

	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = filePathSize;
	ofn.Flags = OFN_FILEMUSTEXIST;

	switch(button) {
		case GAME_BUTTON:
		{
			ofn.lpstrFilter = L"Executable Files (*.exe)\0*.exe\0All Files (*.*)\0*.*\0";
		}
		break;

		case SUBTITLES_BUTTON:
		{
			ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
		}
		break;

		case MENU_LOAD:
		{
			ofn.lpstrFilter = L"Configuration Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
		}
		break;

		case MENU_SAVE:
		{
			ofn.lpstrFilter = L"Configuration Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
		}
		break;
	}
	if (button == MENU_SAVE)
	{
		if (GetSaveFileName(&ofn))
			return true;
		else
			return false;
	}
	else {
		if (GetOpenFileName(&ofn))
			return true;
		else
			return false;
	}
}

bool openFontDialog(HWND hwnd, LOGFONT &lf, HFONT &subtitlesFont, COLORREF &subtitlesColor)
{
	CHOOSEFONT cf;
	HFONT tmp;
	ZeroMemory(&cf, sizeof(CHOOSEFONT));
	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.hwndOwner = hwnd;
	cf.lpLogFont = &lf;
	cf.Flags = CF_EFFECTS | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	cf.rgbColors = subtitlesColor;

	if (ChooseFont(&cf))
	{
		subtitlesColor = cf.rgbColors;
		tmp = CreateFontIndirect(&lf);
		
		if (tmp)
		{
			if (subtitlesFont)
			{
				DeleteObject(subtitlesFont);
			}

			subtitlesFont = tmp;
			return true;
		}
	}

	return false;
}

bool openColorDialog(HWND hwnd, COLORREF &subtitlesColor)
{
	CHOOSECOLOR cc;
	static COLORREF customColors[16] = { 0 };

	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = customColors;
	cc.rgbResult = subtitlesColor;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc))
	{
		subtitlesColor = cc.rgbResult;
		return true;
	}

	return false;
}

bool saveConfig(wchar_t *filename)
{
	if (wcslen(filename) < 5 || wcscmp(filename + wcslen(filename) - 5, L".json") != 0)
	{
		if (wcslen(filename) + 5 < MAX_PATH)
		{
			wcsncat(filename, L".json", MAX_PATH - wcslen(filename) - 1);
		}
	}

	Json::Value root;

	std::wstring gamePathWString(gamePath);
	std::string gamePathString(gamePathWString.begin(), gamePathWString.end());
	std::wstring subtitlesPathWString(gamePath);
	std::string subtitlesPathString(subtitlesPathWString.begin(), subtitlesPathWString.end());
	root["gamePath"] = Json::Value(gamePathString);
	root["subtitlesPath"] = Json::Value(subtitlesPathString);

	for (const std::pair<wchar_t *, Config> &entry : configs)
	{
		const wchar_t* identifier = entry.first;
		std::wstring identifierWString(identifier);
		std::string identifierString(identifierWString.begin(), identifierWString.end());
		const Config& config = entry.second;

		Json::Value configObject;

		// Font
		configObject["fontColor"] = Json::Value(static_cast<Json::UInt>(config.fontColor));
		configObject["fontColorAlpha"] = config.fontColorAlpha;
		
		std::wstring fontFaceNameWString(config.subtitlesFont.lfFaceName);
		std::string fontFaceNameString(fontFaceNameWString.begin(), fontFaceNameWString.end());
		configObject["fontFaceName"] = Json::Value(static_cast<Json::String>(fontFaceNameString));

		configObject["fontHeight"] = Json::Value(static_cast<Json::Int>(config.subtitlesFont.lfHeight));
		configObject["fontWeight"] = Json::Value(static_cast<Json::Int>(config.subtitlesFont.lfWeight));
		configObject["fontItalic"] = config.subtitlesFont.lfItalic;
		configObject["fontUnderline"] = config.subtitlesFont.lfUnderline;
		configObject["fontStrikeout"] = config.subtitlesFont.lfStrikeOut;

		// Alignment
		configObject["horizontalAlignment"] = config.horizontalAlignment;
		configObject["verticalAlignment"] = config.verticalAlignment;

		// Outline
		configObject["outlineWidth"] = config.outlineWidth;
		configObject["outlineColor"] = Json::Value(static_cast<Json::UInt>(config.outlineColor));
		configObject["outlineColorAlpha"] = config.outlineColorAlpha;

		// Shadows
		configObject["shadowsWidth"] = config.shadowsWidth;
		configObject["shadowsColor"] = Json::Value(static_cast<Json::UInt>(config.shadowsColor));
		configObject["shadowsXOffset"] = config.shadowsXOffset;
		configObject["shadowsYOffset"] = config.shadowsYOffset;
		configObject["shadowsColorAlpha"] = config.shadowsColorAlpha;
		configObject["shadowsDiffuse"] = config.shadowsDiffuse;

		// Area
		configObject["areaXPosition"] = config.areaXPosition;
		configObject["areaYPosition"] = config.areaYPosition;
		configObject["areaWidth"] = config.areaWidth;
		configObject["areaHeight"] = config.areaHeight;
		configObject["areaPreview"] = config.areaPreview;

		root[identifierString] = configObject;
	}

	// Create a JSON writer
	Json::StreamWriterBuilder writer;
	writer["indentation"] = "\t";

	std::string jsonStr = Json::writeString(writer, root);

	// Write JSON string to file
	std::ofstream outputFile(filename);
	if (!outputFile.is_open()) {
		return true;
	}
	outputFile << jsonStr;
	outputFile.close();
	return false;
}

bool loadConfig(const wchar_t *filename)
{
	std::ifstream inputFile(filename);
	if (!inputFile.is_open()) {
		return true;
	}
	if (!inputFile.good()) {
		inputFile.close();
		return true;
	}
	Json::Value root;
	try {
		inputFile >> root; // Parse the JSON data from the file
	} catch (const std::exception &e) {
		return true;
	}

	// Alignement conversion map
	std::map<TextAlignment, std::string> alignmentToString =
	{
		{ALIGN_LEFT, "ALIGN_LEFT"},
		{ALIGN_CENTER, "ALIGN_CENTER"},
		{ALIGN_RIGHT, "ALIGN_RIGHT"}
	};

	// Parse global variables
	if (root.isMember("gamePath") && root["gamePath"].isString()) {
		const char* gamePathStr = root["gamePath"].asCString();
		size_t gamePathLength = strlen(gamePathStr);
		if (gamePathLength < MAX_PATH) {
			mbstowcs(gamePath, gamePathStr, MAX_PATH);
			gamePath[MAX_PATH - 1] = L'\0';
		}
		else {
			return true;
		}
	}

	if (root.isMember("subtitlesPath") && root["subtitlesPath"].isString()) {
		const char* subtitlesPathStr = root["subtitlesPath"].asCString();
		size_t subtitlesPathLength = strlen(subtitlesPathStr);
		if (subtitlesPathLength < MAX_PATH) {
			mbstowcs(subtitlesPath, subtitlesPathStr, MAX_PATH);
			subtitlesPath[MAX_PATH - 1] = L'\0';
		}
		else {
			return true;
		}
	}

	// Parse configurations
	configs.clear();
	for (Json::Value::const_iterator itr = root.begin(); itr != root.end(); ++itr) {
		const std::string identifierString = itr.key().asString();
		if (identifierString == "gamePath" || identifierString == "subtitlesPath") {
			continue;
		}

		const std::wstring identifierWString(identifierString.begin(), identifierString.end());
		wchar_t *identifier = wcsdup(identifierWString.c_str());
		const Json::Value &configObject = *itr;
		Config config;

		config.identifier = identifier;

		// Font
		if (configObject.isMember("fontColor")) {
			config.fontColor = static_cast<COLORREF>(configObject["fontColor"].asUInt());
		}
		if (configObject.isMember("fontColorAlpha")) {
			config.fontColorAlpha = configObject["fontColorAlpha"].asInt();
		}
		if (configObject.isMember("fontFaceName") && configObject["fontFaceName"].isString()) {
			if (configObject["fontFaceName"].size() < LF_FACESIZE) {
				std::string faceNameString = configObject["fontFaceName"].asString();
				std::wstring wFaceName = std::wstring(faceNameString.begin(), faceNameString.end());
				wcsncpy(config.subtitlesFont.lfFaceName, wFaceName.c_str(), LF_FACESIZE);
			}
		}
		if (configObject.isMember("fontHeight")) {
			config.subtitlesFont.lfHeight = configObject["fontHeight"].asInt();
		}

		if (configObject.isMember("fontWeight")) {
			config.subtitlesFont.lfWeight = configObject["fontWeight"].asInt();
		}

		if (configObject.isMember("fontItalic")) {
			config.subtitlesFont.lfItalic = configObject["fontItalic"].asBool();
		}

		if (configObject.isMember("fontUnderline")) {
			config.subtitlesFont.lfUnderline = configObject["fontUnderline"].asBool();
		}

		if (configObject.isMember("fontStrikeout")) {
			config.subtitlesFont.lfStrikeOut = configObject["fontStrikeout"].asBool();
		}

		// Alignement
		if (configObject.isMember("horizontalAlignment")) {
			config.horizontalAlignment = static_cast<TextAlignment>(configObject["horizontalAlignment"].asInt());
		}
		if (configObject.isMember("verticalAlignment")) {
			config.verticalAlignment = static_cast<TextAlignment>(configObject["verticalAlignment"].asInt());
		}

		// Outline
		if (configObject.isMember("outlineWidth")) {
			config.outlineWidth = configObject["outlineWidth"].asInt();
		}
		if (configObject.isMember("outlineColor")) {
			config.outlineColor = static_cast<COLORREF>(configObject["outlineColor"].asUInt());
		}
		if (configObject.isMember("outlineColorAlpha")) {
			config.outlineColorAlpha = configObject["outlineColorAlpha"].asInt();
		}

		// Shadows
		if (configObject.isMember("shadowsWidth")) {
			config.shadowsWidth = configObject["shadowsWidth"].asInt();
		}
		if (configObject.isMember("shadowsColor")) {
			config.shadowsColor = static_cast<COLORREF>(configObject["shadowsColor"].asUInt());
		}
		if (configObject.isMember("shadowsXOffset")) {
			config.shadowsXOffset = configObject["shadowsXOffset"].asInt();
		}
		if (configObject.isMember("shadowsYOffset")) {
			config.shadowsYOffset = configObject["shadowsYOffset"].asInt();
		}
		if (configObject.isMember("shadowsColorAlpha")) {
			config.shadowsColorAlpha = configObject["shadowsColorAlpha"].asInt();
		}
		if (configObject.isMember("shadowsDiffuse")) {
			config.shadowsDiffuse = configObject["shadowsDiffuse"].asInt();
		}

		// Area
		if (configObject.isMember("areaXPosition")) {
			config.areaXPosition = configObject["areaXPosition"].asInt();
		}
		if (configObject.isMember("areaYPosition")) {
			config.areaYPosition = configObject["areaYPosition"].asInt();
		}
		if (configObject.isMember("areaWidth")) {
			config.areaWidth = configObject["areaWidth"].asInt();
		}
		if (configObject.isMember("areaHeight")) {
			config.areaHeight = configObject["areaHeight"].asInt();
		}
		if (configObject.isMember("areaPreview")) {
			config.areaPreview = configObject["areaPreview"].asInt();
		}

		configs[identifier] = config;
	}

	inputFile.close();
	return false;
}

bool setAutoloadConfigPath(const wchar_t *path)
{
	wchar_t executablePath[MAX_PATH];

	if (getAutoloadPath(executablePath))
	{
		FILE *file = _wfopen(executablePath, L"w");
		if (file)
		{
			fwprintf(file, L"%s", path);
			fclose(file);
			return true;
		}
		return false;
	}
	return false;
}

bool getAutoloadConfigPath(wchar_t *path)
{
	wchar_t executablePath[MAX_PATH];

	if (getAutoloadPath(executablePath))
	{
		FILE *file = _wfopen(executablePath, L"r");
		if (file)
		{
			if (fgetws(path, MAX_PATH, file) != NULL)
			{
				fclose(file);
				return true;
			}
			fclose(file);
		}
		return false;
	}
	return false;
}

bool getAutoloadPath(wchar_t *executablePath)
{
	if (GetModuleFileName(NULL, executablePath, MAX_PATH) != 0)
	{
		wchar_t *lastBackslash = wcsrchr(executablePath, L'\\');
		if (lastBackslash != nullptr)
		{
			*lastBackslash = L'\0';
		}

		wcscat(executablePath, L"\\autoload.dat");
		return true;
	}
	return false;
}

Gdiplus::StringAlignment getConfigAlignment(TextAlignment alignment)
{
	switch (alignment) {
		case ALIGN_LEFT:
			return Gdiplus::StringAlignment::StringAlignmentNear;
			break;
		case ALIGN_CENTER:
			return Gdiplus::StringAlignment::StringAlignmentCenter;
			break;
		case ALIGN_RIGHT:
			return Gdiplus::StringAlignment::StringAlignmentFar;
			break;
		default:
			return Gdiplus::StringAlignment::StringAlignmentCenter;
	}
}

std::map<wchar_t *, Config, WStringCompare>::iterator getConfig(wchar_t *identifier)
{
	std::map<wchar_t *, Config, WStringCompare>::iterator iter = configs.find(identifier);
	return iter;
}

wchar_t *getSelectedIdentifier(void)
{
	int selectedIndex = ListView_GetNextItem(configList, -1, LVNI_SELECTED);
	
	if (selectedIndex != -1)
	{
		LVITEM item;
		item.mask = LVIF_PARAM;
		item.iItem = selectedIndex;
		item.iSubItem = 0;
		ListView_GetItem(configList, &item);
		if (!item.lParam)
		{
			return NULL;
		}
		wchar_t *identifier = reinterpret_cast<wchar_t *>(item.lParam);
		return identifier;
	}
	return NULL;
}

void cleanup(void)
{
	for (std::map<wchar_t *, Config, WStringCompare>::iterator iter = configs.begin(); iter != configs.end(); iter++)
	{
		free(iter->first);
		free(iter->second.identifier);
	}
	configs.clear();

	DeleteObject(hFont);
	DeleteObject(titleFont);
	DeleteObject(subtitlesHFont);
	DeleteObject(logoBitmap);
	GdiplusShutdown(gdiplusToken);
	DestroyWindow(settingsHWND);
	DestroyWindow(subtitlesHWND);
	DestroyWindow(mainHWND);
}
