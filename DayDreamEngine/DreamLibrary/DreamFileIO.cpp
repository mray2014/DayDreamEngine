#include "DreamFileIO.h"
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
			std::string line;
			while (DreamFileIO::ReadLine(line)) {
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

const bool DreamFileIO::ReadFullFileQuick(std::string& lineOut)
{
	if (readFileStream.is_open()) {
		readFileStream.seekg(0, std::ios::end);
		int lenght = (int)readFileStream.tellg();

		readFileStream.seekg(0, std::ios::beg);
		char* text = new char[lenght + 1];

		readFileStream.read(text, lenght);
		text[lenght] = '\0';

		lineOut = text;
		return true;
	}

	return false;
}

const bool DreamFileIO::ReadFullFile(std::string& lineOut)
{
	lineOut = "";

	if (readFileStream.is_open()) {

		std::string text = "";

		while (!readFileStream.eof()) {
			std::getline(readFileStream, text);
			lineOut.append(text + "\n");
		}

		return true;
	}

	return false;
}

const bool DreamFileIO::ReadLine(std::string& lineOut)
{
	bool endOfFile = false;
	if (readFileStream.is_open()) {
		std::getline(readFileStream, *line);
	}
	lineOut = *line;
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