#pragma once

#include "stdafx.h"

#if defined(_WIN32)
#if 1
#include <windows.h>
#else
#ifndef STDAPI
#define STDAPI extern "C" long __stdcall
#endif
#endif
#else // ! _WIN32
typedef int32_t HRESULT;
#define __stdcall
#endif

#ifndef STDAPI
#define STDAPI extern "C" HRESULT
#endif

#ifdef _MSC_VER
#include <intrin.h>
#else // CLANG? 
#if defined(_TARGET_X86_) || defined(_TARGET_AMD64_)
#include <x86intrin.h>
#endif
#endif
