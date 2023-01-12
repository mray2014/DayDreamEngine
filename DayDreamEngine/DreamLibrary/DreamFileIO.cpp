#include "DreamFileIO.h"
#include <iostream>
#include <fstream>
#include <vector>

#ifdef WINDOWS

static std::wifstream readFileStream_W;
static std::wofstream writeFileStream_W;

static std::ifstream readFileStream;
static std::ofstream writeFileStream;

#endif

static std::wstring* line_W = nullptr;
static std::string* line = nullptr;

bool DreamFileIO::OpenFileRead_W(const wchar_t* filePath, int mode)
{
	readFileStream_W = std::wifstream(filePath, mode);
	bool fileOpened = readFileStream_W.is_open();

	if (!fileOpened) {
		printf("FILE DOES NOT EXIST");
	}
	else {
		line_W = new std::wstring();
	}

	return fileOpened;
}

void DreamFileIO::OpenFileWrite_W(const wchar_t* filePath, FileWriteType type)
{
	switch (type) {
	case FileWriteType::OverWrite: 
	{
		writeFileStream_W = std::wofstream(filePath);
		break;
	}
	case FileWriteType::AppendToEnd:
	{
		std::wstring fileContents = L"";
		if (DreamFileIO::OpenFileRead_W(filePath)) {
			std::wstring newLine;
			while (DreamFileIO::ReadLine_W(newLine)) {
				fileContents += newLine;
				fileContents += L"\n";
			}
			DreamFileIO::CloseFileRead();
		}

		writeFileStream_W = std::wofstream(filePath);
		writeFileStream_W << fileContents;
		break;
	}
	}
}

const bool DreamFileIO::ReadFullFileQuick_W(std::wstring& lineOut)
{
	if (readFileStream_W.is_open()) {
		readFileStream_W.seekg(0, std::ios::end);
		int lenght = (int)readFileStream_W.tellg();

		readFileStream_W.seekg(0, std::ios::beg);
		wchar_t* text = new wchar_t[lenght + 1];

		readFileStream_W.read(text, lenght);
		text[lenght] = '\0';

		lineOut = text;
		return true;
	}

	return false;
}

const bool DreamFileIO::ReadFullFile_W(std::wstring& lineOut)
{
	lineOut = L"";

	if (readFileStream_W.is_open()) {

		std::wstring text = L"";

		while (!readFileStream_W.eof()) {
			std::getline(readFileStream_W, text);
			lineOut.append(text + L"\n");
		}

		return true;
	}

	return false;
}

const bool DreamFileIO::ReadLine_W(std::wstring& lineOut)
{
	bool endOfFile = false;
	if (readFileStream_W.is_open()) {
		std::getline(readFileStream_W, *line_W);
	}
	lineOut = *line_W;
	return !readFileStream_W.eof();
}

void DreamFileIO::CloseFileRead_W()
{
	if (readFileStream_W.is_open()) {
		if (line_W != nullptr) { delete line_W; line_W = nullptr; }
		readFileStream_W.close();
	}
}

void DreamFileIO::CloseFileWrite_W()
{
	if (writeFileStream_W.is_open()) {
		writeFileStream_W.close();
	}
}

void DreamFileIO::WriteLine_W(const wchar_t* lineToWrite)
{
	if (writeFileStream_W.is_open()) {
		writeFileStream_W << lineToWrite << "\n";
	}
}

void DreamFileIO::Write_W(const wchar_t* lineToWrite)
{
	if (writeFileStream_W.is_open()) {
		writeFileStream_W << lineToWrite;
	}
}



bool DreamFileIO::OpenFileRead(const char* filePath, int mode)
{
	readFileStream = std::ifstream(filePath, mode);
	bool fileOpened = readFileStream.is_open();

	if (!fileOpened) {
		printf("FILE DOES NOT EXIST");
	}
	else {
		line = new std::string();
	}

	return fileOpened;
}

void DreamFileIO::OpenFileWrite(const char* filePath, FileWriteType type)
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

const bool DreamFileIO::ReadFullFileQuick(char** lineOut, size_t& lengthOut)
{
	if (readFileStream.is_open()) {

		readFileStream.seekg(0, std::ios::end);		
		int lenght = (int)readFileStream.tellg();

		readFileStream.seekg(0, std::ios::beg);
		*lineOut = new char[lenght];

		readFileStream.read(*lineOut, lenght);
		(*lineOut)[lenght] = '\0';

		lengthOut = lenght;

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
