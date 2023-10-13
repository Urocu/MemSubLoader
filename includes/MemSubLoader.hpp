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
# include "resource.h"
# define GAME_BUTTON 1
# define SUBTITLES_BUTTON 2
# define START_BUTTON 3
# define GAME_FIELD 4
# define SUBTITLES_FIELD 5
# define MENU_LOAD 6
# define MENU_SAVE 7
# define MENU_EXIT 8
# define MENU_SETTINGS 9
# define FONT_NAME_LABEL 10
# define FONT_SIZE_LABEL 11
# define FONT_BUTTON 12
# define COLOR_BUTTON 13
# define APPLY_BUTTON 14

struct Config {
    wchar_t gamePath[MAX_PATH];
    wchar_t subtitlesPath[MAX_PATH];
    COLORREF subtitlesColor;
    LOGFONT subtitlesFont;
	int alignment;
};

// Global variables definition
extern Config config;
extern HWND game_text;
extern HWND subtitles_text;
extern HWND subtitles;
extern HWND subtitlesWin;
extern HWND settingsHWND;
extern HFONT subtitlesHFont;

extern HWND fontValueLabel;
extern HWND fontSizeValueLabel;
extern HWND fontStyleValueLabel;
extern COLORREF subtitlesColor;

// Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

// Windows
int CreateMainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateSettingsWindow(HWND parent);
void SetFontAttributesText(HWND hwnd, LOGFONT& lf);
LRESULT CALLBACK SettingsWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Utilities
void findAddress(uintptr_t &address, int offset, HANDLE hProcess);
bool OpenFileExplorer(HWND hwnd, wchar_t *filePath, int filePathSize, int button);
bool OpenFontDialog(HWND hwnd, LOGFONT& lf, HFONT& subtitlesFont);
bool OpenColorDialog(HWND hwnd, COLORREF& subtitlesColor);
bool SaveConfig(const Config& config, const wchar_t* filename);
bool LoadConfig(Config& config, const wchar_t* filename);

// Game
void game_start(PROCESS_INFORMATION pi);

#endif
