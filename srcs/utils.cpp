#include "MemSubLoader.hpp"

// Find address containing audio ID


// Open File Explorer
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



void Subtitles::findAddress(uintptr_t &address, int offset, HANDLE hProcess)
{
    SIZE_T bytesRead;
    if (ReadProcessMemory(hProcess, (LPCVOID)address, &address, 4, &bytesRead) )
        address += offset;
}

void Subtitles::search_memory(HANDLE hProcess)
{
    address_audio = bAddress_audio;
    address_play = bAddress_play;
    for(int i = 0; i < offset_audio.size();i++)
        findAddress(address_audio,offset_audio[i],hProcess);
    for(int i = 0; i < offset_play.size();i++)
        findAddress(address_play,offset_play[i],hProcess);

}
bool Subtitles::check_audio(HANDLE hProcess)
{
    SIZE_T bytesRead;
    if(!ReadProcessMemory(hProcess, (LPCVOID)address_play, &is_playing, 1, &bytesRead))
        std::cout<<"play error: "<<GetLastError()<<std::endl;
    if(!ReadProcessMemory(hProcess, (LPCVOID)address_audio, &AudioID, 4, &bytesRead))
        std::cout<<"audio error: "<<GetLastError()<<std::endl;
    if (is_playing)
    {
        if(AudioID != lastAudioID && AudioID > 0)
        {
        lastAudioID = AudioID;

        for(int i = 0; i < ID.size(); i++)
        {
            if(AudioID == ID[i])
            {
            SetWindowText(subtitles, Text[i].c_str());
            }
        }
        }
        return true;
    }
    else
        return false;
}

void Subtitles::file_memory(std::wifstream& file)
{
    int num;
    int offset;
    file >> bAddress_audio;

    file >> num;
    for(int i = 0; i<num;i++)
    {
        file >> offset;

        offset_audio.push_back(offset);
    }

    file >> bAddress_play;
    file >> num;

    for(int i = 0; i<num;i++)
    {
        file >> offset;

        offset_play.push_back(offset);
    }
}

void Subtitles::file_text(std::wifstream& file)
{
    std::wstring ws;
    int num;
    while(getline(file, ws))
    {
        if (ws == L"END")
            break;

        int num;
        std::wstring text;
        std::wistringstream iss(ws);
        iss >> num;
        std::getline(iss, text);

        ID.push_back(num);
        Text.push_back(text);
    }
}
