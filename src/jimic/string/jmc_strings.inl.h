
#ifndef _JIMIC_STRING_JMC_STRIGNS_INL_H_
#define _JIMIC_STRING_JMC_STRIGNS_INL_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef _JIMIC_STRING_JMC_STRINGS_H_
#include <jimic/string/jmc_strings.h>
#endif

#include <jimic/string/jm_strings.h>

#include <stdarg.h>
#include <math.h>       // for isnan(), isinf()
#include <float.h>
#include <limits.h>     // for UINT_MAX

/* UINT的最大值 */
#define JIMIC_UINT_MAX                  UINT_MAX

/* UINT的最大值 */
#define JIMIC_UINT_MAX64                (0x00000000FFFFFFFFULL)

#ifndef FMT_NULL_STRING
#define FMT_NULL_STRING                 "(null)"
#endif

//
// Printf() 输出格式控制
// Reference: http://bbs.csdn.net/topics/330107715
//

#ifndef _JIMIC_STDIO_SPRINTF_FORMAT_DEF_
#define _JIMIC_STDIO_SPRINTF_FORMAT_DEF_

/* format sign */
#define FMT_DEFAULT_FLAG        0x00000000      // default flag
#define FMT_SPACE_FLAG          0x00010000      // ' ', space flag
#define FMT_PLUS_FLAG           0x00020000      // '+', plus flag
#define FMT_SHARP_FLAG          0x00040000      // '#', number flag
#define FMT_FIELDWIDTH_FLAG     0x00080000      // '*', asterisk flag
#define FMT_CURRENCY_FLAG       0x00100000      // ',', comma flag

#define FMT_SIGN_MASK           0x00000002      // sign mask
#define FMT_SIGN_MASK_BIT       2               // sign move bit

// 句号: period

/* format align mode */
#define FMT_ALIGN_RIGHT         0x00200000      // align to right
#define FMT_ALIGN_LEFT          0x00400000      // align to left
#define FMT_ALIGN_CENTER        0x00800000      // align to center
#define FMT_ALIGN_MINUS         FMT_ALIGN_LEFT  // '-', minus sign
#define FMT_ALIGN_DEFAULT       FMT_ALIGN_RIGHT
                                                // default is FMT_ALIGN_RIGHT, '+'

/* format fill mode */
#define FMT_FILL_SPACE          ' '             // fill space: ' '
#define FMT_FILL_ZERO           '0'             // fill zero:  '0'
#define FMT_FILL_DEFAULT        FMT_FILL_SPACE
                                                // default is FMT_FILL_SPACE, ' '

#define FMT_DEFAULT_STATUS      (FMT_DEFAULT_FLAG | FMT_ALIGN_DEFAULT | FMT_FILL_DEFAULT)

#endif  /* !_JIMIC_STDIO_SPRINTF_FORMAT_DEF_ */

//
// ASCII码对照表
// Reference: http://ascii.911cha.com/
//

#ifndef FMT_DEFAULT_DOUBLE_PRECISION
#define FMT_DEFAULT_DOUBLE_PRECISION    6
#endif

#ifndef FMT_MAX_DOUBLE_PRECISION
#define FMT_MAX_DOUBLE_PRECISION        17
#endif

#define JM_FIS_NORMAL                   0
#define JM_FIS_INF                      0x01
#define JM_FIS_NAN                      0x02
#define JM_FIS_NAN_OR_INF               0x04
#define JM_FIS_ZERO                     0x08
#define JM_FIS_SUBNORMAL                0x10

////////////////////////////////////////////////////////////////////////////////

/* for float */
#define JM_FLOAT_SIGN_BIT               (0x80000000UL)
#define JM_FLOAT_EXPONENT_MASK          (0x7F800000UL)
#define JM_FLOAT_MANTISSA_MASK          (0x007FFFFFUL)

////////////////////////////////////////////////////////////////////////////////

/* for double */
#define JM_DOUBLE_SIGN_BIT              (0x8000000000000000ULL)
#define JM_DOUBLE_EXPONENT_MASK         (0x7FF0000000000000ULL)
#define JM_DOUBLE_MANTISSA_MASK         (0x000FFFFFFFFFFFFFULL)

#define JM_DOUBLE_EXPONENT_BIAS_MASK    (0x3FF0000000000000ULL)

#define JM_DOUBLE_SIGN_SHIFT            63
#define JM_DOUBLE_EXPONENT_SHIFT        52
#define JM_DOUBLE_MANTISSA_SHIFT        0

/* double exponent 0 is 1023 */
#define JM_DOUBLE_EXPONENT_BIAS         ((JM_DOUBLE_EXPONENT_BIAS_MASK) >> (JM_DOUBLE_EXPONENT_SHIFT))

/* for double (high 32 bits) */
#define JM_DOUBLE_SIGN_BIT32            ((uint32_t)(JM_DOUBLE_SIGN_BIT      >> 32))
#define JM_DOUBLE_EXPONENT_MASK32       ((uint32_t)(JM_DOUBLE_EXPONENT_MASK >> 32))
#define JM_DOUBLE_MANTISSA_MASK_HIGH    ((uint32_t)(JM_DOUBLE_MANTISSA_MASK >> 32))
#define JM_DOUBLE_MANTISSA_MASK_LOW     \
                        ((uint32_t)(JM_DOUBLE_MANTISSA_MASK & 0x00000000FFFFFFFFULL))

