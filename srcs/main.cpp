#include "MemSubLoader.hpp"
using namespace Gdiplus;

// Global variables assignment

// Global resources
std::map<wchar_t *, Config, WStringCompare> configs;
std::wstring loadedConfig;
std::wstring textToDraw;
std::wstring testidentifier;
Config tmpConfig = {};
wchar_t gamePath[MAX_PATH] = {};
wchar_t subtitlesPath[MAX_PATH] = {};
HINSTANCE hInst = NULL;
bool livePreview = false;
int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);
int sub = 0;
int subID = 0;
bool isGameOpened = false;

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
HICON iconLogo = NULL;
HICON iconSubtitles = NULL;
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
	LPWSTR *ArgList;
	int nArgs;
	hInst = hInstance;
	MSG msg = {};
	wchar_t autoloadPath[MAX_PATH] = {};
	wchar_t message[MAX_PATH + 256] = {};
	Config defaultConfig;

	// Check arguments
	ArgList = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if(ArgList == NULL)
		MessageBox(NULL, L"Error: Failed to check arguments", L"Arguments check", MB_ICONERROR);

	GpStatus gdiplusStatus = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	if (gdiplusStatus != GpStatus::Ok)
	{
		MessageBox(NULL, L"Error: Failed to initialize GDI+", L"GDI+ initialization", MB_ICONERROR);
		cleanup();
		return 1;
	}

	setDefaultConfig(defaultConfig);

	// Autoload config using file argument
	if(nArgs > 1 && wcsstr(ArgList[1], L"-config") && ArgList[2])
	{
		if (loadConfig(ArgList[2]))
		{
			wsprintf(message, L"Failed to autoload configuration.\n%s\nAborting.", autoloadPath);
			MessageBox(NULL, message, L"Configuration autoloading", MB_ICONERROR);
			cleanup();
			return 1;
		}
		if (gamePath[0] == L'\0' || subtitlesPath[0] == L'\0')
		{
			MessageBox(NULL, L"Missing game or subtitles path in your autoloaded configuration file. Please check if both path are correct and try again.", L"Configuration autoloading", MB_ICONERROR);
			cleanup();
			return 1;
		}
		if (SubtitlesLoad(subtitlesPath))
		{
			MessageBox(NULL, L"Failed to load subtitles file", L"Subtitles loading", MB_ICONERROR);
			subtitlesPath[0] = '\0';
		}
		startGame(NULL); // We're directly starting the game without creating the main window
	}
	else
	{
		int res = createMainWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
		if (res)
		{
			MessageBox(NULL, L"Error: Failed to initialize main window", L"Window initialization", MB_ICONERROR);
			cleanup();
			return 1;
		}

		// Autoload config using autoload.dat
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
				configs.insert({ wcsdup(L"DEFAULT"), defaultConfig });
			}
			if (SubtitlesLoad(subtitlesPath))
			{
				MessageBox(NULL, L"Failed to load subtitles file", L"Subtitles loading", MB_ICONERROR);
				subtitlesPath[0] = '\0';
			}
		}
		else // No autoload.dat found
		{
			configs.insert({ wcsdup(L"DEFAULT"), defaultConfig });
		}
	}

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	cleanup();
	return 0;
}
