#pragma once
#include <pch.h>

enum FileWriteType {
	OverWrite,
	AppendToEnd
};
class DreamFileIO
{
public:
	static bool OpenFileRead(const char* filePath);
	static void OpenFileWrite(const char* filePath, FileWriteType type);
	static const bool ReadFullFile(char** lineOut);
	static const bool ReadLine(char** lineOut);
	static void CloseFileRead();
	static void CloseFileWrite();
	static void WriteLine(const char* lineToWrite);
	static void Write(const char* lineToWrite);

};

