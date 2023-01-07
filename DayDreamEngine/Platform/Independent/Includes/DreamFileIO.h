#pragma once
#include <pch.h>
#include <string>

enum FileWriteType {
	OverWrite,
	AppendToEnd
};
class DreamFileIO
{
public:
	static bool OpenFileRead(const wchar_t* filePath);
	static void OpenFileWrite(const wchar_t* filePath, FileWriteType type);
	static const bool ReadFullFileQuick(std::wstring& lineOut);
	static const bool ReadFullFile(std::wstring& lineOut);
	static const bool ReadLine(std::wstring& lineOut);
	static void CloseFileRead();
	static void CloseFileWrite();
	static void WriteLine(const wchar_t* lineToWrite);
	static void Write(const char* lineToWrite);

};

