#pragma once

#undef offsetof
#ifndef _CRT_USE_BUILTIN_OFFSETOF
#define offsetof(s, m) (int)(&((s*)0)->m)
#else
#define offsetof(s, m) (int)__builtin_offsetof(s,m)
#endif
