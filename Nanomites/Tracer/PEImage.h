#pragma once
#include <windows.h>

class PEImage
{
public:
  PEImage(DWORD_PTR imageBase);
  ~PEImage();

  DWORD_PTR GetImageBase() { return _imageBase; }
  DWORD_PTR GetEntryPoint();

  const PIMAGE_DOS_HEADER DOS_HEADER() { return _PIMAGE_DOS_HEADER; }
  const PIMAGE_NT_HEADERS NT_HEADERS() { return _PIMAGE_NT_HEADERS; }
  const PIMAGE_SECTION_HEADER SECTION_HEADER() { return _PIMAGE_SECTION_HEADER; }
  const PIMAGE_SECTION_HEADER FindSection(const char* sectionName);
  const PIMAGE_SECTION_HEADER GetSectionHeader(WORD index);

private:
  DWORD_PTR _imageBase;
  PIMAGE_DOS_HEADER _PIMAGE_DOS_HEADER;
  PIMAGE_NT_HEADERS _PIMAGE_NT_HEADERS;
  PIMAGE_SECTION_HEADER _PIMAGE_SECTION_HEADER;
};
