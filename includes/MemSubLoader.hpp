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
# include <stdio.h>
# include "resource.h"
# define GAME_BUTTON 1
# define SUBTITLES_BUTTON 2
# define START_BUTTON 3
# define GAME_FIELD 4
# define SUBTITLES_FIELD 5
# define MENU_EXIT 6
# define MENU_CONFIG 7

// Global variables definition
extern HWND game_text;
extern HWND subtitles_text;
extern HWND subtitles;
extern HWND subtitlesWin;
extern HWND configHWND;
extern wchar_t gamePath[MAX_PATH];
extern wchar_t filePath[MAX_PATH];

// Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

// Windows
int CreateMainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateConfigWindow(HWND parent);
LRESULT CALLBACK ConfigWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Utilities
void findAddress(uintptr_t &address, int offset, HANDLE hProcess);
bool OpenFileExplorer(HWND hwnd, wchar_t *filePath, int filePathSize, int button);

// Game
void game_start(PROCESS_INFORMATION pi);

#endif
