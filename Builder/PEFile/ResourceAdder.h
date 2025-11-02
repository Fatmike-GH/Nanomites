#pragma once
#include <Windows.h>

class ResourceAdder
{
public:
  ResourceAdder();
  ~ResourceAdder();

  bool AddResource(const char* fileName, WORD resourceId, BYTE* buffer, DWORD bufferSize);

};