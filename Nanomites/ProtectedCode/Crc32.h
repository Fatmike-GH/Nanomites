#pragma once
#include <windows.h>

class __declspec(code_seg(".nano")) Crc32
{
public:
  Crc32();
  ~Crc32();

  __declspec(noinline) DWORD Calculate(const BYTE* data, unsigned int len);
};
