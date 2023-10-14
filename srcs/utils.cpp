#include "MemSubLoader.hpp"

// Find address containing audio ID
void findAddress(uintptr_t &address, int offset, HANDLE hProcess)
{
	SIZE_T bytesRead;
	if (ReadProcessMemory(hProcess, (LPCVOID)address, &address, 4, &bytesRead) )
			address += offset;
}

// Open File Explorer
bool OpenFileExplorer(HWND hwnd, wchar_t *filePath, int filePathSize, int button)
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
			ofn.lpstrFilter = L"Configuration Files (*.dat)\0*.dat\0All Files (*.*)\0*.*\0";
		}
		break;

		case MENU_SAVE:
		{
			ofn.lpstrFilter = L"Configuration Files (*.dat)\0*.dat\0All Files (*.*)\0*.*\0";
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

bool OpenFontDialog(HWND hwnd, LOGFONT& lf, HFONT& subtitlesFont)
{
	CHOOSEFONT cf;
	HFONT tmp;
	ZeroMemory(&cf, sizeof(CHOOSEFONT));
	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.hwndOwner = hwnd;
	cf.lpLogFont = &lf;
	cf.Flags = CF_EFFECTS | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	cf.rgbColors = config.subtitlesColor;

	if (ChooseFont(&cf))
	{
		config.subtitlesColor = cf.rgbColors;
		tmp = CreateFontIndirect(&lf);
		
		if (tmp)
		{
			subtitlesFont = tmp;
			return true;
		}
	}

	return false;
}

bool OpenColorDialog(HWND hwnd, COLORREF& subtitlesColor)
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

std::wstring SaveConfig(const Config& config, const wchar_t* filename)
{
	std::wstring filePath(filename);
	wprintf(L"+.dat = %S\n", filePath);

	if (filePath.length() < 4 || filePath.substr(filePath.length() - 4) != L".dat")
	{
		filePath += L".dat";
		wprintf(L"+.dat = %S\n", filePath);
	}

	FILE* file = _wfopen(filePath.c_str(), L"wb");
	if (file) {
		fwrite(&config, sizeof(Config), 1, file);
		fclose(file);
		wprintf(L"return = %S\n", filePath);
		return filePath;
	}
	return L"";
}

std::wstring LoadConfig(Config& config, const wchar_t* filename)
{
	FILE* file = _wfopen(filename, L"rb");
	if (file) {
		fread(&config, sizeof(Config), 1, file);
		fclose(file);
		return filename;
	}
	return L"";
}

#include <Windows.h>

bool SetAutoloadConfigPath(const wchar_t* path)
{
	wchar_t executablePath[MAX_PATH];

	if (GetAutoloadPath(executablePath))
	{
		FILE* file = _wfopen(executablePath, L"w");
		if (file)
		{
			fwprintf(file, L"%S", path);
			fclose(file);
			return true;
		}
		return false;
	}
	return false;
}

bool GetAutoloadConfigPath(wchar_t* path)
{
	wchar_t executablePath[MAX_PATH];

	if (GetAutoloadPath(executablePath))
	{
		FILE* file = _wfopen(executablePath, L"r");
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

bool GetAutoloadPath(wchar_t* executablePath)
{
	if (GetModuleFileName(NULL, executablePath, MAX_PATH) != 0)
	{
		wchar_t* lastBackslash = wcsrchr(executablePath, L'\\');
		if (lastBackslash != nullptr)
		{
			*lastBackslash = L'\0';
		}

		wcscat(executablePath, L"\\autoload.dat");
		return true;
	}
	return false;
}
