#include "MemSubLoader.hpp"

// Config window controls handling
LRESULT CALLBACK ConfiguratorWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				case CONFIGURATOR_EDIT_BUTTON:
				{
					wchar_t *selectedIdentifier = getSelectedIdentifier();
					if (!selectedIdentifier)
					{
						break;
					}

					std::map<wchar_t *, Config, WStringCompare>::iterator iter = getConfig(selectedIdentifier);
					if (iter != configs.end())
					{
						if (!IsWindow(settingsHWND))
						{
							tmpConfig = iter->second;
							if (createSettingsWindow(hwnd))
							{
								MessageBox(hwnd, L"Error: Failed to initialize settings window", L"Window initialization", MB_ICONERROR);
								break;
							}
						}
						else
						{
							int result = MessageBox(hwnd, L"Another instance of the settings window already exists.\nDo you want to continue ? You may lost unsaved changes in the current settings window", L"Window initialization", MB_OKCANCEL | MB_ICONWARNING);
							if (result == IDOK)
							{
								DestroyWindow(settingsHWND);
								tmpConfig = iter->second;
								if (createSettingsWindow(hwnd))
								{
									MessageBox(hwnd, L"Error: Failed to initialize settings window", L"Window initialization", MB_ICONERROR);
									break;
								}
							}
						}
						updateSettingsWindowAttributes(settingsHWND);
						ShowWindow(settingsHWND, SW_SHOW);
					}
					else
					{
						wchar_t message[256] = {};
						wsprintf(message, L"Failed to get config using identifier '%s'", selectedIdentifier);
						MessageBox(hwnd, message, L"Window initialization", MB_ICONERROR);
					}
				}
				break;

				case CONFIGURATOR_NEW_BUTTON:
				{
					wchar_t buffer[33] = { 0 };
					CWin32InputBox::InputBox(L"Create new configuration", L"New configuration name :", buffer, 32, false, hwnd);

					if (buffer[0] != L'\0')
					{
						std::map<wchar_t *, Config, WStringCompare>::iterator iter = getConfig(buffer);
						if (iter != configs.end())
						{
							MessageBox(hwnd, L"New configuration name already exists", L"Create new configuration", MB_ICONERROR);
							break;
						}

						Config newConfig = {};
						newConfig.identifier = wcsdup(buffer);
						HFONT hSystemFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
						GetObject(hSystemFont, sizeof(LOGFONT), &newConfig.subtitlesFont);
						configs.insert({ wcsdup(buffer), newConfig });
						updateConfiguratorWindowAttributes();
					}
					else
					{
						MessageBox(hwnd, L"New configuration name cannot be empty", L"Create new configuration", MB_ICONERROR);
						break;
					}
				}
				break;

				case CONFIGURATOR_DELETE_BUTTON:
				{
					wchar_t *selectedIdentifier = getSelectedIdentifier();
					if (!selectedIdentifier)
					{
						break;
					}

					if (wcscmp(selectedIdentifier, L"DEFAULT") == 0)
					{
						MessageBox(hwnd, L"You cannot delete DEFAULT configuration", L"Delete configuration", MB_ICONERROR);
						break;
					}

					std::map<wchar_t *, Config, WStringCompare>::iterator iter = getConfig(selectedIdentifier);
					if (iter != configs.end())
					{
						wchar_t message[256] = {};
						wsprintf(message, L"Are you sure you want to delete configuration with identifier '%s'?", iter->first);
						int result = MessageBox(hwnd, message, L"Delete configuration", MB_OKCANCEL | MB_ICONWARNING);
						if (result == IDOK)
						{
							free(iter->first);
							free(iter->second.identifier);
							configs.erase(selectedIdentifier);
							updateConfiguratorWindowAttributes();
						}
					}
					else
					{
						wchar_t message[256] = {};
						wsprintf(message, L"Failed to get config using identifier '%s'", selectedIdentifier);
						MessageBox(hwnd, message, L"Window initialization", MB_ICONERROR);
					}
				}
				break;

				case CONFIGURATOR_DUPLICATE_BUTTON:
				{
					wchar_t *selectedIdentifier = getSelectedIdentifier();
					if (!selectedIdentifier)
					{
						break;
					}

					std::map<wchar_t *, Config, WStringCompare>::iterator iter = getConfig(selectedIdentifier);
					if (iter != configs.end())
					{
						wchar_t buffer[33] = { 0 };
						CWin32InputBox::InputBox(L"Duplicate configuration", L"Duplicated configuration name :", buffer, 32, false, hwnd);

						if (buffer[0] != L'\0')
						{
							std::map<wchar_t *, Config, WStringCompare>::iterator iter2 = getConfig(buffer);
							if (iter2 != configs.end())
							{
								MessageBox(hwnd, L"Duplicated configuration name already exists", L"Duplicate configuration", MB_ICONERROR);
								break;
							}

							Config newConfig = iter->second;
							newConfig.identifier = wcsdup(buffer);
							configs.insert({ wcsdup(buffer), newConfig });
							updateConfiguratorWindowAttributes();
						}
						else
						{
							MessageBox(hwnd, L"Duplicated configuration name cannot be empty", L"Duplicate configuration", MB_ICONERROR);
							break;
						}
					}
					else
					{
						wchar_t message[256] = {};
						wsprintf(message, L"Failed to get config using identifier '%s'", selectedIdentifier);
						MessageBox(hwnd, message, L"Window initialization", MB_ICONERROR);
					}
				}
				break;

				case CONFIGURATOR_TODEFAULT_BUTTON:
				{
					wchar_t *selectedIdentifier = getSelectedIdentifier();
					if (!selectedIdentifier)
					{
						break;
					}

					std::map<wchar_t *, Config, WStringCompare>::iterator iter = getConfig(selectedIdentifier);
					if (iter != configs.end())
					{
						wchar_t buffer[8] = L"DEFAULT";
						std::map<wchar_t *, Config, WStringCompare>::iterator iter2 = getConfig(buffer);
						if (iter2 != configs.end())
						{
							wchar_t *defaultIdentifier = iter->second.identifier;
							iter->second = iter2->second;
							iter->second.identifier = defaultIdentifier;

							wchar_t message[256] = {};
							wsprintf(message, L"Configuration '%s' successfully copied to default", selectedIdentifier);
							MessageBox(hwnd, message, L"Copy configuration to default", MB_ICONINFORMATION);
						}
						else
						{
							wchar_t message[256] = {};
							wsprintf(message, L"Failed to get config using identifier '%s'", selectedIdentifier);
							MessageBox(hwnd, message, L"Window initialization", MB_ICONERROR);
						}
					}
					else
					{
						wchar_t message[256] = {};
						wsprintf(message, L"Failed to get config using identifier '%s'", selectedIdentifier);
						MessageBox(hwnd, message, L"Window initialization", MB_ICONERROR);
					}
				}
				break;
			}
		}
		break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			// Window background
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

			EndPaint(hwnd, &ps);
		}
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int createConfiguratorWindow(HWND parent)
{
	const wchar_t CONFIGURATOR_CLASS_NAME[] = L"ConfiguratorWindowClass";
	WNDCLASS configuratorWindowClass = {};
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();

	GetWindowRect(hDesktop, &desktop);
	configuratorWindowClass.lpfnWndProc = ConfiguratorWindowProc;
	configuratorWindowClass.hInstance = GetModuleHandle(NULL);
	configuratorWindowClass.lpszClassName = CONFIGURATOR_CLASS_NAME;
	RegisterClass(&configuratorWindowClass);

	configuratorHWND = CreateWindowEx(0, CONFIGURATOR_CLASS_NAME, L"Subtitles Configurator", WS_SYSMENU | WS_MINIMIZEBOX, (desktop.right / 2) - (486 / 2), (desktop.bottom / 2) - (403 / 2), 486, 403, NULL, NULL, NULL, NULL);
	if (!IsWindow(configuratorHWND))
	{
		return 1;
	}

	configList = CreateWindowEx(0, WC_LISTVIEW, (L""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | LVS_EX_GRIDLINES, 11, 11, 353, 351, configuratorHWND, (HMENU)0, configuratorWindowClass.hInstance, 0);
	SendMessage(configList, WM_SETFONT, (WPARAM)hFont, FALSE);
	ListView_SetExtendedListViewStyle(configList, LVS_EX_FULLROWSELECT);
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_WIDTH;
	lvc.cx = 351;
	lvc.pszText = const_cast<LPWSTR>(L"Identifier");
	ListView_InsertColumn(configList, 0, &lvc);

	HWND cancelButton = CreateWindowEx(0, WC_BUTTON, (L"Edit"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 375, 11, 93, 23, configuratorHWND, (HMENU)CONFIGURATOR_EDIT_BUTTON, configuratorWindowClass.hInstance, 0);
	SendMessage(cancelButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND newButton = CreateWindowEx(0, WC_BUTTON, (L"New"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | 0x00000001, 375, 39, 93, 23, configuratorHWND, (HMENU)CONFIGURATOR_NEW_BUTTON, configuratorWindowClass.hInstance, 0);
	SendMessage(newButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND deleteButton = CreateWindowEx(0, WC_BUTTON, (L"Delete"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 375, 68, 93, 23, configuratorHWND, (HMENU)CONFIGURATOR_DELETE_BUTTON, configuratorWindowClass.hInstance, 0);
	SendMessage(deleteButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND duplicateButton = CreateWindowEx(0, WC_BUTTON, (L"Duplicate"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 375, 96, 93, 23, configuratorHWND, (HMENU)CONFIGURATOR_DUPLICATE_BUTTON, configuratorWindowClass.hInstance, 0);
	SendMessage(duplicateButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND copyToDefaultButton = CreateWindowEx(0, WC_BUTTON, (L"Copy to default"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 375, 125, 93, 23, configuratorHWND, (HMENU)CONFIGURATOR_TODEFAULT_BUTTON, configuratorWindowClass.hInstance, 0);
	SendMessage(copyToDefaultButton, WM_SETFONT, (WPARAM)hFont, FALSE);

	updateConfiguratorWindowAttributes();
	return 0;
}

// Configurator window update
void updateConfiguratorWindowAttributes(void)
{
	ListView_DeleteAllItems(configList);

	int i = 1;
	for (std::map<wchar_t *, Config, WStringCompare>::iterator iter = configs.begin(); iter != configs.end(); iter++)
	{
		LVITEM lvi = {};
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iItem = i;
		lvi.pszText = reinterpret_cast<LPWSTR>(iter->first);
		lvi.lParam = reinterpret_cast<LPARAM>(iter->first);
		ListView_InsertItem(configList, &lvi);
		++i;
	}
}
