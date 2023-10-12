#include "MemSubLoader.hpp"

// Config window controls handling
LRESULT CALLBACK ConfigWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CTLCOLORSTATIC:
		{
			HDC hdcStatic = (HDC)wParam;
			SetBkColor(hdcStatic, GetSysColor(COLOR_WINDOW));
			return (LRESULT)GetSysColorBrush(COLOR_WINDOW);
		}
		break;

		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case FONT_BUTTON:
				{
					if (OpenFontDialog(hwnd, subtitlesLogFont, subtitlesHFont))
					{
						InvalidateRect(hwnd, NULL, TRUE);
						GetObject(subtitlesHFont, sizeof(LOGFONT), &subtitlesLogFont);
						SetFontAttributesText(hwnd, subtitlesLogFont);
					}
				}
				break;

				case COLOR_BUTTON:
				{
					if (OpenColorDialog(hwnd, subtitlesColor))
					{
						InvalidateRect(hwnd, NULL, TRUE);
					}
				}
				break;

				case APPLY_BUTTON:
				{

				}
				break;
			}
			break;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			HBRUSH brush = CreateSolidBrush(subtitlesColor);
			RECT rect = { 115, 85, 150, 100 };
			FillRect(hdc, &rect, brush);
			DeleteObject(brush);
			EndPaint(hwnd, &ps);
		}
		break;

		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			configHWND = NULL;
		}
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Config window update
void SetFontAttributesText(HWND hwnd, LOGFONT& lf)
{
	wchar_t fontSizeText[16];
	int fontSizePixels = lf.lfHeight;
	std::wstring fontStyle;

	SetWindowText(fontValueLabel, lf.lfFaceName);

	if (fontSizePixels < 0) {
		HDC hdc = GetDC(hwnd);
		if (hdc) {
			int logPixelsY = GetDeviceCaps(hdc, LOGPIXELSY);
			ReleaseDC(hwnd, hdc);
			fontSizePixels = MulDiv(-fontSizePixels, logPixelsY, 72);
		}
	}
	swprintf(fontSizeText, sizeof(fontSizeText) / sizeof(fontSizeText[0]), L"%d pixels", fontSizePixels);
	SetWindowText(fontSizeValueLabel, fontSizeText);

	if (lf.lfWeight >= FW_BOLD) {
		fontStyle += L"Bold";
	}
	if (lf.lfItalic) {
		if (!fontStyle.empty()) {
			fontStyle += L", ";
		}
		fontStyle += L"Italic";
	}
	if (lf.lfUnderline) {
		if (!fontStyle.empty()) {
			fontStyle += L", ";
		}
		fontStyle += L"Underlined";
	}
	if (lf.lfStrikeOut) {
		if (!fontStyle.empty()) {
			fontStyle += L", ";
		}
		fontStyle += L"Strikeout";
	}
	if (fontStyle.empty()) {
		fontStyle = L"Regular";
	}
	SetWindowText(fontStyleValueLabel, fontStyle.c_str());
}

// Config window initialization
void CreateConfigWindow(HWND parent, HINSTANCE hInstance)
{
	const wchar_t CONFIG_CLASS_NAME[] = L"ConfigWindowClass";
	WNDCLASS configWindowClass = {};
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();

	GetWindowRect(hDesktop, &desktop);
	configWindowClass.lpfnWndProc = ConfigWindowProc;
	configWindowClass.hInstance = GetModuleHandle(NULL);
	configWindowClass.lpszClassName = CONFIG_CLASS_NAME;
	RegisterClass(&configWindowClass);

	configHWND = CreateWindowEx(0, CONFIG_CLASS_NAME, L"Subtitles Settings", WS_SYSMENU | WS_MINIMIZEBOX, (desktop.right / 2) - (456 / 2), (desktop.bottom / 2) - (218 / 2), 456, 218, NULL, NULL, NULL, NULL);
	HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	
	HWND fontSettingsGroup = CreateWindowEx(0, L"BUTTON", (L"Font settings"), WS_VISIBLE | WS_CHILD | 0x00000007, 20, 11, 410, 130, configHWND, (HMENU)0, hInstance, 0);
	SendMessage(fontSettingsGroup, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontLabel = CreateWindowEx(0, L"STATIC", (L"Font :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 33, 36, 75, 15, configHWND, (HMENU)0, hInstance, 0);
	SendMessage(fontLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	fontValueLabel = CreateWindowEx(0, L"STATIC", (L"undefined"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 115, 36, 243, 15, configHWND, (HMENU)0, hInstance, 0);
	SendMessage(fontValueLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontSizeLabel = CreateWindowEx(0, L"STATIC", (L"Font height :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 33, 52, 75, 15, configHWND, (HMENU)0, hInstance, 0);
	SendMessage(fontSizeLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	fontSizeValueLabel = CreateWindowEx(0, L"STATIC", (L"undefined"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 115, 52, 243, 15, configHWND, (HMENU)0, hInstance, 0);
	SendMessage(fontSizeValueLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontStyleLabel = CreateWindowEx(0, L"STATIC", (L"Font style :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 33, 68, 75, 15, configHWND, (HMENU)0, hInstance, 0);
	SendMessage(fontStyleLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	fontStyleValueLabel = CreateWindowEx(0, L"STATIC", (L"undefined"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 115, 68, 243, 15, configHWND, (HMENU)0, hInstance, 0);
	SendMessage(fontStyleValueLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND colorLabel = CreateWindowEx(0, L"STATIC", (L"Color :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 33, 85, 75, 15, configHWND, (HMENU)0, hInstance, 0);
	SendMessage(colorLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontButton = CreateWindowEx(0, L"BUTTON", (L"Select Font"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 135, 111, 83, 23, configHWND, (HMENU)FONT_BUTTON, hInstance, 0);
	SendMessage(fontButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND colorButton = CreateWindowEx(0, L"BUTTON", (L"Select Color"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 233, 111, 83, 23, configHWND, (HMENU)COLOR_BUTTON, hInstance, 0);
	SendMessage(colorButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND applyButton = CreateWindowEx(0, L"BUTTON", (L"Apply"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 191, 150, 68, 23, configHWND, (HMENU)APPLY_BUTTON, hInstance, 0);
	SendMessage(applyButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	SetFontAttributesText(configHWND, subtitlesLogFont);
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
				{
					if (OpenFileExplorer(hwnd, gamePath, MAX_PATH, LOWORD(wParam)))
						SetWindowText(game_text, PathFindFileName(gamePath));
				}
				break;

				case SUBTITLES_BUTTON:
				{
					if (OpenFileExplorer(hwnd, filePath, MAX_PATH, LOWORD(wParam)))
						SetWindowText(subtitles_text, PathFindFileName(filePath));
				}
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
				{
					PostQuitMessage(0);
				}
				break;

				case MENU_CONFIG:
				{
					ShowWindow(configHWND, SW_SHOW);
				}
				break;
			}
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
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
	CreateConfigWindow(mainHWND, hInstance);

	ShowWindow(mainHWND, nCmdShow);
	return 0;
}
