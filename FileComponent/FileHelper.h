#pragma once


int isDirectory(const WIN32_FIND_DATA &fdFile);

int isDots(const std::wstring &strName);

void CharToByte(char* chars, byte* bytes, unsigned int count);

void ByteToChar(byte* bytes, char* chars, unsigned int count);