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
# include <chrono>
# include <thread>
# include <sstream>
# define GAME 1
# define TRANSLATION 2
# define START 3

// Global variables definition
extern HWND game_text;
extern HWND file_text;
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
bool OpenFileExplorer(HWND hwnd, wchar_t *filePath, int filePathSize, int button);

class Subtitles
{
private:
    uintptr_t bAddress_audio;
    std::vector <int> offset_audio;
    uintptr_t bAddress_play;
    std::vector <int> offset_play;
    void findAddress(uintptr_t &address, int offset, HANDLE hProcess);
public:
    uintptr_t address_audio;
    uintptr_t address_play;
    int AudioID;
    int lastAudioID;
    bool is_playing;
    std::vector <int> ID;
    std::vector <std::wstring> Text;

    void search_memory(HANDLE hProcess);
    bool check_audio(HANDLE hProcess);
    void file_memory(std::wifstream& file);
    void file_text(std::wifstream& file);
};

// Game
void game_start(PROCESS_INFORMATION pi);

#endif
