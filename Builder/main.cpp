#include <iostream>
#include "PEFile\PEFile.h"
#include "PEFile\ResourceAdder.h"
#include "FileWriter\FileWriter.h"
#include "Nanomites\NanomitesCreator.h"
#include "Nanomites\NanomiteMetadata.h"

bool CreateNanomites(const char* exeFile, const char* sectionName);
void WriteFile(const char* exeFile, PEFile& peFile);
void AddMetadataAsResource(const char* exeFile, NanomiteMetadata* metadata);

// --- main program --- Will be executed as post build event in the Builder project; make sure to rebuild the solution after making changes!
int main()
{
  const std::string fileName = "Nanomites.exe";
  const std::string sectionName = ".nano";

  std::cout << "Creating nanomites in section " << sectionName << " of " << fileName << "..." << std::endl;

  if (!CreateNanomites(fileName.c_str(), sectionName.c_str()))
  {
    std::cout << "Creating nanomites failed!" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Creating nanomites finished successfully." << std::endl;
  std::cout << "Press ENTER to exit..." << std::endl;
  std::cin.get();

  return EXIT_SUCCESS;
}

bool CreateNanomites(const char* exeFile, const char* sectionName)
{
  PEFile peFile;
  if (!peFile.OpenFile(exeFile)) return false;

  PIMAGE_SECTION_HEADER sectionHeader = peFile.FindSectionByName(sectionName);
  if (sectionHeader == nullptr) return false;

  NanomitesCreator nanomitesCreator;
  NanomiteMetadata* metadata = nanomitesCreator.Create(peFile, sectionHeader);

  WriteFile(exeFile, peFile);
  AddMetadataAsResource(exeFile, metadata);

  return true;
}

void WriteFile(const char* exeFile, PEFile& peFile)
{
  // Write exe file with nanomites
  FileWriter fileWriter;
  std::ofstream outfile;
  fileWriter.OpenForWriteBin(exeFile, outfile);
  fileWriter.Write(outfile, (BYTE*)(peFile.DOS_HEADER()), peFile.GetBufferSize());
  fileWriter.Close(outfile);
}

void AddMetadataAsResource(const char* exeFile, NanomiteMetadata* metadata)
{
  // Append nanomite meta data as resource
  DWORD metadataSize = sizeof(NanomiteMetadata) + (metadata->ItemCount) * sizeof(Nanomite);
  BYTE* metadataBuffer = new BYTE[metadataSize];
  memset(metadataBuffer, 0, metadataSize);
  memcpy(metadataBuffer, metadata, sizeof(NanomiteMetadata));
  memcpy(metadataBuffer + sizeof(NanomiteMetadata), metadata->Nanomites, metadata->ItemCount * sizeof(Nanomite));

  ResourceAdder resourceAdder;
  resourceAdder.AddResource(exeFile, 1234, metadataBuffer, metadataSize);
}
