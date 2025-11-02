#include <Windows.h>
#include "PEImage.h"

PEImage::PEImage(DWORD_PTR imageBase)
{
  _imageBase = imageBase;
  _PIMAGE_DOS_HEADER = (PIMAGE_DOS_HEADER)_imageBase;
  _PIMAGE_NT_HEADERS = (PIMAGE_NT_HEADERS)((BYTE*)_imageBase + _PIMAGE_DOS_HEADER->e_lfanew);
  _PIMAGE_SECTION_HEADER = IMAGE_FIRST_SECTION(_PIMAGE_NT_HEADERS);
}

PEImage::~PEImage()
{
}

DWORD_PTR PEImage::GetEntryPoint()
{
  DWORD_PTR entryPoint = _PIMAGE_NT_HEADERS->OptionalHeader.ImageBase + _PIMAGE_NT_HEADERS->OptionalHeader.AddressOfEntryPoint;
  return entryPoint;
}

const PIMAGE_SECTION_HEADER PEImage::FindSection(const char* sectionName)
{
  WORD sectionCount = _PIMAGE_NT_HEADERS->FileHeader.NumberOfSections;
  for (int i = 0; i < sectionCount; i++)
  {
    PIMAGE_SECTION_HEADER pSectionHeader = GetSectionHeader(i);
    const char* currentSectionName = (char*)pSectionHeader->Name;
    if (strncmp(currentSectionName, sectionName, IMAGE_SIZEOF_SHORT_NAME) == 0)
    {
      return pSectionHeader;
    }
  }
  return nullptr;
}

const PIMAGE_SECTION_HEADER PEImage::GetSectionHeader(WORD index)
{
  if (index > _PIMAGE_NT_HEADERS->FileHeader.NumberOfSections - 1) return nullptr;
  return &_PIMAGE_SECTION_HEADER[index];
}


