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

// Global resources
extern Config config;
extern std::wstring textToDraw;

// Windows
extern HWND mainHWND;
extern HWND subtitlesHWND;
extern HWND settingsHWND;

// Main window handles
extern HWND gamePathValueLabel;
extern HWND subtitlesPathValueLabel;

// Settings window handles & resources
extern HWND fontValueLabel;
extern HWND fontSizeValueLabel;
extern HWND fontStyleValueLabel;
extern HWND alignmentComboBox;


// Resources
extern HFONT hFont;
extern HFONT titleFont;
extern HFONT subtitlesHFont;
extern HBITMAP logoBitmap;

// GDI+
extern Gdiplus::GdiplusStartupInput gdiplusStartupInput;
extern ULONG_PTR gdiplusToken;

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

// Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

// Windows
int createMainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
void updateMainAttributes(HWND hwnd);
LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int createSettingsWindow(HWND parent);
void updateSubtitlesSettingsAttributes(HWND hwnd, LOGFONT &lf);
LRESULT CALLBACK SettingsWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int createSubtitlesWindow(void);
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
