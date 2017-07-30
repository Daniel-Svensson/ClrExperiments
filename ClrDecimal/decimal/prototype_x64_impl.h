#pragma once

#include <winnt.h>
struct DECIMAL;

// Prototype X64 Implementations
STDAPI VarDecMul_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);
STDAPI VarDecAdd_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);
STDAPI VarDecSub_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);
STDAPI VarDecDiv_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);