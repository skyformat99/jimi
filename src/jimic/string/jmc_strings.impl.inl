
#ifndef _JIMIC_STRING_JMC_STRINGS_IMPL_INL_
#define _JIMIC_STRING_JMC_STRINGS_IMPL_INL_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef _MSC_VER
    #error "jmc_strings.impl.inl only can include in jmc_strings.inl.h or jmc_strings.c!"
#endif // _MSC_VER

#include <jimic/string/jmc_strings.h>
#include <jimic/string/jm_strings.h>

#include <stdarg.h>
#include <math.h>       // for isnan(), isinf()
#include <float.h>
#include <limits.h>     // for UINT_MAX

static const double pow10_table[] = {
    1.0E+19,  1.0E+38,  1.0E+57,  1.0E+76,
    1.0E+95,  1.0E+114, 1.0E+133, 1.0E+152,
    1.0E+171, 1.0E+190, 1.0E+209, 1.0E+228,
    1.0E+247, 1.0E+266, 1.0E+285, 1.0E+304
};

static const double pow10_remain_table[] = {
    1.0E+4, 1.0E+8, 1.0E+12, 1.0E+16
};

JMC_DECLARE_NONSTD(int)
jmc_log10(double val)
{
    fuint64_t *f64;
    int exponent;
    int exp10;
    int exp10_remain;

    f64 = (fuint64_t *)&val;
    // exponent = (exponent_mask32 >> (52 - 30)) - 1023;
    exponent = ((f64->high & JM_DOUBLE_EXPONENT_MASK32) >> JM_DOUBLE_EXPONENT_SHIFT32)
        - JM_DOUBLE_EXPONENT_0_32;

    // exponent is positive (exponent >= 0 && exponent <= 1024)
    if (exponent >= 0 && exponent <= JM_DOUBLE_EXPONENT_MAX) {
        // exp10 = (exponent / 64) * 19;
        exp10 = (exponent >> 6) * 19;
        // exp10_remain = (exponent & 63) * 19 / 64 / 4 * 4;
        //exp10_remain = (((exponent & 63) * 19) >> 8);
        //exp10 += (exp10_remain << 2);
        exp10_remain = (((exponent & 63) * 19) >> 6);
        exp10 += exp10_remain;
    }
    else {
        exp10 = 0;
    }
    return exp10;
}

/* log10(2^64) = 19.265919722494796493679289262368 */

JMC_DECLARE_NONSTD(int)
jmc_log10_fast1(double val)
{
    fuint64_t *f64;
    int exponent;
    int exp10;
    int exp10_remain;

    f64 = (fuint64_t *)&val;
    // exponent = (exponent_mask32 >> (52 - 30)) - 1023;
    exponent = ((f64->high & JM_DOUBLE_EXPONENT_MASK32) >> JM_DOUBLE_EXPONENT_SHIFT32)
        - JM_DOUBLE_EXPONENT_0_32;

    // exponent is positive (exponent >= 0 && exponent <= 1024)
    if (exponent >= 0 && exponent <= JM_DOUBLE_EXPONENT_MAX) {
        // must 19,265,919 < 33,554,432 ( 2^32 / 64 / 2)
        // exp10 = (exponent / 64) * 19265919;
        exp10 = (exponent >> 6) * 19265919;
        // exp10_remain = (exponent & 63) * 19265919 / 64 / 4 * 4;
        //exp10_remain = (((exponent & 63) * 19265919) >> 8);
        //exp10 += (exp10_remain << 2);
        exp10_remain = (((exponent & 63) * 19265919) >> 6);
        exp10 += exp10_remain;
        exp10 = exp10 / 1000000;
    }
    else {
        exp10 = 0;
    }
    return exp10;
}

/**
 * log10(2^64) = 19.265919722494796493679289262368
 * 1048576 / 1000000 = 1.048576
 * log10(2^64) * 1048576 / 1000000 = 20.201781038934703728156254417576
 */

JMC_DECLARE_NONSTD(int)
jmc_log10_fast2(double val)
{
    fuint64_t *f64;
    int exponent;
    int exp10;
    int exp10_remain;

    f64 = (fuint64_t *)&val;
    // exponent = (exponent_mask32 >> (52 - 30)) - 1023;
    exponent = ((f64->high & JM_DOUBLE_EXPONENT_MASK32) >> JM_DOUBLE_EXPONENT_SHIFT32)
        - JM_DOUBLE_EXPONENT_0_32;

    // exponent is positive (exponent >= 0 && exponent <= 1024)
    if (exponent >= 0 && exponent <= JM_DOUBLE_EXPONENT_MAX) {
        // must 20,201,781 < 33,554,432 ( 2^32 / 64 / 2)
        // exp10 = (exponent / 64) * 20201781;
        exp10 = (exponent >> 6) * 20201781;
        // exp10_remain = (exponent & 63) * 20201781 / 64 / 4 * 4;
        //exp10_remain = (((exponent & 63) * 20201781) >> 8);
        //exp10 += (exp10_remain << 2);
        exp10_remain = (((exponent & 63) * 20201781) >> 6);
        exp10 += exp10_remain;
        // exp10 = exp10 / 1048576;
        exp10 >>= 20;
    }
    else {
        exp10 = 0;
    }
    return exp10;
}

