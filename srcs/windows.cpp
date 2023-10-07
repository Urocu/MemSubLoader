#include "MemSubLoader.hpp"

// Config window controls handling
LRESULT CALLBACK ConfigWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case 1: // Handle Apply, goal is to have live preview
					break;
			}
			break;
		}

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

	configHWND = CreateWindowEx(0, CONFIG_CLASS_NAME, L"Subtitle Configuration", WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, NULL, NULL);

	CreateWindow(L"STATIC", L"Font:", WS_CHILD | WS_VISIBLE, 10, 10, 80, 20, configHWND, NULL, NULL, NULL);
	CreateWindow(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 100, 10, 150, 200, configHWND, NULL, NULL, NULL);
	CreateWindow(L"STATIC", L"Font Size:", WS_CHILD | WS_VISIBLE, 10, 40, 80, 20, configHWND, NULL, NULL, NULL);
	CreateWindow(L"EDIT", L"16", WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 40, 50, 20, configHWND, NULL, NULL, NULL);
	CreateWindow(L"STATIC", L"Font Color:", WS_CHILD | WS_VISIBLE, 10, 70, 80, 20, configHWND, NULL, NULL, NULL);
	CreateWindow(L"BUTTON", L"Select Color", WS_CHILD | WS_VISIBLE, 100, 70, 100, 30, configHWND, NULL, NULL, NULL);
	CreateWindow(L"BUTTON", L"Apply", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 150, 200, 80, 30, configHWND, NULL, NULL, NULL);
}

// Main window controls handling
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CTLCOLORSTATIC:
		{
			HDC hdcStatic = (HDC)wParam;
			SetBkColor(hdcStatic, GetSysColor(COLOR_WINDOW));
			return (INT_PTR)GetSysColorBrush(COLOR_WINDOW);
		}
		break;

		case WM_COMMAND:
			{
				switch (LOWORD(wParam))
				{
					case GAME_BUTTON:
						if (OpenFileExplorer(hwnd, gamePath, MAX_PATH, LOWORD(wParam)))
							SetWindowText(game_text, PathFindFileName(gamePath));
						break;

					case SUBTITLES_BUTTON:
						if (OpenFileExplorer(hwnd, filePath, MAX_PATH, LOWORD(wParam)))
							SetWindowText(subtitles_text, PathFindFileName(filePath));
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
								PostMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
								game_start(pi);
							}
							else
								MessageBox(hwnd, L"You must select both game and file", L"Warning", MB_ICONINFORMATION);
						}
						break;

					case MENU_EXIT:
						PostQuitMessage(0);
						break;

					case MENU_CONFIG:
						ShowWindow(configHWND, SW_SHOW);
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
	mainHWND = CreateWindowEx(0, CLASS_NAME, L"MemSubLoader", WS_OVERLAPPEDWINDOW &~WS_MAXIMIZEBOX &~WS_THICKFRAME, (desktop.right / 2) - (500 / 2), (desktop.bottom / 2) - (300 / 2), 500, 300, 0, 0, hInstance, NULL);
	if (mainHWND == NULL)
		return 1;

	// UI
	HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	HFONT titleFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Calibri");
	HBITMAP logoBitmap = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCEW(MSL_LOGO), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);

	// UI Menu
	HMENU hMenu = CreateMenu();
	HMENU hMenu_1 = CreatePopupMenu();
	AppendMenu(hMenu_1, MF_STRING, 0, (L"Load profile"));
	AppendMenu(hMenu_1, MF_STRING, 0, (L"Save profile"));
	AppendMenu(hMenu_1, MF_SEPARATOR, 0, 0);
	AppendMenu(hMenu_1, MF_STRING, MENU_EXIT, (L"Exit"));
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenu_1, (L"File"));
	HMENU hMenu_2 = CreatePopupMenu();
	AppendMenu(hMenu_2, MF_STRING, MENU_CONFIG, (L"Settings"));
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenu_2, (L"Subtitles"));
	SetMenu(mainHWND, hMenu);


	// UI Controls
	HWND logo = CreateWindowEx(0, L"STATIC", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 158, 12, 64, 64, mainHWND, (HMENU)0, hInstance, 0);
	SendMessage(logo, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)logoBitmap);
	HWND titleText = CreateWindowEx(0, L"STATIC", (L"MemSubLoader"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_CENTER, 232, 38, 150, 21, mainHWND, (HMENU)0, hInstance, 0);
	SendMessage(titleText, WM_SETFONT, (WPARAM)titleFont, FALSE);
	HWND pathGroup = CreateWindowEx(0, L"BUTTON", (L"Path"), WS_VISIBLE | WS_CHILD | 0x00000007, 24, 83, 450, 119, mainHWND, (HMENU)0, hInstance, 0);
	SendMessage(pathGroup, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND gameLabel = CreateWindowEx(0, L"STATIC", (L"Game path"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 48, 112, 74, 21, mainHWND, (HMENU)0, hInstance, 0);
	SendMessage(gameLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	game_text = CreateWindowEx(0, L"EDIT", (L"Select a file..."), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 130, 107, 280, 29, mainHWND, (HMENU)GAME_FIELD, hInstance, 0);
	SendMessage(game_text, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND gameExplorerButton = CreateWindowEx(0, L"BUTTON", (L"Browse"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 412, 107, 58, 29, mainHWND, (HMENU)GAME_BUTTON, hInstance, 0);
	SendMessage(gameExplorerButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND subtitlesLabel = CreateWindowEx(0, L"STATIC", (L"Subtitles path"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 28, 159, 92, 21, mainHWND, (HMENU)0, hInstance, 0);
	SendMessage(subtitlesLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	subtitles_text = CreateWindowEx(0, L"EDIT", (L"Select a file..."), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 130, 157, 280, 29, mainHWND, (HMENU)SUBTITLES_FIELD, hInstance, 0);
	SendMessage(subtitles_text, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND subtitlesExplorerButton = CreateWindowEx(0, L"BUTTON", (L"Browse"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 412, 157, 58, 29, mainHWND, (HMENU)SUBTITLES_BUTTON, hInstance, 0);
	SendMessage(subtitlesExplorerButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND startButton = CreateWindowEx(0, L"BUTTON", (L"Start game"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 210, 206, 80, 38, mainHWND, (HMENU)START_BUTTON, hInstance, 0);
	SendMessage(startButton, WM_SETFONT, (WPARAM)hFont, FALSE);

	//Config window
	CreateConfigWindow(mainHWND);

	ShowWindow(mainHWND, nCmdShow);
	return 0;
}
