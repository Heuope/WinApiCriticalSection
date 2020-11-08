#include <iostream>
#include <windows.h>
#include <memoryapi.h>
#include <string>

TCHAR keyName[] = TEXT("WinApiCriticalSection");

int main()
{
	HANDLE fileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CRITICAL_SECTION), keyName);

	if (fileMapping == NULL)
	{
		return -1;
	}

	auto cs = (CRITICAL_SECTION*)MapViewOfFile(fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CRITICAL_SECTION));

	if (cs == NULL) 
	{
		CloseHandle(fileMapping);
		return -2;
	}

	int processId = GetCurrentProcessId();

	while (!TryEnterCriticalSection(cs))
	{
		Sleep(10);
	}

	std::cout << "Process " << processId << " entered" << std::endl;
	std::cout << "Process " << processId << " working hard" << std::endl;
	std::cout << "Process " << processId << " leave" << std::endl;

	LeaveCriticalSection(cs);
	
	UnmapViewOfFile(cs);
	CloseHandle(fileMapping);

	return 0;
}