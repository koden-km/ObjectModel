#pragma once

#define NOMINMAX

#include <winsock2.h>
#include <windows.h>
#include <shlwapi.h>

#undef DeleteFile
#undef CopyFile
#undef GetCurrentDirectory
#undef SetCurrentDirectory
#undef PathIsRelative
