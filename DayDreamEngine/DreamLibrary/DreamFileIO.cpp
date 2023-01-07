#include "DreamFileIO.h"
#include <iostream>
#include <fstream>

#ifdef WINDOWS

static std::wifstream readFileStream;
static std::wofstream writeFileStream;

#endif

static std::wstring* line = nullptr;

bool DreamFileIO::OpenFileRead(const wchar_t* filePath)
{
	
	readFileStream = std::wifstream(filePath);
	bool fileOpened = readFileStream.is_open();

	if (!fileOpened) {
		printf("FILE DOES NOT EXIST");
	}
	else {
		line = new std::wstring();
	}

	return fileOpened;
}

void DreamFileIO::OpenFileWrite(const wchar_t* filePath, FileWriteType type)
{
	switch (type) {
	case FileWriteType::OverWrite: 
	{
		writeFileStream = std::wofstream(filePath);
		break;
	}
	case FileWriteType::AppendToEnd:
	{
		std::wstring fileContents = L"";
		if (DreamFileIO::OpenFileRead(filePath)) {
			std::wstring line;
			while (DreamFileIO::ReadLine(line)) {
				fileContents += line;
				fileContents += L"\n";
			}
			DreamFileIO::CloseFileRead();
		}

		writeFileStream = std::wofstream(filePath);
		writeFileStream << fileContents;
		break;
	}
	}
}

const bool DreamFileIO::ReadFullFileQuick(std::wstring& lineOut)
{
	if (readFileStream.is_open()) {
		readFileStream.seekg(0, std::ios::end);
		int lenght = (int)readFileStream.tellg();

		readFileStream.seekg(0, std::ios::beg);
		wchar_t* text = new wchar_t[lenght + 1];

		readFileStream.read(text, lenght);
		text[lenght] = '\0';

		lineOut = text;
		return true;
	}

	return false;
}

const bool DreamFileIO::ReadFullFile(std::wstring& lineOut)
{
	lineOut = L"";

	if (readFileStream.is_open()) {

		std::wstring text = L"";

		while (!readFileStream.eof()) {
			std::getline(readFileStream, text);
			lineOut.append(text + L"\n");
		}

		return true;
	}

	return false;
}

const bool DreamFileIO::ReadLine(std::wstring& lineOut)
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

void DreamFileIO::WriteLine(const wchar_t* lineToWrite)
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