#include "pch.h"
#include "Utils.h"

std::wstring NtFilePathToDosPath(std::wstring nt_path) {
	WCHAR szDriveStr[MAX_PATH + 1] = { 0 };
	WCHAR szDeviceStr[MAX_PATH + 1] = { 0 };
	WCHAR szDrive[3] = { 0 };
	int cchDevName = 0;

	if (GetLogicalDriveStringsW(MAX_PATH, szDriveStr) == 0)
	{
		return L"";
	}
	for (int i = 0; szDriveStr[i]; i += 4)
	{
		wcsncpy(szDrive, szDriveStr + i, 2);
		if (!QueryDosDeviceW(szDrive, szDeviceStr, MAX_PATH))
		{
			return L"";
		}
		cchDevName = wcslen(szDeviceStr);
		if (wcsnicmp(szDeviceStr, nt_path.c_str(), cchDevName) == 0) //比较前缀
		{
			nt_path.replace(0, cchDevName, szDrive);
			return nt_path;
		}
	}
	return L"";
}