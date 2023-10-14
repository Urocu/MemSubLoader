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
HWND alignmentComboBox = NULL;
HFONT subtitlesHFont = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg = {};
	Config tmp = {};
	wchar_t autoloadPath[MAX_PATH] = {};
	wchar_t message[MAX_PATH + 256] = {};

	if (GetAutoloadConfigPath(autoloadPath)) // Get autoloaded configuration path from autoload.dat
	{
		if (LoadConfig(tmp, autoloadPath)) // Successfully loaded configuration
		{
			config = tmp;
		}
		else // Failed to load configuration
		{
			wchar_t autoload[MAX_PATH];
			if (GetAutoloadPath(autoload))
			{
				DeleteFile(autoload);
			}
			wsprintf(message, L"Failed to autoload configuration :\n%s\nDeleted configuration autoload", autoloadPath);
			MessageBox(NULL, message, L"Configuration autoload", MB_ICONERROR);
			HFONT hSystemFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
			GetObject(hSystemFont, sizeof(LOGFONT), &config.subtitlesFont);
		}
	}
	else // No autoload.dat found
	{
		HFONT hSystemFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		GetObject(hSystemFont, sizeof(LOGFONT), &config.subtitlesFont);
	}

	int res = CreateMainWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	if (res)
	{
		MessageBox(NULL, L"Error: Failed to initialize main window", L"Window initialization", MB_ICONERROR);
		return 1;
	}
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