/**
 * log10(2^64) = 19.265919722494796493679289262368
 * 2^27 / 100000000 = 1.34217728
 * log10(2^64) * 2^27 / 100000000 = 25.858279729836420772040005654498
 * zoom = 2585827972.9836420772040005654498
 */

JMC_DECLARE_NONSTD(int)
jmc_log10_fast(double val)
{
    fuint64_t *f64;
    int exponent;
    int exp10;
    fuint64_t exp10_64;

    f64 = (fuint64_t *)&val;
    // exponent = (exponent_mask32 >> (52 - 30)) - 1023;
    exponent = ((f64->high & JM_DOUBLE_EXPONENT_MASK32) >> JM_DOUBLE_EXPONENT_SHIFT32)
        - JM_DOUBLE_EXPONENT_0_32;

    // exponent is positive (exponent >= 0 && exponent <= 1024)
    if (exponent >= 0 && exponent <= JM_DOUBLE_EXPONENT_MAX) {
        // must 20,201,781 < 33,554,432 ( 2^32 / 64 / 2)
        // exp10 = (exponent / 64) * 2585827972;
        exp10_64.u64 = (uint64_t)(exponent * 2585827972ULL);
        // exp10 = exp10 / 134217728;   // 2^27
        //exp10 >>= 27;
        exp10 = exp10_64.high >> 1;
    }
    else {
        exp10 = 0;
    }
    return exp10;
}

#if defined(JMC_DTOS_INLINE_DECLARE) && (JMC_DTOS_INLINE_DECLARE != 0)
JMC_INLINE_NONSTD(int)
#else
JMC_DECLARE_NONSTD(int)
#endif
jmc_dtos(jm_char *buf, double val, int filed_width, int precision)
{
    int len;
    int64_t i64;
    uint32_t scale32;
    uint64_t scale, frac;
    fuint64_t *f64;
    unsigned int n;
    int num_width;
    int exponent;
    int pow10_index;
    int pow10_remain_index;
    int exp10;
    int exp10_remain;
    double pow10;
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

    if (sizeof(fuint64_t) != sizeof(double)) {
        // maybe have some error!
#ifndef _MSC_VER
        #error "jmc_dtos() maybe have some error!"
#endif // _MSC_VER
        jimic_assert(sizeof(fuint64_t) == sizeof(double));
        return 0;
    }

    exp10 = 0;

    f64 = (fuint64_t *)&val;
    // exponent = (exponent_mask32 >> (52 - 30)) - 1023;
    exponent = ((f64->high & JM_DOUBLE_EXPONENT_MASK32) >> JM_DOUBLE_EXPONENT_SHIFT32)
        - JM_DOUBLE_EXPONENT_0_32;

    // exponent is positive (exponent >= 0 && exponent <= 1024)
    if (exponent >= 0 && exponent <= JM_DOUBLE_EXPONENT_MAX) {
        // pow10_index = exponent / 64 - 1;
        pow10_index = (exponent >> 6) - 1;
        if (pow10_index >= 0) {
            pow10 = pow10_table[pow10_index];
            // exp10 = (exponent / 64) * 19;
            exp10 = (exponent >> 6) * 19;
            // exp10_remain = (exponent & 63) * 19 / 64 / 4 * 4;
            exp10_remain = (((exponent & 63) * 19) >> 8);
            exp10 += (exp10_remain << 2);
            // pow10_remain_index = (exponent & 63) * 19 / 64 / 4 - 1;
            pow10_remain_index = exp10_remain - 1;

            // adjust the double value to 0 ~ 2^64-1
            if (pow10_remain_index >= 0) {
                pow10 *= pow10_remain_table[pow10_remain_index];
                val /= pow10;
            }
        }
        else {
            //
        }
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

    f64 = (fuint64_t *)&val;
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
            len = jmc_i64toa_radix10_ex(buf, -1, i64, FMT_ALIGN_RIGHT, ' ', num_width, num_width);
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
        len = JIMIC_MAX(filed_width, 3);
        filed_width -= 3;
        while (filed_width > 0) {
            *buf++ = ' ';
            filed_width--;
        }
        *buf        = 'N';
        *(buf + 1)  = 'a';
        *(buf + 2)  = 'N';
        *(buf + 3)  = '\0';
        return len;
    }
    else {
        // is +INF or -INF
        if ((f64->high & JM_DOUBLE_SIGN_BIT32) == 0) {
            len = JIMIC_MAX(filed_width, 3);
            filed_width -= 3;
            while (filed_width > 0) {
                *buf++ = ' ';
                filed_width--;
            }
            // '+', 0x2B
            *buf        = 'I';
            *(buf + 1)  = 'n';
            *(buf + 2)  = 'f';
            *(buf + 3)  = '\0';
            return len;
        }
        else {
            len = JIMIC_MAX(filed_width, 4);
            filed_width -= 4;
            while (filed_width > 0) {
                *buf++ = ' ';
                filed_width--;
            }
            // '-', 0x2D
//          *buf        = '+' + ((f64->high & JM_DOUBLE_SIGN_BIT32) >> 30);
            *buf        = '-';
            *(buf + 1)  = 'I';
            *(buf + 2)  = 'n';
            *(buf + 3)  = 'f';
            *(buf + 4)  = '\0';
            return len;
        }
    }
}

