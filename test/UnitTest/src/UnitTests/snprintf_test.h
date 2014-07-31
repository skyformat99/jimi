
#ifndef _JIMI_UNITTEST_SNPRINTF_TEST_H_
#define _JIMI_UNITTEST_SNPRINTF_TEST_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <jimi/platform/jimi_platform_def.h>

/* �Ƿ�ʹ�ü򻯵Ĳ���������, Ϊ1����ʹ�� */
#define VSNPRINTF_SHORT_DISPLAY         0

/* �Ƿ�����jm_snprintf()�Ĳ���, Ϊ1�������� */
#define VSNPRINTF_TEST_JM_SNPRINTF      1

#ifdef __cplusplus
extern "C" {
#endif

void String_Snprintf_Test();

void Sprintf_Preformance_Test_Integer();

void Snprintf_Preformance_Test_Integer1();

void Snprintf_Preformance_Test_Integer2();

void Snprintf_Preformance_Test_Integer3();

void Snprintf_Preformance_Test_Double1();

void Snprintf_Preformance_Test_Double2();

#ifdef __cplusplus
}
#endif

#endif  /* _JIMI_UNITTEST_SNPRINTF_TEST_H_ */