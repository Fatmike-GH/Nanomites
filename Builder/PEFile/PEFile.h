#pragma once
#include <windows.h>
#include <string>

class PEFile
{
public:
  PEFile();
  ~PEFile();

  bool OpenFile(const char* fileName);

  const PIMAGE_DOS_HEADER DOS_HEADER() { return _PIMAGE_DOS_HEADER; }
  const PIMAGE_NT_HEADERS NT_HEADERS() { return _PIMAGE_NT_HEADERS; }
  const PIMAGE_SECTION_HEADER SECTION_HEADER() { return _PIMAGE_SECTION_HEADER; }

  PIMAGE_SECTION_HEADER FindSectionByName(const std::string& sectionName);

  DWORD GetBufferSize() { return _bufferSize; }
  BYTE* GetBuffer() { return _buffer; }

private:
  void DeleteBuffer();
  void Initialize();

private:
  BYTE* _buffer;
  DWORD _bufferSize;

  PIMAGE_DOS_HEADER _PIMAGE_DOS_HEADER;
  PIMAGE_NT_HEADERS _PIMAGE_NT_HEADERS;
  PIMAGE_SECTION_HEADER _PIMAGE_SECTION_HEADER;
};