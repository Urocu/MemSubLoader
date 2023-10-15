#include "MemSubLoader.hpp"

// Config window controls handling
LRESULT CALLBACK SettingsWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
					if (openFontDialog(hwnd, config.subtitlesFont, subtitlesHFont))
					{
						InvalidateRect(hwnd, NULL, TRUE);
						GetObject(subtitlesHFont, sizeof(LOGFONT), &config.subtitlesFont);
						updateSubtitlesSettingsAttributes(hwnd, config.subtitlesFont);
					}
				}
				break;

				case COLOR_BUTTON:
				{
					if (openColorDialog(hwnd, config.subtitlesColor))
					{
						InvalidateRect(hwnd, NULL, TRUE);
					}
				}
				break;

				case ALIGNMENT_COMBOBOX:
				{
					int selectedAlignment = SendMessage(alignmentComboBox, CB_GETCURSEL, 0, 0);
					config.alignment = static_cast<TextAlignment>(selectedAlignment);
				}
				break;

				case APPLY_BUTTON:
				{
					// Apply settings...
				}
				break;
			}
			break;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			HBRUSH brush = CreateSolidBrush(config.subtitlesColor);
			RECT rect = { 115, 118, 135, 138 };
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			FillRect(hdc, &rect, brush);
			DeleteObject(brush);
			EndPaint(hwnd, &ps);
		}
		break;

		case WM_CLOSE:
		{
			ShowWindow(hwnd, SW_HIDE);
			return TRUE;
		}
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Config window update
void updateSubtitlesSettingsAttributes(HWND hwnd, LOGFONT &lf)
{
	wchar_t fontSizeText[16];
	int fontSizePixels = lf.lfHeight;
	std::wstring fontStyle;

	// Font
	SetWindowText(fontValueLabel, lf.lfFaceName);

	// Font height
	if (fontSizePixels < 0) {
		HDC hdc = GetDC(hwnd);
		if (hdc) {
			int logPixelsY = GetDeviceCaps(hdc, LOGPIXELSY);
			ReleaseDC(hwnd, hdc);
			fontSizePixels = MulDiv(-fontSizePixels, logPixelsY, 72);
		}
	}
	swprintf(fontSizeText, L"%d pixels", fontSizePixels);
	SetWindowText(fontSizeValueLabel, fontSizeText);

	// Font style
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

	// Alignment
	SendMessage(alignmentComboBox, CB_SETCURSEL, static_cast<WPARAM>(config.alignment), 0);
}

// Settings window initialization
void createSettingsWindow(HWND parent)
{
	const wchar_t SETTINGS_CLASS_NAME[] = L"ConfigWindowClass";
	WNDCLASS settingsWindowClass = {};
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();

	GetWindowRect(hDesktop, &desktop);
	settingsWindowClass.lpfnWndProc = SettingsWindowProc;
	settingsWindowClass.hInstance = GetModuleHandle(NULL);
	settingsWindowClass.lpszClassName = SETTINGS_CLASS_NAME;
	RegisterClass(&settingsWindowClass);

	settingsHWND = CreateWindowEx(0, SETTINGS_CLASS_NAME, L"Subtitles Settings", WS_SYSMENU | WS_MINIMIZEBOX, (desktop.right / 2) - (456 / 2), (desktop.bottom / 2) - (218 / 2), 456, 250, NULL, NULL, NULL, NULL);

	HWND fontSettingsGroup = CreateWindowEx(0, L"BUTTON", (L"Font settings"), WS_VISIBLE | WS_CHILD | 0x00000007, 20, 11, 410, 163, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontSettingsGroup, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontLabel = CreateWindowEx(0, L"STATIC", (L"Font :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 33, 36, 75, 16, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	fontValueLabel = CreateWindowEx(0, L"STATIC", (L"undefined"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 115, 36, 243, 16, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontValueLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontSizeLabel = CreateWindowEx(0, L"STATIC", (L"Font height :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 33, 56, 75, 16, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontSizeLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	fontSizeValueLabel = CreateWindowEx(0, L"STATIC", (L"undefined"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 115, 56, 243, 16, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontSizeValueLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontStyleLabel = CreateWindowEx(0, L"STATIC", (L"Font style :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 33, 76, 75, 16, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontStyleLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	fontStyleValueLabel = CreateWindowEx(0, L"STATIC", (L"undefined"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 115, 76, 243, 16, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontStyleValueLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND alignmentLabel = CreateWindowEx(0, L"STATIC", (L"Alignment :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 33, 96, 75, 16, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(alignmentLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	alignmentComboBox = CreateWindowEx(0, L"COMBOBOX", (L""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 115, 93, 113, 49, settingsHWND, (HMENU)ALIGNMENT_COMBOBOX, settingsWindowClass.hInstance, 0);
	SendMessage(alignmentComboBox, WM_SETFONT, (WPARAM)hFont, FALSE);
	SendMessage(alignmentComboBox, CB_ADDSTRING, 0, (LPARAM)L"Left");
	SendMessage(alignmentComboBox, CB_ADDSTRING, 0, (LPARAM)L"Center");
	SendMessage(alignmentComboBox, CB_ADDSTRING, 0, (LPARAM)L"Right");
	SendMessage(alignmentComboBox, CB_SETCURSEL, 0, 0);
	HWND colorLabel = CreateWindowEx(0, L"STATIC", (L"Color :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 33, 120, 75, 16, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(colorLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontButton = CreateWindowEx(0, L"BUTTON", (L"Select Font"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 138, 146, 83, 23, settingsHWND, (HMENU)FONT_BUTTON, settingsWindowClass.hInstance, 0);
	SendMessage(fontButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND colorButton = CreateWindowEx(0, L"BUTTON", (L"Select Color"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 236, 146, 83, 23, settingsHWND, (HMENU)COLOR_BUTTON, settingsWindowClass.hInstance, 0);
	SendMessage(colorButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND applyButton = CreateWindowEx(0, L"BUTTON", (L"Apply"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 191, 187, 68, 23, settingsHWND, (HMENU)APPLY_BUTTON, settingsWindowClass.hInstance, 0);
	SendMessage(applyButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	updateSubtitlesSettingsAttributes(settingsHWND, config.subtitlesFont);
}