#define JM_DOUBLE_EXPONENT_BIAS_MASK32  ((uint32_t)(JM_DOUBLE_EXPONENT_BIAS_MASK >> 32))

#define JM_DOUBLE_SIGN_SHIFT32          (63 - 32)
#define JM_DOUBLE_EXPONENT_SHIFT32      (52 - 32)
#define JM_DOUBLE_MANTISSA_SHIFT32      0

/* double exponent 0 is 1023 */
#define JM_DOUBLE_EXPONENT_BIAS_32      (JM_DOUBLE_EXPONENT_BIAS_MASK32 >> JM_DOUBLE_EXPONENT_SHIFT32)

/* double exponent maxium is  1024 = (2047 - 1023) */
#define JM_DOUBLE_EXPONENT_MAX          ((JM_DOUBLE_EXPONENT_MASK32 >> JM_DOUBLE_EXPONENT_SHIFT32) - JM_DOUBLE_EXPONENT_BIAS_32)
/* double exponent minium is -1023 = (0 - 1023) */
#define JM_DOUBLE_EXPONENT_MIN          (0 - JM_DOUBLE_EXPONENT_BIAS_32)

////////////////////////////////////////////////////////////////////////////////

typedef struct fuint64_t {
    union {
#if defined(JIMIC_IS_LITTLE_ENDIAN) && (JIMIC_IS_LITTLE_ENDIAN != 0)
        // 小端存储
        struct {
            uint32_t    low;
            uint32_t    high;
        };
#else
        // 大端存储
        struct {
            uint32_t    high;
            uint32_t    low;
        };
#endif  /* JIMI_IS_LITTLE_ENDIAN */
        uint64_t        u64;
        double          d;
    };
} fuint64_t;

typedef struct fvariant_t {
    union {
        fuint64_t       f64;
        int64_t         i64;
        uint64_t        u64;
        int             i32;
        unsigned char   u32;
        long            l32;
        unsigned long   ul32;
        char            c;
        unsigned char   b;
        short           s16;
        unsigned short  u16;
        double          d;
        float           f;
    };
} fvariant_t;

#if 1
JMC_INLINE_NONSTD(int)
jmc_utoa_r10(char *buf, unsigned int val)
{
    unsigned int digval, digital;
    const char *end;
    char *cur;
    char digits[16];    // 实际最多只会用到10个bytes

    jimic_assert(buf != NULL);

    end = digits + jm_countof(digits);
    cur = (char *)end;
    do {
        cur--;
        digval = val % 10;
        val /= 10;

        *cur = (char)(digval + '0');
    } while (val != 0);

    digital = (unsigned int)(end - cur);

#if 1
    digval = digital;
    while (digval > 0) {
        *buf++ = *cur++;
        digval--;
    }
#elif 0
    do {
        *buf++ = *cur++;
    } while (cur != end);
#else
    while (cur < end)
        *buf++ = *cur++;
#endif
    *buf = '\0';

    return digital;
}
#elif 0
JMC_INLINE_NONSTD(int)
jmc_utoa_r10(char *buf, unsigned int val)
{
    unsigned int digval, digital;
    char *end, *cur;
    char digits[16];    // 实际最多只会用到10个bytes

    jimic_assert(buf != NULL);

    end = digits + jm_countof(digits) - 1;
    cur = end;
    do {
        digval = val % 10;
        val /= 10;

        *cur-- = (char)(digval + '0');
    } while (val != 0);

    digital = end - cur;

#if 0
    do {
        ++cur;
        *buf++ = *cur;
    } while (cur != end);
#else
    cur++;
    while (cur <= end)
        *buf++ = *cur++;
#endif
    *buf = '\0';

    return digital;
}
#else
JMC_INLINE_NONSTD(int)
jmc_utoa_r10(char *buf, unsigned int val)
{
    unsigned int digval, digital;
    char *cur;
    char digits[16];    // 实际最多只会用到10个bytes

    jimic_assert(buf != NULL);

    cur = digits;
    do {
        digval = val % 10;
        val /= 10;

        *cur++ = (char)digval + '0';
    } while (val != 0);

    digital = cur - digits;

#if 0
    do {
        --cur;
        *buf++ = *cur;
    } while (cur != digits);
#else
    cur--;
    while (cur >= digits)
        *buf++ = *cur--;
#endif
    *buf = '\0';

    return digital;
}
#endif

JMC_INLINE_NONSTD(int)
jmc_itoa_r10(char *buf, int val)
{
#if 0
    int sign;
    if (val < 0) {
        *buf++ = '-';
        val = -val;
        sign = 1;
    }
    else {
        sign = 0;
    }
    return jmc_utoa_r10(buf, (unsigned int)val) + sign;
#elif 1
    if (val >= 0) {
        return jmc_utoa_r10(buf, (unsigned int)val);
    }
    else {
        *buf++ = '-';
        val = -val;
        return jmc_utoa_r10(buf, (unsigned int)val) + 1;
    }
#elif 0
    if (val < 0) {
        *buf++ = '-';
        val = -val;
        return jmc_utoa_r10(buf, (unsigned int)val) + 1;
    }
    else
        return jmc_utoa_r10(buf, (unsigned int)val);
#else
    register int sign;
    sign = (val < 0);
    if (val < 0) {
        *buf++ = '-';
        val = -val;
    }
    return jmc_utoa_r10(buf, (unsigned int)val) + sign;
#endif
}

