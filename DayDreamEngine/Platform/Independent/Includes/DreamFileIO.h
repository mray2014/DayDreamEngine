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
	static bool OpenFileRead_W(const wchar_t* filePath, int mode = 1);
	static void OpenFileWrite_W(const wchar_t* filePath, FileWriteType type);
	static const bool ReadFullFileQuick_W(std::wstring& lineOut);
	static const bool ReadFullFile_W(std::wstring& lineOut);
	static const bool ReadLine_W(std::wstring& lineOut);
	static void WriteLine_W(const wchar_t* lineToWrite);
	static void Write_W(const wchar_t* lineToWrite);

	static void CloseFileRead_W();
	static void CloseFileWrite_W();

	static bool OpenFileRead(const char* filePath, int mode = 1);
	static void OpenFileWrite(const char* filePath, FileWriteType type);
	static const bool ReadFullFileQuick(std::string& lineOut);
	static const bool ReadFullFile(std::string& lineOut);
	static const bool ReadLine(std::string& lineOut);
	static void WriteLine(const char* lineToWrite);
	static void Write(const char* lineToWrite);

	static void CloseFileRead();
	static void CloseFileWrite();

};

