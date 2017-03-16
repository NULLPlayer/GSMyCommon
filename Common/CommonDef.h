#ifndef COMMONDEF_H
#define COMMONDEF_H

#include "GSDefs.h"
#include "GSStrUtil.hpp"

#if defined(GS_REAL_DOUBLE)
typedef double GSReal;
#else
typedef float GSReal;
#endif

#endif // COMMONDEF_H