/*! @file
  Chrono : 実体モジュール

  Copyright (C) 2017 Haruhiko Uchida
  The software is released under the MIT license.
  http://opensource.org/licenses/mit-license.php
 */

#include "chrono.h"
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#if !defined(CHRONO_NO_ANY_SLEEP)
#include <unistd.h>
#endif

/*!
  x + y の結果がオーバーロードするか?
*/
#define IS_ADD_OVERFLOW(x, y, min, max) (((y) > 0 && (x) > (max) - (y)) || ((y) < 0 && (x) < (min) - (y)))

/*!
  x - y の結果がオーバーロードするか?
*/
#define IS_SUB_OVERFLOW(x, y, min, max) (((y) > 0 && (x) < (min) + (y)) || ((y) < 0 && (x) > (max) + (y)))

/*******************************************************************************
 * Chrono
 */

intmax_t ChronoGet(chrono_t const * c, chrono_period_t x)
{
    chrono_period_t y = c->period;
    if (0 < y) {
        if (0 < x)
            return c->value * y / x;
        if (x < 0)
            return c->value * -(x * y);
    } else if(y < 0) {
        if (x < 0)
            return c->value * x / y;
        if (0 < x)
            return c->value / -(x * y);
    }
    return 1;
}

static chrono_period_t min(chrono_period_t x, chrono_period_t y)
{
    return x < y ? x : y;
}

static chrono_period_t max(chrono_period_t x, chrono_period_t y)
{
    return x > y ? x : y;
}

static uintmax_t gcd(uintmax_t x, uintmax_t y)
{
    for(uintmax_t z; (z = x % y); x = y, y = z);
    return y;
}

static void cast(chrono_t * c, uintmax_t scale)
{
    c->value /= (intmax_t)scale;
    if (c->period < 0)
        c->period /= (chrono_period_t)scale;
    else
        c->period *= (chrono_period_t)scale;
}

static void arith(chrono_t * c1, chrono_t * c2, bool (*f)(intmax_t *, intmax_t))
{
    uintmax_t scale = gcd(llabs(c1->value), llabs(c2->value));
    cast(c1, scale);
    cast(c2, scale);

    chrono_period_t cp = min(c1->period, c2->period);
    intmax_t x = ChronoGet(c1, cp);
    if (!f(&x, ChronoGet(c2, cp))) {
        // 桁溢れした
        cp = max(c1->period, c2->period);
        x = ChronoGet(c1, cp);
        f(&x, ChronoGet(c2, cp));
    }
    c1->value = x;
    c1->period = cp;
}

static bool add(intmax_t * x, intmax_t y)
{
    if (IS_ADD_OVERFLOW(*x, y, INTMAX_MIN, INTMAX_MAX))
        return false;
    *x = *x + y;
    return true;
}

void ChronoAdd(chrono_t * c1, chrono_t const * c2)
{
    chrono_t c3 = *c2;
    arith(c1, &c3, add);
}

void ChronoAddValue(chrono_t * c, intmax_t value, chrono_period_t cp)
{
    chrono_t rhs = ChronoInit(value, cp);
    arith(c, &rhs, add);
}

static bool sub(intmax_t * x, intmax_t y)
{
    if (IS_SUB_OVERFLOW(*x, y, INTMAX_MIN, INTMAX_MAX))
        return false;
    *x = *x - y;
    return true;
}

void ChronoSub(chrono_t * c1, chrono_t const * c2)
{
    chrono_t c3 = *c2;
    arith(c1, &c3, sub);
}

void ChronoSubValue(chrono_t * c, intmax_t value, chrono_period_t cp)
{
    chrono_t rhs = ChronoInit(value, cp);
    arith(c, &rhs, sub);
}

void ChronoToTimeT(chrono_t const * c, time_t * t)
{
    *t = ChronoGet(c, chrono_seconds);
}

#ifndef CHRONO_NO_TIMEVAL
void ChronoToTimeVal(chrono_t const * c, struct timeval * tv)
{
    ChronoToTimeT(c, &tv->tv_sec);
    tv->tv_usec = (c->period >= 0) ? 0
        : ChronoGet(c, chrono_microseconds) % 1000000;
}
#endif

