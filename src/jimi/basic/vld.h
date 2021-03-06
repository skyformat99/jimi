
#ifndef _JIMI_BASIC_VLD_H_
#define _JIMI_BASIC_VLD_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jimi/config/config.h"
#include "jimic/config/config.h"

/**********************************************************
 *
 *  Use Visual Leak Detector(vld) for Visual C++,
 *  Homepage: http://vld.codeplex.com/
 *
 **********************************************************/
#ifdef _MSC_VER

#if defined(_DEBUG) || !defined(NDEBUG)

#if defined(JIMI_USE_VLD) && (JIMI_USE_VLD != 0)
// 如果你没有安装vld(Visual Leak Detector), 请注释掉这一句. vld的官网请看上面.
#include <vld.h>
#endif  /* JIMI_USE_VLD */

#endif  /* _DEBUG */

#endif  /* _MSC_VER */

#endif  /* !_JIMI_BASIC_VLD_H_ */