#if defined(JMC_DTOS_INLINE_DECLARE) && (JMC_DTOS_INLINE_DECLARE != 0)
JMC_INLINE_NONSTD(int)
#else
JMC_DECLARE_NONSTD(int)
#endif
jmc_dtos_ex(jm_char *buf, size_t count, double val, unsigned int flag,
            unsigned int fill, int filed_width, int precision)
{
    int len;
    int64_t i64;
    uint32_t scale32;
    uint64_t scale, frac;
    fuint64_t *f64;
    unsigned int n;
    int num_width;

    int exponent;
    int pow10_index;
    int pow10_remain_index;
    int exp10;
    int exp10_remain;
    double pow10;

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

    exp10 = (int)floor(log(val) / log((double)10.0));

    exp10 = 0;

    f64 = (fuint64_t *)&val;
    // exponent = (exponent_mask32 >> (52 - 30)) - 1023;
    exponent = ((f64->high & JM_DOUBLE_EXPONENT_MASK32) >> JM_DOUBLE_EXPONENT_SHIFT32)
        - JM_DOUBLE_EXPONENT_0_32;

    // exponent is positive (exponent >= 0 && exponent <= 1024)
    if (exponent >= 0 && exponent <= JM_DOUBLE_EXPONENT_MAX) {
        // pow10_index = exponent / 64 - 1;
        pow10_index = (exponent >> 6) - 1;
        if (pow10_index >= 0) {
            pow10 = pow10_table[pow10_index];
            // exp10 = (exponent / 64) * 19;
            exp10 = (exponent >> 6) * 19;
            // exp10_remain = (exponent & 63) * 19 / 64 / 4 * 4;
            exp10_remain = (((exponent & 63) * 19) >> 8);
            exp10 += (exp10_remain << 2);
            // pow10_remain_index = (exponent & 63) * 19 / 64 / 4 - 1;
            pow10_remain_index = exp10_remain - 1;

            // adjust the double value to 0 ~ 2^64-1
            if (pow10_remain_index >= 0) {
                pow10 *= pow10_remain_table[pow10_remain_index];
                val /= pow10;
            }
        }
        else {
            //
        }
    }

    if (sizeof(fuint64_t) != sizeof(double)) {
        // maybe have some error!
#ifndef _MSC_VER
        #error "jmc_dtos_ex() maybe have some error!"
#endif // _MSC_VER
        jimic_assert(sizeof(fuint64_t) == sizeof(double));
        return 0;
    }

    if (precision > 0) {
#if 0
        if (precision <= 10) {
            jimic_assert(precision >= 0 && precision <= 10);
            scale = scales32[precision];
        }
        else {
            jimic_assert(precision > 10 && precision <= 20);
            scale = scales64[precision - 11];
        }
#else
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
#endif
        num_width = filed_width - precision - 1;
    }
    else if (precision < 0) {
        jimic_assert(precision < 0);
        precision = FMT_DEFAULT_DOUBLE_PRECISION;
        scale = 1000000;
        num_width = filed_width - FMT_DEFAULT_DOUBLE_PRECISION - 1;
    }
    else {
        scale = 1;
        num_width = filed_width;
    }

    f64 = (fuint64_t *)&val;
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

        // for integer part of double
        if (((flag & FMT_ALIGN_LEFT) != 0) || (num_width < 0))
            num_width = 0;
        len = jmc_i64toa_radix10_ex(buf, -1, i64, flag, fill, num_width, num_width);
        filed_width -= len;
        buf += len;

        if (precision > 0) {
            // output decimal
            *buf++ = '.';

            // for fractional part of double
#if 0
            filed_width--;
            if (precision < filed_width) {
                num_width = precision;
                precision = filed_width;
                filed_width = num_width;
            }
            len += jmc_u64toa_radix10_ex(buf, -1, frac, FMT_ALIGN_LEFT,
                                         '0', precision, filed_width - 1) + 1;
#else
            filed_width--;
            if (precision >= filed_width) {
                len += jmc_u64toa_radix10_ex(buf, -1, frac, FMT_ALIGN_LEFT,
                                             '0', precision, filed_width) + 1;
            }
            else {
                len += jmc_u64toa_radix10_ex(buf, -1, frac, FMT_ALIGN_LEFT,
                                             '0', filed_width, precision) + 1;
            }
#endif
        }
        return len;
    }
    else if (((f64->high & JM_DOUBLE_MANTISSA_MASK_HIGH) != 0)
             || ((f64->low & JM_DOUBLE_MANTISSA_MASK_LOW) != 0)) {
        // is NaN, not a number
        len = JIMIC_MAX(filed_width, 3);
        filed_width -= 3;
        if ((flag & FMT_ALIGN_LEFT) != 0) {
            // align to left
            *buf        = 'N';
            *(buf + 1)  = 'a';
            *(buf + 2)  = 'N';
            *(buf + 3)  = ' ';
            buf += 3;
            while (filed_width > 0) {
                *buf++ = ' ';
                filed_width--;
            }
            *buf = '\0';
            return len;
        }
        else {
            // align to right
            while (filed_width > 0) {
                *buf++ = ' ';
                filed_width--;
            }
            *buf        = 'N';
            *(buf + 1)  = 'a';
            *(buf + 2)  = 'N';
            *(buf + 3)  = '\0';
            return len;
        }
    }
    else {
        // is +INF or -INF
        if ((f64->high & JM_DOUBLE_SIGN_BIT32) == 0) {
            if ((flag & FMT_PLUS_FLAG) == 0) {
                len = JIMIC_MAX(filed_width, 3);
                filed_width -= 3;
                if ((flag & FMT_ALIGN_LEFT) != 0) {
                    // align to left
                    *buf        = 'I';
                    *(buf + 1)  = 'n';
                    *(buf + 2)  = 'f';
                    *(buf + 3)  = ' ';
                    buf += 3;
                    while (filed_width > 0) {
                        *buf++ = ' ';
                        filed_width--;
                    }
                    *buf = '\0';
                    return len;
                }
                else {
                    // align to right
                    while (filed_width > 0) {
                        *buf++ = ' ';
                        filed_width--;
                    }
                    *buf        = 'I';
                    *(buf + 1)  = 'n';
                    *(buf + 2)  = 'f';
                    *(buf + 3)  = '\0';
                    return len;
                }
            }
            else {
                len = JIMIC_MAX(filed_width, 4);
                filed_width -= 4;
                if ((flag & FMT_ALIGN_LEFT) != 0) {
                    // align to left
                    // '+', 0x2B
                    *buf        = '+';
                    *(buf + 1)  = 'I';
                    *(buf + 2)  = 'n';
                    *(buf + 3)  = 'f';
                    buf += 4;
                    while (filed_width > 0) {
                        *buf++ = ' ';
                        filed_width--;
                    }
                    *buf = '\0';
                    return len;
                }
                else {
                    // align to right
                    while (filed_width > 0) {
                        *buf++ = ' ';
                        filed_width--;
                    }
                    // '+', 0x2B
                    *buf        = '+';
                    *(buf + 1)  = 'I';
                    *(buf + 2)  = 'n';
                    *(buf + 3)  = 'f';
                    *(buf + 4)  = '\0';
                    return len;
                }
            }

        }
        else {
            len = JIMIC_MAX(filed_width, 4);
            filed_width -= 4;
            if ((flag & FMT_ALIGN_LEFT) != 0) {
                // align to left
                // '-', 0x2D
                *buf        = '-';
                *(buf + 1)  = 'I';
                *(buf + 2)  = 'n';
                *(buf + 3)  = 'f';
                buf += 4;
                while (filed_width > 0) {
                    *buf++ = ' ';
                    filed_width--;
                }
                *buf = '\0';
                return len;
            }
            else {
                // align to right
                while (filed_width > 0) {
                    *buf++ = ' ';
                    filed_width--;
                }
                // '-', 0x2D
                *buf        = '-';
                *(buf + 1)  = 'I';
                *(buf + 2)  = 'n';
                *(buf + 3)  = 'f';
                *(buf + 4)  = '\0';
                return len;
            }
        }
    }
}