#ifndef CHRONO_NO_TIMESPEC
void ChronoToTimeSpec(chrono_t const * c, struct timespec * tv)
{
    ChronoToTimeT(c, &tv->tv_sec);
    tv->tv_nsec = (c->period >= 0) ? 0
        : ChronoGet(c, chrono_nanoseconds) % 1000000000;
}
#endif

#ifndef CHRONO_NO_ANY_SLEEP
int ChronoSleepFor(chrono_t const * c)
{
    if (c->value <= 0)
        return 0;
#if !defined(CHRONO_NO_NANOSLEEP)
    struct timespec tv;
    ChronoToTimeSpec(c, &tv);
    return nanosleep(&tv, NULL);
#elif !defined(CHRONO_NO_USLEEP)
    return usleep(ChronoGet(c, chrono_microseconds));
#else
    return sleep(ChronoGet(c, chrono_seconds));
#endif
}

int ChronoSleepForValue(intmax_t value, chrono_period_t cp)
{
    chrono_t c = ChronoInit(value, cp);
    return ChronoSleepFor(&c);
}
#endif

#if !defined(CHRONO_NO_CLOCK_GETTIME)
/*******************************************************************************
 * spec
 */

static void specZero(struct timespec * tv)
{
    tv->tv_sec = 0;
    tv->tv_nsec = 0;
}

static void specMin(struct timespec * tv)
{
    tv->tv_sec = CHRONO_TIME_T_MIN;
    tv->tv_nsec = 0;
}

static void specMax(struct timespec * tv)
{
    tv->tv_sec= CHRONO_TIME_T_MAX;
    tv->tv_nsec = LONG_MAX;
}

static bool specNow(struct timespec * tv, int clock_id)
{
    return clock_gettime(clock_id, tv) == 0;
}

static bool specDHMS(struct timespec * tv, int days, int hours, int minutes, int seconds)
{
    tv->tv_sec = ((((days * 24) + hours) * 60 + minutes) * 60 + seconds);
    tv->tv_nsec = 0;
    return true;
}

static void specIncr(struct timespec * tv)
{
    if ((++tv->tv_nsec) == 0)
        if (++tv->tv_sec != 0)
            tv->tv_nsec = 0;
}

static void specDecr(struct timespec * tv)
{
    if ((tv->tv_nsec--) == 0)
        if (tv->tv_sec-- != 0)
            tv->tv_nsec = LONG_MAX;
}

static bool specAdd(struct timespec * tv, chrono_t const * c)
{
    if (c->period < chrono_seconds)
        tv->tv_nsec += ChronoGet(c, chrono_nanoseconds) % -chrono_nanoseconds;
    tv->tv_sec += ChronoGet(c, chrono_seconds);
    return true;
}

static bool specDiff(struct timespec const * lhs, struct timespec const * rhs, chrono_t * c)
{
    int overflow = IS_SUB_OVERFLOW(lhs->tv_sec, rhs->tv_sec, INTMAX_MIN, INTMAX_MAX);
    if (overflow) {
        c->value = INTMAX_MAX;
        c->period = chrono_seconds;
    } else {
        c->value = llabs(lhs->tv_sec - rhs->tv_sec);
        if (c->value < INTMAX_MAX / -chrono_nanoseconds) {
            c->value *= -chrono_nanoseconds;
            c->value += llabs(lhs->tv_nsec - rhs->tv_nsec);
            c->period = chrono_nanoseconds;
        } else {
            c->period = chrono_seconds;
        }
    }
    return true;
}

static int specComp(struct timespec const * lhs, struct timespec const * rhs)
{
    return lhs->tv_sec < rhs->tv_sec ?   -1
        :  lhs->tv_sec > rhs->tv_sec ?    1
        :  lhs->tv_nsec < rhs->tv_nsec ? -1
        :  lhs->tv_nsec > rhs->tv_nsec ?  1
        :                                 0;
}

static void specToTimeT(struct timespec const * src, time_t * t)
{
    *t = src->tv_sec;
}

