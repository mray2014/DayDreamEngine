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
	static void WriteLine(const wchar_t* lineToWrite);
	static void Write(const wchar_t* lineToWrite);

	static bool OpenFileRead(const char* filePath);
	static void OpenFileWrite(const char* filePath, FileWriteType type);
	static const bool ReadFullFileQuick(std::string& lineOut);
	static const bool ReadFullFile(std::string& lineOut);
	static const bool ReadLine(std::string& lineOut);
	static void WriteLine(const char* lineToWrite);
	static void Write(const char* lineToWrite);

	static void CloseFileRead();
	static void CloseFileWrite();

};

