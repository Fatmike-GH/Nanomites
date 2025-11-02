#include "Crc32.h"

#pragma optimize( "", off )

Crc32::Crc32()
{
}

Crc32::~Crc32()
{
}

DWORD Crc32::Calculate(const BYTE* data, unsigned int len)
{
  DWORD crc = 0xFFFFFFFF;

  for (unsigned int i = 0; i < len; i++)
  {
    BYTE ch = data[i];
    for (unsigned int j = 0; j < 8; j++)
    {
      DWORD b = (ch ^ crc) & 1;
      crc >>= 1;
      if (b)
      {
        crc = crc ^ 0xEDB88320;
      }
      ch >>= 1;
    }
  }
  return ~crc;
}

#pragma optimize( "", on )