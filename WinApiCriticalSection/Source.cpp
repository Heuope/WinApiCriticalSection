#include <iostream>
#include <windows.h>
#include <memoryapi.h>
#include <string>
#include "process.h"

TCHAR name[] = TEXT("WinApiCriticalSection");

int main()
{
	HANDLE fileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CRITICAL_SECTION), name);

	if (fileMapping == NULL)
	{
		return -1;
	}

	auto criticalSection = (CRITICAL_SECTION*)MapViewOfFile(fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CRITICAL_SECTION));

	if (criticalSection == NULL)
	{
		return -2;
	}

	InitializeCriticalSection(criticalSection);



	DeleteCriticalSection(criticalSection);
	UnmapViewOfFile(criticalSection);
	CloseHandle(fileMapping);

	return 0;
}