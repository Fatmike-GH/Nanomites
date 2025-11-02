#define ZYDIS_STATIC_BUILD
#include <windows.h>
#include <map>
#include "Disassembler.h"

Disassembler::Disassembler()
{
}

Disassembler::~Disassembler()
{
}

bool Disassembler::GetRelativeJumps(PEFile& peFile, PIMAGE_SECTION_HEADER sectionHeader, std::vector<RelativeJump>& result)
{
  DWORD sectionOffset = sectionHeader->PointerToRawData;
  DWORD sectionLength = sectionHeader->SizeOfRawData;
  DWORD fileOffset = sectionOffset;
  do
  {
    ZydisDisassembledInstruction instructionInfo;
    GetInstruction(peFile, fileOffset, instructionInfo);
    if (IsRelativeJump(instructionInfo))
    {
      RelativeJump relativeJump;
      relativeJump.Rva = fileOffset - sectionOffset;
      relativeJump.Opcode = instructionInfo.info.opcode;
      relativeJump.OpcodeLength = instructionInfo.info.length;
      relativeJump.JmpLength = (DWORD)instructionInfo.info.raw.imm->value.u;

      result.push_back(relativeJump);
    }
    fileOffset += instructionInfo.info.length;
  } while (fileOffset < sectionOffset + sectionLength);

  return true;
}

bool Disassembler::GetCCs(PEFile& peFile, PIMAGE_SECTION_HEADER sectionHeader, std::set<DWORD>& rvas)
{
  DWORD sectionOffset = sectionHeader->PointerToRawData;
  DWORD sectionLength = sectionHeader->SizeOfRawData;
  DWORD fileOffset = sectionOffset;
  do
  {
    BYTE* offset = (BYTE*)(fileOffset + (DWORD_PTR)peFile.DOS_HEADER());
    if (*offset == 0xCC)
    {
      DWORD rva = fileOffset - sectionOffset;
      rvas.insert(rva);
    }
    fileOffset += 1;
  } while (fileOffset < sectionOffset + sectionLength);

  return true;
}

void Disassembler::GetInstruction(PEFile& peFile, DWORD_PTR offset, ZydisDisassembledInstruction& instructionInfo)
{
  offset = offset + (DWORD_PTR)peFile.DOS_HEADER();
#ifdef _WIN64
  ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, 0, (void*)offset, peFile.GetBufferSize() - offset, &instructionInfo);
#else
  ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_COMPAT_32, 0, (void*)offset, peFile.GetBufferSize() - offset, &instructionInfo);
#endif
}

bool Disassembler::IsRelativeJump(ZydisDisassembledInstruction& instructionInfo)
{
  // Check if the instruction is a jump (either conditional or unconditional).
  const auto category = instructionInfo.info.meta.category;
  bool isJump = (category == ZYDIS_CATEGORY_COND_BR) || (category == ZYDIS_CATEGORY_UNCOND_BR);

  // Check if the instruction uses relative addressing.
  bool isRelative = (instructionInfo.info.attributes & ZYDIS_ATTRIB_IS_RELATIVE);

  return isJump && isRelative;
}
