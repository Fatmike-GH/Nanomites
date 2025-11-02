#pragma once
#include <Windows.h>

struct Nanomite;

struct NanomiteMetadata
{
  DWORD ItemCount;
  Nanomite* Nanomites;
};