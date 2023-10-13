#include "MemSubLoader.hpp"

// Global variables assignment

Config config = {};

// Main window
HWND game_text = NULL;
HWND subtitles_text = NULL;
HWND subtitles = NULL;
HWND subtitlesWin = NULL;
HWND settingsHWND = NULL;

// Settings window
HWND fontValueLabel = NULL;
HWND fontSizeValueLabel = NULL;
HWND fontStyleValueLabel = NULL;
COLORREF subtitlesColor = NULL;
HFONT subtitlesHFont = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg = {};
	Config tmp = {};
	if (!LoadConfig(tmp, L"config.dat"))
	{
		HFONT hSystemFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		GetObject(hSystemFont, sizeof(LOGFONT), &config.subtitlesFont);
	}
	else
	{
		config = tmp;
	}

	int res = CreateMainWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	if (res)
	{
		printf("Error: Couldn't initialize main window\n");
		return 1;
	}
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
