#include "Tracer.h"
#include "NanomiteMetadata.h"
#include "Nanomite.h"
#include "PEImage.h"
#include "SectionInfo.h"

Tracer::Tracer()
{
  _exceptionHandler = nullptr;
  _nanomiteSection = nullptr;
  _imageBase = 0;
}

Tracer::~Tracer()
{
  StopTracing();
}

Tracer& Tracer::Instance()
{
  static Tracer _instance;
  return _instance;
}

void Tracer::StartTracing(DWORD_PTR imageBase, SectionInfo* nanomitesSection, NanomiteMetadata* metadata)
{
#define CALL_FIRST 1  
#define CALL_LAST 0
  _imageBase = imageBase;
  _nanomiteSection = nanomitesSection;
  ReadNanomiteMetadata(metadata);
  if (_exceptionHandler == nullptr)
  {
    _exceptionHandler = AddVectoredExceptionHandler(CALL_FIRST, VectoredHandlerBreakPoint);
  }
}

void Tracer::StopTracing()
{
  _imageBase = 0;
  _nanomiteSection = nullptr;
  _nanomiteLookup.clear();
  if (_exceptionHandler != nullptr)
  {
    RemoveVectoredExceptionHandler(_exceptionHandler);
    _exceptionHandler = nullptr;
  }
}

SectionInfo* Tracer::CreateSectionInfo(const char* sectionName, DWORD_PTR imageBase)
{
  PEImage peImage(imageBase);
  PIMAGE_SECTION_HEADER section = peImage.FindSection(sectionName);
  if (section == nullptr) return nullptr;

  SectionInfo* sectionInfo = new SectionInfo();
  DWORD_PTR sectionStart = imageBase + section->VirtualAddress;
  DWORD_PTR sectionSize = section->Misc.VirtualSize;
  DWORD_PTR sectionEnd = sectionStart + sectionSize - 1;

  sectionInfo->SetSectionStart(sectionStart);
  sectionInfo->SetSectionEnd(sectionEnd);
  sectionInfo->SetSectionSize(sectionSize);

  return sectionInfo;
}

LONG WINAPI Tracer::VectoredHandlerBreakPoint(_EXCEPTION_POINTERS* ExceptionInfo)
{
  if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
  {
    if (Tracer::Instance().ResolveNanomite(ExceptionInfo->ContextRecord))
    {
      return EXCEPTION_CONTINUE_EXECUTION;
    }
  }
  return EXCEPTION_CONTINUE_SEARCH;
}

bool Tracer::ResolveNanomite(PCONTEXT context)
{
  DWORD_PTR eip = GetInstructionPointer(context);
  if (eip < _nanomiteSection->GetSectionStart() || eip > _nanomiteSection->GetSectionEnd()) return false;

  DWORD_PTR va = eip;
  DWORD rva = (DWORD)(va - _imageBase);
  Nanomite* nanomite = GetNanomite(rva);
  if (nanomite == nullptr) return false;

  if (ExecuteJump(nanomite, context))
  {
    eip = eip + nanomite->OpcodeLength + (char)nanomite->JumpLength; // (char) : Cast to signed type for correct calculation!
    SetInstructionPointer(context, eip);
  }
  else
  {
    eip += nanomite->OpcodeLength;
    SetInstructionPointer(context, eip);
  }

  return true;
}

bool Tracer::ExecuteJump(Nanomite* nanomite, PCONTEXT context)
{
  JumpType jumpType = (JumpType)nanomite->JumpType;
  DWORD eflags = context->EFlags;

  switch (jumpType)
  {
  case JumpType::JS:
  {
    return SF(eflags);
  }
  break;
  case JO:
  {
    return OF(eflags);
  }
  break;
  case JumpType::JG:
  {
    return (!ZF(eflags) && (SF(eflags) == OF(eflags)));
  }
  break;
  case JumpType::JB:
  {
    return CF(eflags);
  }
  break;
  case JumpType::JL:
  {
    return (SF(eflags) != OF(eflags));
  }
  break;
  case JumpType::JNO:
  {
    return (!OF(eflags));
  }
  break;
  case JumpType::JCXZ:
  {
#ifdef _WIN64
    return context->Rcx == 0;
#else
    return context->Ecx == 0;
#endif    
  }
  break;
  case JumpType::JNS:
  {
    return (!SF(eflags));
  }
  break;
  case JumpType::JGE:
  {
    return (SF(eflags) == OF(eflags));
  }
  break;
  case JumpType::JNB:
  {
    return (!CF(eflags));
  }
  break;
  case JumpType::JNE:
  {
    return (!ZF(eflags));
  }
  break;
  case JumpType::JLE:
  {
    return (ZF(eflags) || (SF(eflags) != OF(eflags)));
  }
  break;
  case JumpType::JE:
  {
    return (ZF(eflags));
  }
  break;
  case JumpType::JNP:
  {
    return (!PF(eflags));
  }
  break;
  case JumpType::JA:
  {
    return (!CF(eflags) && !ZF(eflags));
  }
  break;
  case JumpType::JBE:
  {
    return (CF(eflags) || ZF(eflags));
  }
  break;
  case JumpType::JP:
  {
    return (PF(eflags));
  }
  break;
  case JumpType::JMP:
  {
    return true;
  }
  break;
  default:
    break;
  }
  return false;
}

Nanomite* Tracer::GetNanomite(DWORD rva)
{
  auto iter = _nanomiteLookup.find(rva);
  if (iter == _nanomiteLookup.end()) return nullptr;
  return iter->second;
}

void Tracer::ReadNanomiteMetadata(NanomiteMetadata* metadata)
{
  _nanomiteLookup.clear();
  if (metadata == nullptr || metadata->ItemCount == 0) return;

  Nanomite* first = reinterpret_cast<Nanomite*>(reinterpret_cast<BYTE*>(metadata) + sizeof(NanomiteMetadata));
  for (DWORD i = 0; i < metadata->ItemCount; i++)
  {
    Nanomite* nanomite = first + i;
    _nanomiteLookup[nanomite->Rva] = nanomite;
  }
}

void Tracer::SetInstructionPointer(PCONTEXT& context, DWORD_PTR value)
{
#ifdef _WIN64
  context->Rip = value;
#else
  context->Eip = value;
#endif  
}

DWORD_PTR Tracer::GetInstructionPointer(PCONTEXT& context)
{
#ifdef _WIN64
  return context->Rip;
#else
  return context->Eip;
#endif
}