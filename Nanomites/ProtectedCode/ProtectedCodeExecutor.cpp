#include <iostream>
#include "ProtectedCodeExecutor.h"
#include "Crc32.h"

#pragma optimize( "", off )

ProtectedCodeExecutor::ProtectedCodeExecutor()
{
}

ProtectedCodeExecutor::~ProtectedCodeExecutor()
{
} 

void ProtectedCodeExecutor::EnterText()
{
  std::cout << "Protected Code: Executing 'EnterText()'..." << std::endl;

  std::cout << "Please enter some text: ";
  std::cin >> _input;
}

DWORD ProtectedCodeExecutor::GetCrc32()
{
  std::cout << "Protected Code: Executing 'GetCrc32()'..." << std::endl;

  Crc32 crc32;
  DWORD result = crc32.Calculate(reinterpret_cast<const BYTE*>(_input.c_str()), static_cast<unsigned int>(_input.length()));
  return result;
}

#pragma optimize( "", on )