#ifndef MEMSUBLOADER_HPP
# define MEMSUBLOADER_HPP
#ifndef UNICODE
# define UNICODE
#endif
# define _WIN32_WINNT 0x0501
# include <windows.h>
# include <commdlg.h>
# include <tchar.h>
# include <iostream>
# include <shlwapi.h>
# include <fstream>
# include <string>
# include <vector>
# include <stdio.h>
# define GAME 1
# define TRANSLATION 2
# define START 3

// Global variables definition
extern HWND game_text;
extern HWND file_text;
extern HWND subtitles;
extern HWND gameHWND;
extern HWND configHWND;
extern wchar_t gamePath[MAX_PATH];
extern wchar_t filePath[MAX_PATH];
extern PROCESS_INFORMATION pi;
extern STARTUPINFO si;
extern HWND gWindow;

// Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

// Windows
int CreateMainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateConfigWindow(HWND parent);
LRESULT CALLBACK ConfigWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Utilities
bool findAddress(uintptr_t &address, int offset);
bool OpenFileExplorer(HWND hwnd, wchar_t *filePath, int filePathSize, int button);

// Game
void game_start();

#endif
