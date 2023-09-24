#include "MemSubLoader.hpp"

void game_start()
{
	SIZE_T bytesRead;
	int audID;
	uintptr_t baseaddress;
	uintptr_t addressToRead;
	std::vector<std::wstring > Text;
	std::wstring ws(filePath);
	std::string SfilePath(ws.begin(), ws.end());
	std::wifstream subfile;
	int num;
	int offset[6];

	subfile.open(SfilePath);
	if (subfile.is_open() && !subfile.eof())
	{
		subfile >> baseaddress;
		subfile >> num;
		for (int i = 0; i < num; i++)
			subfile >> offset[i];
		while (getline(subfile, ws))
		{
			Text.push_back(ws);
		}
	}
	else
		SetWindowText(subtitles, L"File failed to open");
	subfile.close();
	int lastID = 0;
	SetWindowText(subtitles, Text[0].c_str());
	while (WaitForSingleObject(pi.hProcess, 0) == WAIT_TIMEOUT)
	{
		if (ReadProcessMemory(pi.hProcess, (LPCVOID) addressToRead, &audID, sizeof(audID), &bytesRead) && lastID != audID && audID < Text.size() && audID > 0)
		{
			SetWindowText(subtitles, Text[audID].c_str());
			lastID = audID;
		}

		MSG msg = {};

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		addressToRead = baseaddress;
		for (int i = 0; i < num; i++)
			findAddress(addressToRead, offset[i]);
		DWORD Width = GetSystemMetrics(SM_CXSCREEN);
		DWORD Height = GetSystemMetrics(SM_CYSCREEN);
		SetWindowPos(subtitles, HWND_TOPMOST, 0, Height - 100, Width, 100, SWP_SHOWWINDOW);
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	DestroyWindow(subtitles);
}
