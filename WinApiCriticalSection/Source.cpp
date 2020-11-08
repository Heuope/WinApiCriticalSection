#include <iostream>
#include <windows.h>
#include <memoryapi.h>
#include <string>

#define MAX_PROCESSORS 10

TCHAR keyName[] = TEXT("WinApiCriticalSection");

std::wstring GetExePath() 
{
	TCHAR buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos);
}

int main()
{
	HANDLE fileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CRITICAL_SECTION), keyName);

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

	STARTUPINFO sturtupInfos[MAX_PROCESSORS];
	PROCESS_INFORMATION processInformations[MAX_PROCESSORS];

	for (int i = 0; i < MAX_PROCESSORS; i++)
	{
		ZeroMemory(&sturtupInfos[i], sizeof(STARTUPINFO));
		ZeroMemory(&processInformations[i], sizeof(PROCESS_INFORMATION));
		sturtupInfos[i].cb = sizeof(STARTUPINFO);

		if (!CreateProcess(std::wstring(GetExePath() + L"\\Process.exe").c_str(), NULL, 0, 0, TRUE, 0, 0, NULL, &sturtupInfos[i], &processInformations[i]))
		{
			std::cout << "Failed to create process Error:" << GetLastError() << std::endl;
		}
	}

	for (int i = 0; i < MAX_PROCESSORS; i++) 
	{
		DWORD res = WaitForSingleObject(processInformations[i].hProcess, INFINITE);
		CloseHandle(processInformations[i].hProcess);
		CloseHandle(processInformations[i].hThread);
	}

	DeleteCriticalSection(criticalSection);
	UnmapViewOfFile(criticalSection);
	CloseHandle(fileMapping);

	return 0;
}