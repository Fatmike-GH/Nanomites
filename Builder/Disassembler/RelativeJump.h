#pragma once
#include <Windows.h>

struct RelativeJump
{
  DWORD Rva;
  DWORD Opcode;
  DWORD OpcodeLength;
  DWORD JmpLength;
};
