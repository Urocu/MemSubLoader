#include "MemSubLoader.hpp"

std::vector <Subtitles> subtitles;
std::wstring SubInfo;

std::wstring jsonUnicodeToWstring(const Json::Value& value)
{
	std::string str = value.asString();
	int wstrLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	std::wstring wstr(wstrLength, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], wstrLength);
	return wstr;
}

// Reads address stored in memory and adds offset to it if needed
void Subtitles::findAddress(uintptr_t &address, int offset, HANDLE hProcess)
{
	SIZE_T bytesRead;
	if (ReadProcessMemory(hProcess, (LPCVOID)address, &address, 4, &bytesRead) )
		address += offset;
}

// Finds dynamic address using baseaddress and offsets
void Subtitles::searchMemory(HANDLE hProcess)
{
	address_audio = bAddress_audio;
	address_play = bAddress_play;
	for(size_t i = 0; i < offset_audio.size(); i++)
		findAddress(address_audio,offset_audio[i],hProcess);
	for(size_t i = 0; i < offset_play.size(); i++)
		findAddress(address_play,offset_play[i],hProcess);
}

// Checks if there is audio playing and if yes, which one
bool Subtitles::checkAudio(HANDLE hProcess, int place)
{
	SIZE_T bytesRead;
	ReadProcessMemory(hProcess, (LPCVOID)address_play, &is_playing, 1, &bytesRead);
	ReadProcessMemory(hProcess, (LPCVOID)address_audio, &AudioID, 4, &bytesRead);

	if (is_playing)
	{
		if (AudioID != lastAudioID && AudioID > 0)
		{
			lastAudioID = AudioID;
			for(size_t i = 0; i < dialog.size(); i++)
			{
				if (AudioID == dialog[i].ID)
				{
					if(dialog[i].Timer.size() == 0)
					{
						textToDraw = dialog[i].Text[0];
					}
					else // If subtitles have timer
					{
						textToDraw = L"";
						KillTimer(mainHWND,1);
						sub = place;
						subID = i;
						SetTimer(mainHWND, 2, dialog[i].Timer[0],NULL);
					}
					testidentifier = dialog[i].identifier;
					invalidateWindow(subtitlesHWND);
					break;
				}
			}
		}
		return true;
	}
	else
	{
		lastAudioID = 0;
		return false;
	}
}

bool loadSubtitles(wchar_t *fileName)
{
	subtitles.clear();
	std::ifstream inputFile(fileName);
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
		inputFile.close();
		return true;
	}
	if(root.isMember("Addresses"))
	{
		for(int i = 0; i < root["Addresses"].size(); i++)
		{
			subtitles.push_back(Subtitles());
			Json::Value address;
			address = root["Addresses"][i];

			subtitles[i].bAddress_audio = address["BaseAddressAudio"].asInt();
			for(int j = 0; j < address["OffsetsAudio"].size();j++)
				subtitles[i].offset_audio.push_back(address["OffsetsAudio"][j].asInt());

			subtitles[i].bAddress_play = address["BaseAddressPlay"].asInt();
			for(int j = 0; j < address["OffsetsPlay"].size();j++)
				subtitles[i].offset_play.push_back(address["OffsetsPlay"][j].asInt());
		}
	}
	std::string sub = "Subtitles";
	for(int i = 0; i < root["Addresses"].size(); i++)
	{
		if(root.isMember(sub+std::to_string(i)))
		{


			int s = root[sub+std::to_string(i)].size();
			for(int j = 0; j< s; j++)
			{
				int id;
				std::wstring ident;
				std::vector<std::wstring> text;
				std::vector<u_int> timer;

				Json::Value dialog;
				dialog =root[sub+std::to_string(i)][j];
				if(dialog.isMember("ID"))
				{
					id = dialog["ID"].asUInt();
				}

				if(dialog.isMember("Identifier"))
				{
					const std::string identString = dialog["Identifier"].asString();
					const std::wstring identWString(identString.begin(), identString.end());
					ident = identWString;
				}

				if(dialog.isMember("Text"))
				{
					Json::Value subs = dialog["Text"];
					if(subs.size() == 0)
					{
						const std::wstring textWString = jsonUnicodeToWstring(subs);
						text.push_back(textWString);
					}
					else
					for(int j = 0; j < subs.size();j++)
					{
						const std::wstring textWString = jsonUnicodeToWstring(subs[j]);
						text.push_back(textWString);
					}
				}

				if(dialog.isMember("Timer"))
				{
					Json::Value time = dialog["Timer"];
					if(time.size() == 0)
					{
						timer.push_back(time.asInt());
					}
					else
					for(int j = 0; j < time.size();j++)
					{
						timer.push_back(time[j].asInt());
					}
				}
				Dialog test = {id,text,timer,ident};
				subtitles[i].dialog.push_back(test);

			}
		}
	}

	if(root.isMember("Info"))
	{
		const std::wstring textWString = jsonUnicodeToWstring(root["Info"]);
		SubInfo = textWString;
		std::wcout<<SubInfo;
	}

	if(root.isMember("Identifiers"))
	{
		root = root["Identifiers"];
		//configs.clear();
		for (Json::Value::const_iterator itr = root.begin(); itr != root.end(); ++itr) {
			const std::string identifierString = itr.key().asString();

			const std::wstring identifierWString(identifierString.begin(), identifierString.end());
			wchar_t *identifier = wcsdup(identifierWString.c_str());

			if(identifier == configs[identifier].identifier)
				continue;

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
			checkConfig(config);
			configs[identifier] = config;
			}
	}
	inputFile.close();
	return false;
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
			ofn.lpstrFilter = L"Subtitles Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
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

		case MENU_CREATESHORTCUT:
		{
			ofn.lpstrFilter = L"Shortcut Files (*.lnk)\0*.lnk\0All Files (*.*)\0*.*\0";
		}
	}
	if (button == MENU_SAVE || button == MENU_CREATESHORTCUT)
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
	std::wstring subtitlesPathWString(subtitlesPath);
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
		configObject["fontItalic"] = static_cast<bool>(config.subtitlesFont.lfItalic);
		configObject["fontUnderline"] = static_cast<bool>(config.subtitlesFont.lfUnderline);
		configObject["fontStrikeout"] = static_cast<bool>(config.subtitlesFont.lfStrikeOut);

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
	loadedConfig = filename;
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
		inputFile.close();
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
			inputFile.close();
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
			inputFile.close();
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
		checkConfig(config);
		configs[identifier] = config;
	}

	inputFile.close();
	loadedConfig = filename;
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
		case ALIGN_CENTER:
			return Gdiplus::StringAlignment::StringAlignmentCenter;
		case ALIGN_RIGHT:
			return Gdiplus::StringAlignment::StringAlignmentFar;
		default:
			return Gdiplus::StringAlignment::StringAlignmentCenter;
	}
}

