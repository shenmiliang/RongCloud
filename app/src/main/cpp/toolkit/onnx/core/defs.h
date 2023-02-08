#ifndef DEEP_TOOLKIT_CORE_DEFS_H
#define DEEP_TOOLKIT_CORE_DEFS_H

#include "toolkit/defs.h"

#ifdef DEEP_TOOLKIT_WIN32
# define DEEP_TOOLKIT_CHAR wchar_t
#else
# define DEEP_TOOLKIT_CHAR char
#endif

#endif