#ifndef CHRONO_NO_TIMEVAL
static void specToTimeVal(struct timespec const * src, struct timeval * tv)
{
    tv->tv_sec = src->tv_sec;
    tv->tv_usec = src->tv_nsec * chrono_microseconds / chrono_nanoseconds;
}
#endif

#ifndef CHRONO_NO_TIMESPEC
static void specToTimeSpec(struct timespec const * src, struct timespec * tv)
{
    *tv = *src;
}
#endif


#elif !defined(CHRONO_NO_GETTIMEOFDAY)
/*******************************************************************************
 * val
 */

static void valZero(struct timeval * tv)
{
    tv->tv_sec = 0;
    tv->tv_usec = 0;
}

static void valMin(struct timeval * tv)
{
    tv->tv_sec = CHRONO_TIME_T_MIN;
    tv->tv_usec = 0;
}

static void valMax(struct timeval * tv)
{
    tv->tv_sec= CHRONO_TIME_T_MAX;
    tv->tv_usec = LONG_MAX;
}

static bool valNow(struct timeval * tv)
{
    return (gettimeofday(tv, NULL) == 0);
}

static bool valDHMS(struct timeval * tv, int days, int hours, int minutes, int seconds)
{
    tv->tv_sec = ((((days * 24) + hours) * 60 + minutes) * 60 + seconds);
    tv->tv_usec = 0;
    return true;
}

static void valIncr(struct timeval * tv)
{
    if ((++tv->tv_usec) == 0)
        if (++tv->tv_sec != 0)
            tv->tv_usec = 0;
}

static void valDecr(struct timeval * tv)
{
    if ((tv->tv_usec--) == 0)
        if (tv->tv_sec-- != 0)
            tv->tv_usec = LONG_MAX;
}

static bool valAdd(struct timeval * tv, chrono_t const * c)
{
    if (c->period < chrono_seconds)
        tv->tv_usec += ChronoGet(c, chrono_microseconds) % -chrono_microseconds;
    tv->tv_sec += ChronoGet(c, chrono_seconds);
    return true;
}

static bool valDiff(struct timeval const * lhs, struct timeval const * rhs, chrono_t * c)
{
    c->value = labs(lhs->tv_sec - rhs->tv_sec);
    if (c->value == INTMAX_MIN) {
        --c->value;
        c->period = chrono_seconds;
    } else if (c->value < INTMAX_MAX / -chrono_microseconds) {
        c->value *= -chrono_nanoseconds;
        c->value += labs(lhs->tv_usec - rhs->tv_usec);
        c->period = chrono_microseconds;
    } else {
        c->period= chrono_microseconds;
    }
    return true;
}

static int valComp(struct timeval const * lhs, struct timeval const * rhs)
{
    return lhs->tv_sec < rhs->tv_sec ?   -1
        :  lhs->tv_sec > rhs->tv_sec ?    1
        :  lhs->tv_usec < rhs->tv_usec ? -1
        :  lhs->tv_usec > rhs->tv_usec ?  1
        :                                 0;
}

static void valToTimeT(struct timeval const * src, time_t * t)
{
    *t = src->tv_sec;
}

#ifndef CHRONO_NO_TIMEVAL
static void valToTimeVal(struct timeval const * src, struct timeval * tv)
{
    *tv = *src;
}
#endif

#ifndef CHRONO_NO_TIMESPEC
static void valToTimeSpec(struct timeval const * src, struct timespec * tv)
{
    tv->tv_sec = src->tv_sec;
    tv->tv_nsec = src->tv_usec * chrono_nanoseconds / chrono_microseconds;
}
#endif

#else
/*******************************************************************************
 * time
 */

static void timeZero(time_t * t)
{
    *t = 0;
}

static void timeMin(time_t * t)
{
    *t = CHRONO_TIME_T_MIN;
}

static void timeMax(time_t * t)
{
    *t = CHRONO_TIME_T_MAX;
}

static bool timeNow(time_t * t)
{
    time(t);
    return true;
}

static bool timeDHMS(time_t * t, int days, int hours, int minutes, int seconds)
{
    *t = (((days * 24) + hours) * 60 + minutes) * 60 + seconds;
    return true;
}

