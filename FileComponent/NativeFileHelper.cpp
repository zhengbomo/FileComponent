#include "pch.h"
#include "NativeFileHelper.h"
#include "FileHelper.h"
#include "StringHelper.h"

#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <fstream>
#include <stdio.h>  
#include <stdlib.h>  

using namespace FileComponent;
using namespace Platform;
using namespace Windows::Storage;


using namespace Platform;

/** 写文件0=失败，1=成功，2=写入长度不正确 */
int writeFile(Platform::String^ strPath, char *data, int length) {
	HANDLE hFile;

	char *DataBuffer = data;

	DWORD dwBytesToWrite = length;
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag = FALSE;

	hFile = CreateFile2(strPath->Data(),
		GENERIC_WRITE,          
		FILE_SHARE_READ,		
		CREATE_ALWAYS,			
		NULL);                  

	int result = 0;
	if (hFile != INVALID_HANDLE_VALUE) {
		bErrorFlag = WriteFile(
			hFile,          
			DataBuffer,		
			dwBytesToWrite, 
			&dwBytesWritten,
			NULL);          

		if (bErrorFlag != FALSE)
		{
			if (dwBytesWritten != dwBytesToWrite)
			{
				result = 2;
				printf("Error: dwBytesWritten != dwBytesToWrite\n");
			}
			else
			{
				result = 1;
			}
		}
	}
	CloseHandle(hFile);
	return result;
}

Platform::Array<byte>^ readFile(Platform::String ^strPath)
{
	HANDLE pfile = CreateFile2(strPath->Data(),                // name of the write
		GENERIC_READ,          // open for writing
		FILE_SHARE_READ,		// do not share
		OPEN_EXISTING,			// default security
		NULL);                  // no attr. template

	LARGE_INTEGER  fileSize;
	GetFileSizeEx(pfile, &fileSize);
	int64 size = fileSize.QuadPart;
	char* buffer = new char[size]; // 最后一位为 '/0',C-Style 字符串的结束符

	DWORD readsize;

	ReadFile(pfile, buffer, size, &readsize, NULL);
	CloseHandle(pfile);

	byte *byteData = (byte *)buffer;
	Platform::Array<byte>^ result = ref new Platform::Array<byte>(byteData, size);

	delete[] buffer;
	return result;
}

