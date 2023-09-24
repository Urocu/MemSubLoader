#include "MemSubLoader.hpp"

// Global variables assignment
HWND game_text = NULL;
HWND file_text = NULL;
HWND subtitles = NULL;
HWND gameHWND = NULL;
HWND configHWND = NULL;
wchar_t gamePath[MAX_PATH] = {};
wchar_t filePath[MAX_PATH] = {};
PROCESS_INFORMATION pi;
STARTUPINFO si;
HWND gWindow = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg = {};

	int res = CreateMainWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	if (res)
	{
		printf("Error: Couldn't initialize main window\n");
		return 1;
	}
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}