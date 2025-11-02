#include "ResourceAdder.h"

ResourceAdder::ResourceAdder()
{
}

ResourceAdder::~ResourceAdder()
{
}

bool ResourceAdder::AddResource(const char* fileName, WORD resourceId, BYTE* buffer, DWORD bufferSize)
{
  HANDLE updateHandle = BeginUpdateResourceA(fileName, FALSE);
  BOOL result = UpdateResourceW(updateHandle, RT_RCDATA, MAKEINTRESOURCE(resourceId), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), buffer, bufferSize);
  EndUpdateResource(updateHandle, FALSE);
  if (!result)
  {
    EndUpdateResourceA(updateHandle, TRUE);
    return false;
  }
  if (!EndUpdateResourceA(updateHandle, FALSE))
  {
    return false;
  }
  return true;
}