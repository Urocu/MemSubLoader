#include "MemSubLoader.hpp"
using namespace Gdiplus;

// Global variables assignment

Config config = {};

// Main window
HWND gamePathValueLabel = NULL;
HWND subtitlesPathValueLabel = NULL;
HWND subtitles = NULL;
HWND subtitlesHWND = NULL;
HWND settingsHWND = NULL;

// Settings window
HWND fontValueLabel = NULL;
HWND fontSizeValueLabel = NULL;
HWND fontStyleValueLabel = NULL;
HWND alignmentComboBox = NULL;
HFONT subtitlesHFont = NULL;

// Resources
HFONT hFont = NULL;
HFONT titleFont = NULL;
HBITMAP logoBitmap = NULL;

// GDI+
Gdiplus::GdiplusStartupInput gdiplusStartupInput = NULL;
ULONG_PTR gdiplusToken;

std::wstring textToDraw;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg = {};
	Config tmp = {};
	wchar_t autoloadPath[MAX_PATH] = {};
	wchar_t message[MAX_PATH + 256] = {};

	GpStatus gdiplusStatus = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	if (gdiplusStatus != GpStatus::Ok)
	{
		MessageBox(NULL, L"Error: Failed to initialize GDI+", L"GDI+ Initialization", MB_ICONERROR);
		cleanup();
		return 1;
	}
	if (getAutoloadConfigPath(autoloadPath)) // Get autoloaded configuration path from autoload.dat
	{
		if (loadConfig(tmp, autoloadPath)) // Successfully loaded configuration
		{
			config = tmp;
		}
		else // Failed to load configuration
		{
			wchar_t autoload[MAX_PATH];
			if (getAutoloadPath(autoload))
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

	int res = createMainWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	if (res)
	{
		MessageBox(NULL, L"Error: Failed to initialize main window", L"Window initialization", MB_ICONERROR);
		cleanup();
		return 1;
	}
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	cleanup();
	return 0;
}
