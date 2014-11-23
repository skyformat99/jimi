
#ifndef _JIMI_SYSTEM_CONSOLE_H_
#define _JIMI_SYSTEM_CONSOLE_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jimi/core/jimi_def.h"
#include "jimi/internal/NonCopyable.h"

#include "jimic/system/get_char.h"
#include "jimic/system/console.h"

#include <stdio.h>
#include <stdarg.h>

#ifdef _MSC_VER
#include <conio.h>
#endif // _MSC_VER

NS_JIMI_BEGIN

class Console : public internal::NonCopyable
{
public:
#if defined(JIMI_HAS_DEFAULTED_FUNCTIONS) && (JIMI_HAS_DEFAULTED_FUNCTIONS != 0)
    //! Allow default construction
    JIMI_CONSTEXPR Console() = default;
    ~Console() = default;
#else
    //! Allow default construction
    Console() {}
    ~Console() {}
#endif

    static void Write(const char *fmt, ...) {
        va_list arg_list;
        va_start(arg_list, fmt);
        vprintf(fmt, arg_list);
        va_end(arg_list);
    }

    static void WriteLine(const char *fmt = NULL, ...) {
        va_list arg_list;
        if (fmt != NULL) {
            va_start(arg_list, fmt);
            vprintf(fmt, arg_list);
            va_end(arg_list);
        }
        printf("\n");
    }

    static int ReadKey(bool enabledCpuWarmup = false, bool displayTips = true,
                       bool echoInput = false) {
        int keyCode;
        if (displayTips) {
#if 1
            printf("Press any key to continue ...");
#else
            printf("请按任意键继续 ...");
#endif
            keyCode = jimi_getch();
            printf("\n");
        }
        else {
            keyCode = jimi_getch();
            if (echoInput) {
                if (keyCode != EOF)
                    printf("%c", (char)keyCode);
                else
                    printf("EOF: (%d)", keyCode);
            }
        }

        // 暂停后, 预热/唤醒一下CPU, 至少500毫秒
        if (enabledCpuWarmup) {
            jimi_cpu_warmup(500);
            //printf("\n");
        }
        return keyCode;
    }

    static int ReadKey_NewLine(bool enabledCpuWarmup = false, bool displayTips = true,
                               bool echoInput = false) {
        int keyCode;
        keyCode = ReadKey(false, displayTips, echoInput);
        printf("\n");

        // 暂停后, 预热/唤醒一下CPU, 至少500毫秒
        if (enabledCpuWarmup) {
            jimi_cpu_warmup(500);
            //printf("\n");
        }
        return keyCode;
    }

    static int Read() {
        int keyCode = 0;
        return keyCode;
    }
};

static class Console Console;

NS_JIMI_END

#endif  /* !_JIMI_SYSTEM_CONSOLE_H_ */
