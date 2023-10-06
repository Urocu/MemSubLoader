#include "MemSubLoader.hpp"

// Find address containing audio ID
void findAddress(uintptr_t &address, int offset, HANDLE hProcess)
{
    SIZE_T bytesRead;
    if (ReadProcessMemory(hProcess, (LPCVOID)address, &address, 4, &bytesRead) )
            address += offset;
}

// Open File Explorer
bool OpenFileExplorer(HWND hwnd, wchar_t *filePath, int filePathSize, int button)
{
	OPENFILENAME ofn = {
		sizeof(OPENFILENAME)
	};

	ofn.hwndOwner = hwnd;
	if (button == GAME_BUTTON)
		ofn.lpstrFilter = L"Executable Files (*.exe)\0*.exe\0All Files (*.*)\0*.*\0";
	else
		ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = filePathSize;
	ofn.Flags = OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
		return true;
	else
		return false;
}
