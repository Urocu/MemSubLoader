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
	if (button == GAME_BUTTON)
		ofn.lpstrFilter = L"Executable Files (*.exe)\0*.exe\0All Files (*.*)\0*.*\0";
	else
		ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = filePathSize;
	ofn.Flags = OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
		return true;
	else
		return false;
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
	cf.rgbColors = subtitlesColor;

	if (ChooseFont(&cf))
	{
		subtitlesColor = cf.rgbColors;
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
