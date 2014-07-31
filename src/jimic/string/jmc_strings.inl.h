
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

/* С�˻���, Ϊ1��ʾС�˴洢 */
#define JIMI_IS_LITTLE_ENDIAN           1

/* UINT�����ֵ */
#define JIMIC_UINT_MAX                  UINT_MAX

//
// Printf() �����ʽ����
// Reference: http://bbs.csdn.net/topics/330107715
//

#ifndef _JIMIC_STRING_SPRINTF_FORMAT_DEF_
#define _JIMIC_STRING_SPRINTF_FORMAT_DEF_

/* format sign */
#define FMT_DEFAULT_FLAG        0x00000000      // default flag
#define FMT_SPACE_FLAG          0x00010000      // ' ', space flag
#define FMT_PLUS_FLAG           0x00020000      // '+', plus flag
#define FMT_SHARP_FLAG          0x00040000      // '#', number flag
#define FMT_FIELDWIDTH_FLAG     0x00080000      // '*', asterisk flag
#define FMT_CURRENCY_FLAG       0x00100000      // ',', comma flag

#define FMT_SIGN_MASK           0x00000002      // sign mask
#define FMT_SIGN_MASK_BIT       2               // sign move bit

// ���: period

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

#endif  /* !_JIMIC_STRING_SPRINTF_FORMAT_DEF_ */

//
// ASCII����ձ�
// Reference: http://ascii.911cha.com/
//

#ifndef FMT_DEFAULT_DOUBLE_PRECISION
#define FMT_DEFAULT_DOUBLE_PRECISION    6
#endif

#ifndef FMT_MAX_DOUBLE_PRECISION
#define FMT_MAX_DOUBLE_PRECISION        20
#endif

#define JM_FIS_NORMAL                   0
#define JM_FIS_INF                      0x01
#define JM_FIS_NAN                      0x02
#define JM_FIS_NAN_OR_INF               0x04
#define JM_FIS_ZERO                     0x08
#define JM_FIS_SUBNORMAL                0x10

#define JM_FLOAT_SIGN_BIT               (0x80000000UL)
#define JM_FLOAT_EXPONENT_MASK          (0x7F800000UL)
#define JM_FLOAT_MANTISSA_MASK          (0x007FFFFFUL)

#define JM_DOUBLE_SIGN_BIT              (0x8000000000000000ULL)
#define JM_DOUBLE_EXPONENT_MASK         (0x7FF0000000000000ULL)
#define JM_DOUBLE_MANTISSA_MASK         (0x000FFFFFFFFFFFFFULL)

#define JM_DOUBLE_SIGN_BIT32            (uint32_t)(JM_DOUBLE_SIGN_BIT      >> 32)
#define JM_DOUBLE_EXPONENT_MASK32       (uint32_t)(JM_DOUBLE_EXPONENT_MASK >> 32)
#define JM_DOUBLE_MANTISSA_MASK_HIGH    (uint32_t)(JM_DOUBLE_MANTISSA_MASK >> 32)
#define JM_DOUBLE_MANTISSA_MASK_LOW     \
                        (uint32_t)(JM_DOUBLE_MANTISSA_MASK & 0x00000000FFFFFFFFULL)

typedef struct fuint64_s {
    union {
#if defined(JIMI_IS_LITTLE_ENDIAN) && (JIMI_IS_LITTLE_ENDIAN != 0)
        // С�˴洢
        struct {
            uint32_t    low;
            uint32_t    high;
        };
#else
        // ��˴洢
        struct {
            uint32_t    high;
            uint32_t    low;
        };
#endif  /* JIMI_IS_LITTLE_ENDIAN */
        uint64_t        u64;
        double          d;
    };
} fuint64_s;