#if defined(JMC_DTOS_INLINE_DECLARE) && (JMC_DTOS_INLINE_DECLARE != 0)
JMC_INLINE_NONSTD(int)
#else
JMC_DECLARE_NONSTD(int)
#endif
jmc_dtos_ex2(jm_char *buf, size_t count, double val, unsigned int flag,
             unsigned int fill, int filed_width, int precision)
{
    int len;
    int64_t i64;
    uint32_t scale32;
    uint64_t scale, frac;
    fuint64_t *f64;
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

    if (sizeof(fuint64_t) != sizeof(double)) {
        // maybe have some error!
#ifndef _MSC_VER
        #error "jmc_dtos_ex2() maybe have some error!"
#endif // _MSC_VER
        jimic_assert(sizeof(fuint64_t) == sizeof(double));
        return 0;
    }

    if ((flag & FMT_ALIGN_LEFT) != 0) {
        // align to left
        if (precision > 0) {
#if 0
            if (precision <= 10) {
                jimic_assert(precision >= 0 && precision <= 10);
                scale = scales32[precision];
            }
            else {
                jimic_assert(precision > 10 && precision <= 20);
                scale = scales64[precision - 11];
            }
#else
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
#endif
        }
        else if (precision < 0) {
            jimic_assert(precision < 0);
            precision = FMT_DEFAULT_DOUBLE_PRECISION;
            scale = 1000000;
        }
        else {
            scale = 1;
        }

        f64 = (fuint64_t *)&val;
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

            // for integer part of double
            len = jmc_i64toa_radix10_ex(buf, -1, i64, flag, fill, 0, 0);
            filed_width -= len;
            buf += len;

            if (precision > 0) {
                // output decimal
                *buf++ = '.';

                // for fractional part of double
                if (precision >= filed_width - 1) {
                    len += jmc_u64toa_radix10_ex(buf, -1, frac, FMT_ALIGN_LEFT,
                                                 '0', precision, filed_width - 1) + 1;
                }
                else {
                    len += jmc_u64toa_radix10_ex(buf, -1, frac, FMT_ALIGN_LEFT,
                                                 '0', filed_width - 1, precision) + 1;
                }
            }
            return len;
        }
        else if (((f64->high & JM_DOUBLE_MANTISSA_MASK_HIGH) != 0)
                 || ((f64->low & JM_DOUBLE_MANTISSA_MASK_LOW) != 0)) {
            // is NaN, not a number
            len = JIMIC_MAX(filed_width, 3);
            filed_width -= 3;
            // align to left
            *buf        = 'N';
            *(buf + 1)  = 'a';
            *(buf + 2)  = 'N';
            *(buf + 3)  = ' ';
            buf += 3;
            while (filed_width > 0) {
                *buf++ = ' ';
                filed_width--;
            }
            *buf = '\0';
            return len;
        }
        else {
            // is +INF or -INF
            if ((f64->high & JM_DOUBLE_SIGN_BIT32) == 0) {
                if ((flag & FMT_PLUS_FLAG) == 0) {
                    len = JIMIC_MAX(filed_width, 3);
                    filed_width -= 3;
                    // align to left
                    *buf        = 'I';
                    *(buf + 1)  = 'n';
                    *(buf + 2)  = 'f';
                    *(buf + 3)  = ' ';
                    buf += 3;
                    while (filed_width > 0) {
                        *buf++ = ' ';
                        filed_width--;
                    }
                    *buf = '\0';
                    return len;
                }
                else {
                    len = JIMIC_MAX(filed_width, 4);
                    filed_width -= 4;
                    // align to left
                    // '+', 0x2B
                    *buf        = '+';
                    *(buf + 1)  = 'I';
                    *(buf + 2)  = 'n';
                    *(buf + 3)  = 'f';
                    buf += 4;
                    while (filed_width > 0) {
                        *buf++ = ' ';
                        filed_width--;
                    }
                    *buf = '\0';
                    return len;
                }

            }
            else {
                len = JIMIC_MAX(filed_width, 4);
                filed_width -= 4;
                // align to left
                // '-', 0x2D
                *buf        = '-';
                *(buf + 1)  = 'I';
                *(buf + 2)  = 'n';
                *(buf + 3)  = 'f';
                buf += 4;
                while (filed_width > 0) {
                    *buf++ = ' ';
                    filed_width--;
                }
                *buf = '\0';
                return len;
            }
        }
    }
    else {
        // align to right
        if (precision > 0) {
#if 0
            if (precision <= 10) {
                jimic_assert(precision >= 0 && precision <= 10);
                scale = scales32[precision];
            }
            else {
                jimic_assert(precision > 10 && precision <= 20);
                scale = scales64[precision - 11];
            }
#else
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
#endif
            num_width = filed_width - precision - 1;
        }
        else if (precision < 0) {
            jimic_assert(precision < 0);
            precision = FMT_DEFAULT_DOUBLE_PRECISION;
            scale = 1000000;
            num_width = filed_width - FMT_DEFAULT_DOUBLE_PRECISION - 1;
        }
        else {
            scale = 1;
            num_width = filed_width;
        }

        f64 = (fuint64_t *)&val;
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

            // for integer part of double
            if (num_width < 0)
                num_width = 0;
            len = jmc_i64toa_radix10_ex(buf, -1, i64, flag, fill, num_width, num_width);
            filed_width -= len;
            buf += len;

            if (precision > 0) {
                // output decimal
                *buf++ = '.';

                // for fractional part of double
                if (precision >= filed_width - 1) {
                    len += jmc_u64toa_radix10_ex(buf, -1, frac, FMT_ALIGN_LEFT,
                                                 '0', precision, filed_width - 1) + 1;
                }
                else {
                    len += jmc_u64toa_radix10_ex(buf, -1, frac, FMT_ALIGN_LEFT,
                                                 '0', filed_width - 1, precision) + 1;
                }
            }
            return len;
        }
        else if (((f64->high & JM_DOUBLE_MANTISSA_MASK_HIGH) != 0)
                 || ((f64->low & JM_DOUBLE_MANTISSA_MASK_LOW) != 0)) {
            // is NaN, not a number
            len = JIMIC_MAX(filed_width, 3);
            filed_width -= 3;
            // align to right
            while (filed_width > 0) {
                *buf++ = ' ';
                filed_width--;
            }
            *buf        = 'N';
            *(buf + 1)  = 'a';
            *(buf + 2)  = 'N';
            *(buf + 3)  = '\0';
            return len;
        }
        else {
            // is +INF or -INF
            if ((f64->high & JM_DOUBLE_SIGN_BIT32) == 0) {
                if ((flag & FMT_PLUS_FLAG) == 0) {
                    len = JIMIC_MAX(filed_width, 3);
                    filed_width -= 3;
                    // align to right
                    while (filed_width > 0) {
                        *buf++ = ' ';
                        filed_width--;
                    }
                    *buf        = 'I';
                    *(buf + 1)  = 'n';
                    *(buf + 2)  = 'f';
                    *(buf + 3)  = '\0';
                    return len;
                }
                else {
                    len = JIMIC_MAX(filed_width, 4);
                    filed_width -= 4;
                    // align to left
                    // '+', 0x2B
                    *buf        = '+';
                    *(buf + 1)  = 'I';
                    *(buf + 2)  = 'n';
                    *(buf + 3)  = 'f';
                    buf += 4;
                    while (filed_width > 0) {
                        *buf++ = ' ';
                        filed_width--;
                    }
                    *buf = '\0';
                    return len;
                }

            }
            else {
                len = JIMIC_MAX(filed_width, 4);
                filed_width -= 4;
                // align to right
                while (filed_width > 0) {
                    *buf++ = ' ';
                    filed_width--;
                }
                // '-', 0x2D
                *buf        = '-';
                *(buf + 1)  = 'I';
                *(buf + 2)  = 'n';
                *(buf + 3)  = 'f';
                *(buf + 4)  = '\0';
                return len;
            }
        }
    }
}

