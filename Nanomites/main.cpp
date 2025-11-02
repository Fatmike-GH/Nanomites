#include <iostream>
#include <format>
#include "Tracer\Tracer.h"
#include "Tracer\SectionInfo.h"
#include "ProtectedCode\ProtectedCodeExecutor.h"

NanomiteMetadata* LoadMetaDataFromResource(LPCWSTR resourceName, LPCWSTR resourceType);

// --- main program : Builder.exe will be executed as post build event in the Builder project; make sure to rebuild the solution after making changes!
int main()
{
  NanomiteMetadata* metadata = LoadMetaDataFromResource(MAKEINTRESOURCE(1234), RT_RCDATA);

  const DWORD_PTR imageBase = (DWORD_PTR)GetModuleHandle(nullptr);
  SectionInfo* nanomitesSection = Tracer::Instance().CreateSectionInfo(".nano", imageBase);
  
  std::cout << "Unprotected code : Calling protected code..." << std::endl;

  // Tracing protected section .nano (protected methods from .nano may be called)
  Tracer::Instance().StartTracing(imageBase, nanomitesSection, metadata);
  ProtectedCodeExecutor* executor = new ProtectedCodeExecutor();
  executor->EnterText();
  DWORD checksum = executor->GetCrc32();
  delete executor;
  Tracer::Instance().StopTracing();

  std::cout << "Unprotected code : The calculated CRC32 is " << std::format("0x{:08X}", checksum) << std::endl;
  std::cout << "Unprotected code : End of Demo." << std::endl;

  delete nanomitesSection;

  std::cout << "Press ENTER to exit..." << std::endl;
  std::cin.ignore();
  std::cin.get();
}

NanomiteMetadata* LoadMetaDataFromResource(LPCWSTR resourceName, LPCWSTR resourceType)
{
  DWORD resourceSize = 0;

  HMODULE moduleHandle = GetModuleHandle(nullptr);
  HRSRC resourceHandle = FindResourceW(moduleHandle, resourceName, resourceType);
  if (resourceHandle == nullptr) return nullptr;

  resourceSize = SizeofResource(moduleHandle, resourceHandle);
  if (resourceSize == 0) return nullptr;

  HGLOBAL resourceDataHandle = LoadResource(moduleHandle, resourceHandle);
  if (resourceDataHandle == nullptr) return nullptr;

  LPVOID resourceData = LockResource(resourceDataHandle);
  if (resourceData == nullptr) return nullptr;

  BYTE* buffer = new BYTE[resourceSize];
  memcpy(buffer, resourceData, resourceSize);
  return reinterpret_cast<NanomiteMetadata*>(buffer);
}