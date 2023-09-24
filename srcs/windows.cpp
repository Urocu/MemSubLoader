#include "MemSubLoader.hpp"

LRESULT CALLBACK ConfigWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1: // Handle Apply, goal is to have live preview
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		configHWND = NULL;
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CreateConfigWindow(HWND parent)
{
	const wchar_t CONFIG_CLASS_NAME[] = L"ConfigWindowClass";
	WNDCLASS configWindowClass = {};

	configWindowClass.lpfnWndProc = ConfigWindowProc;
	configWindowClass.hInstance = GetModuleHandle(NULL);
	configWindowClass.lpszClassName = CONFIG_CLASS_NAME;
	RegisterClass(&configWindowClass);

	configHWND = CreateWindowEx(WS_EX_LAYERED, CONFIG_CLASS_NAME, L"Subtitle Configuration", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, NULL, NULL);

	CreateWindow(L"STATIC", L"Font:", WS_CHILD | WS_VISIBLE, 10, 10, 80, 20, configHWND, NULL, NULL, NULL);
	CreateWindow(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 100, 10, 150, 200, configHWND, NULL, NULL, NULL);
	CreateWindow(L"STATIC", L"Font Size:", WS_CHILD | WS_VISIBLE, 10, 40, 80, 20, configHWND, NULL, NULL, NULL);
	CreateWindow(L"EDIT", L"16", WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 40, 50, 20, configHWND, NULL, NULL, NULL);
	CreateWindow(L"STATIC", L"Font Color:", WS_CHILD | WS_VISIBLE, 10, 70, 80, 20, configHWND, NULL, NULL, NULL);
	CreateWindow(L"BUTTON", L"Select Color", WS_CHILD | WS_VISIBLE, 100, 70, 100, 30, configHWND, NULL, NULL, NULL);
	CreateWindow(L"BUTTON", L"Apply", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 150, 200, 80, 30, configHWND, NULL, NULL, NULL);
	ShowWindow(configHWND, SW_SHOW);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_COMMAND:
			{
				switch (LOWORD(wParam))
				{
					case GAME:
						if (OpenFileExplorer(hwnd, gamePath, MAX_PATH, LOWORD(wParam)))
							SetWindowText(game_text, PathFindFileName(gamePath));
						break;

					case TRANSLATION:
						if (OpenFileExplorer(hwnd, filePath, MAX_PATH, LOWORD(wParam)))
							SetWindowText(file_text, PathFindFileName(filePath));
						break;

					case START:
						{
							if (gamePath[0] != NULL && filePath[0] != NULL)
							{
								ZeroMemory(&si, sizeof(si));
								si.cb = sizeof(si);
								ZeroMemory(&pi, sizeof(pi));
								CreateProcess(gamePath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
								WaitForInputIdle(pi.hProcess, INFINITE);
								subtitles = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, L"STATIC", L"", WS_VISIBLE | WS_POPUP, 50, 100, 640, 100, hwnd, NULL, NULL, NULL);
								SetLayeredWindowAttributes(subtitles, RGB(255, 255, 255), 128, LWA_ALPHA);
								SetWindowText(subtitles, L"Loading...");
								CreateConfigWindow(hwnd);
								game_start();
							}
							else
								MessageBox(hwnd, L"You must select both game and file", L"Warning", MB_ICONINFORMATION);
						}
						break;
				}
			}

			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_CREATE:
			{
				HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
				game_text = CreateWindow(L"STATIC", L"No Game", WS_CHILD | WS_VISIBLE, 50, 100, 150, 20, hwnd, NULL, NULL, NULL);
				file_text = CreateWindow(L"STATIC", L"No File", WS_CHILD | WS_VISIBLE, 440, 100, 150, 20, hwnd, NULL, NULL, NULL);
				SendMessage(game_text, WM_SETFONT, (WPARAM) hFont, TRUE);
				SendMessage(file_text, WM_SETFONT, (WPARAM) hFont, TRUE);
			}

			break;

		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);
				FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
				EndPaint(hwnd, &ps);
			}

			break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int CreateMainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const wchar_t CLASS_NAME[] = L"MainWindowClass";
	WNDCLASS wc = {};
	HWND mainHWND;
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);
	GetWindowRect(hDesktop, &desktop);

	mainHWND = CreateWindowEx(0, CLASS_NAME, L"MemSubLoader", WS_OVERLAPPEDWINDOW &~WS_MAXIMIZEBOX &~WS_THICKFRAME, desktop.right / 2 - 320, desktop.bottom / 2 - 120, 640, 240, NULL, NULL, hInstance, NULL);
	if (mainHWND == NULL)
	{
		return 1;
	}

	CreateWindow(L"BUTTON", L"Choose Game", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 50, 50, 150, 50, mainHWND, (HMENU) GAME, hInstance, NULL);
	CreateWindow(L"BUTTON", L"Choose File", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 440, 50, 150, 50, mainHWND, (HMENU) TRANSLATION, hInstance, NULL);
	CreateWindow(L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 245, 150, 150, 50, mainHWND, (HMENU) START, hInstance, NULL);
	ShowWindow(mainHWND, nCmdShow);
	return 0;
}
