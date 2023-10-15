#include "MemSubLoader.hpp"

// Used when the game starts
void gameStart(PROCESS_INFORMATION pi)
{
	SIZE_T bytesRead;
	int audID;
	uintptr_t baseaddress;
	uintptr_t addressToRead;
	std::vector <std::wstring> Text;
	std::wstring ws( config.subtitlesPath );
	std::string SfilePath( ws.begin(), ws.end() );
	std::wifstream subfile;

	subfile.open(SfilePath); // Opens file containing base address, offsets and text for subtitles
	int num;
	int offset[6];
	if(subfile.is_open() && !subfile.eof())
	{
		subfile>>baseaddress;
		subfile>>num;
		for(int i = 0; i<num;i++)
		   subfile>>offset[i];
		while(getline(subfile, ws))
		{
			Text.push_back(ws);
		}
	}
	else
		SetWindowText(subtitles, L"File failed to open");
	subfile.close();

	int lastID = 0;
	DWORD Width = 0;
	DWORD Height = 0;

	// GDI+ Init
	HDC hdc = GetDC(subtitlesWin);

	while (WaitForSingleObject( pi.hProcess, 0 ) == WAIT_TIMEOUT)
	{
		// Read memory of audioID's address
		if (ReadProcessMemory(pi.hProcess, (LPCVOID)addressToRead, &audID, sizeof(audID), &bytesRead) && lastID != audID && audID < Text.size() && audID > 0)
		{
			textToDraw = Text[audID].c_str();
			InvalidateRect(subtitlesWin, NULL, FALSE);

			lastID=audID;
		}
		// process messages, otherwise the software will freeze
		MSG msg = { };
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		addressToRead = baseaddress;
		for( int i = 0; i < num; i++)
			findAddress(addressToRead, offset[i], pi.hProcess);
		// If width and height of the screen changed, resize and reposition the subtitles window
		if(Width != GetSystemMetrics(SM_CXSCREEN) && Height != GetSystemMetrics(SM_CYSCREEN))
		{
			Width = GetSystemMetrics(SM_CXSCREEN);
			Height = GetSystemMetrics(SM_CYSCREEN);
			SetWindowPos(subtitlesWin, HWND_TOPMOST, 0, Height-100, Width, 100, 0);
			SetWindowPos(subtitles, 0, 0, 0, Width, 100, 0);
		}

	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	DestroyWindow(subtitlesWin);
	DestroyWindow(settingsHWND);
}
