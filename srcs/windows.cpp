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
					case GAME_BUTTON:
						if (OpenFileExplorer(hwnd, gamePath, MAX_PATH, LOWORD(wParam)))
							SetWindowText(game_text, PathFindFileName(gamePath));
						break;

					case TRANSLATION_BUTTON:
						if (OpenFileExplorer(hwnd, filePath, MAX_PATH, LOWORD(wParam)))
							SetWindowText(translation_text, PathFindFileName(filePath));
						break;

					case START_BUTTON: // When both game and file was selected
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
								PostMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, NULL);
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

		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);

				// Set the background color
				COLORREF bgColor = RGB(250, 250, 250);
				HBRUSH hBrush = CreateSolidBrush(bgColor);
				HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

				// Fill the client area with the background color
				FillRect(hdc, &ps.rcPaint, hBrush);

				// Clean up
				SelectObject(hdc, hOldBrush);
				DeleteObject(hBrush);

				EndPaint(hwnd, &ps);
			}
			break;

		case WM_CTLCOLORSTATIC:
			{
			    HDC hdcStatic = (HDC)wParam;
			    SetBkMode(hdcStatic, TRANSPARENT);
			    return (INT_PTR)GetStockObject(NULL_BRUSH);
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
	mainHWND = CreateWindowEx(0, CLASS_NAME, L"MemSubLoader", WS_OVERLAPPEDWINDOW &~WS_MAXIMIZEBOX &~WS_THICKFRAME, (desktop.right / 2) - (524 / 2), (desktop.bottom / 2) - (273 / 2), 524, 273, 0, 0, hInstance, NULL);
	if (mainHWND == NULL)
		return 1;

	HICON folderIcon = LoadIcon(hInstance, IDI_HAND);
	HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	HFONT titleFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Calibri");

	HWND titleText = CreateWindowEx(0, L"STATIC", L"MemSubLoader", WS_VISIBLE | WS_CHILD | WS_GROUP | SS_CENTER | SS_NOPREFIX, 165, 18, 188, 24, mainHWND, (HMENU)0, hInstance, NULL);
	SendMessage(titleText, WM_SETFONT, (LPARAM)titleFont, FALSE);
	HWND gameGroup = CreateWindowEx(0, L"BUTTON", L"Game Path", WS_VISIBLE | WS_CHILD | WS_GROUP | BS_GROUPBOX, 5, 49, 510, 49, mainHWND, (HMENU)0, hInstance, NULL);
	SendMessage(gameGroup, WM_SETFONT, (LPARAM)hFont, FALSE);
	game_text = CreateWindowEx(0, L"EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL, 15, 65, 450, 23, mainHWND, (HMENU) GAME_FIELD, hInstance, NULL);
	SendMessage(game_text, WM_SETFONT, (LPARAM)hFont, FALSE);
	HWND gameExplorerButton = CreateWindowEx(0, L"BUTTON", L"Browse", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON, 470, 65, 42, 23, mainHWND, (HMENU) GAME_BUTTON, hInstance, NULL);
	SendMessage(gameExplorerButton, WM_SETFONT, (LPARAM)hFont, FALSE);
	HWND translationGroup = CreateWindowEx(0, L"BUTTON",L"Subtitles Path", WS_VISIBLE | WS_CHILD | WS_GROUP | BS_GROUPBOX, 5, 114, 510, 49, mainHWND, (HMENU)0, hInstance, NULL);
	SendMessage(translationGroup, WM_SETFONT, (LPARAM)hFont, FALSE);
	translation_text = CreateWindowEx(0, L"EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL, 15, 130, 450, 23, mainHWND, (HMENU) TRANSLATION_FIELD, hInstance, NULL);
	SendMessage(translation_text, WM_SETFONT, (LPARAM)hFont, FALSE);
	HWND translationExplorerButton = CreateWindowEx(0, L"BUTTON", L"Browse", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON, 470, 130, 42, 23, mainHWND, (HMENU) TRANSLATION_BUTTON, hInstance, NULL);
	SendMessage(translationExplorerButton, WM_SETFONT, (LPARAM)hFont, FALSE);
	HWND startButton = CreateWindowEx(0, L"BUTTON", L"Start game", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON, 225, 179, 75, 33, mainHWND, (HMENU) START_BUTTON, hInstance, NULL);
	SendMessage(startButton, WM_SETFONT, (LPARAM)hFont, FALSE);

	ShowWindow(mainHWND, nCmdShow);
	return 0;
}
