#pragma once
#include <Windows.h>

class SectionInfo
{
public:
  SectionInfo();
  ~SectionInfo();

  void Clear();

  void SetSectionStart(DWORD_PTR address) { _sectionStart = address; }
  DWORD_PTR GetSectionStart() { return _sectionStart; }

  void SetSectionEnd(DWORD_PTR address) { _sectionEnd = address; }
  DWORD_PTR GetSectionEnd() { return _sectionEnd; }

  void SetSectionSize(SIZE_T size) { _sectionSize = size; }
  SIZE_T GetSectionSize() { return _sectionSize; }

private:
  DWORD_PTR _sectionStart;
  DWORD_PTR _sectionEnd;
  SIZE_T _sectionSize;
};