typedef struct fvariant_t {
    union {
        fuint64_s       f64;
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
jmc_utoa_radix10(jm_char *buf, unsigned int val)
{
    unsigned int digval, digital;
    const jm_char *end;
    jm_char *cur;
    char digits[16];    // ʵ�����ֻ���õ�10��bytes

    jimic_assert(buf != NULL);

    end = digits + jm_countof(digits);
    cur = (jm_char *)end;
    do {
        cur--;
        digval = val % 10;
        val /= 10;

        *cur = (jm_char)(digval + '0');
    } while (val != 0);

    digital = end - cur;
    digval = digital;

#if 1
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
jmc_utoa_radix10(jm_char *buf, unsigned int val)
{
    unsigned int digval, digital;
    jm_char *end, *cur;
    char digits[16];    // ʵ�����ֻ���õ�10��bytes

    jimic_assert(buf != NULL);

    end = digits + jm_countof(digits) - 1;
    cur = end;
    do {
        digval = val % 10;
        val /= 10;

        *cur-- = (jm_char)(digval + '0');
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
jmc_utoa_radix10(jm_char *buf, unsigned int val)
{
    unsigned int digval, digital;
    jm_char *cur;
    char digits[16];    // ʵ�����ֻ���õ�10��bytes

    jimic_assert(buf != NULL);

    cur = digits;
    do {
        digval = val % 10;
        val /= 10;

        *cur++ = (jm_char)digval + '0';
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
jmc_itoa_radix10(jm_char *buf, int val)
{
#if 1
    if (val >= 0) {
        return jmc_utoa_radix10(buf, val);
    }
    else {
        *buf++ = '-';
        val = -val;
        return jmc_utoa_radix10(buf, val) + 1;
    }
#elif 0
    if (val < 0) {
        *buf++ = '-';
        val = -val;
        return jmc_utoa_radix10(buf, val) + 1;
    }
    else
        return jmc_utoa_radix10(buf, val);
#else
    register int sign;
    sign = (val < 0);
    if (val < 0) {
        *buf++ = '-';
        val = -val;
    }
    return jmc_utoa_radix10(buf, val) + sign;
#endif
}

JMC_INLINE_NONSTD(int)
jmc_ultoa_radix10(jm_char *buf, unsigned long val)
{
    unsigned long digval, digital;
    jm_char *cur;
    char digits[16];    // ʵ�����ֻ���õ�10��bytes

    cur = digits;
    do {
        digval = val % 10;
        val /= 10;

        *cur++ = (jm_char)digval + '0';
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

JMC_INLINE_NONSTD(int)
jmc_ltoa_radix10(jm_char *buf, long val)
{
#if 1
    if (val >= 0) {
        return jmc_ultoa_radix10(buf, val);
    }
    else {
        *buf++ = '-';
        val = -val;
        return jmc_ultoa_radix10(buf, val) + 1;
    }        
#else
    int sign;
    sign = (val < 0);
    if (val < 0) {
        *buf++ = '-';
        val = -val;
    }
    return jmc_ultoa_radix10(buf, val) + sign;
#endif
}

JMC_INLINE_NONSTD(int)
jmc_u64toa_radix10(jm_char *buf, uint64_t val)
{
    unsigned int digval, digital;
    uint32_t val32;
    jm_char *cur;
    char digits[32];    // ʵ�����ֻ���õ�20��bytes

    cur = digits;
    if (val <= (uint64_t)UINT_MAX) {
        val32 = (uint32_t)val;
        do {
            digval = val32 % 10;
            val32 /= 10;

            *cur++ = (jm_char)(digval + '0');
        } while (val32 != 0);
    }
    else {
        do {
            digval = val % 10;
            val /= 10;

            *cur++ = (jm_char)(digval + '0');
        } while (val != 0);
    }

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

JMC_INLINE_NONSTD(int)
jmc_i64toa_radix10(jm_char *buf, int64_t val)
{
#if 1
    if (val >= 0) {
        return jmc_u64toa_radix10(buf, val);
    }
    else {
        *buf++ = '-';
        val = -val;
        return jmc_u64toa_radix10(buf, val) + 1;
    }        
#else
    int sign;
    sign = (val < 0);
    if (val < 0) {
        *buf++ = '-';
        val = -val;
    }
    return jmc_u64toa_radix10(buf, val) + sign;
#endif
}

JMC_INLINE_NONSTD(int)
jmc_utoa_radix10_ex(jm_char *buf, size_t count, unsigned int val, unsigned int flag,
                    unsigned int fill, unsigned int width, int length)
{
    unsigned int digval, digital;
    int fill_cnt;
    int padding;
    jm_char *end, *cur;
    int sign_char;
    char digits[16];    // ʵ�����ֻ���õ�10��bytes

    jimic_assert(buf != NULL);
    jimic_assert(count != 0);

    end = digits + jm_countof(digits) - 1;
    cur = end;
    do {
        digval = val % 10;
        val /= 10;

        *cur-- = (jm_char)(digval + '0');
    } while (val != 0);

    digital = end - cur;

    if ((flag & (FMT_SIGN_MASK | FMT_SPACE_FLAG | FMT_SHARP_FLAG)) == 0) {
        sign_char = '\0';
        fill_cnt = width - digital;
#if 0
        if (fill_cnt > 0) {
            goto utoa_radix10_ex_L001;
        }
        else {
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
#endif
    }
    else {
        if ((flag & FMT_SPACE_FLAG) == 0) {
            // '+' is 0x2B, '-' is 0x2D
            sign_char = '+' + (flag & FMT_SIGN_MASK);
            fill_cnt = width - digital - 1;
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
            fill_cnt = width - digital - 1;
        }
    }

    if (fill_cnt <= 0) {
        // add sign
        if (sign_char != '\0')
            *buf++ = (jm_char)sign_char;
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
//utoa_radix10_ex_L001:
        // when legnth <= 0 || legnth >= witdh, align to right or left is same
        if (length <= 0 || length >= (int)width) {
            // add sign
            if (sign_char != '\0')
                *buf++ = (jm_char)sign_char;

            // fill normal
            do {
                *buf++ = fill;
                fill_cnt--;
            } while (fill_cnt > 0);
        }
        else {
            if ((flag & FMT_ALIGN_LEFT) == 0) {
                // align to right, when (length < width)
                jimic_assert(length < (int)width);

                padding = length - digital;
                if (padding > 0) {
                    // fill right padding space
                    while (fill_cnt > padding) {
                        *buf++ = ' ';
                        fill_cnt--;
                    }

                    // add sign
                    if (sign_char != '\0')
                        *buf++ = (jm_char)sign_char;

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
                        *buf++ = (jm_char)sign_char;
                }
            }
            else {
                // align to left, when (length < width)
                jimic_assert(length < (int)width);

                // add sign
                if (sign_char != '\0')
                    *buf++ = (jm_char)sign_char;

                // fill normal
                padding = length - digital;
                if (padding > 0) {
                    fill_cnt -= padding;
                    do {
                        *buf++ = fill;
                        padding--;
                    } while (padding > 0);
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

                goto utoa_radix10_ex_exit;
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

utoa_radix10_ex_exit:
    *buf = '\0';

    return width;
}

JMC_INLINE_NONSTD(int)
jmc_itoa_radix10_ex(jm_char *buf, size_t count, int val, unsigned int flag,
                    unsigned int fill, unsigned int width, int length)
{
#if 1
    if (val < 0) {
        flag |= FMT_SIGN_MASK;
        val = -val;
    }
    return jmc_utoa_radix10_ex(buf, count, val, flag, fill, width, length);
#elif 0
    if ((flag & FMT_SPACE_FLAG) == 0) {
        if (val >= 0) {
            return jmc_utoa_radix10_ex(buf, count, val, flag, fill, width, length);
        }
        else {
            *buf++ = '-';
            val = -val;
            flag |= FMT_SIGN_MASK;
            return jmc_utoa_radix10_ex(buf, count, val, flag, fill, width, length) + 1;
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
        return jmc_utoa_radix10_ex(buf, count, val, flag, fill, width, length) + 1;
    }
#elif 0
    if ((flag & FMT_SPACE_FLAG) == 0) {
        if (val < 0) {
            *buf++ = '-';
            val = -val;
            return jmc_utoa_radix10_ex(buf, count, val, flag, fill, width, length) + 1;
        }
        else
            return jmc_utoa_radix10_ex(buf, count, val, flag, fill, width, length);
    }
    else {
        if (val >= 0) {
            *buf++ = '+';
        }
        else {
            *buf++ = '-';
            val = -val;
        }
        return jmc_utoa_radix10_ex(buf, count, val, flag, fill, width, length) + 1;
    }
#else
    int sign;
    if ((flag & FMT_SPACE_FLAG) == 0) {
        sign = (val < 0);
        if (val < 0) {
            *buf++ = '-';
            val = -val;
        }
        return jmc_utoa_radix10_ex(buf, count, val, flag, fill, width, length) + sign;
    }
    else {
        if (val < 0) {
            *buf++ = '-';
            val = -val;
        }
        else {
            *buf++ = '+';
        }
        return jmc_utoa_radix10_ex(buf, count, val, flag, fill, width, length) + 1;
    }
#endif
}

JMC_INLINE_NONSTD(int)
jmc_u64toa_radix10_ex(jm_char *buf, size_t count, uint64_t val, unsigned int flag,
                      unsigned int fill, unsigned int width, int length)
{
    unsigned int digval, digital;
    uint32_t val32;
    jm_char *cur;
    int sign_char;
    int padding;
    char digits[32];    // ʵ�����ֻ���õ�20��bytes

    cur = digits;
    if (val <= (uint64_t)JIMIC_UINT_MAX) {
        val32 = (uint32_t)val;
        do {
            digval = val32 % 10;
            val32 /= 10;

            *cur++ = (jm_char)(digval + '0');
        } while (val32 != 0);
    }
    else {
        do {
            digval = val % 10;
            val /= 10;

            *cur++ = (jm_char)(digval + '0');
        } while (val != 0);
    }

    digital = cur - digits;

    if ((flag & FMT_SIGN_MASK) == 0) {
        sign_char = '\0';
        padding = width - digital;
        if (padding < 0)
            width = digital;
    }
    else {
        sign_char = '-';
        padding = width - digital - 1;
        if (padding < 0)
            width = digital + 1;
    }

    if ((flag & FMT_ALIGN_LEFT) == 0) {
        // align to right
        while (padding > 0) {
            *buf++ = (jm_char)fill;
            padding--;
        }
    }

    if (sign_char != '\0')
        *buf++ = sign_char;

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

    if (padding <= 0) {
        *buf = '\0';
        return width;
    }
    else {
        *buf++ = '0';
        padding--;

        while (padding > 0) {
            *buf++ = '0';
            padding--;
        }

        *buf = '\0';
        return width;
    }
}

JMC_INLINE_NONSTD(int)
jmc_i64toa_radix10_ex(jm_char *buf, size_t count, int64_t val, unsigned int flag,
                      unsigned int fill, unsigned int width, int length)
{
    if (val < 0) {
        flag |= FMT_SIGN_MASK;
        val = -val;
    }
    return jmc_u64toa_radix10_ex(buf, count, val, flag, fill, width, length);
}

JMC_INLINE_NONSTD(int)
jmc_u64toa_radix10_for_integer_part(jm_char *buf, uint64_t val, int sign,
                                    unsigned int filed_width)
{
    unsigned int digval, digital;
    uint32_t val32;
    jm_char *cur;
    int padding;
    char digits[32];    // ʵ�����ֻ���õ�20��bytes
    const unsigned int fill = ' ';

    cur = digits;
    if (val <= (uint64_t)JIMIC_UINT_MAX) {
        val32 = (uint32_t)val;
        do {
            digval = val32 % 10;
            val32 /= 10;

            *cur++ = (jm_char)(digval + '0');
        } while (val32 != 0);
    }
    else {
        do {
            digval = val % 10;
            val /= 10;

            *cur++ = (jm_char)(digval + '0');
        } while (val != 0);
    }

#if 1
    digital = (cur - digits) + sign;
    padding = filed_width - digital;
    if (padding < 0) {
        filed_width = digital;
    }
    else {
        // align to right, fill ' ' into right padding space
        while (padding > 0) {
            // fill is ' '
            *buf++ = (jm_char)fill;
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
        *buf++ = (jm_char)fill;
        padding--;
    }
#endif

    if (sign != 0)
        *buf++ = (jm_char)'-';

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
jmc_i64toa_radix10_for_integer_part(jm_char *buf, int64_t val,
                                    unsigned int filed_width)
{
#if 0
    if (val >= 0)
        return jmc_u64toa_radix10_for_integer_part(buf,  val, 0, filed_width);
    else
        return jmc_u64toa_radix10_for_integer_part(buf, -val, 1, filed_width);   
#else
    int sign;
    if (val >= 0) {
        sign = 0;
    }
    else {
        sign = 1;
        val = -val;
    }
    return jmc_u64toa_radix10_for_integer_part(buf, val, sign, filed_width);
#endif
}

JMC_INLINE_NONSTD(int)
jmc_u64toa_radix10_for_frac_part(jm_char *buf, uint64_t val, unsigned int precision)
{
    unsigned int digval, digital;
    uint32_t val32;
    jm_char *cur;
    int padding;
    char digits[32];    // ʵ�����ֻ���õ�20��bytes
    const unsigned int fill = '0';

    cur = digits;
    if (val <= (uint64_t)JIMIC_UINT_MAX) {
        val32 = (uint32_t)val;
        do {
            digval = val32 % 10;
            val32 /= 10;

            *cur++ = (jm_char)(digval + '0');
        } while (val32 != 0);
    }
    else {
        do {
            digval = val % 10;
            val /= 10;

            *cur++ = (jm_char)(digval + '0');
        } while (val != 0);
    }

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

    padding = precision - digital;
    if (padding <= 0) {
        *buf = '\0';
        return digital;
    }
    else {
        // fractional part's tail is like as "0000000...."

        // fill is '0'
        *buf++ = (jm_char)fill;
        padding--;

        while (padding > 0) {
            // fill is '0'
            *buf++ = (jm_char)fill;
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
#ifndef _MSC_VER
        #error "jmc_isnan_f() maybe have some error!"
#endif // _MSC_VER
        return 0;
    }
}

JMC_INLINE_NONSTD(int)
jmc_isinf_f(float val)
{
    uint32_t *u32;
#if defined(_DEBUG)
    uint32_t exponent;
#endif
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
#ifndef _MSC_VER
        #error "jmc_isinf_f() maybe have some error!"
#endif // _MSC_VER
        return 0;
    }
}

JMC_INLINE_NONSTD(int)
jmc_is_nan_or_inf_f(float val)
{
    uint32_t *u32;
    if (sizeof(uint32_t) == sizeof(float)) {
        u32 = (uint32_t *)&val;
        if ((*u32 & JM_FLOAT_EXPONENT_MASK) == JM_FLOAT_EXPONENT_MASK)
            return 1;
        else
            return 0;
    }
    else {
        // maybe have some error!
#ifndef _MSC_VER
        #error "jmc_is_nan_or_inf_f() maybe have some error!"
#endif // _MSC_VER
        return 0;
    }
}

JMC_INLINE_NONSTD(int)
jmc_isnan_d(double val)
{
    fuint64_s *f64;
#if defined(_DEBUG)
    uint32_t exponent;
#endif
    if (sizeof(fuint64_s) == sizeof(double)) {
        f64 = (fuint64_s *)&val;
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
#ifndef _MSC_VER
        #error "jmc_isnan_d() maybe have some error!"
#endif // _MSC_VER
        return 0;
    }
}

JMC_INLINE_NONSTD(int)
jmc_isinf_d(double val)
{
    fuint64_s *f64;
#if defined(_DEBUG)
    uint32_t exponent;
#endif
    if (sizeof(fuint64_s) == sizeof(double)) {
        f64 = (fuint64_s *)&val;
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
#ifndef _MSC_VER
        #error "jmc_isnan_d() maybe have some error!"
#endif // _MSC_VER
        return 0;
    }
}

JMC_INLINE_NONSTD(int)
jmc_is_nan_or_inf_d(double val)
{
    fuint64_s *f64;
    if (sizeof(fuint64_s) == sizeof(double)) {
        f64 = (fuint64_s *)&val;
        if ((f64->high & JM_DOUBLE_EXPONENT_MASK32) == JM_DOUBLE_EXPONENT_MASK32)
            return 1;
        else
            return 0;
    }
    else {
        // maybe have some error!
#ifndef _MSC_VER
        #error "jmc_is_nan_or_inf_d() maybe have some error!"
#endif // _MSC_VER
        return 0;
    }
}

//
// Zero:   exponent(ָ��)Ϊ0, ��mantissa(β��)Ϊ0
//
// ������: exponent(ָ��)Ϊ0, ��mantissa(β��)��Ϊ0
//
// +/-INF: ����������С, exponent(ָ��)Ϊ���ֵ, ��mantissa(β��)Ϊ0
//
// NaN:    Not a Number, exponent(ָ��)Ϊ���ֵ, ��mantissa(β��)��Ϊ0
//

JMC_INLINE_NONSTD(int)
jmc_ftest(float val)
{
    uint32_t *u32;
    uint32_t exponent;
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
#ifndef _MSC_VER
        #error "jmc_ftest() maybe have some error!"
#endif // _MSC_VER
        return JM_FIS_NORMAL;
    }
}

//
// Zero:   exponent(ָ��)Ϊ0, ��mantissa(β��)Ϊ0
//
// ������: exponent(ָ��)Ϊ0, ��mantissa(β��)��Ϊ0
//
// +/-INF: ����������С, exponent(ָ��)Ϊ���ֵ, ��mantissa(β��)Ϊ0
//
// NaN:    Not a Number, exponent(ָ��)Ϊ���ֵ, ��mantissa(β��)��Ϊ0
//
// Reference: http://zh.wikipedia.org/wiki/IEEE_754
// Reference: http://zh.wikipedia.org/wiki/%E9%9B%99%E7%B2%BE%E5%BA%A6%E6%B5%AE%E9%BB%9E%E6%95%B8
//

JMC_INLINE_NONSTD(int)
jmc_dtest(double val)
{
    fuint64_s *f64;
    uint32_t exponent;
    if (sizeof(fuint64_s) == sizeof(double)) {
        f64 = (fuint64_s *)&val;
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
#ifndef _MSC_VER
        #error "jmc_dtest() maybe have some error!"
#endif // _MSC_VER
        jimic_assert(sizeof(fuint64_s) == sizeof(double));
        return JM_FIS_NORMAL;
    }
}

JMC_INLINE_NONSTD(int)
jmc_ftos(jm_char *buf, float val, unsigned int width, int precision)
{
    return 0;
}

JMC_INLINE_NONSTD(int)
jmc_ftos_ex(jm_char *buf, size_t count, float val, unsigned int flag,
            unsigned int fill, unsigned int width, int precision)
{
    return 0;
}

JMC_INLINE_NONSTD(int)
jmc_dtos(jm_char *buf, double val, unsigned int filed_width, int precision)
{
    int len;
    int64_t i64;
    uint32_t scale32;
    uint64_t scale, frac;
    fuint64_s *f64;
    unsigned int n;
    int num_width;
#if 0
    static const uint32_t scales32[] = {
        1, 10, 100, 1000, 10000, 100000,
        1000000, 10000000, 100000000, 1000000000
    };
    static const uint64_t scales64[] = {
        10000000000,     100000000000,     1000000000000,     10000000000000,
        100000000000000, 1000000000000000, 10000000000000000, 100000000000000000,
        1000000000000000000, 10000000000000000000
    };
#endif

    if (sizeof(fuint64_s) != sizeof(double)) {
        // maybe have some error!
#ifndef _MSC_VER
        #error "jmc_dtos() maybe have some error!"
#endif // _MSC_VER
        jimic_assert(sizeof(fuint64_s) == sizeof(double));
        return 0;
    }

    if (precision < 0) {
        jimic_assert(precision < 0);
        precision = FMT_DEFAULT_DOUBLE_PRECISION;
        scale = 1000000;
        num_width = filed_width - FMT_DEFAULT_DOUBLE_PRECISION - 1;
    }
    else if (precision > 0) {
#if 0
        if (precision <= 10) {
            jimic_assert(precision >= 0 && precision <= 10);
            scale = scales32[precision];
        }
        else {
            jimic_assert(precision > 10 && precision <= 20);
            scale = scales64[precision - 11];
        }
#elif 1
        if (precision <= 10) {
            jimic_assert(precision >= 0 && precision <= 10);
            scale32 = 1;
            for (n = precision; n > 0; --n)
                scale32 *= 10;
            scale = scale32;
        }
        else {
            jimic_assert(precision > 10 && precision <= 20);
            scale = 10000000000;
            for (n = precision - 11; n > 0; --n)
                scale *= 10;
        }
#else
        jimic_assert(precision >= 0 && precision <= 20);
        scale = 1;
        for (n = precision; n > 0; --n)
            scale *= 10;
#endif
        num_width = filed_width - precision - 1;
    }
    else {
        scale = 1;
        num_width = filed_width;
    }

    f64 = (fuint64_s *)&val;
    // is NaN or INF ? (exponent is maxium ?)
    if ((f64->high & JM_DOUBLE_EXPONENT_MASK32) != JM_DOUBLE_EXPONENT_MASK32) {
        i64 = (int64_t)val;
        //if (val >= 0.0) {
        if (i64 >= 0) {
            frac = (uint64_t)((val - (double)i64) * scale + 0.5);
            if (frac == scale) {
                i64++;
                frac = 0;
            }
        }
        else {
            frac = (uint64_t)(((double)i64 - val) * scale + 0.5);
            if (frac == scale) {
                i64--;
                frac = 0;
            }
        }
#if 0
        if (((precision != 0) && ((int)filed_width <= (precision + 1 + 1)))
            || ((precision == 0) && (filed_width <= (0 + 1)))) {
#else
        if (num_width <= 1) {
#endif
            len = jmc_i64toa_radix10(buf, i64);
            buf += len;
        }
        else {
#if 1
            // for integer part of double
            len = jmc_i64toa_radix10_for_integer_part(buf, i64, num_width);
#else
            len = jmc_i64toa_radix10_ex(buf, -1, i64, FMT_ALIGN_RIGHT, ' ', num_width, 0);
#endif
            buf += len;
        }

        if (precision > 0) {
            // output decimal
            *buf++ = '.';
#if 1
            // for fractional part of double
            len += jmc_u64toa_radix10_for_frac_part(buf, frac, precision) + 1;
#else
            len += jmc_u64toa_radix10_ex(buf, -1, frac, FMT_ALIGN_LEFT, '0', precision, 0) + 1;
#endif
        }

        return len;
    }
    else if (((f64->high & JM_DOUBLE_MANTISSA_MASK_HIGH) != 0)
             || ((f64->low & JM_DOUBLE_MANTISSA_MASK_LOW) != 0)) {
        // is NaN, not a number
        *buf        = 'N';
        *(buf + 1)  = 'a';
        *(buf + 2)  = 'N';
        *(buf + 3)  = '\0';
        return 3;
    }
    else {
        // is +INF or -INF
        if ((f64->high & JM_DOUBLE_SIGN_BIT32) == 0) {
            // '+', 0x2B
            *buf        = 'I';
            *(buf + 1)  = 'n';
            *(buf + 2)  = 'f';
            *(buf + 3)  = '\0';
            return 3;
        }
        else {
            // '-', 0x2D
//          *buf        = '+' + ((f64->high & JM_DOUBLE_SIGN_BIT32) >> 30);
            *buf        = '-';
            *(buf + 1)  = 'I';
            *(buf + 2)  = 'n';
            *(buf + 3)  = 'f';
            *(buf + 4)  = '\0';
            return 4;
        }
    }
}

JMC_INLINE_NONSTD(int)
jmc_dtos_ex(jm_char *buf, size_t count, double val, unsigned int flag,
            unsigned int fill, unsigned int width, int precision)
{
    return 0;
}

JMC_INLINE_NONSTD(int)
jmc_ptohex(jm_char *buf, void *p)
{
    return 0;
}

JMC_INLINE_NONSTD(int)
jmc_uitohex(jm_char *buf, uint32_t val, int isUpper)
{
    return 0;
}

JMC_INLINE_NONSTD(int)
jmc_ui64tohex(jm_char *buf, uint64_t val)
{
    return 0;
}

JMC_INLINE_NONSTD(size_t)
jmc_strlen(jm_char *str)
{
    return jm_strlen(str);
}

JMC_INLINE_NONSTD(size_t)
jmc_strcpy(jm_char *dest, JM_CONST jm_char *src)
{
    return 0;
}

JMC_INLINE_NONSTD(size_t)
jmc_strncpy(jm_char *dest, size_t countOfElements, JM_CONST jm_char *src, size_t count)
{
    return 0;
}

JMC_INLINE_NONSTD(size_t)
jmc_strncpy_fast(jm_char *dest, size_t countOfElements, JM_CONST jm_char *src, size_t count)
{
    jm_char *end;
    count = JIMIC_MIN(count, countOfElements - 1);
    end = (jm_char *)src + count;
    while (src < end) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return count;
}

JMC_INLINE_NONSTD(size_t)
jmc_strncpy_ex(jm_char *dest, size_t countOfElements, JM_CONST jm_char *src, size_t count,
               unsigned int flag, unsigned int fill, unsigned int width, int length)
{
    jm_char *end;
    size_t copy_len, len;
    int fill_cnt, padding;
    count = JIMIC_MIN(count, countOfElements - 1);
    if (length < 0) length = 0;
    copy_len = JIMIC_MIN(JIMIC_MIN((unsigned int)length, width), count);
    len = JIMIC_MAX(width, count);
    end = (jm_char *)src + copy_len;

    fill_cnt = width - copy_len;
    if (fill_cnt <= 0) {
        // copy from src
        while (src < end) {
            *dest++ = *src++;
        }
        *dest = '\0';
        return copy_len;
    }
    else {
        // when legnth == 0 || legnth >= witdh, align to right or left is same
        if (length == 0 || length >= (int)width) {
            // fill normal
            do {
                *dest++ = fill;
                fill_cnt--;
            } while (fill_cnt > 0);
        }
        else {
            if ((flag & FMT_ALIGN_LEFT) == 0) {
                // align to right, when (length < width)
                jimic_assert(length < (int)width);

                // fill right padding space
                padding = length - count;
                if (padding > 0) {
                    // fill right padding space
                    while (fill_cnt > padding) {
                        *dest++ = ' ';
                        fill_cnt--;
                    }

                    // fill normal
                    while (fill_cnt > 0) {
                        *dest++ = fill;
                        fill_cnt--;
                    }
                }
                else {
                    // fill right padding space
                    while (fill_cnt > 0) {
                        *dest++ = ' ';
                        fill_cnt--;
                    }
                }
            }
            else {
                // align to left, when (length < width)
                jimic_assert(length < (int)width);

                // fill normal
                padding = length - count;
                if (padding > 0) {
                    fill_cnt -= padding;
                    do {
                        *dest++ = fill;
                        padding--;
                    } while (padding > 0);
                }

                // copy from src
                while (src < end) {
                    *dest++ = *src++;
                }

                // fill left padding space
                while (fill_cnt > 0) {
                    *dest++ = ' ';
                    fill_cnt--;
                }
                goto jmc_strncpy_ex_exit;
            }
        }
    }

    // copy from src
    while (src < end) {
        *dest++ = *src++;
    }

jmc_strncpy_ex_exit:
    *dest = '\0';
    return len;
}

#endif  /* !_JIMIC_STRING_JMC_STRIGNS_INL_H_ */