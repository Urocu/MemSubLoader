#include "MemSubLoader.hpp"

// Initialize subtitles windows then start the game
void startGame(HWND hwnd)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Opens the game
	CreateProcess(gamePath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	WaitForInputIdle(pi.hProcess, INFINITE);

	if (createSubtitlesWindow())
	{
		MessageBox(hwnd, L"Error: Failed to initialize subtitles window", L"Window initialization", MB_ICONERROR);
	}

	PostMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	scanGame(pi);
}

// Where the magic happens, scans specific memory addresses for ID and bool values and draws subtitles accordingly
void scanGame(PROCESS_INFORMATION pi)
{
    int num = subtitles.size();
    unsigned int currentTimer = 1;
	bool is = false;

	isGameOpened = true;

    // set a 100ms timer
	SetTimer(mainHWND, 1, 100, NULL);
    // process messages, otherwise the software will freeze
    MSG msg = { };
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (msg.message == WM_TIMER)
        {
            switch(msg.wParam)
            {
            case 1: // If the subtitles aren't timed
                is = false;
                for(int i = 0; i < num; i++)
                {
                    subtitles[i].searchMemory(pi.hProcess);
                    if (subtitles[i].checkAudio(pi.hProcess, i))
                        is = true;
                }
                if (!is && textToDraw !=L"")
                {
                    textToDraw = L"";
                    invalidateWindow(subtitlesHWND);
                }
                updateDebugWindowAttributes();
                break;
            case 2: // If the subtitles are timed
                if(subtitles[sub].dialog[subID].Timer.size() > currentTimer && subtitles[sub].dialog[subID].Timer[currentTimer] > subtitles[sub].dialog[subID].Timer[currentTimer-1])
                {
                    textToDraw = subtitles[sub].dialog[subID].Text[currentTimer-1];
                    invalidateWindow(subtitlesHWND);
                    SetTimer(mainHWND,2,subtitles[sub].dialog[subID].Timer[currentTimer] - subtitles[sub].dialog[subID].Timer[currentTimer-1],NULL);
                    currentTimer++;
                }
                else // End of timed subtitles
                {
                    textToDraw = subtitles[sub].dialog[subID].Text[currentTimer-1];
                    invalidateWindow(subtitlesHWND);
                    KillTimer(mainHWND,2);
                    SetTimer(mainHWND, 1, 100, NULL);
                    currentTimer = 1;
                }
                updateDebugWindowAttributes();
                break;
            }

        }
        if(WaitForSingleObject( pi.hProcess, 0 ) != WAIT_TIMEOUT)
            break;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
	}
    //Makes sure to close the app completely in case the main window is closed while playing the game
	if(!IsWindow(mainHWND))
        PostQuitMessage(0);
	KillTimer(mainHWND, 1);
	KillTimer(mainHWND, 2);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	DestroyWindow(subtitlesHWND);
	isGameOpened = false;

	if (isTrayVisible)
	{
	    PostMessage(mainHWND, WM_SYSCOMMAND, SC_RESTORE, 0);
		ShowWindow(mainHWND, SW_SHOW); // Restore window
	}
}
