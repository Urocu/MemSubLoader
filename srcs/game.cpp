#include "MemSubLoader.hpp"

// Used when the game starts
void gameStart(PROCESS_INFORMATION pi)
{
    int num = subtitles.size();
	bool is = false;
    // set a 100ms timer
	SetTimer(NULL, 1, 100, nullptr);
    // process messages, otherwise the software will freeze
    MSG msg = { };
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        //
        if (msg.message == WM_TIMER)
        {
            for(int i = 0; i < num; i++)
                {
                    is = false;
                    subtitles[i].search_memory(pi.hProcess);
                    if (subtitles[i].check_audio(pi.hProcess))
                        is = true;
                }
                if (!is && textToDraw !=L"")
                {
                    textToDraw = L"";
                    invalidateWindow(subtitlesHWND);
                }
        }
        if(WaitForSingleObject( pi.hProcess, 0 ) != WAIT_TIMEOUT)
            break;
	}
    //Makes sure to close the app completely in case the main window is closed while playing the game
	if(!IsWindow(mainHWND))
        PostQuitMessage(0);
	KillTimer(NULL, 1);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	DestroyWindow(subtitlesHWND);
}