#if defined(JMC_STRNCPY_EX_INLINE_DECLARE) && (JMC_STRNCPY_EX_INLINE_DECLARE != 0)
JMC_INLINE_NONSTD(size_t)
#else
JMC_DECLARE_NONSTD(size_t)
#endif
jmc_strncpy_ex(jm_char *dest, size_t countOfElements, JM_CONST jm_char *src, size_t count,
               unsigned int flag, unsigned int fill, unsigned int width, int length)
{
    jm_char *end;
    size_t copy_len;
    int fill_cnt, padding;

    jimic_assert(dest != NULL);
    jimic_assert(src  != NULL);

    count = JIMIC_MIN(count, countOfElements - 1);
    copy_len = JIMIC_MIN(JIMIC_MIN((unsigned int)length, width), count);
    end = (jm_char *)src + copy_len;

    fill_cnt = width - copy_len;
    if (fill_cnt <= 0) {
        // copy from src
        while (src < end) {
            *dest++ = *src++;
        }

        return copy_len;
    }
    else {
        // when legnth <= 0 || legnth >= witdh, align to right or left is same
        if (length <= 0 || length >= (int)width) {
            // fill normal
            while (fill_cnt > 0) {
                *dest++ = fill;
                fill_cnt--;
            }

            // copy from src
            while (src < end) {
                *dest++ = *src++;
            }

            return width;
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

                // copy from src
                while (src < end) {
                    *dest++ = *src++;
                }

                return width;
            }
            else {
                // align to left, when (length < width)
                jimic_assert(length < (int)width);

                // copy from src
                while (src < end) {
                    *dest++ = *src++;
                }

                // fill normal
                padding = length - count;
                if (padding > 0) {
                    fill_cnt -= padding;
                    while (padding > 0) {
                        *dest++ = fill;
                        padding--;
                    }
                }

                // fill left padding space
                while (fill_cnt > 0) {
                    *dest++ = ' ';
                    fill_cnt--;
                }

                return width;
            }
        }
    }
}

