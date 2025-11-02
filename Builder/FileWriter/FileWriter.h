#pragma once
#include <windows.h>
#include <fstream>

class FileWriter
{
public:
  FileWriter();
  ~FileWriter();

  void OpenForWriteBin(const char* fileName, std::ofstream& fileStream);
  void Write(std::ofstream& fileStream, BYTE* buffer, DWORD bufferSize);
  void Close(std::ofstream& fileStream);  
};