static void timeIncr(time_t * t)
{
    *t += 1;
}

static void timeDecr(time_t * t)
{
    *t -= 1;
}

static bool timeAdd(time_t * t, chrono_t const * c)
{
    intmax_t v = ChronoGet(c, chrono_seconds);
    if (IS_ADD_OVERFLOW(*t, v, INTMAX_MIN, INTMAX_MAX))
        return false;
    *t += v;
    return true;
}

static bool timeDiff(time_t const * t1, time_t const * t2, chrono_t * c)
{
    *c = ChronoInit(labs(*t1 - *t2), chrono_seconds);
    return true;
}

static int timeComp(time_t const * t1, time_t const * t2)
{
    return *t1 - *t2;
}

static void timeToTimeT(time_t const * t, time_t * dst)
{
    *dst = *t;
}

#ifndef CHRONO_NO_TIMEVAL
static void timeToTimeVal(time_t const * t, struct timeval * tv)
{
    tv->tv_sec = *t;
    tv->tv_usec = 0;
}
#endif

#ifndef CHRONO_NO_TIMESPEC
static void timeToTimeSpec(time_t const * t, struct timespec * tv)
{
    tv->tv_sec = *t;
    tv->tv_nsec = 0;
}
#endif

#endif  // !defined(CHRONO_NO_TIMESPEC) || !defined(CHRONO_NO_TIMEVAL)
/*******************************************************************************
 * ChronoSys
 */

#if !defined(CHRONO_NO_CLOCK_GETTIME)
# define sys(func) sys_1(spec, func)
#elif !defined(CHRONO_NO_GETTIMEOFDAY)
# define sys(func) sys_1(val, func)
#else
# define sys(func) sys_1(time, func)
#endif

#ifdef sys
# define sys_1(prefix, func) prefix ## func
# include "chrono_sys.h"

void ChronoSysZero(chrono_sys_t * cs)
{
    sys(Zero(&cs->time_point));
}

void ChronoSysMin(chrono_sys_t * cs)
{
    sys(Min(&cs->time_point));
}

void ChronoSysMax(chrono_sys_t * cs)
{
    sys(Max(&cs->time_point));
}

bool ChronoSysNow(chrono_sys_t * cs)
{
#if !defined(CHRONO_NO_CLOCK_GETTIME)
    return sys(Now(&cs->time_point, CLOCK_REALTIME));
#else
    return sys(Now(&cs->time_point));
#endif
}

bool ChronoSysDHMS(chrono_sys_t * cs, int days, int hours, int minutes, int seconds)
{
    return sys(DHMS(&cs->time_point, days, hours, minutes, seconds));
}

void ChronoSysIncr(chrono_sys_t * cs)
{
    sys(Incr(&cs->time_point));
}

void ChronoSysDecr(chrono_sys_t * cs)
{
    sys(Decr(&cs->time_point));
}

bool ChronoSysAdd(chrono_sys_t * cs, chrono_t const * c)
{
    return sys(Add(&cs->time_point, c));
}

bool ChronoSysAddValue(chrono_sys_t * cs, intmax_t value, chrono_period_t period)
{
    chrono_t c = ChronoInit(value, period);
    return ChronoSysAdd(cs, &c);
}

bool ChronoSysDiff(chrono_sys_t const * cs1, chrono_sys_t const * cs2, chrono_t * c)
{
    return sys(Diff(&cs1->time_point, &cs2->time_point, c));
}

bool ChronoSysDiffNow(chrono_sys_t const * cs, chrono_t * c)
{
    chrono_sys_t now;
    ChronoSysNow(&now);
    return ChronoSysDiff(cs, &now, c);
}

int ChronoSysComp(chrono_sys_t const * cs1, chrono_sys_t const * cs2)
{
    return sys(Comp(&cs1->time_point, &cs2->time_point));
}

void ChronoSysToTimeT(chrono_sys_t const * cs, time_t * t)
{
    sys(ToTimeT(&cs->time_point, t));
}

