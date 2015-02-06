
#ifndef _JIMI_CONFIG_CONFIG_H_
#define _JIMI_CONFIG_CONFIG_H_

#include "jimi/config/config-pre.h"

#if defined(__linux__) || defined(__LINUX__)
#include "jimi/config/linux/config.h"
#elif defined(__MINGW__) || defined(__MINGW32__) || defined(__MINGW64__)
#include "jimi/config/mingw/config.h"
#elif defined(__CYGWIN__)
#include "jimi/config/cygwin/config.h"
#elif defined(_WIN32) || defined(_MSC_VER) || defined(__ICL) || defined(__INTEL_COMPILER)
#include "jimi/config/win32/config.h"
#else
#include "jimi/config/default/config.h"
#endif

#include "jimi/config/config-pre.h"

#endif  /* !_JIMI_CONFIG_CONFIG_H_ */
