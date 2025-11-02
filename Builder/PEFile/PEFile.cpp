#include "PEFile.h"

PEFile::PEFile()
{
  _buffer = nullptr;
  _bufferSize = 0;
  _PIMAGE_DOS_HEADER = nullptr;
  _PIMAGE_NT_HEADERS = nullptr;
  _PIMAGE_SECTION_HEADER = nullptr;
}

PEFile::~PEFile()
{
  DeleteBuffer();
}

bool PEFile::OpenFile(const char* fileName)
{
  DeleteBuffer();
  HANDLE hFile = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE)
  {
    return false;
  }
  _bufferSize = GetFileSize(hFile, 0);
  if (_bufferSize == 0)
  {
    CloseHandle(hFile);
    return false;
  }
  _buffer = new BYTE[_bufferSize];
  if (_buffer == NULL)
  {
    CloseHandle(hFile);
    return false;
  }
  DWORD	bytesRead = 0;
  if (!ReadFile(hFile, _buffer, _bufferSize, &bytesRead, NULL)) return false;
  CloseHandle(hFile);

  Initialize();

  return true;
}

PIMAGE_SECTION_HEADER PEFile::FindSectionByName(const std::string& sectionName)
{
  WORD sectionCount = NT_HEADERS()->FileHeader.NumberOfSections;
  PIMAGE_SECTION_HEADER pSectionHeader = SECTION_HEADER();
  for (int i = 0; i < sectionCount; i++, pSectionHeader++)
  {
    const char* section = (char*)pSectionHeader->Name;
    if (sectionName.compare(section) == 0)
    {
      return pSectionHeader;
    }
  }
  return nullptr;
}

void PEFile::DeleteBuffer()
{
  if (_buffer != nullptr)
  {
    delete[] _buffer;
    _buffer = nullptr;
  }
}

void PEFile::Initialize()
{
  _PIMAGE_DOS_HEADER = (PIMAGE_DOS_HEADER)_buffer;
  _PIMAGE_NT_HEADERS = (PIMAGE_NT_HEADERS)(_buffer + _PIMAGE_DOS_HEADER->e_lfanew);
  _PIMAGE_SECTION_HEADER = IMAGE_FIRST_SECTION(_PIMAGE_NT_HEADERS);
}