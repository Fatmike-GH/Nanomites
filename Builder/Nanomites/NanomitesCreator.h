#pragma once
#include <Windows.h>
#include <set>
#include <vector>
#include "Nanomite.h"
#include "NanomiteMetadata.h"
#include "..\PEFile\PEFile.h"
#include "..\Disassembler\RelativeJump.h"

class NanomitesCreator
{
public:
  NanomitesCreator();
  ~NanomitesCreator();

  NanomiteMetadata* Create(PEFile& peFile, PIMAGE_SECTION_HEADER sectionHeader);

private:
  void ProcessRealJumps(PEFile& peFile, PIMAGE_SECTION_HEADER sectionHeader, const std::vector<RelativeJump>& relativeJumps, std::vector<Nanomite>& outNanomites);
  void ProcessFakeJumps(PIMAGE_SECTION_HEADER sectionHeader, const std::set<DWORD>& fakeNanomiteRVAs, std::vector<Nanomite>& outNanomites) const;
  void SortNanomitesByRva(std::vector<Nanomite>& nanomites) const;
  NanomiteMetadata* CreateMetadata(PIMAGE_SECTION_HEADER sectionHeader, const std::vector<Nanomite>& nanomites) const;
  void WriteNanomite(PEFile& peFile, PIMAGE_SECTION_HEADER sectionHeader, Nanomite& nanomite);
  BYTE GetRandomShortJump() const;
  BYTE GetRandomByte() const;
  BYTE GetRandomByte(int min, int max) const;
  JumpType ToJumpType(DWORD opcode) const;
};

