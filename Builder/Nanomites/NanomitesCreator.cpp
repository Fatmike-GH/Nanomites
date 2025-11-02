#include <time.h>
#include <algorithm>
#include "NanomitesCreator.h"
#include "..\Disassembler\Disassembler.h"

NanomitesCreator::NanomitesCreator()
{
  srand((int)time(NULL));
}

NanomitesCreator::~NanomitesCreator()
{
}

NanomiteMetadata* NanomitesCreator::Create(PEFile& peFile, PIMAGE_SECTION_HEADER sectionHeader)
{
  Disassembler disasm;

  // Get all 0xCC bytes of section to use them as fake nanomites
  std::set<DWORD> fakeNanomiteRVAs;
  disasm.GetCCs(peFile, sectionHeader, fakeNanomiteRVAs);

  // Find all real relative jumps of section
  std::vector<RelativeJump> relativeJumps;
  disasm.GetRelativeJumps(peFile, sectionHeader, relativeJumps);

  // Process jumps into a single list
  std::vector<Nanomite> nanomites;
  ProcessRealJumps(peFile, sectionHeader, relativeJumps, nanomites);
  ProcessFakeJumps(sectionHeader, fakeNanomiteRVAs, nanomites);

  // Sort by rva
  SortNanomitesByRva(nanomites);

  // Create the final metadata output structure, which will be written into the resource section of the target executable
  return CreateMetadata(sectionHeader, nanomites);
}

void NanomitesCreator::ProcessRealJumps(PEFile& peFile, PIMAGE_SECTION_HEADER sectionHeader, const std::vector<RelativeJump>& relativeJumps, std::vector<Nanomite>& outNanomites)
{
  for (const auto& jump : relativeJumps)
  {
    JumpType jumpType = ToJumpType(jump.Opcode);
    if (jumpType == JumpType::UNKNOWN) continue;

    Nanomite nanomite;
    nanomite.Rva = jump.Rva;
    nanomite.JumpType = static_cast<DWORD>(jumpType);
    nanomite.JumpLength = jump.JmpLength;
    nanomite.OpcodeLength = jump.OpcodeLength;

    // Patch the file with 0xCC + random bytes
    WriteNanomite(peFile, sectionHeader, nanomite);

    nanomite.Rva += sectionHeader->VirtualAddress; // Make RVA relative to ImageBase
    outNanomites.push_back(nanomite);
  }
}

void NanomitesCreator::ProcessFakeJumps(PIMAGE_SECTION_HEADER sectionHeader, const std::set<DWORD>& fakeNanomiteRVAs, std::vector<Nanomite>& outNanomites) const
{
  for (DWORD rva : fakeNanomiteRVAs)
  {
    Nanomite fake;
    fake.Rva = rva + sectionHeader->VirtualAddress; // Make RVA relative to ImageBase
    fake.JumpType = static_cast<DWORD>(ToJumpType(GetRandomShortJump()));
    fake.JumpLength = GetRandomByte(0x02, 0xA0);
    fake.OpcodeLength = 2;

    outNanomites.push_back(fake);
  }
}

void NanomitesCreator::SortNanomitesByRva(std::vector<Nanomite>& nanomites) const
{
  std::sort(nanomites.begin(), nanomites.end(), [](const Nanomite& r1, const Nanomite& r2) -> bool
  {
    return r1.Rva < r2.Rva;
  });
}

NanomiteMetadata* NanomitesCreator::CreateMetadata(PIMAGE_SECTION_HEADER sectionHeader, const std::vector<Nanomite>& nanomites) const
{
  NanomiteMetadata* result = new NanomiteMetadata();
  result->ItemCount = (DWORD)nanomites.size();

  // Allocate the array in the result struct
  result->Nanomites = new Nanomite[result->ItemCount];

  std::copy(nanomites.begin(), nanomites.end(), result->Nanomites);

  return result;
}

void NanomitesCreator::WriteNanomite(PEFile& peFile, PIMAGE_SECTION_HEADER sectionHeader, Nanomite& nanomite)
{
  BYTE* peFileBuffer = peFile.GetBuffer();

  const DWORD sectionOffset = sectionHeader->PointerToRawData;
  DWORD offset = nanomite.Rva + sectionOffset;
  BYTE* fileOffset = peFileBuffer + offset;

  *fileOffset = 0xCC;

  // Fill the remaining bytes with random data
  for (unsigned int i = 1; i < nanomite.OpcodeLength; ++i)
  {
    fileOffset[i] = GetRandomByte();
  }
}

BYTE NanomitesCreator::GetRandomShortJump() const
{
  // 0x70 (JO_S) to 0x7F (JG_S)
  return GetRandomByte(0x70, 0x7F);
}

BYTE NanomitesCreator::GetRandomByte() const
{
  const int min = 0;
  const int max = 255;
  return GetRandomByte(0, 255);
}

BYTE NanomitesCreator::GetRandomByte(const int min, const int max) const
{
  BYTE result = (BYTE)(((double)rand() / RAND_MAX) * (max - min) + min);
  return result;
}

JumpType NanomitesCreator::ToJumpType(DWORD opcode) const
{
  switch (opcode)
  {
    // Conditional Jumps
  case 0x70: case 0x80: return JumpType::JO;
  case 0x71: case 0x81: return JumpType::JNO;
  case 0x72: case 0x82: return JumpType::JB;
  case 0x73: case 0x83: return JumpType::JNB;
  case 0x74: case 0x84: return JumpType::JE;
  case 0x75: case 0x85: return JumpType::JNE;
  case 0x76: case 0x86: return JumpType::JBE;
  case 0x77: case 0x87: return JumpType::JA;
  case 0x78: case 0x88: return JumpType::JS;
  case 0x79: case 0x89: return JumpType::JNS;
  case 0x7A: case 0x8A: return JumpType::JP;
  case 0x7B: case 0x8B: return JumpType::JNP;
  case 0x7C: case 0x8C: return JumpType::JL;
  case 0x7D: case 0x8D: return JumpType::JGE;
  case 0x7E: case 0x8E: return JumpType::JLE;
  case 0x7F: case 0x8F: return JumpType::JG;

    // Other Jumps
  case 0xE3: return JumpType::JCXZ;
  case 0xEB: case 0xE9: return JumpType::JMP;

    // Default case
  default:
    return JumpType::UNKNOWN;
  }
}