/* jmc_strncpy_ex_fast() 是否使用memcpy复制字符串 */
#define JMC_STRNCPY_EX_FAST_USE_MEMCPY       1

#if defined(JMC_STRNCPY_EX_INLINE_DECLARE) && (JMC_STRNCPY_EX_INLINE_DECLARE != 0)
JMC_INLINE_NONSTD(size_t)
#else
JMC_DECLARE_NONSTD(size_t)
#endif
jmc_strncpy_ex_fast(jm_char *dest, size_t countOfElements, JM_CONST jm_char *src, size_t count,
                    unsigned int flag, unsigned int fill, unsigned int width, int length)
{
#if defined(JMC_STRNCPY_EX_FAST_USE_MEMCPY) && (JMC_STRNCPY_EX_FAST_USE_MEMCPY != 0)
    // jmc_strncpy_ex_fast() use memcpy
#else
    jm_char *end;
#endif
    size_t copy_len;
    int fill_cnt, padding;

    jimic_assert(dest != NULL);
    jimic_assert(src  != NULL);

    count = JIMIC_MIN(count, countOfElements - 1);
    copy_len = JIMIC_MIN(JIMIC_MIN((unsigned int)length, width), count);
#if defined(JMC_STRNCPY_EX_FAST_USE_MEMCPY) && (JMC_STRNCPY_EX_FAST_USE_MEMCPY != 0)
    // jmc_strncpy_ex_fast() use memcpy
#else
    end = (jm_char *)src + copy_len;
#endif

    fill_cnt = width - copy_len;
    if (fill_cnt <= 0) {

#if defined(JMC_STRNCPY_EX_FAST_USE_MEMCPY) && (JMC_STRNCPY_EX_FAST_USE_MEMCPY != 0)
        // copy from src
        memcpy(dest, src, copy_len * sizeof(jm_char));
#else
        // copy from src
        while (src < end) {
            *dest++ = *src++;
        }
#endif

        return copy_len;
    }
    else {
        // when legnth <= 0 || legnth >= witdh, align to right or left is same
        if (length <= 0 || length >= (int)width) {
            // fill normal
            while (fill_cnt > 0) {
                *dest++ = fill;
                fill_cnt--;
            }

#if defined(JMC_STRNCPY_EX_FAST_USE_MEMCPY) && (JMC_STRNCPY_EX_FAST_USE_MEMCPY != 0)
            // copy from src
            memcpy(dest, src, copy_len * sizeof(jm_char));
#else
            // copy from src
            while (src < end) {
                *dest++ = *src++;
            }
#endif
            return width;
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

#if defined(JMC_STRNCPY_EX_FAST_USE_MEMCPY) && (JMC_STRNCPY_EX_FAST_USE_MEMCPY != 0)
                // copy from src
                memcpy(dest, src, copy_len * sizeof(jm_char));
#else
                // copy from src
                while (src < end) {
                    *dest++ = *src++;
                }
#endif
                return width;
            }
            else {
                // align to left, when (length < width)
                jimic_assert(length < (int)width);

#if defined(JMC_STRNCPY_EX_FAST_USE_MEMCPY) && (JMC_STRNCPY_EX_FAST_USE_MEMCPY != 0)
                // copy from src
                memcpy(dest, src, copy_len * sizeof(jm_char));
                dest += copy_len;
#else
                // copy from src
                while (src < end) {
                    *dest++ = *src++;
                }
#endif
                // fill normal
                padding = length - count;
                if (padding > 0) {
                    fill_cnt -= padding;
                    while (padding > 0) {
                        *dest++ = fill;
                        padding--;
                    }
                }

                // fill left padding space
                while (fill_cnt > 0) {
                    *dest++ = ' ';
                    fill_cnt--;
                }

                return width;
            }
        }
    }
}

