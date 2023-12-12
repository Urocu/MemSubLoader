#include "MemSubLoader.hpp"

// Used when the game starts
void gameStart(PROCESS_INFORMATION pi)
{
    int num = subtitles.size();
	bool is = false;
	const std::chrono::milliseconds frame_duration(1000 / 60);
	while (WaitForSingleObject( pi.hProcess, 0 ) == WAIT_TIMEOUT)
	{
	    auto start_time = std::chrono::high_resolution_clock::now();
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
		// process messages, otherwise the software will freeze
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		MSG msg = { };
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) || elapsed_time < frame_duration)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			end_time = std::chrono::high_resolution_clock::now();
            elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
            std::this_thread::sleep_for((std::chrono::milliseconds)10);
		}
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	DestroyWindow(subtitlesHWND);
}
