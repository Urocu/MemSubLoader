#ifndef MEMSUBLOADER_HPP
# define MEMSUBLOADER_HPP
# define UNICODE
# define _WIN32_WINNT 0x0501
# define _WIN32_IE 0x0300
# include <windows.h>
# include <objidl.h>
# include <objbase.h>
# include <shellapi.h>
# include <commctrl.h>
# include <shlwapi.h>
# include <fstream>
# include <string>
# include <vector>
# include <map>
# include <stdio.h>
# include <sstream>
# include <gdiplus.h>
# include "OutlineText.h"
# include "json.h"
# include "Win32InputBox.h"
# include "resource.h"
#include "stdafx.h"
#include "winnls.h"
#include "shobjidl.h"
#include "shlguid.h"

// Main controls
# define GAME_BUTTON 1
# define SUBTITLES_BUTTON 2
# define START_BUTTON 3
# define GAME_FIELD 4
# define SUBTITLES_FIELD 5

// Main menu controls
# define MENU_LOAD 6
# define MENU_SAVE 7
# define MENU_SETAUTOLOAD 8
# define MENU_CREATESHORTCUT 9
# define MENU_EXIT 10
# define MENU_SETTINGS 11

// Settings controls
# define FONT_COLOR_BUTTON 12
# define FONT_BUTTON 13
# define FONT_COLOR_ALPHA_EDIT 14
# define FONT_COLOR_ALPHA_UPDOWN 15
# define ALIGNMENT_HORIZONTAL_COMBOBOX 16
# define ALIGNMENT_VERTICAL_COMBOBOX 17
# define OUTLINE_WIDTH_EDIT 18
# define OUTLINE_WIDTH_UPDOWN 19
# define OUTLINE_COLOR_BUTTON 20
# define OUTLINE_COLOR_ALPHA_EDIT 21
# define OUTLINE_COLOR_ALPHA_UPDOWN 22
# define SHADOWS_COLOR_BUTTON 23
# define SHADOWS_COLOR_ALPHA_EDIT 24
# define SHADOWS_COLOR_ALPHA_UPDOWN 25
# define SHADOWS_WIDTH_EDIT 26
# define SHADOWS_WIDTH_UPDOWN 27
# define SHADOWS_XOFFSET_EDIT 28
# define SHADOWS_XOFFSET_UPDOWN 29
# define SHADOWS_YOFFSET_EDIT 30
# define SHADOWS_YOFFSET_UPDOWN 31
# define SHADOWS_DIFFUSE_CHECKBOX 32
# define AREA_XPOS_EDIT 33
# define AREA_XPOS_UPDOWN 34
# define AREA_YPOS_EDIT 35
# define AREA_YPOS_UPDOWN 36
# define AREA_WIDTH_EDIT 37
# define AREA_WIDTH_UPDOWN 38
# define AREA_HEIGHT_EDIT 39
# define AREA_HEIGHT_UPDOWN 40
# define AREA_PREVIEW_CHECKBOX 41
# define CONFIGURATOR_EDIT_BUTTON 42
# define CONFIGURATOR_NEW_BUTTON 43
# define CONFIGURATOR_DELETE_BUTTON 44
# define CONFIGURATOR_DUPLICATE_BUTTON 45
# define CONFIGURATOR_TODEFAULT_BUTTON 46

// Tray controls
# define TRAY_SHOW 47
# define TRAY_OPEN 48
# define TRAY_EXIT 49

// Debug
# define MENU_DEBUG 50

// Save & Cancel controls
# define SAVE_BUTTON 47
# define CANCEL_BUTTON 48



# define FONT_ALPHA_MIN 0
# define FONT_ALPHA_MAX 255
# define OUTLINE_ALPHA_MIN 0
# define OUTLINE_ALPHA_MAX 255
# define SHADOWS_ALPHA_MIN 0
# define SHADOWS_ALPHA_MAX 255
# define OUTLINE_WIDTH_MIN 0
# define OUTLINE_WIDTH_MAX 128
# define SHADOWS_WIDTH_MIN 0
# define SHADOWS_WIDTH_MAX 128
# define AREA_XPOS_MIN 0
# define AREA_YPOS_MIN 0
# define AREA_WIDTH_MIN 0
# define AREA_HEIGHT_MIN 0

using namespace Gdiplus;

enum TextAlignment {
	ALIGN_LEFT = 0,
	ALIGN_CENTER,
	ALIGN_RIGHT,
};

struct Config {
	wchar_t *identifier;

	// Font
	COLORREF fontColor;
	LOGFONT subtitlesFont;
	int fontColorAlpha;

	// Alignment
	TextAlignment horizontalAlignment;
	TextAlignment verticalAlignment;

	// Outline
	int outlineWidth;
	COLORREF outlineColor;
	int outlineColorAlpha;

	// Shadows
	int shadowsWidth;
	COLORREF shadowsColor;
	int shadowsXOffset;
	int shadowsYOffset;
	int shadowsColorAlpha;
	bool shadowsDiffuse;

	// Area
	int areaXPosition;
	int areaYPosition;
	int areaWidth;
	int areaHeight;
	bool areaPreview;
};

struct WStringCompare
{
	bool operator()(const wchar_t* str1, const wchar_t* str2) const
	{
		return wcscmp(str1, str2) < 0;
	}
};

