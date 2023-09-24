#include "MemSubLoader.hpp"

bool findAddress(uintptr_t &address, int offset)
{
	SIZE_T bytesRead;
	int newAddress;

	if (ReadProcessMemory(pi.hProcess, (LPCVOID) address, &newAddress, sizeof(newAddress), &bytesRead))
	{
		address = newAddress;
		address += offset;
		return true;
	}
	return false;
}

bool OpenFileExplorer(HWND hwnd, wchar_t *filePath, int filePathSize, int button)
{
	OPENFILENAME ofn = {
		sizeof(OPENFILENAME)
	};

	ofn.hwndOwner = hwnd;
	if (button == GAME)
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
