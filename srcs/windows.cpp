#include "MemSubLoader.hpp"

// Config window controls handling
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

// Config window initialization
void CreateConfigWindow(HWND parent)
{
	const wchar_t CONFIG_CLASS_NAME[] = L"ConfigWindowClass";
	WNDCLASS configWindowClass = {};

	configWindowClass.lpfnWndProc = ConfigWindowProc;
	configWindowClass.hInstance = GetModuleHandle(NULL);
	configWindowClass.lpszClassName = CONFIG_CLASS_NAME;
	RegisterClass(&configWindowClass);

	configHWND = CreateWindow(CONFIG_CLASS_NAME, L"Subtitles Configuration", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, NULL, NULL);
	ShowWindow(configHWND, SW_SHOW);
}

// Main window controls handling
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

					case START: // When both game and file was selected
						{
							if (gamePath[0] != NULL && filePath[0] != NULL)
							{
								STARTUPINFO si;
								PROCESS_INFORMATION pi;
								ZeroMemory(&si, sizeof(si));
								si.cb = sizeof(si);
								ZeroMemory(&pi, sizeof(pi));
								// Opens the game
								CreateProcess(gamePath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
								WaitForInputIdle(pi.hProcess, INFINITE);
								// Creates subtitles window
								subtitlesWin = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, L"MainWindowClass", L"Subtitles", WS_VISIBLE | WS_POPUP, 50, 100, 640, 100, NULL, NULL, NULL, NULL);
								subtitles = CreateWindow(L"STATIC", L"No File", WS_CHILD | WS_VISIBLE, 0, 0, 640, 100, subtitlesWin, NULL, NULL, NULL);
								SetLayeredWindowAttributes(subtitlesWin, RGB(255, 255, 255), 128, LWA_ALPHA);
								SetWindowText(subtitles, L"Loading...");
								CreateConfigWindow(hwnd);
								PostMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
								game_start(pi);
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

// Main window initialization
int CreateMainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const wchar_t CLASS_NAME[] = L"MainWindowClass";
	WNDCLASS wc = {};
	HWND mainHWND;
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();

	GetWindowRect(hDesktop, &desktop);
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);

	// Creates main window and positions it at the middle of the screen
	mainHWND = CreateWindowEx(0, CLASS_NAME, L"MemSubLoader", WS_OVERLAPPEDWINDOW &~WS_MAXIMIZEBOX &~WS_THICKFRAME, desktop.right / 2 - 320, desktop.bottom / 2 - 120, 640, 240, NULL, NULL, hInstance, NULL);
	if (mainHWND == NULL)
		return 1;

	HWND subsPath = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 50, 50, 330, 24, mainHWND, (HMENU) GAME_FIELD, hInstance, NULL);
	HWND gamePath = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 50, 90, 330, 24, mainHWND, (HMENU) TRANSLATION_FIELD, hInstance, NULL);

	HICON folderIcon = LoadIcon(hInstance, IDI_HAND);

	HWND subsButton = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_ICON, 10, 50, 30, 24, mainHWND, (HMENU)GAME, hInstance, NULL);
	SendMessage(subsButton, BM_SETIMAGE, IMAGE_ICON, (LPARAM)folderIcon);

	HWND gameButton = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_ICON, 10, 90, 30, 24, mainHWND, (HMENU)TRANSLATION, hInstance, NULL);
	SendMessage(gameButton, BM_SETIMAGE, IMAGE_ICON, (LPARAM)folderIcon);

	HWND startButton = CreateWindow(L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 245, 150, 150, 50, mainHWND, (HMENU)START, hInstance, NULL);

	ShowWindow(mainHWND, nCmdShow);
	return 0;
}
