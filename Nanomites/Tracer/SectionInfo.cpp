#include "SectionInfo.h"

SectionInfo::SectionInfo()
{
  Clear();
}

SectionInfo::~SectionInfo()
{

}

void SectionInfo::Clear()
{
  _sectionStart = 0;
  _sectionEnd = 0;
  _sectionSize = 0;
}