JMC_INLINE_NONSTD(int)
jmc_ultoa_r10(char *buf, unsigned long val)
{
    unsigned long digval, digital;
    char *cur;
    char digits[16];    // 实际最多只会用到10个bytes

    cur = digits;
    do {
        digval = val % 10;
        val /= 10;

        *cur++ = (char)digval + '0';
    } while (val != 0);

    digital = (unsigned long)(cur - digits);

#if 0
    do {
        --cur;
        *buf++ = *cur;
    } while (cur != digits);
#else
    cur--;
    while (cur >= digits)
        *buf++ = *cur--;
#endif
    *buf = '\0';

    return digital;
}

JMC_INLINE_NONSTD(int)
jmc_ltoa_r10(char *buf, long val)
{
#if 1
    if (val >= 0) {
        return jmc_ultoa_r10(buf, val);
    }
    else {
        *buf++ = '-';
        val = -val;
        return jmc_ultoa_r10(buf, val) + 1;
    }        
#else
    int sign;
    sign = (val < 0);
    if (val < 0) {
        *buf++ = '-';
        val = -val;
    }
    return jmc_ultoa_r10(buf, val) + sign;
#endif
}

JMC_INLINE_NONSTD(int)
jmc_u64toa_r10(char *buf, uint64_t val)
{
    unsigned int digval, digital;
    uint32_t val32;
    char *cur;
    fuint64_t *u64;
    char digits[32];    // 实际最多只会用到20个bytes

    cur = digits;
#if 0
    if (val <= (uint64_t)JIMIC_UINT_MAX64) {
        val32 = (uint32_t)val;
#else
    u64 = (fuint64_t *)&val;
    if (u64->high == 0) {
        val32 = (uint32_t)u64->low;
#endif
        do {
            digval = val32 % 10;
            val32 /= 10;

            *cur++ = (char)(digval + '0');
        } while (val32 != 0);
    }
    else {
        do {
            digval = val % 10;
            val /= 10;

            *cur++ = (char)(digval + '0');
        } while (val != 0);
    }

    digital = (unsigned int)(cur - digits);

#if 0
    do {
        --cur;
        *buf++ = *cur;
    } while (cur != digits);
#else
    cur--;
    while (cur >= digits)
        *buf++ = *cur--;
#endif
    *buf = '\0';

    return digital;
}

JMC_INLINE_NONSTD(int)
jmc_i64toa_r10(char *buf, int64_t val)
{
#if 1
    if (val >= 0) {
        return jmc_u64toa_r10(buf, val);
    }
    else {
        *buf++ = '-';
        val = -val;
        return jmc_u64toa_r10(buf, val) + 1;
    }        
#else
    int sign;
    sign = (val < 0);
    if (val < 0) {
        *buf++ = '-';
        val = -val;
    }
    return jmc_u64toa_r10(buf, val) + sign;
#endif
}

JMC_INLINE_NONSTD(int)
jmc_utoa_r10_ex(char *buf, size_t count, unsigned int val, unsigned int flag,
                    unsigned int fill, unsigned int filed_width, int length)
{
    unsigned int digval, digital;
    char *end, *cur;
    int sign_char;
    int fill_cnt;
    int padding;
    char digits[16];    // 实际最多只会用到10个bytes

    jimic_assert(buf != NULL);
    jimic_assert(count != 0);

    end = digits + jm_countof(digits) - 1;
    cur = end;
    do {
        digval = val % 10;
        val /= 10;

        *cur-- = (char)(digval + '0');
    } while (val != 0);

    digital = (unsigned int)(end - cur);

    if ((flag & (FMT_SIGN_MASK | FMT_SPACE_FLAG | FMT_PLUS_FLAG)) == 0) {
        sign_char = '\0';
        fill_cnt = filed_width - digital;
    }
    else {
        if ((flag & FMT_SPACE_FLAG) == 0) {
            // '+' is 0x2B, '-' is 0x2D
            sign_char = '+' + (flag & FMT_SIGN_MASK);
            fill_cnt = filed_width - digital - 1;
        }
        else {
            // ' ' is 0x20, '-' is 0x2D
#if 0
            sign_char = ((flag & FMT_SIGN_MASK) == 0) ? ' ' : '-';
#else
            if ((flag & FMT_SIGN_MASK) == 0)
                sign_char = ' ';
            else
                sign_char = '-';
#endif
            fill_cnt = filed_width - digital - 1;
        }
    }

    if (fill_cnt <= 0) {
        // add sign
        if (sign_char != '\0')
            *buf++ = (char)sign_char;
#if 0
        do {
            ++cur;
            *buf++ = *cur;
        } while (cur != end);
#else
        cur++;
        while (cur <= end)
            *buf++ = *cur++;
#endif
        *buf = '\0';

        if (sign_char == '\0')
            return digital;
        else
            return digital + 1;
    }
    else {
        // when legnth <= 0 || legnth >= witdh, align to right or left is same
        if (length <= 0 || length >= (int)filed_width) {
            if (fill == FMT_FILL_SPACE) {
                // fill normal
                while (fill_cnt > 0) {
                    *buf++ = fill;
                    fill_cnt--;
                }

                // add sign
                if (sign_char != '\0')
                    *buf++ = (char)sign_char;
            }
            else {
                // add sign
                if (sign_char != '\0')
                    *buf++ = (char)sign_char;

                // fill normal
                while (fill_cnt > 0) {
                    *buf++ = fill;
                    fill_cnt--;
                }
            }
        }
        else {
            if ((flag & FMT_ALIGN_LEFT) == 0) {
                // align to right, when (length < filed_width)
                jimic_assert(length < (int)filed_width);

                padding = length - digital;
                if (padding > 0) {
                    // fill right padding space
                    while (fill_cnt > padding) {
                        *buf++ = ' ';
                        fill_cnt--;
                    }

                    // add sign
                    if (sign_char != '\0')
                        *buf++ = (char)sign_char;

                    // fill normal
                    while (fill_cnt > 0) {
                        *buf++ = fill;
                        fill_cnt--;
                    }
                }
                else {
                    // fill right padding space
                    while (fill_cnt > 0) {
                        *buf++ = ' ';
                        fill_cnt--;
                    }

                    // add sign
                    if (sign_char != '\0')
                        *buf++ = (char)sign_char;
                }
            }
            else {
                // align to left, when (length < filed_width)
                jimic_assert(length < (int)filed_width);

                // add sign
                if (sign_char != '\0')
                    *buf++ = (char)sign_char;

                // fill normal
                padding = length - digital;
                if (padding > 0) {
                    fill_cnt -= padding;
                    while (padding > 0) {
                        *buf++ = fill;
                        padding--;
                    }
                }

#if 0
                do {
                    ++cur;
                    *buf++ = *cur;
                } while (cur != end);
#else
                cur++;
                while (cur <= end)
                    *buf++ = *cur++;
#endif
                // fill left padding space
                while (fill_cnt > 0) {
                    *buf++ = ' ';
                    fill_cnt--;
                }

                goto utoa_r10_ex_exit;
            }
        }
    }

#if 0
    do {
        ++cur;
        *buf++ = *cur;
    } while (cur != end);
#else
    cur++;
    while (cur <= end)
        *buf++ = *cur++;
#endif

utoa_r10_ex_exit:
    *buf = '\0';

    return filed_width;
}

JMC_INLINE_NONSTD(int)
jmc_itoa_r10_ex(char *buf, size_t count, int val, unsigned int flag,
                    unsigned int fill, unsigned int filed_width, int length)
{
#if 1
    if (val < 0) {
        flag |= FMT_SIGN_MASK;
        val = -val;
    }

    return jmc_utoa_r10_ex(buf, count, val, flag, fill, filed_width, length);
#elif 0
    if ((flag & FMT_SPACE_FLAG) == 0) {
        if (val >= 0) {
            return jmc_utoa_r10_ex(buf, count, val, flag, fill, filed_width, length);
        }
        else {
            *buf++ = '-';
            val = -val;
            flag |= FMT_SIGN_MASK;
            return jmc_utoa_r10_ex(buf, count, val, flag, fill, filed_width, length) + 1;
        }
    }
    else {
        if (val >= 0) {
            *buf++ = '+';
        }
        else {
            *buf++ = '-';
            val = -val;
        }
        flag |= FMT_SIGN_MASK;
        return jmc_utoa_r10_ex(buf, count, val, flag, fill, filed_width, length) + 1;
    }
#elif 0
    if ((flag & FMT_SPACE_FLAG) == 0) {
        if (val < 0) {
            *buf++ = '-';
            val = -val;
            return jmc_utoa_r10_ex(buf, count, val, flag, fill, filed_width, length) + 1;
        }
        else
            return jmc_utoa_r10_ex(buf, count, val, flag, fill, filed_width, length);
    }
    else {
        if (val >= 0) {
            *buf++ = '+';
        }
        else {
            *buf++ = '-';
            val = -val;
        }
        return jmc_utoa_r10_ex(buf, count, val, flag, fill, filed_width, length) + 1;
    }
#else
    int sign;
    if ((flag & FMT_SPACE_FLAG) == 0) {
        sign = (val < 0);
        if (val < 0) {
            *buf++ = '-';
            val = -val;
        }
        return jmc_utoa_r10_ex(buf, count, val, flag, fill, filed_width, length) + sign;
    }
    else {
        if (val < 0) {
            *buf++ = '-';
            val = -val;
        }
        else {
            *buf++ = '+';
        }
        return jmc_utoa_r10_ex(buf, count, val, flag, fill, filed_width, length) + 1;
    }
#endif
}

JMC_INLINE_NONSTD(int)
jmc_u64toa_r10_ex(char *buf, size_t count, uint64_t val, unsigned int flag,
                      unsigned int fill, unsigned int filed_width, int length)
{
    unsigned int digval, digital;
    uint32_t val32;
    char *end, *cur;
    int sign_char;
    int fill_cnt;
    int padding;
    fuint64_t *u64;
    char digits[32];    // 实际最多只会用到20个bytes

    static_assert(sizeof(fuint64_t) == sizeof(uint64_t), "jmc_u64toa_r10_ex() maybe have some error!");

    if (sizeof(fuint64_t) != sizeof(uint64_t)) {
        // maybe have some error!
        jimic_assert(sizeof(fuint64_t) == sizeof(uint64_t));
        return -1;
    }

    end = digits + jm_countof(digits) - 1;
    cur = end;
#if 0
    if (val <= (uint64_t)JIMIC_UINT_MAX64) {
        val32 = (uint32_t)val;
#else
    u64 = (fuint64_t *)&val;
    if (u64->high == 0) {
        val32 = (uint32_t)u64->low;
#endif
        do {
            digval = val32 % 10;
            val32 /= 10;

            *cur-- = (char)(digval + '0');
        } while (val32 != 0);
    }
    else {
        do {
            digval = val % 10;
            val /= 10;

            *cur-- = (char)(digval + '0');
        } while (val != 0);
    }

    digital = (unsigned int)(end - cur);

    if ((flag & (FMT_SIGN_MASK | FMT_SPACE_FLAG | FMT_PLUS_FLAG)) == 0) {
        sign_char = '\0';
        fill_cnt = filed_width - digital;
    }
    else {
        if ((flag & FMT_SPACE_FLAG) == 0) {
            // '+' is 0x2B, '-' is 0x2D
            sign_char = '+' + (flag & FMT_SIGN_MASK);
            fill_cnt = filed_width - digital - 1;
        }
        else {
            // ' ' is 0x20, '-' is 0x2D
#if 0
            sign_char = ((flag & FMT_SIGN_MASK) == 0) ? ' ' : '-';
#else
            if ((flag & FMT_SIGN_MASK) == 0)
                sign_char = ' ';
            else
                sign_char = '-';
#endif
            fill_cnt = filed_width - digital - 1;
        }
    }

    if (fill_cnt <= 0) {
        // add sign
        if (sign_char != '\0')
            *buf++ = (char)sign_char;
#if 0
        do {
            ++cur;
            *buf++ = *cur;
        } while (cur != end);
#else
        cur++;
        while (cur <= end)
            *buf++ = *cur++;
#endif
        *buf = '\0';

        if (sign_char == '\0')
            return digital;
        else
            return digital + 1;
    }
    else {
        // when legnth <= 0 || legnth >= witdh, align to right or left is same
        if (length <= 0 || length >= (int)filed_width) {
            if (fill == FMT_FILL_SPACE) {
                // fill normal
                while (fill_cnt > 0) {
                    *buf++ = fill;
                    fill_cnt--;
                }

                // add sign
                if (sign_char != '\0')
                    *buf++ = (char)sign_char;
            }
            else {
                // add sign
                if (sign_char != '\0')
                    *buf++ = (char)sign_char;

                // fill normal
                while (fill_cnt > 0) {
                    *buf++ = fill;
                    fill_cnt--;
                }
            }
        }
        else {
            if ((flag & FMT_ALIGN_LEFT) == 0) {
                // align to right, when (length < filed_width)
                jimic_assert(length < (int)filed_width);

                padding = length - digital;
                if (padding > 0) {
                    // fill right padding space
                    while (fill_cnt > padding) {
                        *buf++ = ' ';
                        fill_cnt--;
                    }

                    // add sign
                    if (sign_char != '\0')
                        *buf++ = (char)sign_char;

                    // fill normal
                    while (fill_cnt > 0) {
                        *buf++ = fill;
                        fill_cnt--;
                    }
                }
                else {
                    // fill right padding space
                    while (fill_cnt > 0) {
                        *buf++ = ' ';
                        fill_cnt--;
                    }

                    // add sign
                    if (sign_char != '\0')
                        *buf++ = (char)sign_char;
                }
            }
            else {
                // align to left, when (length < filed_width)
                jimic_assert(length < (int)filed_width);

                // add sign
                if (sign_char != '\0')
                    *buf++ = (char)sign_char;

                // fill normal
                padding = length - digital;
                if (padding > 0) {
                    fill_cnt -= padding;
                    while (padding > 0) {
                        *buf++ = fill;
                        padding--;
                    }
                }

#if 0
                do {
                    ++cur;
                    *buf++ = *cur;
                } while (cur != end);
#else
                cur++;
                while (cur <= end)
                    *buf++ = *cur++;
#endif
                // fill left padding space
                while (fill_cnt > 0) {
                    *buf++ = ' ';
                    fill_cnt--;
                }

                goto u64toa_r10_ex_exit;
            }
        }
    }

#if 0
    do {
        ++cur;
        *buf++ = *cur;
    } while (cur != end);
#else
    cur++;
    while (cur <= end)
        *buf++ = *cur++;
#endif

u64toa_r10_ex_exit:
    *buf = '\0';

    return filed_width;
}

JMC_INLINE_NONSTD(int)
jmc_i64toa_r10_ex(char *buf, size_t count, int64_t val, unsigned int flag,
                      unsigned int fill, unsigned int filed_width, int length)
{
    if (val < 0) {
        flag |= FMT_SIGN_MASK;
        val = -val;
    }
    return jmc_u64toa_r10_ex(buf, count, val, flag, fill, filed_width, length);
}

JMC_INLINE_NONSTD(int)
jmc_u64toa_r10_integer(char *buf, uint64_t val, int sign,
                                    unsigned int filed_width)
{
    unsigned int digval, digital;
    uint32_t val32;
    char *cur;
    int padding;
    fuint64_t *u64;
    char digits[32];    // 实际最多只会用到20个bytes
    const unsigned int fill = ' ';

    cur = digits;
#if 0
    if (val <= (uint64_t)JIMIC_UINT_MAX64) {
        val32 = (uint32_t)val;
#else
    u64 = (fuint64_t *)&val;
    if (u64->high == 0) {
        val32 = (uint32_t)u64->low;
#endif
        do {
            digval = val32 % 10;
            val32 /= 10;

            *cur++ = (char)(digval + '0');
        } while (val32 != 0);
    }
    else {
        do {
            digval = val % 10;
            val /= 10;

            *cur++ = (char)(digval + '0');
        } while (val != 0);
    }

#if 1
    digital = (unsigned int)(cur - digits) + sign;
    padding = filed_width - digital;
    if (padding < 0) {
        filed_width = digital;
    }
    else {
        // align to right, fill ' ' into right padding space
        while (padding > 0) {
            // fill is ' '
            *buf++ = (char)fill;
            padding--;
        }
    }
#else
    digital = cur - digits;
    if (sign == 0) {
        padding = filed_width - digital;
        if (padding < 0)
            filed_width = digital;
    }
    else {
        padding = filed_width - digital - 1;
        if (padding < 0)
            filed_width = digital + 1;
    }

    // align to right
    while (padding > 0) {
        // fill is ' '
        *buf++ = (char)fill;
        padding--;
    }
#endif

    if (sign != 0)
        *buf++ = (char)'-';

#if 0
    do {
        --cur;
        *buf++ = *cur;
    } while (cur != digits);
#else
    cur--;
    while (cur >= digits)
        *buf++ = *cur--;
#endif

    *buf = '\0';
    return filed_width;
}

JMC_INLINE_NONSTD(int)
jmc_i64toa_r10_integer(char *buf, int64_t val,
                                    unsigned int filed_width)
{
#if 0
    if (val >= 0)
        return jmc_u64toa_r10_integer(buf,  val, 0, filed_width);
    else
        return jmc_u64toa_r10_integer(buf, -val, 1, filed_width);   
#else
    int sign;
    if (val >= 0) {
        sign = 0;
    }
    else {
        sign = 1;
        val = -val;
    }
    return jmc_u64toa_r10_integer(buf, val, sign, filed_width);
#endif
}

JMC_INLINE_NONSTD(int)
jmc_u64toa_r10_frac(char *buf, uint64_t val, unsigned int precision)
{
    unsigned int digval, digital;
    uint32_t val32;
    char *cur;
    int padding;
    fuint64_t *u64;
    char digits[32];    // 实际最多只会用到20个bytes
    const unsigned int fill = '0';

    cur = digits;
#if 0
    if (val <= (uint64_t)JIMIC_UINT_MAX64) {
        val32 = (uint32_t)val;
#else
    u64 = (fuint64_t *)&val;
    if (u64->high == 0) {
        val32 = (uint32_t)u64->low;
#endif
        do {
            digval = val32 % 10;
            val32 /= 10;

            *cur++ = (char)(digval + '0');
        } while (val32 != 0);
    }
    else {
        do {
            digval = val % 10;
            val /= 10;

            *cur++ = (char)(digval + '0');
        } while (val != 0);
    }

    digital = (unsigned int)(cur - digits);

#if 0
    do {
        --cur;
        *buf++ = *cur;
    } while (cur != digits);
#else
    cur--;
    while (cur >= digits)
        *buf++ = *cur--;
#endif

    padding = precision - digital;
    if (padding <= 0) {
        *buf = '\0';
        return digital;
    }
    else {
        // fractional part's tail is like as "0000000...."

        while (padding > 0) {
            // fill is '0'
            *buf++ = (char)fill;
            padding--;
        }

        *buf = '\0';
        return precision;
    }
}

JMC_INLINE_NONSTD(int)
jmc_isnan_f(float val)
{
    uint32_t *u32;
#if defined(_DEBUG)
    uint32_t exponent;
#endif

    static_assert(sizeof(uint32_t) == sizeof(float), "jmc_isnan_f() maybe have some error!");

    if (sizeof(uint32_t) == sizeof(float)) {
        u32 = (uint32_t *)&val;
#if defined(_DEBUG)
        exponent = *u32 & JM_FLOAT_EXPONENT_MASK;
#endif
        if (((*u32 & JM_FLOAT_EXPONENT_MASK) == JM_FLOAT_EXPONENT_MASK)
            && ((*u32 & JM_FLOAT_MANTISSA_MASK) != 0))
            return 1;
        else
            return 0;
    }
    else {
        // maybe have some error!
        jimic_assert(sizeof(uint32_t) == sizeof(float));
        return -1;
    }
}

JMC_INLINE_NONSTD(int)
jmc_isinf_f(float val)
{
    uint32_t *u32;
#if defined(_DEBUG)
    uint32_t exponent;
#endif

    static_assert(sizeof(uint32_t) == sizeof(float), "jmc_isinf_f() maybe have some error!");

    if (sizeof(uint32_t) == sizeof(float)) {
        u32 = (uint32_t *)&val;
#if defined(_DEBUG)
        exponent = *u32 & JM_FLOAT_EXPONENT_MASK;
#endif
        if (((*u32 & JM_FLOAT_EXPONENT_MASK) == JM_FLOAT_EXPONENT_MASK)
            && ((*u32 & JM_FLOAT_MANTISSA_MASK) == 0))
            return 1;
        else
            return 0;
    }
    else {
        // maybe have some error!
        jimic_assert(sizeof(uint32_t) == sizeof(float));
        return -1;
    }
}

JMC_INLINE_NONSTD(int)
jmc_is_nan_or_inf_f(float val)
{
    uint32_t *u32;

    static_assert(sizeof(uint32_t) == sizeof(float), "jmc_is_nan_or_inf_f() maybe have some error!");

    if (sizeof(uint32_t) == sizeof(float)) {
        u32 = (uint32_t *)&val;
        if ((*u32 & JM_FLOAT_EXPONENT_MASK) == JM_FLOAT_EXPONENT_MASK)
            return 1;
        else
            return 0;
    }
    else {
        // maybe have some error!
        jimic_assert(sizeof(uint32_t) == sizeof(float));
        return -1;
    }
}

JMC_INLINE_NONSTD(int)
jmc_isnan_d(double val)
{
    fuint64_t *f64;
#if defined(_DEBUG)
    uint32_t exponent;
#endif

    static_assert(sizeof(fuint64_t) == sizeof(double), "jmc_isnan_d() maybe have some error!");

    if (sizeof(fuint64_t) == sizeof(double)) {
        f64 = (fuint64_t *)&val;
#if defined(_DEBUG)
        exponent = f64->high & JM_DOUBLE_EXPONENT_MASK32;
#endif
        if (((f64->high & JM_DOUBLE_EXPONENT_MASK32) == JM_DOUBLE_EXPONENT_MASK32)
            && (((f64->high & JM_DOUBLE_MANTISSA_MASK_HIGH) != 0)
            || ((f64->low & JM_DOUBLE_MANTISSA_MASK_LOW) != 0)))
            return 1;
        else
            return 0;
    }
    else {
        // maybe have some error!
        jimic_assert(sizeof(fuint64_t) == sizeof(double));
        return -1;
    }
}

JMC_INLINE_NONSTD(int)
jmc_isinf_d(double val)
{
    fuint64_t *f64;
#if defined(_DEBUG)
    uint32_t exponent;
#endif

    static_assert(sizeof(fuint64_t) == sizeof(double), "jmc_isinf_d() maybe have some error!");

    if (sizeof(fuint64_t) == sizeof(double)) {
        f64 = (fuint64_t *)&val;
#if defined(_DEBUG)
        exponent = f64->high & JM_DOUBLE_EXPONENT_MASK32;
#endif
        if (((f64->high & JM_DOUBLE_EXPONENT_MASK32) == JM_DOUBLE_EXPONENT_MASK32)
            && (((f64->high & JM_DOUBLE_MANTISSA_MASK_HIGH) == 0)
            && ((f64->low & JM_DOUBLE_MANTISSA_MASK_LOW) == 0)))
            return 1;
        else
            return 0;
    }
    else {
        // maybe have some error!
        jimic_assert(sizeof(fuint64_t) == sizeof(double));
        return -1;
    }
}

JMC_INLINE_NONSTD(int)
jmc_is_nan_or_inf_d(double val)
{
    fuint64_t *f64;

    static_assert(sizeof(fuint64_t) == sizeof(double), "jmc_is_nan_or_inf_d() maybe have some error!");

    if (sizeof(fuint64_t) == sizeof(double)) {
        f64 = (fuint64_t *)&val;
        if ((f64->high & JM_DOUBLE_EXPONENT_MASK32) == JM_DOUBLE_EXPONENT_MASK32)
            return 1;
        else
            return 0;
    }
    else {
        // maybe have some error!
        jimic_assert(sizeof(fuint64_t) == sizeof(double));
        return -1;
    }
}

//
// Zero:   exponent(指数)为0, 且mantissa(尾数)为0
//
// 下溢数: exponent(指数)为0, 且mantissa(尾数)不为0
//
// +/-INF: 无穷大或无穷小, exponent(指数)为最大值, 且mantissa(尾数)为0
//
// NaN:    Not a Number, exponent(指数)为最大值, 且mantissa(尾数)不为0
//

JMC_INLINE_NONSTD(int)
jmc_ftest(float val)
{
    uint32_t *u32;
    uint32_t exponent;

    static_assert(sizeof(uint32_t) == sizeof(float), "jmc_ftest() maybe have some error!");

    if (sizeof(uint32_t) == sizeof(float)) {
        u32 = (uint32_t *)&val;
        exponent = *u32 & JM_FLOAT_EXPONENT_MASK;
        if (exponent == JM_FLOAT_EXPONENT_MASK) {
            if ((*u32 & JM_FLOAT_MANTISSA_MASK) != 0)
                return (JM_FIS_NAN_OR_INF | JM_FIS_NAN);
            else
                return (JM_FIS_NAN_OR_INF | JM_FIS_INF);
        }
        else if (exponent == 0) {
            if ((*u32 & JM_FLOAT_MANTISSA_MASK) == 0)
                return JM_FIS_ZERO;
            else
                return JM_FIS_SUBNORMAL;
        }
        return JM_FIS_NORMAL;
    }
    else {
        // maybe have some error!
        jimic_assert(sizeof(uint32_t) == sizeof(float));
        return JM_FIS_NORMAL;
    }
}

//
// Zero:   exponent(指数)为0, 且mantissa(尾数)为0
//
// 下溢数: exponent(指数)为0, 且mantissa(尾数)不为0
//
// +/-INF: 无穷大或无穷小, exponent(指数)为最大值, 且mantissa(尾数)为0
//
// NaN:    Not a Number, exponent(指数)为最大值, 且mantissa(尾数)不为0
//
// Reference: http://zh.wikipedia.org/wiki/IEEE_754
// Reference: http://zh.wikipedia.org/wiki/%E9%9B%99%E7%B2%BE%E5%BA%A6%E6%B5%AE%E9%BB%9E%E6%95%B8
//

JMC_INLINE_NONSTD(int)
jmc_dtest(double val)
{
    fuint64_t *f64;
    uint32_t exponent;

    static_assert(sizeof(fuint64_t) == sizeof(double), "jmc_dtest() maybe have some error!");

    if (sizeof(fuint64_t) == sizeof(double)) {
        f64 = (fuint64_t *)&val;
        exponent = f64->high & JM_DOUBLE_EXPONENT_MASK32;
        if (exponent == JM_DOUBLE_EXPONENT_MASK32) {
            if (((f64->high & JM_DOUBLE_MANTISSA_MASK_HIGH) != 0)
                || ((f64->low & JM_DOUBLE_MANTISSA_MASK_LOW) != 0))
                return (JM_FIS_NAN_OR_INF | JM_FIS_NAN);
            else
                return (JM_FIS_NAN_OR_INF | JM_FIS_INF);
        }
        else if (exponent == 0) {
            if (((f64->high & JM_DOUBLE_MANTISSA_MASK_HIGH) == 0)
                && ((f64->low & JM_DOUBLE_MANTISSA_MASK_LOW) == 0))
                return JM_FIS_ZERO;
            else
                return JM_FIS_SUBNORMAL;
        }
        return JM_FIS_NORMAL;
    }
    else {
        // maybe have some error!
        jimic_assert(sizeof(fuint64_t) == sizeof(double));
        return JM_FIS_NORMAL;
    }
}

JMC_INLINE_NONSTD(int)
jmc_ptohex(char *buf, void *p)
{
    return 0;
}

JMC_INLINE_NONSTD(int)
jmc_uitohex(char *buf, uint32_t val, int isUpper)
{
    return 0;
}

JMC_INLINE_NONSTD(int)
jmc_ui64tohex(char *buf, uint64_t val)
{
    return 0;
}

JMC_INLINE_NONSTD(size_t)
jmc_strlen(char *str)
{
    return jm_strlen(str);
}

JMC_INLINE_NONSTD(size_t)
jmc_strcpy(char *dest, jm_const char *src)
{
    return 0;
}

JMC_INLINE_NONSTD(size_t)
jmc_strncpy(char *dest, size_t countOfElements, jm_const char *src, size_t count)
{
    char *end;
    if (countOfElements == (size_t)-1) {
        // do nothing !!
    }
    else
        count = JIMIC_MIN(count, countOfElements - 1);
    end = (char *)src + count;
    while (src < end) {
        *dest++ = *src++;
    }
    //*dest = '\0';
    return count;
}

JMC_INLINE_NONSTD(size_t)
jmc_strncpy_null(char *dest, size_t countOfElements, jm_const char *src, size_t count)
{
    if (countOfElements == (size_t)-1) {
        // do nothing !!
    }
    else
        count = JIMIC_MIN(count, countOfElements - 1);
    // has including the null terminator.
    memcpy(dest, src, (count + 1) * sizeof(char));
    return count;
}

JMC_INLINE_NONSTD(size_t)
jmc_strncpy_fast(char *dest, size_t countOfElements, jm_const char *src, size_t count)
{
    if (countOfElements == (size_t)-1) {
        // do nothing !!
    }
    else
        count = JIMIC_MIN(count, countOfElements - 1);
    memcpy(dest, src, count * sizeof(char));
    return count;
}

JMC_INLINE_NONSTD(size_t)
jmc_out_null_string(char *dest, size_t countOfElements)
{
    /* Write "(null)" if there's space.  */
    if (countOfElements == (size_t)-1) {
        *(dest + 0)  = '(';
        *(dest + 1)  = 'n';
        *(dest + 2)  = 'u';
        *(dest + 3)  = 'l';
        *(dest + 4)  = 'l';
        *(dest + 5)  = ')';
        *(dest + 6)  = '\0';
        *(dest + 7)  = '\0';
        return 6;
    }
    else {
        if (countOfElements >= (size_t)sizeof(FMT_NULL_STRING)) {
            *(dest + 0)  = '(';
            *(dest + 1)  = 'n';
            *(dest + 2)  = 'u';
            *(dest + 3)  = 'l';
            *(dest + 4)  = 'l';
            *(dest + 5)  = ')';
            *(dest + 6)  = '\0';
            return 6;
        }
        else {
            *dest = '\0';
            return 0;
        }
    }
}

#if defined(JMC_STRNCPY_EX_INLINE_DECLARE) && (JMC_STRNCPY_EX_INLINE_DECLARE != 0)

/* implement source code for .cpp or .inl.h */
#include <jimic/string/jmc_strings.impl.inl>

#endif  /* JMC_STRNCPY_EX_INLINE_DECLARE */

#endif  /* !_JIMIC_STRING_JMC_STRIGNS_INL_H_ */
