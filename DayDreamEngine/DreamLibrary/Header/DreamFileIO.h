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
	static bool OpenFileRead(const char* filePath);
	static void OpenFileWrite(const char* filePath, FileWriteType type);
	static const bool ReadFullFileQuick(std::string& lineOut);
	static const bool ReadFullFile(std::string& lineOut);
	static const bool ReadLine(std::string& lineOut);
	static void CloseFileRead();
	static void CloseFileWrite();
	static void WriteLine(const char* lineToWrite);
	static void Write(const char* lineToWrite);

};

