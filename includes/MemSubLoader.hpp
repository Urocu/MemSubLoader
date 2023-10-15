#ifndef MEMSUBLOADER_HPP
# define MEMSUBLOADER_HPP
#ifndef UNICODE
# define UNICODE
#endif
# define _WIN32_WINNT 0x0501
# include <windows.h>
# include <iostream>
# include <shlwapi.h>
# include <fstream>
# include <string>
# include <vector>
# include <cstdio>
# include <stdio.h>
# include <gdiplus.h>
# include "resource.h"
# define GAME_BUTTON 1
# define SUBTITLES_BUTTON 2
# define START_BUTTON 3
# define GAME_FIELD 4
# define SUBTITLES_FIELD 5
# define MENU_LOAD 6
# define MENU_SAVE 7
# define MENU_SETAUTOLOAD 8
# define MENU_EXIT 9
# define MENU_SETTINGS 10
# define FONT_NAME_LABEL 11
# define FONT_SIZE_LABEL 12
# define FONT_BUTTON 13
# define COLOR_BUTTON 14
# define ALIGNMENT_COMBOBOX 15
# define APPLY_BUTTON 16
# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480
# define SUBTITLES_XPOS 0
# define SUBTITLES_YPOS 380
# define SUBTITLES_WIDTH 640
# define SUBTITLES_HEIGHT 100

using namespace Gdiplus;

enum TextAlignment {
	ALIGN_LEFT = 0,
	ALIGN_CENTER,
	ALIGN_RIGHT,
};

struct Config {
	wchar_t gamePath[MAX_PATH];
	wchar_t subtitlesPath[MAX_PATH];
	COLORREF subtitlesColor;
	LOGFONT subtitlesFont;
	TextAlignment alignment;
};

// Global variables definition
extern Config config;

extern HWND gamePathValueLabel;
extern HWND subtitlesPathValueLabel;
extern HWND subtitles;
extern HWND subtitlesWin;
extern HWND settingsHWND;

extern HWND fontValueLabel;
extern HWND fontSizeValueLabel;
extern HWND fontStyleValueLabel;
extern HWND alignmentComboBox;

extern HFONT hFont;
extern HFONT titleFont;
extern HFONT subtitlesHFont;
extern HBITMAP logoBitmap;

extern Gdiplus::GdiplusStartupInput gdiplusStartupInput;
extern ULONG_PTR gdiplusToken;

extern std::wstring textToDraw;

// Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

// Windows
int createMainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
void updateMainAttributes(HWND hwnd);
LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void createSettingsWindow(HWND parent);
void updateSubtitlesSettingsAttributes(HWND hwnd, LOGFONT &lf);
LRESULT CALLBACK SettingsWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void createSubtitlesWindow(void);
LRESULT CALLBACK subtitlesWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Utilities
void findAddress(uintptr_t &address, int offset, HANDLE hProcess);

bool openFileExplorer(HWND hwnd, wchar_t *filePath, int filePathSize, int button);
bool openFontDialog(HWND hwnd, LOGFONT &lf, HFONT &subtitlesFont);
bool openColorDialog(HWND hwnd, COLORREF &subtitlesColor);

bool saveConfig(const Config &config, wchar_t *filename);
bool loadConfig(Config &config, const wchar_t *filename);
bool setAutoloadConfigPath(const wchar_t *path);
bool getAutoloadConfigPath(wchar_t *path);
bool getAutoloadPath(wchar_t *executablePath);

void cleanup(void);

// Game
void gameStart(PROCESS_INFORMATION pi);

#endif
