#include "MemSubLoader.hpp"

// Main window controls handling
LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

		case WM_SIZE:
		{
			if (wParam == SIZE_MINIMIZED)
			{
				AddTrayIcon(hwnd, GetModuleHandle(NULL));
				ShowWindow(hwnd, SW_HIDE); // Hide window in taskbar
			}
			else if (isTrayVisible)
			{
				RemoveTrayIcon();
			}
		}
		break;

		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case GAME_BUTTON:
				{
					if (openFileExplorer(hwnd, gamePath, MAX_PATH, LOWORD(wParam)))
						SetWindowText(gamePathValueLabel, gamePath);
				}
				break;

				case SUBTITLES_BUTTON:
				{
					if (openFileExplorer(hwnd, subtitlesPath, MAX_PATH, LOWORD(wParam)))
					{
						SetWindowText(subtitlesPathValueLabel, subtitlesPath);
						if(SubtitlesLoad(subtitlesPath))
							{
								MessageBox(hwnd, L"Failed to load subtitles file", L"Configuration autoloading", MB_ICONERROR);
								subtitlesPath[0] = '\0';
							}
					}
				}
				break;

				case START_BUTTON: // When both game and file was selected
				{
					if (gamePath[0] == L'\0' || subtitlesPath[0] == L'\0')
					{
						MessageBox(hwnd, L"You must select both game and file", L"Warning", MB_ICONWARNING);
					}
					else
					{
						ShowWindow(mainHWND, SW_MINIMIZE); // Minimize main window
						startGame(hwnd); // Start the game and pass main window handle
					}
				}
				break;

				case MENU_LOAD:
				{
					wchar_t selectedFile[MAX_PATH] = {};
					wchar_t message[MAX_PATH + 256] = {};

					if (openFileExplorer(hwnd, selectedFile, MAX_PATH, MENU_LOAD))
					{
						std::map<wchar_t *, Config, WStringCompare> tmp = configs;

						if (!loadConfig(selectedFile))
						{
							InvalidateRect(hwnd, NULL, TRUE);
							updateMainAttributes(hwnd);
							wsprintf(message, L"Configuration loaded successfully :\n%s", selectedFile);
							MessageBox(hwnd, message, L"Configuration loading", MB_ICONINFORMATION);
						}
						else
						{
							configs = tmp;
							wsprintf(message, L"Failed to load configuration :\n%s", selectedFile);
							MessageBox(hwnd, message, L"Configuration loading", MB_ICONERROR);
						}
					}
				}
				break;

				case MENU_SAVE:
				{
					wchar_t selectedFile[MAX_PATH] = {};
					wchar_t message[MAX_PATH + 256] = {};

					if (openFileExplorer(hwnd, selectedFile, MAX_PATH, MENU_SAVE))
					{
						if (!saveConfig(selectedFile))
						{
							wsprintf(message, L"Configuration saved successfully :\n%s", selectedFile);
							MessageBox(hwnd, message, L"Configuration saving", MB_ICONINFORMATION);
						}
						else
						{
							wsprintf(message, L"Failed to save configuration :\n%s", selectedFile);
							MessageBox(hwnd, message, L"Configuration saving", MB_ICONERROR);
						}
					}
				}
				break;

				case MENU_SETAUTOLOAD:
				{
					wchar_t selectedFile[MAX_PATH] = {};
					wchar_t message[MAX_PATH + 256] = {};

					if (openFileExplorer(hwnd, selectedFile, MAX_PATH, MENU_LOAD))
					{
						if (setAutoloadConfigPath(selectedFile))
						{
							wsprintf(message, L"Configuration autoload set successfully :\n%s", selectedFile);
							MessageBox(hwnd, message, L"Setting configuration autoloading", MB_ICONINFORMATION);
						}
						else {
							wsprintf(message, L"Failed to save configuration :\n%s", selectedFile);
							MessageBox(hwnd, message, L"Setting configuration autoloading", MB_ICONERROR);
						}
					}
				}
				break;

				case MENU_CREATESHORTCUT:
				{
					if (loadedConfig == L"")
					{
						MessageBox(NULL, L"You must save/load a configuration to be able to create a shortcut.", L"Create shortcut", MB_ICONERROR);
					}
					else if (gamePath[0] == L'\0' || subtitlesPath[0] == L'\0')
					{
						MessageBox(NULL, L"Missing game or subtitles path in your autoloaded configuration file. Please check if both paths are correct and try again.", L"Create shortcut", MB_ICONERROR);
					}
					else
					{
						wchar_t selectedFile[MAX_PATH] = {};

						// Open the file explorer dialog with the appropriate filter
						if (openFileExplorer(hwnd, selectedFile, MAX_PATH, MENU_CREATESHORTCUT))
						{
							// Ensure that the path ends with ".lnk"
							std::wstring shortcutPath = selectedFile;
							if (shortcutPath.length() < 4 || shortcutPath.substr(shortcutPath.length() - 4) != L".lnk")
							{
								shortcutPath += L".lnk";
							}

							// Get the path of the current executable
							wchar_t exePath[MAX_PATH];
							GetModuleFileName(NULL, exePath, MAX_PATH);

							// Create the shortcut
							if (createShortcut(exePath, (L"-config " + loadedConfig).c_str(), NULL, shortcutPath.c_str()))
							{
								MessageBox(NULL, L"Shortcut created successfully", L"Create shortcut", MB_ICONINFORMATION);
							}
							else
							{
								MessageBox(NULL, L"Failed to create shortcut", L"Create shortcut", MB_ICONERROR);
							}
						}
					}
				}
				break;

				case MENU_EXIT:
				{
					PostQuitMessage(0);
				}
				break;

				case MENU_SETTINGS:
				{
					if (!IsWindow(configuratorHWND))
					{
						if (createConfiguratorWindow(hwnd))
						{
							MessageBox(hwnd, L"Error: Failed to initialize configurator window", L"Window initialization", MB_ICONERROR);
							break;
						}
					}
					ShowWindow(configuratorHWND, SW_SHOW);
				}
				break;

				case TRAY_OPEN:
				{
					ShowWindow(hwnd, SW_SHOWNORMAL);
				}
				break;
				
				case TRAY_EXIT:
				{
					PostMessage(hwnd, WM_CLOSE, 0, 0);
				}
				break;
			}
		}
		break;

		case WM_SYSCOMMAND:
		{
			if (wParam == SC_RESTORE)
			{
				ShowWindow(hwnd, SW_RESTORE);
			}
		}
		break;

		case WM_DESTROY:
		{
			RemoveTrayIcon();
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

		case TRAY_SHOW:
			switch (LOWORD(lParam))
			{
				case WM_RBUTTONDOWN:
				case WM_CONTEXTMENU:
				// Right-click or context menu requested on the tray icon
				{
					POINT pt;
					GetCursorPos(&pt);

					HMENU hPopupMenu = CreatePopupMenu();
					AppendMenu(hPopupMenu, MF_STRING, TRAY_OPEN, L"Open");
					AppendMenu(hPopupMenu, MF_STRING, TRAY_EXIT, L"Quit");

					SetForegroundWindow(hwnd); // Ensure the menu is on top
					TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
					DestroyMenu(hPopupMenu);
				}
				break;
			}
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Update paths text
void updateMainAttributes(HWND hwnd)
{
	if (gamePath[0] != L'\0')
	{
		SetWindowText(gamePathValueLabel, gamePath);
	}
	if (subtitlesPath[0] != L'\0')
	{
		SetWindowText(subtitlesPathValueLabel, subtitlesPath);
	}
}

// Main window initialization
int createMainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const wchar_t CLASS_NAME[] = L"MainWindowClass";
	WNDCLASS wc = {};
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();

	GetWindowRect(hDesktop, &desktop);
	wc.lpfnWndProc = mainWindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);

	// Creates main window and positions it at the middle of the screen
	mainHWND = CreateWindowEx(0, CLASS_NAME, L"MemSubLoader", WS_OVERLAPPEDWINDOW &~WS_MAXIMIZEBOX &~WS_THICKFRAME, (desktop.right / 2) - (500 / 2), (desktop.bottom / 2) - (300 / 2), 500, 300, 0, 0, hInstance, NULL);
	if (!IsWindow(mainHWND))
	{
		return 1;
	}

	// Window icon & subtitles icon
	iconLogo = LoadIcon(hInstance, MAKEINTRESOURCE(MSL_ICON));
	iconSubtitles = LoadIcon(hInstance, MAKEINTRESOURCE(SUBTITLES_ICON));
	SendMessage(mainHWND, WM_SETICON, ICON_SMALL, (LPARAM)iconLogo);
	SendMessage(mainHWND, WM_SETICON, ICON_BIG, (LPARAM)iconLogo);

	// UI
	hFont = CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Ms Shell Dlg");
	titleFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Calibri");
	logoBitmap = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCEW(MSL_LOGO), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);

	// UI Menu
	HMENU hMenu = CreateMenu();
	HMENU hMenu_1 = CreatePopupMenu();
	AppendMenu(hMenu_1, MF_STRING, MENU_LOAD, (L"Load configuration"));
	AppendMenu(hMenu_1, MF_STRING, MENU_SAVE, (L"Save configuration"));
	AppendMenu(hMenu_1, MF_STRING, MENU_SETAUTOLOAD, (L"Set autoloaded configuration"));
	AppendMenu(hMenu_1, MF_STRING, MENU_CREATESHORTCUT, (L"Create shortcut using loaded config"));
	AppendMenu(hMenu_1, MF_SEPARATOR, 0, 0);
	AppendMenu(hMenu_1, MF_STRING, MENU_EXIT, (L"Exit"));
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenu_1, (L"File"));
	HMENU hMenu_2 = CreatePopupMenu();
	AppendMenu(hMenu_2, MF_STRING, MENU_SETTINGS, (L"Settings"));
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenu_2, (L"Subtitles"));
	SetMenu(mainHWND, hMenu);


	// UI Controls
	HWND logo = CreateWindowEx(0, L"STATIC", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 158, 12, 64, 64, mainHWND, (HMENU)0, hInstance, 0);
	SendMessage(logo, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)logoBitmap);
	HWND titleText = CreateWindowEx(0, L"STATIC", (L"MemSubLoader"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_CENTER, 226, 38, 160, 21, mainHWND, (HMENU)0, hInstance, 0);
	SendMessage(titleText, WM_SETFONT, (WPARAM)titleFont, FALSE);
	HWND pathGroup = CreateWindowEx(0, L"BUTTON", (L"Path"), WS_VISIBLE | WS_CHILD | 0x00000007, 24, 83, 450, 119, mainHWND, (HMENU)0, hInstance, 0);
	SendMessage(pathGroup, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND gameLabel = CreateWindowEx(0, L"STATIC", (L"Game path"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 48, 112, 74, 21, mainHWND, (HMENU)0, hInstance, 0);
	SendMessage(gameLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	gamePathValueLabel = CreateWindowEx(0, L"EDIT", (L"Select a file..."), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 130, 107, 280, 29, mainHWND, (HMENU)GAME_FIELD, hInstance, 0);
	SendMessage(gamePathValueLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND gameExplorerButton = CreateWindowEx(0, L"BUTTON", (L"Browse"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 412, 107, 58, 29, mainHWND, (HMENU)GAME_BUTTON, hInstance, 0);
	SendMessage(gameExplorerButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND subtitlesLabel = CreateWindowEx(0, L"STATIC", (L"Subtitles path"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 28, 159, 92, 21, mainHWND, (HMENU)0, hInstance, 0);
	SendMessage(subtitlesLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	subtitlesPathValueLabel = CreateWindowEx(0, L"EDIT", (L"Select a file..."), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 130, 157, 280, 29, mainHWND, (HMENU)SUBTITLES_FIELD, hInstance, 0);
	SendMessage(subtitlesPathValueLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND subtitlesExplorerButton = CreateWindowEx(0, L"BUTTON", (L"Browse"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 412, 157, 58, 29, mainHWND, (HMENU)SUBTITLES_BUTTON, hInstance, 0);
	SendMessage(subtitlesExplorerButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND startButton = CreateWindowEx(0, L"BUTTON", (L"Start game"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 210, 206, 80, 38, mainHWND, (HMENU)START_BUTTON, hInstance, 0);
	SendMessage(startButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	updateMainAttributes(mainHWND);

	return 0;
}
