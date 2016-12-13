#include "pch.h"
#include "FileHelper.h"

int isDirectory(const WIN32_FIND_DATA &fdFile)
{
	return (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

int isDots(const std::wstring &strName)
{
	return strName == L"." || strName == L"..";
}

void CharToByte(char* chars, byte* bytes, unsigned int count) 
{
	for (unsigned int i = 0; i < count; i++)
		bytes[i] = (byte)chars[i];
}

void ByteToChar(byte* bytes, char* chars, unsigned int count) 
{
	for (unsigned int i = 0; i < count; i++)
		chars[i] = (char)bytes[i];
}