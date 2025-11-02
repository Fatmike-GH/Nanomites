#pragma once
#include <set>
#include <vector>
#include "RelativeJump.h"
#include "..\PEFile\PEFile.h"
#include "..\Zydis\include\Zydis.h"

class Disassembler
{
public:
  Disassembler();
  ~Disassembler();

  bool GetRelativeJumps(PEFile& peFile, PIMAGE_SECTION_HEADER sectionHeader, std::vector<RelativeJump>& result);
  bool GetCCs(PEFile& peFile, PIMAGE_SECTION_HEADER sectionHeader, std::set<DWORD>& rvas);

private:
  void GetInstruction(PEFile& peFile, DWORD_PTR offset, ZydisDisassembledInstruction& instructionInfo);
  bool IsRelativeJump(ZydisDisassembledInstruction& instructionInfo);
};