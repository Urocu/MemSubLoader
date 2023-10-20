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
				case FONT_COLOR_BUTTON:
				{
					if (openColorDialog(hwnd, config.fontColor))
					{
						InvalidateRect(hwnd, NULL, TRUE);
					}
				}
				break;

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

				case FONT_COLOR_ALPHA_EDIT:
				{
					handleEdit(hwnd, config.fontColorAlpha, oldFontColorAlpha, L"Font color alpha", FONT_COLOR_ALPHA_EDIT, FONT_ALPHA_MIN, FONT_ALPHA_MAX, wParam);
				}
				break;

				case ALIGNMENT_HORIZONTAL_COMBOBOX:
				{
					int selectedAlignment = SendMessage(alignmentHorizontalComboBox, CB_GETCURSEL, 0, 0);
					config.horizontalAlignment = static_cast<TextAlignment>(selectedAlignment);
				}
				break;

				case ALIGNMENT_VERTICAL_COMBOBOX:
				{
					int selectedAlignment = SendMessage(alignmentVerticalComboBox, CB_GETCURSEL, 0, 0);
					config.verticalAlignment = static_cast<TextAlignment>(selectedAlignment);
				}
				break;

				case OUTLINE_WIDTH_EDIT:
				{
					handleEdit(hwnd, config.outlineWidth, oldOutlineWidth, L"Outline width", OUTLINE_WIDTH_EDIT, OUTLINE_WIDTH_MIN, OUTLINE_WIDTH_MAX, wParam);
				}
				break;

				case OUTLINE_COLOR_BUTTON:
				{
					if (openColorDialog(hwnd, config.outlineColor))
					{
						InvalidateRect(hwnd, NULL, TRUE);
					}
				}
				break;

				case OUTLINE_COLOR_ALPHA_EDIT:
				{
					handleEdit(hwnd, config.outlineColorAlpha, oldOutlineColorAlpha, L"Outline color alpha", OUTLINE_COLOR_ALPHA_EDIT, OUTLINE_ALPHA_MIN, OUTLINE_ALPHA_MAX, wParam);
				}
				break;

				case SHADOWS_COLOR_BUTTON:
				{
					if (openColorDialog(hwnd, config.shadowsColor))
					{
						InvalidateRect(hwnd, NULL, TRUE);
					}
				}
				break;

				case SHADOWS_COLOR_ALPHA_EDIT:
				{
					handleEdit(hwnd, config.shadowsColorAlpha, oldShadowsColorAlpha, L"Outline color alpha", SHADOWS_COLOR_ALPHA_EDIT, SHADOWS_ALPHA_MIN, SHADOWS_ALPHA_MAX, wParam);
				}
				break;

				case SHADOWS_WIDTH_EDIT:
				{
					handleEdit(hwnd, config.shadowsWidth, oldShadowsWidth, L"Shadows width", SHADOWS_WIDTH_EDIT, SHADOWS_WIDTH_MIN, SHADOWS_WIDTH_MAX, wParam);
				}
				break;

				case SHADOWS_XOFFSET_EDIT:
				{
					handleEdit(hwnd, config.shadowsXOffset, oldShadowsXOffset, L"Shadows X offset", SHADOWS_XOFFSET_EDIT, SHADOWS_XOFFSET_MIN, SHADOWS_XOFFSET_MAX, wParam);
				}
				break;

				case SHADOWS_YOFFSET_EDIT:
				{
					handleEdit(hwnd, config.shadowsYOffset, oldShadowsYOffset, L"Shadows Y offset", SHADOWS_YOFFSET_EDIT, SHADOWS_YOFFSET_MIN, SHADOWS_YOFFSET_MAX, wParam);
				}
				break;

				case SHADOWS_DIFFUSE_CHECKBOX:
				{
					if (HIWORD(wParam) == BN_CLICKED)
					{
						config.shadowsDiffuse = (IsDlgButtonChecked(hwnd, SHADOWS_DIFFUSE_CHECKBOX) == BST_CHECKED);
					}
				}

				case AREA_XPOS_EDIT:
				{
					handleEdit(hwnd, config.areaXPosition, oldAreaXPosition, L"Area X position", AREA_XPOS_EDIT, AREA_XPOS_MIN, AREA_XPOS_MAX, wParam);
				}
				break;

				case AREA_YPOS_EDIT:
				{
					handleEdit(hwnd, config.areaYPosition, oldAreaYPosition, L"Area Y position", AREA_YPOS_EDIT, AREA_YPOS_MIN, AREA_YPOS_MAX, wParam);
				}
				break;

				case AREA_WIDTH_EDIT:
				{
					handleEdit(hwnd, config.areaWidth, oldAreaWidth, L"Area width", AREA_WIDTH_EDIT, AREA_WIDTH_MIN, AREA_WIDTH_MAX, wParam);
				}
				break;

				case AREA_HEIGHT_EDIT:
				{
					handleEdit(hwnd, config.areaHeight, oldAreaHeight, L"Area height", AREA_HEIGHT_EDIT, AREA_HEIGHT_MIN, AREA_HEIGHT_MAX, wParam);
				}
				break;

				case SAVE_BUTTON:
				{
					// Apply settings...
				}
				break;

				case CANCEL_BUTTON:
				{

				}
				break;
			}
			break;
		}

		case WM_NOTIFY:
		{
			switch (LOWORD(wParam))
			{
				case FONT_COLOR_ALPHA_UPDOWN:
				{
					handleUpdown(hwnd, config.fontColorAlpha, oldFontColorAlpha, L"Font color alpha", FONT_COLOR_ALPHA_EDIT, FONT_ALPHA_MIN, FONT_ALPHA_MAX, lParam);
				}
				break;

				case OUTLINE_WIDTH_UPDOWN:
				{
					handleUpdown(hwnd, config.outlineWidth, oldOutlineWidth, L"Outline width", OUTLINE_WIDTH_EDIT, OUTLINE_WIDTH_MIN, OUTLINE_WIDTH_MAX, lParam);
				}
				break;

				case OUTLINE_COLOR_ALPHA_UPDOWN:
				{
					handleUpdown(hwnd, config.outlineColorAlpha, oldOutlineColorAlpha, L"Outline color alpha", OUTLINE_COLOR_ALPHA_EDIT, OUTLINE_ALPHA_MIN, OUTLINE_ALPHA_MAX, lParam);
				}
				break;

				case SHADOWS_WIDTH_UPDOWN:
				{
					handleUpdown(hwnd, config.shadowsWidth, oldShadowsWidth, L"Shadows width", SHADOWS_WIDTH_EDIT, SHADOWS_WIDTH_MIN, SHADOWS_WIDTH_MAX, lParam);
				}
				break;

				case SHADOWS_XOFFSET_UPDOWN:
				{
					handleUpdown(hwnd, config.shadowsXOffset, oldShadowsXOffset, L"Shadows X offset", SHADOWS_XOFFSET_EDIT, SHADOWS_XOFFSET_MIN, SHADOWS_XOFFSET_MAX, lParam);
				}
				break;

				case SHADOWS_YOFFSET_UPDOWN:
				{
					handleUpdown(hwnd, config.shadowsYOffset, oldShadowsYOffset, L"Shadows Y offset", SHADOWS_YOFFSET_EDIT, SHADOWS_YOFFSET_MIN, SHADOWS_YOFFSET_MAX, lParam);
				}
				break;

				case SHADOWS_COLOR_ALPHA_UPDOWN:
				{
					handleUpdown(hwnd, config.shadowsColorAlpha, oldShadowsColorAlpha, L"Shadows color alpha", SHADOWS_COLOR_ALPHA_EDIT, SHADOWS_ALPHA_MIN, SHADOWS_ALPHA_MAX, lParam);
				}
				break;

				case AREA_XPOS_UPDOWN:
				{
					handleUpdown(hwnd, config.areaXPosition, oldAreaXPosition, L"Area X position", AREA_XPOS_EDIT, AREA_XPOS_MIN, AREA_XPOS_MAX, lParam);
				}
				break;

				case AREA_YPOS_UPDOWN:
				{
					handleUpdown(hwnd, config.areaYPosition, oldAreaYPosition, L"Area Y position", AREA_YPOS_EDIT, AREA_YPOS_MIN, AREA_YPOS_MAX, lParam);
				}
				break;

				case AREA_WIDTH_UPDOWN:
				{
					handleUpdown(hwnd, config.areaWidth, oldAreaWidth, L"Area width", AREA_WIDTH_EDIT, AREA_WIDTH_MIN, AREA_WIDTH_MAX, lParam);
				}
				break;

				case AREA_HEIGHT_UPDOWN:
				{
					handleUpdown(hwnd, config.areaHeight, oldAreaHeight, L"Area height", AREA_HEIGHT_EDIT, AREA_HEIGHT_MIN, AREA_HEIGHT_MAX, lParam);
				}
				break;

				case AREA_PREVIEW_CHECKBOX:
				{
					if (HIWORD(wParam) == BN_CLICKED)
					{
						config.areaPreview = (IsDlgButtonChecked(hwnd, AREA_PREVIEW_CHECKBOX) == BST_CHECKED);
					}
				}
				break;
			}
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			// Window background
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

			// Font color square + outline
			HBRUSH fontColorBrush = CreateSolidBrush(config.fontColor);
			RECT fontColorRect = { 86, 114, 106, 134 };
			FillRect(hdc, &fontColorRect, fontColorBrush);
			FrameRect(hdc, &fontColorRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

			// Outline color square + outline
			HBRUSH outlineColorBrush = CreateSolidBrush(config.outlineColor);
			RECT outlineColorRect = { 379, 148, 399, 168 };
			FillRect(hdc, &outlineColorRect, outlineColorBrush);
			FrameRect(hdc, &outlineColorRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

			// Shadows color square + outline
			HBRUSH shadowsColorBrush = CreateSolidBrush(config.shadowsColor);
			RECT shadowsColorRect = { 110, 260, 130, 280 };
			FillRect(hdc, &shadowsColorRect, shadowsColorBrush);
			FrameRect(hdc, &shadowsColorRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

			DeleteObject(fontColorBrush);
			DeleteObject(outlineColorBrush);
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
	int cursorPosition;

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
	SendMessage(alignmentHorizontalComboBox, CB_SETCURSEL, static_cast<WPARAM>(config.horizontalAlignment), 0);
	SendMessage(alignmentVerticalComboBox, CB_SETCURSEL, static_cast<WPARAM>(config.verticalAlignment), 0);

	oldOutlineWidth = config.outlineWidth;
	oldShadowsWidth = config.shadowsWidth;
	oldShadowsXOffset = config.shadowsXOffset;
	oldShadowsYOffset = config.shadowsYOffset;
	oldAreaXPosition = config.areaXPosition;
	oldAreaYPosition = config.areaYPosition;
	oldAreaWidth = config.areaWidth;
	oldAreaHeight = config.areaHeight;
	oldFontColorAlpha = config.fontColorAlpha;
	oldOutlineColorAlpha = config.outlineColorAlpha;
	oldShadowsColorAlpha = config.shadowsColorAlpha;

	// Font
	cursorPosition = SendDlgItemMessage(hwnd, FONT_COLOR_ALPHA_EDIT, EM_GETSEL, 0, 0);
	SetDlgItemInt(settingsHWND, FONT_COLOR_ALPHA_EDIT, config.fontColorAlpha, TRUE);
	SendDlgItemMessage(hwnd, FONT_COLOR_ALPHA_EDIT, EM_SETSEL, cursorPosition, cursorPosition);

	// Outline
	cursorPosition = SendDlgItemMessage(hwnd, OUTLINE_WIDTH_EDIT, EM_GETSEL, 0, 0);
	SetDlgItemInt(settingsHWND, OUTLINE_WIDTH_EDIT, config.outlineWidth, TRUE);
	SendDlgItemMessage(hwnd, OUTLINE_WIDTH_EDIT, EM_SETSEL, cursorPosition, cursorPosition);

	cursorPosition = SendDlgItemMessage(hwnd, OUTLINE_COLOR_ALPHA_EDIT, EM_GETSEL, 0, 0);
	SetDlgItemInt(settingsHWND, OUTLINE_COLOR_ALPHA_EDIT, config.outlineColorAlpha, TRUE);
	SendDlgItemMessage(hwnd, OUTLINE_COLOR_ALPHA_EDIT, EM_SETSEL, cursorPosition, cursorPosition);

	// Shadows
	cursorPosition = SendDlgItemMessage(hwnd, SHADOWS_WIDTH_EDIT, EM_GETSEL, 0, 0);
	SetDlgItemInt(settingsHWND, SHADOWS_WIDTH_EDIT, config.shadowsWidth, TRUE);
	SendDlgItemMessage(hwnd, SHADOWS_WIDTH_EDIT, EM_SETSEL, cursorPosition, cursorPosition);

	cursorPosition = SendDlgItemMessage(hwnd, SHADOWS_COLOR_ALPHA_EDIT, EM_GETSEL, 0, 0);
	SetDlgItemInt(settingsHWND, SHADOWS_COLOR_ALPHA_EDIT, config.shadowsColorAlpha, TRUE);
	SendDlgItemMessage(hwnd, SHADOWS_COLOR_ALPHA_EDIT, EM_SETSEL, cursorPosition, cursorPosition);

	cursorPosition = SendDlgItemMessage(hwnd, SHADOWS_XOFFSET_EDIT, EM_GETSEL, 0, 0);
	SetDlgItemInt(settingsHWND, SHADOWS_XOFFSET_EDIT, config.shadowsXOffset, TRUE);
	SendDlgItemMessage(hwnd, SHADOWS_XOFFSET_EDIT, EM_SETSEL, cursorPosition, cursorPosition);

	cursorPosition = SendDlgItemMessage(hwnd, SHADOWS_YOFFSET_EDIT, EM_GETSEL, 0, 0);
	SetDlgItemInt(settingsHWND, SHADOWS_YOFFSET_EDIT, config.shadowsYOffset, TRUE);
	SendDlgItemMessage(hwnd, SHADOWS_YOFFSET_EDIT, EM_SETSEL, cursorPosition, cursorPosition);

	if (config.shadowsDiffuse)
	{
		CheckDlgButton(hwnd, SHADOWS_DIFFUSE_CHECKBOX, BST_CHECKED);
	}
	else
	{
		CheckDlgButton(hwnd, SHADOWS_DIFFUSE_CHECKBOX, BST_UNCHECKED);
	}

	// Area
	cursorPosition = SendDlgItemMessage(hwnd, AREA_XPOS_EDIT, EM_GETSEL, 0, 0);
	SetDlgItemInt(settingsHWND, AREA_XPOS_EDIT, config.areaXPosition, TRUE);
	SendDlgItemMessage(hwnd, AREA_XPOS_EDIT, EM_SETSEL, cursorPosition, cursorPosition);

	cursorPosition = SendDlgItemMessage(hwnd, AREA_YPOS_EDIT, EM_GETSEL, 0, 0);
	SetDlgItemInt(settingsHWND, AREA_YPOS_EDIT, config.areaYPosition, TRUE);
	SendDlgItemMessage(hwnd, AREA_YPOS_EDIT, EM_SETSEL, cursorPosition, cursorPosition);

	cursorPosition = SendDlgItemMessage(hwnd, AREA_WIDTH_EDIT, EM_GETSEL, 0, 0);
	SetDlgItemInt(settingsHWND, AREA_WIDTH_EDIT, config.areaWidth, TRUE);
	SendDlgItemMessage(hwnd, AREA_WIDTH_EDIT, EM_SETSEL, cursorPosition, cursorPosition);

	cursorPosition = SendDlgItemMessage(hwnd, AREA_HEIGHT_EDIT, EM_GETSEL, 0, 0);
	SetDlgItemInt(settingsHWND, AREA_HEIGHT_EDIT, config.areaHeight, TRUE);
	SendDlgItemMessage(hwnd, AREA_HEIGHT_EDIT, EM_SETSEL, cursorPosition, cursorPosition);

	if (config.areaPreview)
	{
		CheckDlgButton(hwnd, AREA_PREVIEW_CHECKBOX, BST_CHECKED);
	}
	else
	{
		CheckDlgButton(hwnd, AREA_PREVIEW_CHECKBOX, BST_UNCHECKED);
	}
}

// Settings window initialization
int createSettingsWindow(HWND parent)
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

	settingsHWND = CreateWindowEx(0, SETTINGS_CLASS_NAME, L"Subtitles Settings", WS_SYSMENU | WS_MINIMIZEBOX, (desktop.right / 2) - (521 / 2), (desktop.bottom / 2) - (432 / 2), 521, 432, NULL, NULL, NULL, NULL);
	if (!IsWindow(settingsHWND))
	{
		return 1;
	}

	// Font
	HWND fontGroup = CreateWindowEx(0, WC_BUTTON, (L"Font"), WS_VISIBLE | WS_CHILD | 0x00000007, 11, 11, 270, 195, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontGroup, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontLabel = CreateWindowEx(0, WC_STATIC, (L"Font :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 21, 36, 60, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	fontValueLabel = CreateWindowEx(0, WC_STATIC, (L"undefined"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 86, 36, 188, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontValueLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontSizeLabel = CreateWindowEx(0, WC_STATIC, (L"Font size :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 21, 62, 60, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontSizeLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	fontSizeValueLabel = CreateWindowEx(0, WC_STATIC, (L"undefined"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 86, 62, 188, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontSizeValueLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontStyleLabel = CreateWindowEx(0, WC_STATIC, (L"Font style :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 21, 88, 60, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontStyleLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	fontStyleValueLabel = CreateWindowEx(0, WC_STATIC, (L"undefined"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 86, 88, 188, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontStyleValueLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontColorLabel = CreateWindowEx(0, WC_STATIC, (L"Font color :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 21, 115, 60, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontColorLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontColorButton = CreateWindowEx(0, WC_BUTTON, (L"Select"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 121, 112, 68, 23, settingsHWND, (HMENU)FONT_COLOR_BUTTON, settingsWindowClass.hInstance, 0);
	SendMessage(fontColorButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontColorAlphaLabel = CreateWindowEx(0, WC_STATIC, (L"Color alpha :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 21, 143, 60, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(fontColorAlphaLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontColorAlphaEdit = CreateWindowEx(0, WC_EDIT, (L""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 86, 140, 173, 23, settingsHWND, (HMENU)FONT_COLOR_ALPHA_EDIT, settingsWindowClass.hInstance, 0);
	SendMessage(fontColorAlphaEdit, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND fontColorAlphaUpdown = CreateWindowEx(0, UPDOWN_CLASS, (L""), WS_VISIBLE | WS_CHILD | UDS_ARROWKEYS, 260, 140, 17, 23, settingsHWND, (HMENU)FONT_COLOR_ALPHA_UPDOWN, settingsWindowClass.hInstance, 0);
	SendMessage(fontColorAlphaUpdown, WM_SETFONT, (WPARAM)hFont, FALSE);
	SendMessage(fontColorAlphaUpdown, UDM_SETRANGE, 0, MAKELPARAM(FONT_ALPHA_MAX, FONT_ALPHA_MIN));
	SendMessage(fontColorAlphaUpdown, UDM_SETPOS, 0, MAKELPARAM(config.fontColorAlpha, 0));
	HWND fontButton = CreateWindowEx(0, WC_BUTTON, (L"Select Font"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 111, 171, 68, 23, settingsHWND, (HMENU)FONT_BUTTON, settingsWindowClass.hInstance, 0);
	SendMessage(fontButton, WM_SETFONT, (WPARAM)hFont, FALSE);

	// Alignment
	HWND alignmentGroup = CreateWindowEx(0, WC_BUTTON, (L"Alignment"), WS_VISIBLE | WS_CHILD | 0x00000007, 288, 11, 216, 81, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(alignmentGroup, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND alignmentHorizontalLabel = CreateWindowEx(0, WC_STATIC, (L"Horizontal alignment :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 296, 36, 105, 16, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(alignmentHorizontalLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	alignmentHorizontalComboBox = CreateWindowEx(0, WC_COMBOBOX, (L""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | CBS_HASSTRINGS, 408, 34, 83, 49, settingsHWND, (HMENU)ALIGNMENT_HORIZONTAL_COMBOBOX, settingsWindowClass.hInstance, 0);
	SendMessage(alignmentHorizontalComboBox, WM_SETFONT, (WPARAM)hFont, FALSE);
	SendMessage(alignmentHorizontalComboBox, WM_SETFONT, (WPARAM)hFont, FALSE);
	SendMessage(alignmentHorizontalComboBox, CB_ADDSTRING, 0, (LPARAM)L"Left");
	SendMessage(alignmentHorizontalComboBox, CB_ADDSTRING, 0, (LPARAM)L"Center");
	SendMessage(alignmentHorizontalComboBox, CB_ADDSTRING, 0, (LPARAM)L"Right");
	SendMessage(alignmentHorizontalComboBox, CB_SETCURSEL, 0, 0);
	HWND alignmentVerticalLabel = CreateWindowEx(0, WC_STATIC, (L"Vertical Alignment :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 296, 60, 105, 16, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(alignmentVerticalLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	alignmentVerticalComboBox = CreateWindowEx(0, WC_COMBOBOX, (L""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | CBS_HASSTRINGS, 408, 59, 83, 49, settingsHWND, (HMENU)ALIGNMENT_VERTICAL_COMBOBOX, settingsWindowClass.hInstance, 0);
	SendMessage(alignmentVerticalComboBox, WM_SETFONT, (WPARAM)hFont, FALSE);
	SendMessage(alignmentVerticalComboBox, CB_ADDSTRING, 0, (LPARAM)L"Top");
	SendMessage(alignmentVerticalComboBox, CB_ADDSTRING, 0, (LPARAM)L"Center");
	SendMessage(alignmentVerticalComboBox, CB_ADDSTRING, 0, (LPARAM)L"Bottom");
	SendMessage(alignmentVerticalComboBox, CB_SETCURSEL, 0, 0);

	// Outline
	HWND outlineGroup = CreateWindowEx(0, WC_BUTTON, (L"Outline"), WS_VISIBLE | WS_CHILD | 0x00000007, 288, 93, 216, 114, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(outlineGroup, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND outlineWidthLabel = CreateWindowEx(0, WC_STATIC, (L"Outline width :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 296, 117, 75, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(outlineWidthLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND outlineWidthEdit = CreateWindowEx(0, WC_EDIT, (L""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 378, 114, 105, 23, settingsHWND, (HMENU)OUTLINE_WIDTH_EDIT, settingsWindowClass.hInstance, 0);
	SendMessage(outlineWidthEdit, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND outlineWidthUpdown = CreateWindowEx(0, UPDOWN_CLASS, (L""), WS_VISIBLE | WS_CHILD | UDS_ARROWKEYS, 483, 114, 17, 23, settingsHWND, (HMENU)OUTLINE_WIDTH_UPDOWN, settingsWindowClass.hInstance, 0);
	SendMessage(outlineWidthUpdown, WM_SETFONT, (WPARAM)hFont, FALSE);
	SendMessage(outlineWidthUpdown, UDM_SETRANGE, 0, MAKELPARAM(OUTLINE_WIDTH_MAX, OUTLINE_WIDTH_MIN));
	SendMessage(outlineWidthUpdown, UDM_SETPOS, 0, MAKELPARAM(config.outlineWidth, 0));
	HWND outlineColorLabel = CreateWindowEx(0, WC_STATIC, (L"Outline color :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 294, 150, 75, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(outlineColorLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND outlineColorButton = CreateWindowEx(0, WC_BUTTON, (L"Select"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 414, 146, 68, 23, settingsHWND, (HMENU)OUTLINE_COLOR_BUTTON, settingsWindowClass.hInstance, 0);
	SendMessage(outlineColorButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND outlineColorAlphaLabel = CreateWindowEx(0, WC_STATIC, (L"Color alpha :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 303, 182, 60, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(outlineColorAlphaLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND outlineColorAlphaEdit = CreateWindowEx(0, WC_EDIT, (L""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 378, 180, 105, 23, settingsHWND, (HMENU)OUTLINE_COLOR_ALPHA_EDIT, settingsWindowClass.hInstance, 0);
	SendMessage(outlineColorAlphaEdit, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND outlineColorAlphaUpdown = CreateWindowEx(0, UPDOWN_CLASS, (L""), WS_VISIBLE | WS_CHILD | UDS_ARROWKEYS, 483, 180, 17, 23, settingsHWND, (HMENU)OUTLINE_COLOR_ALPHA_UPDOWN, settingsWindowClass.hInstance, 0);
	SendMessage(outlineColorAlphaUpdown, WM_SETFONT, (WPARAM)hFont, FALSE);
	SendMessage(outlineColorAlphaUpdown, UDM_SETRANGE, 0, MAKELPARAM(OUTLINE_ALPHA_MAX, OUTLINE_ALPHA_MIN));
	SendMessage(outlineColorAlphaUpdown, UDM_SETPOS, 0, MAKELPARAM(config.outlineColorAlpha, 0));

	// Shadows
	HWND shadowsGroup = CreateWindowEx(0, WC_BUTTON, (L"Shadows"), WS_VISIBLE | WS_CHILD | 0x00000007, 11, 206, 215, 187, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsGroup, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND shadowsWidthLabel = CreateWindowEx(0, WC_STATIC, (L"Shadows width :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 20, 231, 83, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsWidthLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND shadowsWidthEdit = CreateWindowEx(0, WC_EDIT, (L""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 110, 228, 90, 23, settingsHWND, (HMENU)SHADOWS_WIDTH_EDIT, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsWidthEdit, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND shadowsWidthUpdown = CreateWindowEx(0, UPDOWN_CLASS, (L""), WS_VISIBLE | WS_CHILD | UDS_ARROWKEYS, 200, 228, 17, 23, settingsHWND, (HMENU)SHADOWS_WIDTH_UPDOWN, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsWidthUpdown, WM_SETFONT, (WPARAM)hFont, FALSE);
	SendMessage(shadowsWidthUpdown, UDM_SETRANGE, 0, MAKELPARAM(SHADOWS_WIDTH_MAX, SHADOWS_WIDTH_MIN));
	SendMessage(shadowsWidthUpdown, UDM_SETPOS, 0, MAKELPARAM(config.shadowsWidth, 0));
	HWND shadowsColorLabel = CreateWindowEx(0, WC_STATIC, (L"Shadows color :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 18, 263, 83, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsColorLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND shadowsColorButton = CreateWindowEx(0, WC_BUTTON, (L"Select"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 145, 258, 68, 23, settingsHWND, (HMENU)SHADOWS_COLOR_BUTTON, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsColorButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND shadowsColorAlphaLabel = CreateWindowEx(0, WC_STATIC, (L"Color alpha :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 18, 293, 68, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsColorAlphaLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND shadowsColorAlphaEdit = CreateWindowEx(0, WC_EDIT, (L""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 90, 291, 113, 23, settingsHWND, (HMENU)SHADOWS_COLOR_ALPHA_EDIT, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsColorAlphaEdit, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND shadowsColorAlphaUpdown = CreateWindowEx(0, UPDOWN_CLASS, (L""), WS_VISIBLE | WS_CHILD | UDS_ARROWKEYS, 203, 291, 17, 23, settingsHWND, (HMENU)SHADOWS_COLOR_ALPHA_UPDOWN, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsColorAlphaUpdown, WM_SETFONT, (WPARAM)hFont, FALSE);
	SendMessage(shadowsColorAlphaUpdown, UDM_SETRANGE, 0, MAKELPARAM(SHADOWS_ALPHA_MAX, SHADOWS_ALPHA_MIN));
	SendMessage(shadowsColorAlphaUpdown, UDM_SETPOS, 0, MAKELPARAM(config.shadowsColorAlpha, 0));
	HWND shadowsXOffsetLabel = CreateWindowEx(0, WC_STATIC, (L"X offset :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 18, 319, 68, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsXOffsetLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND shadowsXOffsetEdit = CreateWindowEx(0, WC_EDIT, (L""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 90, 315, 113, 23, settingsHWND, (HMENU)SHADOWS_XOFFSET_EDIT, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsXOffsetEdit, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND shadowsXOffsetUpdown = CreateWindowEx(0, UPDOWN_CLASS, (L""), WS_VISIBLE | WS_CHILD | UDS_ARROWKEYS, 203, 315, 17, 23, settingsHWND, (HMENU)SHADOWS_XOFFSET_UPDOWN, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsXOffsetUpdown, WM_SETFONT, (WPARAM)hFont, FALSE);
	SendMessage(shadowsXOffsetUpdown, UDM_SETRANGE, 0, MAKELPARAM(SHADOWS_XOFFSET_MAX, SHADOWS_XOFFSET_MIN));
	SendMessage(shadowsXOffsetUpdown, UDM_SETPOS, 0, MAKELPARAM(config.shadowsXOffset, 0));
	HWND shadowsYOffsetLabel = CreateWindowEx(0, WC_STATIC, (L"Y offset :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 18, 343, 68, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsYOffsetLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND shadowsYOffsetEdit = CreateWindowEx(0, WC_EDIT, (L""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 90, 340, 113, 23, settingsHWND, (HMENU)SHADOWS_YOFFSET_EDIT, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsYOffsetEdit, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND shadowsYOffsetUpdown = CreateWindowEx(0, UPDOWN_CLASS, (L""), WS_VISIBLE | WS_CHILD | UDS_ARROWKEYS, 203, 340, 17, 23, settingsHWND, (HMENU)SHADOWS_YOFFSET_UPDOWN, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsYOffsetUpdown, WM_SETFONT, (WPARAM)hFont, FALSE);
	SendMessage(shadowsYOffsetUpdown, UDM_SETRANGE, 0, MAKELPARAM(SHADOWS_YOFFSET_MAX, SHADOWS_YOFFSET_MIN));
	SendMessage(shadowsYOffsetUpdown, UDM_SETPOS, 0, MAKELPARAM(config.shadowsYOffset, 0));
	HWND shadowsDiffuseCheckbox = CreateWindowEx(0, WC_BUTTON, (L"Diffuse shadows"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | 0x00000003, 71, 372, 102, 13, settingsHWND, (HMENU)SHADOWS_DIFFUSE_CHECKBOX, settingsWindowClass.hInstance, 0);
	SendMessage(shadowsDiffuseCheckbox, WM_SETFONT, (WPARAM)hFont, FALSE);

	// Area
	HWND areaGroup = CreateWindowEx(0, WC_BUTTON, (L"Area"), WS_VISIBLE | WS_CHILD | 0x00000007, 233, 206, 272, 106, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(areaGroup, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND areaXPositionLabel = CreateWindowEx(0, WC_STATIC, (L"X Position :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 240, 231, 60, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(areaXPositionLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND areaXPositionEdit = CreateWindowEx(0, WC_EDIT, (L""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 306, 228, 53, 23, settingsHWND, (HMENU)AREA_XPOS_EDIT, settingsWindowClass.hInstance, 0);
	SendMessage(areaXPositionEdit, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND areaXPositionUpdown = CreateWindowEx(0, UPDOWN_CLASS, (L""), WS_VISIBLE | WS_CHILD | UDS_ARROWKEYS, 359, 228, 17, 23, settingsHWND, (HMENU)AREA_XPOS_UPDOWN, settingsWindowClass.hInstance, 0);
	SendMessage(areaXPositionUpdown, WM_SETFONT, (WPARAM)hFont, FALSE);
	SendMessage(areaXPositionUpdown, UDM_SETRANGE, 0, MAKELPARAM(AREA_XPOS_MAX, AREA_XPOS_MIN));
	SendMessage(areaXPositionUpdown, UDM_SETPOS, 0, MAKELPARAM(config.areaXPosition, 0));
	HWND areaYPositionLabel = CreateWindowEx(0, WC_STATIC, (L"Y Position :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 240, 255, 60, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(areaYPositionLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND areaYPositionEdit = CreateWindowEx(0, WC_EDIT, (L""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 306, 252, 53, 23, settingsHWND, (HMENU)AREA_YPOS_EDIT, settingsWindowClass.hInstance, 0);
	SendMessage(areaYPositionEdit, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND areaYPositionUpdown = CreateWindowEx(0, UPDOWN_CLASS, (L""), WS_VISIBLE | WS_CHILD | UDS_ARROWKEYS, 359, 252, 17, 23, settingsHWND, (HMENU)AREA_YPOS_UPDOWN, settingsWindowClass.hInstance, 0);
	SendMessage(areaYPositionUpdown, WM_SETFONT, (WPARAM)hFont, FALSE);
	SendMessage(areaYPositionUpdown, UDM_SETRANGE, 0, MAKELPARAM(AREA_YPOS_MAX, AREA_YPOS_MIN));
	SendMessage(areaYPositionUpdown, UDM_SETPOS, 0, MAKELPARAM(config.areaYPosition, 0));
	HWND areaWidthLabel = CreateWindowEx(0, WC_STATIC, (L"Width :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 375, 231, 45, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(areaWidthLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND areaWidthEdit = CreateWindowEx(0, WC_EDIT, (L""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 426, 228, 53, 23, settingsHWND, (HMENU)AREA_WIDTH_EDIT, settingsWindowClass.hInstance, 0);
	SendMessage(areaWidthEdit, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND areaWidthUpdown = CreateWindowEx(0, UPDOWN_CLASS, (L""), WS_VISIBLE | WS_CHILD | UDS_ARROWKEYS, 479, 228, 17, 23, settingsHWND, (HMENU)AREA_WIDTH_UPDOWN, settingsWindowClass.hInstance, 0);
	SendMessage(areaWidthUpdown, WM_SETFONT, (WPARAM)hFont, FALSE);
	SendMessage(areaWidthUpdown, UDM_SETRANGE, 0, MAKELPARAM(AREA_WIDTH_MAX, AREA_WIDTH_MIN));
	SendMessage(areaWidthUpdown, UDM_SETPOS, 0, MAKELPARAM(config.areaWidth, 0));
	HWND areaHeightLabel = CreateWindowEx(0, WC_STATIC, (L"Height :"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, 375, 255, 45, 15, settingsHWND, (HMENU)0, settingsWindowClass.hInstance, 0);
	SendMessage(areaHeightLabel, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND areaHeightEdit = CreateWindowEx(0, WC_EDIT, (L""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, 426, 252, 53, 23, settingsHWND, (HMENU)AREA_HEIGHT_EDIT, settingsWindowClass.hInstance, 0);
	SendMessage(areaHeightEdit, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND areaHeightUpdown = CreateWindowEx(0, UPDOWN_CLASS, (L""), WS_VISIBLE | WS_CHILD | UDS_ARROWKEYS, 479, 252, 17, 23, settingsHWND, (HMENU)AREA_HEIGHT_UPDOWN, settingsWindowClass.hInstance, 0);
	SendMessage(areaHeightUpdown, WM_SETFONT, (WPARAM)hFont, FALSE);
	SendMessage(areaHeightUpdown, UDM_SETRANGE, 0, MAKELPARAM(AREA_HEIGHT_MAX, AREA_HEIGHT_MIN));
	SendMessage(areaHeightUpdown, UDM_SETPOS, 0, MAKELPARAM(config.areaHeight, 0));
	HWND areaPreviewCheckbox = CreateWindowEx(0, WC_BUTTON, (L"Area preview"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | 0x00000003, 326, 288, 83, 13, settingsHWND, (HMENU)AREA_PREVIEW_CHECKBOX, settingsWindowClass.hInstance, 0);
	SendMessage(areaPreviewCheckbox, WM_SETFONT, (WPARAM)hFont, FALSE);

	// Save & Cancel
	HWND saveButton = CreateWindowEx(0, WC_BUTTON, (L"Save"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 281, 345, 83, 23, settingsHWND, (HMENU)SAVE_BUTTON, settingsWindowClass.hInstance, 0);
	SendMessage(saveButton, WM_SETFONT, (WPARAM)hFont, FALSE);
	HWND cancelButton = CreateWindowEx(0, WC_BUTTON, (L"Cancel"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 375, 345, 83, 23, settingsHWND, (HMENU)CANCEL_BUTTON, settingsWindowClass.hInstance, 0);
	SendMessage(cancelButton, WM_SETFONT, (WPARAM)hFont, FALSE);

	updateSubtitlesSettingsAttributes(settingsHWND, config.subtitlesFont);
	return 0;
}

void handleUpdown(HWND hwnd, int &value, int &oldValue, const wchar_t *name, int id, int min, int max, LPARAM lParam)
{
	NMUPDOWN* upDown = (NMUPDOWN*)lParam;
	int newValue = value;
	wchar_t title[256] = {};
	wchar_t message[256] = {};

	if (upDown->iDelta > 0)
	{
		newValue += 1;
	}
	else if (upDown->iDelta < 0)
	{
		newValue -= 1;
	}
	if (newValue > max)
	{
		wsprintf(title, L"%s out of range", name);
		wsprintf(message, L"%s cannot be superior to %i pixels", name, max);
		MessageBox(NULL, message, title, MB_ICONWARNING);
		int cursorPosition = SendDlgItemMessage(hwnd, id, EM_GETSEL, 0, 0);
    	value = oldValue;
		SetDlgItemInt(settingsHWND, id, value, TRUE);
		SendDlgItemMessage(hwnd, id, EM_SETSEL, cursorPosition, cursorPosition);
	}
	else if (newValue < min)
	{
		wsprintf(title, L"%s out of range", name);
		wsprintf(message, L"%s cannot be inferior to %i pixels", name, min);
		MessageBox(NULL, message, title, MB_ICONWARNING);
		int cursorPosition = SendDlgItemMessage(hwnd, id, EM_GETSEL, 0, 0);
    	value = oldValue;
		SetDlgItemInt(settingsHWND, id, value, TRUE);
		SendDlgItemMessage(hwnd, id, EM_SETSEL, cursorPosition, cursorPosition);
	}
	else
	{
		int cursorPosition = SendDlgItemMessage(hwnd, id, EM_GETSEL, 0, 0);
		value = newValue;
		oldValue = value;
		SetDlgItemInt(settingsHWND, id, value, TRUE);
		SendDlgItemMessage(hwnd, id, EM_SETSEL, cursorPosition, cursorPosition);
	}
}

void handleEdit(HWND hwnd, int &value, int &oldValue, const wchar_t *name, int id, int min, int max, WPARAM wParam)
{
	if (HIWORD(wParam) == EN_CHANGE)
    {
        WCHAR buffer[256];
		wchar_t title[256] = {};
		wchar_t message[256] = {};
        GetDlgItemText(hwnd, id, buffer, sizeof(buffer) / sizeof(buffer[0]));
        int newValue = _wtoi(buffer);

		if (newValue > max)
		{
			wsprintf(title, L"%s out of range", name);
			wsprintf(message, L"%s cannot be superior to %i pixels", name, max);
			int cursorPosition = SendDlgItemMessage(hwnd, id, EM_GETSEL, 0, 0);
        	value = oldValue;
			SetDlgItemInt(settingsHWND, id, value, TRUE);
			SendDlgItemMessage(hwnd, id, EM_SETSEL, cursorPosition, cursorPosition);
		}
		else if (newValue < min)
		{
			wsprintf(title, L"%s out of range", name);
			wsprintf(message, L"%s cannot be inferior to %i pixels", name, min);
			int cursorPosition = SendDlgItemMessage(hwnd, id, EM_GETSEL, 0, 0);
        	value = oldValue;
			SetDlgItemInt(settingsHWND, id, value, TRUE);
			SendDlgItemMessage(hwnd, id, EM_SETSEL, cursorPosition, cursorPosition);
		}
		else if (newValue != value)
		{
			int cursorPosition = SendDlgItemMessage(hwnd, id, EM_GETSEL, 0, 0);
        	value = newValue;
			oldValue = value;
			SetDlgItemInt(settingsHWND, id, value, TRUE);
			SendDlgItemMessage(hwnd, id, EM_SETSEL, cursorPosition, cursorPosition);
		}
    }
}
