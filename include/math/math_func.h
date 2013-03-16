#ifndef _MATH_FUNC_H
#define _MATH_FUNC_H

#include <cmath>

static inline float call_sinf(float x)
{
    return sinf(x);
}

static inline float call_sinh(float x)
{
    return sinh(x);
}

static inline float call_cosf(float x)
{
    return cosf(x);
}

static inline float call_cosh(float x)
{
    return cosh(x);
}

static inline float call_tanf(float x)
{
    return tanf(x);
}

static inline float call_tanh(float x)
{
    return tanh(x);
}

static inline float call_sqr(float x)
{
    return sqrt(x);
}

static inline float call_cbr(float x)
{
    return cbrt(x);
}

static inline float call_acos(float x)
{
    return acos(x);
}

static inline float call_acosh(float x)
{
    return acosh(x);
}

static inline float call_asin(float x)
{
    return asin(x);
}

static inline float call_asinh(float x)
{
    return asinh(x);
}

static inline float call_atan(float x)
{
    return atan(x);
}

static inline float call_atanh(float x)
{
    return atanh(x);
}

static inline float call_log(float x)
{
    return log(x);
}

static inline float call_log10(float x)
{
    return log10(x);
}

static inline float call_exp(float x)
{
    return exp(x);
}

static inline float call_exp2(float x)
{
    return exp2(x);
}

#endif
