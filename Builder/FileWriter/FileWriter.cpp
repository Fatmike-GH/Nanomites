#include "FileWriter.h"

FileWriter::FileWriter()
{
}

FileWriter::~FileWriter()
{
}

void FileWriter::OpenForWriteBin(const char* fileName, std::ofstream& fileStream)
{
  fileStream.open(fileName, std::ios::out | std::ios::binary);
}

void FileWriter::Write(std::ofstream& fileStream, BYTE* buffer, DWORD bufferSize)
{
  fileStream.write((char*)buffer, bufferSize);
}

void FileWriter::Close(std::ofstream& fileStream)
{
  fileStream.close();
}