#ifndef CHRONO_NO_TIMEVAL
void ChronoSysToTimeVal(chrono_sys_t const * cs, struct timeval * tv)
{
    sys(ToTimeVal(&cs->time_point, tv));
}
#endif

#ifndef CHRONO_NO_TIMESPEC
void ChronoSysToTImeSpec(chrono_sys_t const * cs, struct timespec * tv)
{
    sys(ToTimeSpec(&cs->time_point, tv));
}
#endif

#ifndef CHRONO_NO_ANY_SLEEP
int ChronoSysSleepUntil(chrono_sys_t const * cs, intmax_t value, chrono_period_t cp)
{
    chrono_t c1;
    chrono_t c2 = ChronoInit(value, cp);
    ChronoSysDiffNow(cs, &c1);
    ChronoSub(&c1, &c2);
    c1.value = llabs(c1.value);
    return ChronoSleepFor(&c1);
}
#endif

#endif // sys
/*******************************************************************************
 * ChronoMno
 */

#if !defined(CHRONO_NO_CLOCK_GETTIME)
# define mno(func) mno_1(spec, func)
#elif !defined(CHRONO_MNO_NO_UPTIME)
# define mno(func) mno_1(time, func)
#include <sys/sysinfo.h>
static bool timeUptime(time_t * t)
{
    struct sysinfo info;
    if (sysinfo(&info) != 0)
        return false;
    *t = info.uptime;
    return true;
}
#endif

#ifdef mno
# define mno_1(prefix, func) prefix ## func
# include "chrono_mno.h"

void ChronoMnoZero(chrono_mno_t * cm)
{
    mno(Zero(&cm->time_point));
}

void ChronoMnoMin(chrono_mno_t * cm)
{
    mno(Min(&cm->time_point));
}

void ChronoMnoMax(chrono_mno_t * cm)
{
    mno(Max(&cm->time_point));
}

bool ChronoMnoNow(chrono_mno_t * cm)
{
#if !defined(CHRONO_NO_CLOCK_GETTIME)
    return mno(Now(&cm->time_point, CLOCK_MONOTONIC));
#else
    return mno(Uptime(&cm->time_point));
#endif
}

bool ChronoMnoDHMS(chrono_mno_t * cm, int days, int hours, int minutes, int seconds)
{
    return mno(DHMS(&cm->time_point, days, hours, minutes, seconds));
}

void ChronoMnoIncr(chrono_mno_t * cm)
{
    mno(Incr(&cm->time_point));
}

void ChronoMnoDecr(chrono_mno_t * cm)
{
    mno(Decr(&cm->time_point));
}

bool ChronoMnoAdd(chrono_mno_t * cm, chrono_t const * c)
{
    return mno(Add(&cm->time_point, c));
}

bool ChronoMnoAddValue(chrono_mno_t * cm, intmax_t value, chrono_period_t cp)
{
    chrono_t c = ChronoInit(value, cp);
    return ChronoMnoAdd(cm, &c);
}

bool ChronoMnoDiff(chrono_mno_t const * cm1, chrono_mno_t const * cm2, chrono_t * c)
{
    return mno(Diff(&cm1->time_point, &cm2->time_point, c));
}

bool ChronoMnoDiffNow(chrono_mno_t const * cm, chrono_t * c)
{
    chrono_mno_t now;
    ChronoMnoNow(&now);
    return ChronoMnoDiff(cm, &now, c);
}

int ChronoMnoComp(chrono_mno_t const * cm1, chrono_mno_t const * cm2)
{
    return mno(Comp(&cm1->time_point, &cm2->time_point));
}

void ChronoMnoToTimeT(chrono_mno_t const * cm, time_t * t)
{
    mno(ToTimeT(&cm->time_point, t));
}

#ifndef CHRONO_NO_TIMEVAL
void ChronoMnoToTimeVal(chrono_mno_t const * cm, struct timeval * tv)
{
    mno(ToTimeVal(&cm->time_point, tv));
}
#endif

#ifndef CHRONO_NO_TIMESPEC
void ChronoMnoToTimeSpec(chrono_mno_t const * cm, struct timespec * tv)
{
    mno(ToTimeSpec(&cm->time_point, tv));
}
#endif