struct Dialog
{
    unsigned int ID;
    std::vector <std::wstring> Text;
    std::vector <u_int> Timer;
    std::wstring identifier;
};


struct Subtitles
{
		uintptr_t bAddress_audio;
		std::vector <int> offset_audio;
		uintptr_t bAddress_play;
		std::vector <int> offset_play;
		void findAddress(uintptr_t &address, int offset, HANDLE hProcess);

		uintptr_t address_audio;
		uintptr_t address_play;
		unsigned int AudioID;
		unsigned int lastAudioID;
		bool is_playing;
		std::vector <Dialog> dialog;

		void searchMemory(HANDLE hProcess);
		bool checkAudio(HANDLE hProcess, int place);
};

// Global variables definition

// Global resources

extern std::vector <Subtitles> subtitles;
extern std::wstring subInfo;
extern std::map<wchar_t *, Config, WStringCompare> configs;
extern std::wstring loadedConfig;
extern std::wstring textToDraw;
extern std::wstring testidentifier;
extern Config tmpConfig;
extern wchar_t gamePath[MAX_PATH];
extern wchar_t subtitlesPath[MAX_PATH];
extern HINSTANCE hInst;
extern bool livePreview;
extern int screenWidth;
extern int screenHeight;
extern int sub;
extern int subID;
extern bool isGameOpened;

extern int debugLastID;
extern int debugLastIndex;

// Windows
extern HWND mainHWND;
extern HWND subtitlesHWND;
extern HWND settingsHWND;
extern HWND configuratorHWND;
extern HWND debugHWND;

// Tray
extern NOTIFYICONDATA niData;
extern bool isTrayVisible;

// Main window handles
extern HWND gamePathValueLabel;
extern HWND subtitlesPathValueLabel;

// Settings window handles
extern HWND fontValueLabel;
extern HWND fontSizeValueLabel;
extern HWND fontStyleValueLabel;
extern HWND alignmentHorizontalComboBox;
extern HWND alignmentVerticalComboBox;

// Configurator window handles
extern HWND configList;

// Debug window handles
extern HWND debugList;
extern HWND subtitleList;

// Resources
extern HFONT hFont;
extern HICON iconLogo;
extern HICON iconSubtitles;
extern HFONT titleFont;
extern HFONT subtitlesHFont;
extern HBITMAP logoBitmap;

// GDI+
extern Gdiplus::GdiplusStartupInput gdiplusStartupInput;
extern ULONG_PTR gdiplusToken;

// Old config values

// Font
extern int oldFontColorAlpha;

// Outline
extern int oldOutlineWidth;
extern int oldOutlineColorAlpha;

// Shadows
extern int oldShadowsWidth;
extern int oldShadowsXOffset;
extern int oldShadowsYOffset;
extern int oldShadowsColorAlpha;

// Area
extern int oldAreaXPosition;
extern int oldAreaYPosition;
extern int oldAreaWidth;
extern int oldAreaHeight;

// Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

// Windows
int createMainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
void updateMainAttributes(HWND hwnd);
LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int createSettingsWindow(HWND parent);
void updateSettingsWindowAttributes(HWND hwnd);
LRESULT CALLBACK SettingsWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int createSubtitlesWindow(void);
LRESULT CALLBACK subtitlesWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void handleUpdown(HWND hwnd, int &value, int &oldValue, const wchar_t *name, int id, int min, int max, LPARAM lParam);
void handleEdit(HWND hwnd, int &value, int &oldValue, const wchar_t *name, int id, int min, int max, WPARAM wParam);

int createConfiguratorWindow(HWND parent);
void updateConfiguratorWindowAttributes();
LRESULT CALLBACK ConfiguratorWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int createDebugWindow(HWND parent);
void createDebugWindowAttributes();
void updateDebugWindowAttributes();
LRESULT CALLBACK DebugWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// Utilities
std::wstring jsonUnicodeToWstring(const Json::Value& value);
void findAddress(uintptr_t &address, int offset, HANDLE hProcess);

bool openFileExplorer(HWND hwnd, wchar_t *filePath, int filePathSize, int button);
bool openFontDialog(HWND hwnd, LOGFONT &lf, HFONT &subtitlesFont, COLORREF &subtitlesColor);
bool openColorDialog(HWND hwnd, COLORREF &subtitlesColor);

void invalidateWindow(HWND hwnd);

bool loadSubtitles(wchar_t *fileName);

bool saveConfig(wchar_t *filename);
bool loadConfig(const wchar_t *filename);
bool setAutoloadConfigPath(const wchar_t *path);
bool getAutoloadConfigPath(wchar_t *path);
bool getAutoloadPath(wchar_t *executablePath);
Gdiplus::StringAlignment getConfigAlignment(TextAlignment alignment);
std::map<wchar_t *, Config, WStringCompare>::iterator getConfig(wchar_t * identifier);
void setDefaultConfig(Config &defaultConfig);
void checkConfig(Config &config);
wchar_t *getSelectedIdentifier(void);
int getSelectedSubtitle(void);


void cleanup(void);

// Shortcut
bool createShortcut(const wchar_t *targetPath, const wchar_t *arguments, const wchar_t *workingDir, const wchar_t *shortcutPath);

// Tray
void addTrayIcon(HWND hWnd, HINSTANCE hInstance);
void removeTrayIcon();

// Game
void startGame(HWND hwnd);
void scanGame(PROCESS_INFORMATION pi);

#endif
