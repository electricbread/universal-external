#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>


HANDLE hProc = NULL;
DWORD procID;


bool attachProc(char* procName)
{
	PROCESSENTRY32 procEntry32;

	procEntry32.dwSize - sizeof(PROCESSENTRY32);

	auto hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcSnap == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed to take Snapshot! (Failed)" << std::endl;
		return false;
	}

	while (Process32Next(hProcSnap, &procEntry32))
	{
		std::cout << procEntry32.szExeFile << std::endl;

		if (!strcmp(procName, procEntry32.szExeFile))
		{
			std::cout << "Found Process" << procEntry32.szExeFile << " with ProcessID " << procEntry32.th32ProcessID << std::endl;
			hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
			procID = procEntry32.th32ProcessID;

			if (hProc == NULL)
			{
				std::cout << "Failed getting process." << std::endl;

			}
			CloseHandle(hProcSnap);
			return true;
		}
	}

	std::cout << "Couldn't find " << procName << " in the process snapshot." << std::endl;
	CloseHandle(hProcSnap);
	return false;

}


template <class dataType> 
void wpm(dataType valtowrite, DWORD addresstowrite)
{
	WriteProcessMemory(hProc, (PVOID)addresstowrite, &valtowrite, sizeof(dataType), 0);

}

template <class dataType>
dataType rpm(DWORD addresstoread)
{
	dataType rpmBuffer;
	ReadProcessMemory(hProc, (PVOID)addresstoread, &rpmBuffer, sizeof(dataType), 0);
	return rpmBuffer;
}

int main()
{
	std::cout << "Bread External" << std::endl;
// your offsets go here
	DWORD offset1 = 0x20664DF8;


	attachProc((char*)"Your EXE here");
	while (true)
	{

		wpm<int>(1337, offset1);
}	
}