#ifndef CHRONO_NO_ANY_SLEEP
int ChronoMnoSleepUntil(chrono_mno_t const * cm, intmax_t value, chrono_period_t cp)
{
    chrono_t c1;
    chrono_t c2 = ChronoInit(value, cp);
    ChronoMnoDiffNow(cm, &c1);
    ChronoSub(&c1, &c2);
    c1.value = llabs(c1.value);
    return ChronoSleepFor(&c1);
}
#endif

#endif // mno
/*******************************************************************************
 * ChronoMno
 */

#if !defined(CHRONO_NO_CLOCK_GETTIME)
# define cpu(func) cpu_1(spec, func)
#endif

#ifdef cpu
# define cpu_1(prefix, func) prefix ## func
# include "chrono_cpu.h"

void ChronoCpuZero(chrono_cpu_t * cc)
{
    cpu(Zero(&cc->time_point));
}

void ChronoCpuMin(chrono_cpu_t * cc)
{
    cpu(Min(&cc->time_point));
}

void ChronoCpuMax(chrono_cpu_t * cc)
{
    cpu(Max(&cc->time_point));
}

bool ChronoCpuNow(chrono_cpu_t * cc)
{
#ifndef CHRONO_NO_CLOCK_GETTIME
    return cpu(Now(&cc->time_point, CLOCK_PROCESS_CPUTIME_ID));
#else
    return cpu(Now(&cc->time_point));
#endif

}

bool ChronoCpuDHMS(chrono_cpu_t * cc, int days, int hours, int minutes, int seconds)
{
    return cpu(DHMS(&cc->time_point, days, hours, minutes, seconds));
}

void ChronoCpuIncr(chrono_cpu_t * cc)
{
    cpu(Incr(&cc->time_point));
}

void ChronoCpuDecr(chrono_cpu_t * cc)
{
    cpu(Decr(&cc->time_point));
}

bool ChronoCpuAdd(chrono_cpu_t * cc, chrono_t const * c)
{
    return cpu(Add(&cc->time_point, c));
}

bool ChronoCpuAddValue(chrono_cpu_t * cc, intmax_t value, chrono_period_t cp)
{
    chrono_t c = ChronoInit(value, cp);
    return ChronoCpuAdd(cc, &c);
}

bool ChronoCpuDiff(chrono_cpu_t const * cc1, chrono_cpu_t const * cc2, chrono_t * c)
{
    return cpu(Diff(&cc1->time_point, &cc2->time_point, c));
}

bool ChronoCpuDiffNow(chrono_cpu_t const * cc, chrono_t * c)
{
    chrono_cpu_t now;
    ChronoCpuNow(&now);
    return ChronoCpuDiff(cc, &now, c);
}

int ChronoCpuComp(chrono_cpu_t const * cc1, chrono_cpu_t const * cc2)
{
    return cpu(Comp(&cc1->time_point, &cc2->time_point));
}

void ChronoCpuToTimeT(chrono_cpu_t const * cc, time_t * t)
{
    cpu(ToTimeT(&cc->time_point, t));
}

#ifndef CHRONO_NO_TIMEVAL
void ChronoCpuToTimeVal(chrono_cpu_t const * cc, struct timeval * tv)
{
    cpu(ToTimeVal(&cc->time_point, tv));
}
#endif

#ifndef CHRONO_NO_TIMESPEC
void ChronoCpuToTimeSpec(chrono_cpu_t const * cc, struct timespec * tv)
{
    cpu(ToTimeSpec(&cc->time_point, tv));
}
#endif

#ifndef CHRONO_NO_ANY_SLEEP
int ChronoCpuSleepUntil(chrono_cpu_t const * cc, intmax_t value, chrono_period_t cp)
{
    chrono_t c1;
    chrono_t c2 = ChronoInit(value, cp);
    ChronoCpuDiffNow(cc, &c1);
    ChronoSub(&c1, &c2);
    c1.value = llabs(c1.value);
    return ChronoSleepFor(&c1);
}
#endif

#endif  // cpu
