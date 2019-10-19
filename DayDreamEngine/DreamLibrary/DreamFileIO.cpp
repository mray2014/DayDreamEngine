#include "DreamFileIO.h"
#include <string>
#include <iostream>
#include <fstream>

#ifdef WINDOWS

static std::ifstream readFileStream;
static std::ofstream writeFileStream;

#endif

static std::string* line = nullptr;

bool DreamFileIO::OpenFileRead(const char * filePath)
{
	
	readFileStream = std::ifstream(filePath);
	bool fileOpened = readFileStream.is_open();

	if (!fileOpened) {
		printf("FILE DOES NOT EXIST");
	}
	else {
		line = new std::string();
	}

	return fileOpened;
}

void DreamFileIO::OpenFileWrite(const char * filePath, FileWriteType type)
{
	switch (type) {
	case FileWriteType::OverWrite: 
	{
		writeFileStream = std::ofstream(filePath);
		break;
	}
	case FileWriteType::AppendToEnd:
	{
		std::string fileContents = "";
		if (DreamFileIO::OpenFileRead(filePath)) {
			char* line;
			while (DreamFileIO::ReadLine(&line)) {
				fileContents += line;
				fileContents += "\n";
			}
			DreamFileIO::CloseFileRead();
		}

		writeFileStream = std::ofstream(filePath);
		writeFileStream << fileContents;
		break;
	}
	}
}

const bool DreamFileIO::ReadLine(char** lineOut)
{
	bool endOfFile = false;
	if (readFileStream.is_open()) {
		std::getline(readFileStream, *line);
	}
	*lineOut = &(*line)[0];
	return !readFileStream.eof();
}

void DreamFileIO::CloseFileRead()
{
	if (readFileStream.is_open()) {
		if (line != nullptr) { delete line; line = nullptr; }
		readFileStream.close();
	}
}
void DreamFileIO::CloseFileWrite()
{
	if (writeFileStream.is_open()) {
		writeFileStream.close();
	}
}

void DreamFileIO::WriteLine(const char* lineToWrite)
{
	if (writeFileStream.is_open()) {
		writeFileStream << lineToWrite << "\n";
	}
}
void DreamFileIO::Write(const char* lineToWrite)
{
	if (writeFileStream.is_open()) {
		writeFileStream << lineToWrite;
	}
}