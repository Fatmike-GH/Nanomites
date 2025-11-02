#pragma once
#include <Windows.h>
#include <map>

struct NanomiteMetadata;
struct Nanomite;
class SectionInfo;

class Tracer
{
public:
  static Tracer& Instance();

  SectionInfo* CreateSectionInfo(const char* sectionName, DWORD_PTR imageBase);

  void StartTracing(DWORD_PTR imageBase, SectionInfo* nanomitesSection, NanomiteMetadata* metadata);
  void StopTracing();

private:
  Tracer();
  ~Tracer();

  static LONG WINAPI VectoredHandlerBreakPoint(_EXCEPTION_POINTERS* ExceptionInfo);

  bool ResolveNanomite(PCONTEXT context);

  bool ExecuteJump(Nanomite* nanomite, PCONTEXT context);
  bool ZF(DWORD eflags) { return (eflags & 0x00000040) != 0; }
  bool OF(DWORD eflags) { return (eflags & 0x00000800) != 0; }
  bool SF(DWORD eflags) { return (eflags & 0x00000080) != 0; }
  bool CF(DWORD eflags) { return (eflags & 0x00000001) != 0; }
  bool PF(DWORD eflags) { return (eflags & 0x00000004) != 0; }

  Nanomite* GetNanomite(DWORD rva);
  void ReadNanomiteMetadata(NanomiteMetadata* metadata);

  void SetInstructionPointer(PCONTEXT& context, DWORD_PTR value);
  DWORD_PTR GetInstructionPointer(PCONTEXT& context);

private:
  PVOID _exceptionHandler;
  DWORD_PTR _imageBase;
  SectionInfo* _nanomiteSection;
  std::map<DWORD, Nanomite*> _nanomiteLookup;
};

