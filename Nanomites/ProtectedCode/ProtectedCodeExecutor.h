#pragma once
#include <Windows.h>
#include <string>

class __declspec(code_seg(".nano")) ProtectedCodeExecutor
{
public:
  ProtectedCodeExecutor();
  ~ProtectedCodeExecutor();

  __declspec(noinline) void EnterText();
  __declspec(noinline) DWORD GetCrc32();

private:
  std::string _input;
};