std::map<wchar_t *, Config, WStringCompare>::iterator getConfig(wchar_t *identifier)
{
	std::map<wchar_t *, Config, WStringCompare>::iterator iter = configs.find(identifier);
	return iter;
}

void setDefaultConfig(Config &defaultConfig)
{
	// Font
	defaultConfig.fontColor = RGB(255, 255, 255);
	defaultConfig.identifier = wcsdup(L"DEFAULT");
	HFONT hSystemFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	GetObject(hSystemFont, sizeof(LOGFONT), &defaultConfig.subtitlesFont);
	defaultConfig.fontColorAlpha = 255;

	// Alignment
	defaultConfig.horizontalAlignment = ALIGN_CENTER;
	defaultConfig.verticalAlignment = ALIGN_RIGHT;

	// Outline
	defaultConfig.outlineWidth = 4;
	defaultConfig.outlineColor = RGB(0, 0, 0);
	defaultConfig.outlineColorAlpha = 128;

	// Shadows
	defaultConfig.shadowsWidth = 0;
	defaultConfig.shadowsColor = RGB(0, 0, 0);
	defaultConfig.shadowsXOffset = 0;
	defaultConfig.shadowsYOffset = 0;
	defaultConfig.shadowsColorAlpha = 255;
	defaultConfig.shadowsDiffuse = false;

	// Area
	defaultConfig.areaXPosition = 0;
	defaultConfig.areaYPosition = 90;
	defaultConfig.areaWidth = 100;
	defaultConfig.areaHeight = 10;
	defaultConfig.areaPreview = false;
}

void checkConfig(Config &config)
{
	if(config.areaXPosition > 100)
		config.areaXPosition = 100;
	if(config.areaYPosition > 100)
		config.areaYPosition = 100;
	if(config.areaWidth > 100)
		config.areaWidth = 100;
	if(config.areaHeight > 100)
		config.areaHeight = 100;
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

void invalidateWindow(HWND hwnd)
{
	if (hwnd != NULL)
	{
		InvalidateRect(hwnd, NULL, FALSE);
	}
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
	DeleteObject(iconLogo);
	DeleteObject(iconSubtitles);
	DeleteObject(titleFont);
	DeleteObject(subtitlesHFont);
	DeleteObject(logoBitmap);
	GdiplusShutdown(gdiplusToken);
	DestroyWindow(settingsHWND);
	DestroyWindow(subtitlesHWND);
	DestroyWindow(mainHWND);
}

bool createShortcut(const wchar_t *targetPath, const wchar_t *arguments, const wchar_t *workingDir, const wchar_t *shortcutPath)
{
	HRESULT hres;
	CoInitialize(NULL);

	// Create instance of IShellLink
	IShellLink *pShellLink;
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&pShellLink);
	if (SUCCEEDED(hres))
	{
		// Set the path of the file to execute
		pShellLink->SetPath(targetPath);

		// Set the arguments
		if (arguments != nullptr)
			pShellLink->SetArguments(arguments);

		// Set the working directory
		if (workingDir != nullptr)
			pShellLink->SetWorkingDirectory(workingDir);

		// Save the shortcut to disk
		IPersistFile *pPersistFile;
		hres = pShellLink->QueryInterface(IID_IPersistFile, (void **)&pPersistFile);
		if (SUCCEEDED(hres))
		{
			hres = pPersistFile->Save(shortcutPath, TRUE);
			pPersistFile->Release();
		}
		pShellLink->Release();
	}
	CoUninitialize();
	return SUCCEEDED(hres);
}

void addTrayIcon(HWND hWnd, HINSTANCE hInstance)
{
	niData.cbSize = sizeof(NOTIFYICONDATA);
	niData.hWnd = hWnd;
	niData.uID = 1; // Unique ID for the tray icon
	niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	niData.uCallbackMessage = TRAY_SHOW;
	niData.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MSL_ICON)); // Icon for the tray
	lstrcpy(niData.szTip, L"MemSubLoader"); // Tooltip text
	Shell_NotifyIcon(NIM_ADD, &niData);
	isTrayVisible = true;
}

void removeTrayIcon()
{
	if (isTrayVisible) {
		Shell_NotifyIcon(NIM_DELETE, &niData);
		isTrayVisible = false;
	}
}
