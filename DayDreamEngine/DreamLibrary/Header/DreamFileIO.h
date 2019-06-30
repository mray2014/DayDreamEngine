#pragma once
#include <pch.h>

enum FileType {
	_txt,
	_json
};
class DreamFileIO
{
public:
	static bool OpenFile(const char* filePath);
	static const char* ReadLine();
	static void CloseFile();
	static void WriteLine();
	

};

