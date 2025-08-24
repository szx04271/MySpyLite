#include "pch.h"
#include "Utils.h"
#include "StyleListCtrl.h"
#include "Constants.h"
#include "MySpyLite.h"

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

std::vector<StyleMap::const_iterator> GetOtherStyles(const StyleMap& all, const CStyleListCtrl& occupied_list) {
	std::vector<StyleMap::const_iterator> ret;

	auto occupied_count = occupied_list.GetItemCount();
	bool occupied;
	int i;
	for (auto it = all.begin(); it != all.end(); ++it) {
		occupied = false;
		for (i = 0; i < occupied_count; ++i) {
			if (static_cast<DWORD>(occupied_list.GetItemData(i)) == it->first) {
				occupied = true;
				break;
			}
		}
		
		if (!occupied) {
			ret.emplace_back(it);
		}
	}

	return ret;
}

std::wstring GetTempFilePath() {
	WCHAR temp_path[MAX_PATH + 1];
	GetTempPathW(MAX_PATH + 1, temp_path);
	std::wstring ret(MAX_PATH, 0);
	GetTempFileNameW(temp_path, L"XXZ", 0, &ret[0]);
	return ret;
}

bool ReleaseWndProcFinderDll() {
	if (!theApp.m_wndProcFinderDllPath.empty()) {
		return true;
	}

	// load resouce dll
	auto hMod = AfxGetInstanceHandle();
	HRSRC hDllRsrc = FindResourceW(hMod, MAKEINTRESOURCEW(IDR_WNDPROC_FINDER), L"DLL");
	if (!hDllRsrc) {
		return false;
	}

	auto dll_size = SizeofResource(hMod, hDllRsrc);
	auto hMem = LoadResource(hMod, hDllRsrc);
	auto dll_data = static_cast<BYTE*>(LockResource(hMem));

	theApp.m_wndProcFinderDllPath = GetTempFilePath();
	HANDLE hDllFile = CreateFileW(theApp.m_wndProcFinderDllPath.c_str(), GENERIC_WRITE, FILE_SHARE_READ,
		nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hDllFile == INVALID_HANDLE_VALUE) {
		DeleteFileW(theApp.m_wndProcFinderDllPath.c_str());
		theApp.m_wndProcFinderDllPath.clear();
		return false;
	}

	DWORD junk;
	WriteFile(hDllFile, dll_data, dll_size, &junk, nullptr);
	CloseHandle(hDllFile);

	return true;
}

bool InjectDll(DWORD pid, const std::wstring& dll_path) {
	bool ret = false;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProcess) {
		auto mem_size = (dll_path.length() + 1) * 2;
		auto mem_addr = VirtualAllocEx(hProcess, nullptr, mem_size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (mem_addr) {
			bool written = WriteProcessMemory(hProcess, mem_addr, dll_path.c_str(), mem_size, nullptr);
			if (written) {
				auto hThread = CreateRemoteThread(hProcess, nullptr, 0,
					reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryW), mem_addr, 0, nullptr);
				if (hThread) {
					CloseHandle(hThread);
					ret = true;
				}
			}
			VirtualFree(mem_addr, 0, MEM_RELEASE);
		}
		CloseHandle(hProcess);
	}

	return ret;
}

int8_t Is64BitProcess(HANDLE hProcess) {
	constexpr bool isSpylite64Bit =
#ifdef _M_X64
		1
#else
		0
#endif
		;

	bool isOS64Bit;
	BOOL isWow64;
	BOOL success;
	if (isSpylite64Bit) {
		isOS64Bit = true;
	}
	else {
		success = IsWow64Process(GetCurrentProcess(), &isWow64);
		if (!success) {
			return -1;
		}
		else {
			isOS64Bit = isWow64 ? true : false;
		}
	}

	if (!isOS64Bit) {
		return 0;
	}
	else {
		success = IsWow64Process(hProcess, &isWow64);
		if (!success) {
			return -1;
		}
		else {
			return isWow64 ? 0 : 1;
		}
	}
}