unsigned __int64 getFolderSize(std::wstring path, std::wstring mask) {
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;
	std::wstring spec;
	std::stack<std::wstring> directories;

	directories.push(path);

	unsigned __int64 result = 0;

	while (!directories.empty()) {
		path = directories.top();
		spec = path + L"\\" + mask;
		directories.pop();

		hFind = FindFirstFileEx(spec.c_str(), FindExInfoStandard, &ffd, FINDEX_SEARCH_OPS::FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH);
		if (hFind == INVALID_HANDLE_VALUE) {
			return result;
		}
		
		do {
			if (wcscmp(ffd.cFileName, L".") != 0 &&
				wcscmp(ffd.cFileName, L"..") != 0) {
				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					directories.push(path + L"\\" + ffd.cFileName);
				}
				else {
					//This is file name
					result += ffd.nFileSizeLow + (ffd.nFileSizeHigh * MAXDWORD);
					//files.push_back(path + "\\" + ffd.cFileName);
				}
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		if (GetLastError() != ERROR_NO_MORE_FILES) {
			FindClose(hFind);
			return result;
		}

		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	}

	return result;
}


int NativeFileHelper::FileExists(Platform::String^ pathOfFolder)
{
	DWORD ftyp = GetFileAttributesW(pathOfFolder->Data());
	if (ftyp != INVALID_FILE_ATTRIBUTES && !(ftyp & FILE_ATTRIBUTE_DIRECTORY))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int NativeFileHelper::DirectoryExists(Platform::String^ pathOfFolder)
{
	DWORD ftyp = GetFileAttributesW(pathOfFolder->Data());
	if (ftyp != INVALID_FILE_ATTRIBUTES && (ftyp & FILE_ATTRIBUTE_DIRECTORY))
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

std::wstring getDirectory(std::wstring path)
{
	return path.substr(0, path.find_last_of(L"/\\"));
}

int NativeFileHelper::RemoveFile(Platform::String^ pathOfFile)
{
	int result = ::DeleteFile(pathOfFile->Data());
	if (result != 0) {
		return 1;
	}
	else {
		return 0;
	}
}

uint64 NativeFileHelper::GetFolderSize(Platform::String^ pathOfFolder)
{
	return getFolderSize(pathOfFolder->Data(), L"*");
}

uint64 NativeFileHelper::GetFileSize(Platform::String^ pathOfFile)
{
	HANDLE pfile = CreateFile2(pathOfFile->Data(),                // name of the write
		GENERIC_READ,          // open for writing
		FILE_SHARE_READ,		// do not share
		OPEN_EXISTING,			// default security
		NULL);                  // no attr. template

	LARGE_INTEGER  fileSize;
	GetFileSizeEx(pfile, &fileSize);
	return fileSize.QuadPart;
}

int removeDirectory(const std::wstring &strDir)
{
	WIN32_FIND_DATA fdFile;
	::memset(&fdFile, 0, sizeof(fdFile));

	HANDLE hFind = INVALID_HANDLE_VALUE;

	std::wstring strSearch = strDir + L"\\*.*";
	hFind = ::FindFirstFile(strSearch.data(), &fdFile);

	if (hFind == INVALID_HANDLE_VALUE) {
		return 0;
	}

	do {
		std::wstring strDelete = strDir + L"\\" + fdFile.cFileName;

		if (isDirectory(fdFile) == TRUE) {
			if (isDots(fdFile.cFileName) == TRUE) {
				continue;
			}
			removeDirectory(strDelete);
		}
		else {

			::DeleteFile(strDelete.data());
		}

	} while (::FindNextFile(hFind, &fdFile) == TRUE);

	::FindClose(hFind);
	::RemoveDirectory(strDir.data());

	return 1;
}


int NativeFileHelper::RemoveDirectory(Platform::String^ pathOfFolder)
{
	return removeDirectory(pathOfFolder->Data());
}


/** 写文件，返回值：0=失败，1=成功，2=写入长度不正确 */
int NativeFileHelper::WriteFile(Platform::String^ pathOfFolder, const Platform::Array<byte>^ data)
{
	//如果目录不存在则创建
	const std::wstring directory = getDirectory(pathOfFolder->Data());
	CreateDirectoryW(directory.c_str(), NULL);

	uint32 length = data->Length;
	unsigned char* content = data->Data;
	char *charContent = (char *)content;

	return writeFile(pathOfFolder, charContent, length);
}


Platform::Array<byte>^ NativeFileHelper::ReadFile(Platform::String^ pathOfFile)
{
	return readFile(pathOfFile);
}


/** 获取文件夹子项目：1=当前目录文件，2=当前目录文件夹，3=当前目录所有子文件（含子文件夹） */
Platform::Array<Platform::String^>^ getSubItems(Platform::String^ pathOfFolder, BOOL isDirectory)
{
	std::wstring path = pathOfFolder->Data();
	
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;
	std::wstring spec;

	Platform::Collections::Vector<Platform::String^>^ items = ref new  Platform::Collections::Vector<Platform::String^>();

	unsigned __int64 result = 0;

	spec = path + L"\\*";

	hFind = FindFirstFileEx(spec.c_str(), FindExInfoStandard, &ffd, FINDEX_SEARCH_OPS::FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH);
	if (hFind == INVALID_HANDLE_VALUE) {
		return ref new Platform::Array<Platform::String^>(0);
	}

	do {
		if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0) {
			if (isDirectory == TRUE) {
				//文件夹
				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					items->Append(ref new Platform::String((path + L"\\" + ffd.cFileName).c_str()));
				}
			}
			else  if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				//文件
				items->Append(ref new Platform::String((path + L"\\" + ffd.cFileName).c_str()));
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	if (GetLastError() == ERROR_NO_MORE_FILES) {
		FindClose(hFind);

		Platform::Array<Platform::String^>^ array = ref new Platform::Array<Platform::String^>(items->Size);
		int i = 0;
		for each (Platform::String^ item in items)
		{
			array[i] = item;
			i++;
		}
		return array;
	}
	else {
		FindClose(hFind);
		return ref new Platform::Array<Platform::String^>(0);
	}
}

Platform::Array<Platform::String^>^ NativeFileHelper::GetSubFiles(Platform::String^ pathOfFolder)
{
	return getSubItems(pathOfFolder, FALSE);
}

Platform::Array<Platform::String^>^ NativeFileHelper::GetSubDirectories(Platform::String^ pathOfFolder)
{
	return getSubItems(pathOfFolder, TRUE);
}

int NativeFileHelper::RenameFile(Platform::String^ source, Platform::String^ dest)
{
	return MoveFileExW(source->Data(),
		dest->Data(), NULL);
}

int NativeFileHelper::RenameFolder(Platform::String^ source, Platform::String^ dest)
{
	return MoveFileExW(source->Data(),
		dest->Data(), NULL);
}

Platform::Array<Platform::String^>^ NativeFileHelper::GetAllFiles(Platform::String^ pathOfFolder)
{
	std::wstring path = pathOfFolder->Data();

	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;
	std::wstring spec;
	std::stack<std::wstring> directories;

	Platform::Collections::Vector<Platform::String^>^ files = ref new  Platform::Collections::Vector<Platform::String^>();


	directories.push(path);

	unsigned __int64 result = 0;

	while (!directories.empty()) {
		path = directories.top();
		spec = path + L"\\*";
		directories.pop();

		hFind = FindFirstFileEx(spec.c_str(), FindExInfoStandard, &ffd, FINDEX_SEARCH_OPS::FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH);
		if (hFind == INVALID_HANDLE_VALUE) {
			return ref new Platform::Array<Platform::String^>(0);
		}

		do {
			if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0) {
				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					directories.push(path + L"\\" + ffd.cFileName);
				}
				else {
					files->Append(ref new Platform::String((path + L"\\" + ffd.cFileName).c_str()));
				}
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		if (GetLastError() != ERROR_NO_MORE_FILES) {
			FindClose(hFind);
		}

		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	}

	Platform::Array<Platform::String^>^ array = ref new Platform::Array<Platform::String^>(files->Size);
	int i = 0;
	for each (Platform::String^ item in files)
	{
		array[i] = item;
		i++;
	}
	return array;
}