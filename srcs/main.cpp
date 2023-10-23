#include "MemSubLoader.hpp"
using namespace Gdiplus;

// Global variables assignment

// Global resources
std::map<wchar_t *, Config, WStringCompare> configs;
std::wstring textToDraw;
Config tmpConfig = {};
wchar_t gamePath[MAX_PATH] = {};
wchar_t subtitlesPath[MAX_PATH] = {};
HINSTANCE hInst = NULL;

// Windows
HWND mainHWND = NULL;
HWND subtitlesHWND = NULL;
HWND settingsHWND = NULL;
HWND configuratorHWND = NULL;

// Main window handles
HWND gamePathValueLabel = NULL;
HWND subtitlesPathValueLabel = NULL;

// Settings window handles
HWND fontValueLabel = NULL;
HWND fontSizeValueLabel = NULL;
HWND fontStyleValueLabel = NULL;
HWND alignmentHorizontalComboBox = NULL;
HWND alignmentVerticalComboBox = NULL;

// Configurator window handles
HWND configList = NULL;

// Resources
HFONT hFont = NULL;
HFONT titleFont = NULL;
HFONT subtitlesHFont = NULL;
HBITMAP logoBitmap = NULL;

// GDI+
Gdiplus::GdiplusStartupInput gdiplusStartupInput = NULL;
ULONG_PTR gdiplusToken = 0UL;

// Old config values

// Font
int oldFontColorAlpha = 0;

// Outline
int oldOutlineWidth = 0;
int oldOutlineColorAlpha;

// Shadows
int oldShadowsWidth = 0;
int oldShadowsXOffset = 0;
int oldShadowsYOffset = 0;
int oldShadowsColorAlpha;

// Area
int oldAreaXPosition = 0;
int oldAreaYPosition = 0;
int oldAreaWidth = 0;
int oldAreaHeight = 0;


// Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	hInst = hInstance;
	MSG msg = {};
	wchar_t autoloadPath[MAX_PATH] = {};
	wchar_t message[MAX_PATH + 256] = {};

	GpStatus gdiplusStatus = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	if (gdiplusStatus != GpStatus::Ok)
	{
		MessageBox(NULL, L"Error: Failed to initialize GDI+", L"GDI+ initialization", MB_ICONERROR);
		cleanup();
		return 1;
	}
	if (getAutoloadConfigPath(autoloadPath)) // Get autoloaded configuration path from autoload.dat
	{
		if (loadConfig(autoloadPath)) // Failed to load configuration
		{
			wchar_t autoload[MAX_PATH];
			if (getAutoloadPath(autoload))
			{
				DeleteFile(autoload);
			}
			wsprintf(message, L"Failed to autoload configuration :\n%s\nDeleted configuration autoload", autoloadPath);
			MessageBox(NULL, message, L"Configuration autoloading", MB_ICONERROR);
			
			Config defaultConfig = {};
			defaultConfig.identifier = wcsdup(L"DEFAULT");
			HFONT hSystemFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
			GetObject(hSystemFont, sizeof(LOGFONT), &defaultConfig.subtitlesFont);
			configs.insert({ wcsdup(L"DEFAULT"), defaultConfig });
		}
	}
	else // No autoload.dat found
	{
		Config defaultConfig = {};
		defaultConfig.identifier = wcsdup(L"DEFAULT");
		HFONT hSystemFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		GetObject(hSystemFont, sizeof(LOGFONT), &defaultConfig.subtitlesFont);
		configs.insert({ wcsdup(L"DEFAULT"), defaultConfig });
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