/* 是否使用直接顺序写入字符串 */
#define OUT_NULL_STRING_EX_DIRECT_WRITE     1

#if defined(JMC_STRNCPY_EX_INLINE_DECLARE) && (JMC_STRNCPY_EX_INLINE_DECLARE != 0)
JMC_INLINE_NONSTD(size_t)
#else
JMC_DECLARE_NONSTD(size_t)
#endif
jmc_out_null_string_ex(jm_char *dest, size_t countOfElements, unsigned int flag,
                       unsigned int fill, unsigned int width, int length)
{
#if !defined(OUT_NULL_STRING_EX_DIRECT_WRITE) || (OUT_NULL_STRING_EX_DIRECT_WRITE == 0)
    jm_char *end;
    jm_char *src;
#endif  /* OUT_NULL_STRING_EX_DIRECT_WRITE */
    size_t count;
    size_t copy_len;
    int fill_cnt, padding;

    jimic_assert(dest != NULL);

    count = JIMIC_MIN(6, countOfElements - 1);
    copy_len = JIMIC_MIN(JIMIC_MIN((unsigned int)length, width), count);

    fill_cnt = width - copy_len;
    if (fill_cnt <= 0) {
        /* Write "(null)" if there's space.  */
        if (copy_len >= (int)sizeof(FMT_NULL_STRING) - 1) {
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
    else {
#if !defined(OUT_NULL_STRING_EX_DIRECT_WRITE) || (OUT_NULL_STRING_EX_DIRECT_WRITE == 0)
        src = FMT_NULL_STRING;
        end = (jm_char *)src + copy_len;
#endif  /* OUT_NULL_STRING_EX_DIRECT_WRITE */

        // when legnth <= 0 || legnth >= witdh, align to right or left is same
        if (length <= 0 || length >= (int)width) {
            // fill normal
            while (fill_cnt > 0) {
                *dest++ = fill;
                fill_cnt--;
            }

#if defined(OUT_NULL_STRING_EX_DIRECT_WRITE) && (OUT_NULL_STRING_EX_DIRECT_WRITE != 0)
            // copy from "(null)"
            *(dest + 0)  = '(';
            *(dest + 1)  = 'n';
            *(dest + 2)  = 'u';
            *(dest + 3)  = 'l';
            *(dest + 4)  = 'l';
            *(dest + 5)  = ')';
            *(dest + 6)  = '\0';
#else
            // copy from src
            while (src < end) {
                *dest++ = *src++;
            }
#endif  /* OUT_NULL_STRING_EX_DIRECT_WRITE */
            return width;
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

#if defined(OUT_NULL_STRING_EX_DIRECT_WRITE) && (OUT_NULL_STRING_EX_DIRECT_WRITE != 0)
                // copy from "(null)"
                *(dest + 0) = '(';
                *(dest + 1) = 'n';
                *(dest + 2) = 'u';
                *(dest + 3) = 'l';
                *(dest + 4) = 'l';
                *(dest + 5) = ')';
                *(dest + 6) = '\0';
#else
                // copy from src
                while (src < end) {
                    *dest++ = *src++;
                }
#endif  /* OUT_NULL_STRING_EX_DIRECT_WRITE */

                return width;
            }
            else {
                // align to left, when (length < width)
                jimic_assert(length < (int)width);

#if defined(OUT_NULL_STRING_EX_DIRECT_WRITE) && (OUT_NULL_STRING_EX_DIRECT_WRITE != 0)
                // copy from "(null)"
                *(dest + 0) = '(';
                *(dest + 1) = 'n';
                *(dest + 2) = 'u';
                *(dest + 3) = 'l';
                *(dest + 4) = 'l';
                *(dest + 5) = ')';
                *(dest + 6) = '\0';
                dest += 6;
#else
                // copy from src
                while (src < end) {
                    *dest++ = *src++;
                }
#endif  /* OUT_NULL_STRING_EX_DIRECT_WRITE */

                // fill normal
                padding = length - count;
                if (padding > 0) {
                    fill_cnt -= padding;
                    while (padding > 0) {
                        *dest++ = fill;
                        padding--;
                    }
                }

                // fill left padding space
                while (fill_cnt > 0) {
                    *dest++ = ' ';
                    fill_cnt--;
                }

                return width;
            }
        }
    }
}

#endif  /* !_JIMIC_STRING_JMC_STRINGS_IMPL_INL_ */
