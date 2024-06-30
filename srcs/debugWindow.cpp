#include "MemSubLoader.hpp"

// Config window controls handling
LRESULT CALLBACK DebugWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

int createDebugWindow(HWND parent)
{
	const wchar_t DEBUG_CLASS_NAME[] = L"DebugWindowClass";
	WNDCLASS debugWindowClass = {};
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();

	GetWindowRect(hDesktop, &desktop);
	debugWindowClass.lpfnWndProc = DebugWindowProc;
	debugWindowClass.hInstance = GetModuleHandle(NULL);
	debugWindowClass.lpszClassName = DEBUG_CLASS_NAME;
	RegisterClass(&debugWindowClass);

	debugHWND = CreateWindowEx(0, DEBUG_CLASS_NAME, L"Debug", WS_SYSMENU | WS_MINIMIZEBOX, (desktop.right / 2) - (382 / 2), (desktop.bottom / 2) - (403 / 2), 382, 403, NULL, NULL, NULL, NULL);
	if (!IsWindow(debugHWND))
	{
		return 1;
	}

    debugList = CreateWindowEx(0, WC_LISTVIEW, (L""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | LVS_EX_GRIDLINES, 11, 11, 353, 175, debugHWND, (HMENU)0, debugWindowClass.hInstance, 0);
	SendMessage(debugList, WM_SETFONT, (WPARAM)hFont, FALSE);
	ListView_SetExtendedListViewStyle(debugList, LVS_EX_FULLROWSELECT);
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lvc.cx = 117;
	lvc.pszText = const_cast<LPWSTR>(L"Name");
	ListView_InsertColumn(debugList, 0, &lvc);

	lvc.pszText = const_cast<LPWSTR>(L"Address");
	ListView_InsertColumn(debugList, 1, &lvc);

	lvc.pszText = const_cast<LPWSTR>(L"Variable");
	ListView_InsertColumn(debugList, 2, &lvc);

	subtitleList = CreateWindowEx(0, WC_LISTVIEW, (L""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | LVS_EX_GRIDLINES, 11, 185, 353, 175, debugHWND, (HMENU)0, debugWindowClass.hInstance, 0);
	SendMessage(subtitleList, WM_SETFONT, (WPARAM)hFont, FALSE);
	ListView_SetExtendedListViewStyle(subtitleList, LVS_EX_FULLROWSELECT);
	lvc.mask = LVCF_TEXT | LVCF_WIDTH;
	lvc.cx = 351;
	lvc.pszText = const_cast<LPWSTR>(L"Subtitles");
	ListView_InsertColumn(subtitleList, 0, &lvc);

	// Window icon
	SendMessage(debugHWND, WM_SETICON, ICON_SMALL, (LPARAM)iconLogo);
	SendMessage(debugHWND, WM_SETICON, ICON_BIG, (LPARAM)iconLogo);
    SetWindowPos(debugHWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	createDebugWindowAttributes();
	return 0;
}



// Debug window update
void createDebugWindowAttributes(void)
{
    int i = 0;
	for (std::vector<Subtitles>::iterator iter = subtitles.begin(); iter != subtitles.end(); iter++)
	{
		LVITEM lvi = {};
		lvi.mask = LVIF_TEXT;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText = const_cast<LPWSTR>(L"Audio Address");
		ListView_InsertItem(debugList, &lvi);
		++i;
		lvi.iItem = i;
		lvi.pszText = const_cast<LPWSTR>(L"Play Address");
		ListView_InsertItem(debugList, &lvi);
		++i;
	}
}

void updateDebugWindowAttributes(void)
{
    int index = getSelectedSubtitle();
    int i = 0;
    for (std::vector<Subtitles>::iterator iter = subtitles.begin(); iter != subtitles.end(); iter++)
    {

        std::wstringstream wss;
	    std::wstring ws;
		LVITEM lvi = {};
		lvi.mask = LVIF_TEXT;
		lvi.iItem = i;
		lvi.iSubItem = 1;
        wss << iter->address_audio;
        ws = wss.str();
		lvi.pszText = const_cast<LPWSTR>(ws.c_str());
		ListView_SetItem(debugList, &lvi);
		wss.str(L"");
		lvi.iSubItem = 2;
        wss << iter->AudioID;
        ws = wss.str();
		lvi.pszText = const_cast<LPWSTR>(ws.c_str());
		ListView_SetItem(debugList, &lvi);
		wss.str(L"");
		++i;

		lvi.iItem = i;
		lvi.iSubItem = 1;
        wss << iter->address_play;
        ws = wss.str();
		lvi.pszText = const_cast<LPWSTR>(ws.c_str());
		ListView_SetItem(debugList, &lvi);
		wss.str(L"");
		lvi.iSubItem = 2;
        if(iter->is_playing)
            lvi.pszText = const_cast<LPWSTR>(L"True");
		else
            lvi.pszText = const_cast<LPWSTR>(L"False");
		ListView_SetItem(debugList, &lvi);
		++i;
    }


    if(index != debugLastIndex || subtitles[index].AudioID != debugLastID)
    {
        debugLastIndex = index;
        debugLastID = subtitles[index].AudioID;

        ListView_DeleteAllItems(subtitleList);
        for(size_t i = 0; i < subtitles[index].dialog.size(); i++)
        {
            if (subtitles[index].AudioID == subtitles[index].dialog[i].ID)
            {
                for(size_t j = 0; j < subtitles[index].dialog[i].Text.size();j++)
                {
                    LVITEM lvi = {};
                    lvi.mask = LVIF_TEXT;
                    lvi.iItem = j;
                    lvi.pszText = const_cast<LPWSTR>(subtitles[index].dialog[i].Text[j].c_str());
                    ListView_InsertItem(subtitleList, &lvi);
                }
                break;
            }
        }
    }
}
