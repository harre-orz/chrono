/*! @file
  Chrono : CPU時間モジュール

  Copyright (C) 2017 Nippon C.A.D. Co.,Ltd. All Rights Reserved.
  This code was designed and coded by Haruhiko Uchida.
*/

#ifndef CHRONO_CPU_H
#define CHRONO_CPU_H

#include "chrono.h"

/*!
  CPU時刻.
 */
#if !defined(CHRONO_NO_CLOCK_GETTIME)
# if !defined(CLOCK_PROCESS_CPUTIME_ID)
#  error "Undeclared CLOCK_PROCESS_CPUTIME_ID"
# endif
  typedef struct {
    struct timespec time_point;
  } chrono_cpu_t;
#else
# error "Disabled ChronoCpu"
#endif


/*!
  CPU時刻 0 を cc に設定する.
 */
extern void ChronoCpuZero(chrono_cpu_t * cc);


/*!
  最小のCPU時刻を cc に設定する.
 */
extern void ChronoCpuMin(chrono_cpu_t * cc);


/*!
  最大のCPU時刻を cc に設定する.
 */
extern void ChronoCpuMax(chrono_cpu_t * cc);


/*!
  現在のCPU時刻を cc に設定する.
 */
extern bool ChronoCpuNow(chrono_cpu_t * cc);


/*!
  (days, hours, minutes, seconds) をCPU時刻 cc に設定する.
*/
extern bool ChronoCpuDHMS(chrono_cpu_t * cc, int days, int hours, int minutes, int seconds);


/*!
  CPU時刻 cc を最大分解能でインクリメントする.
 */
extern void ChronoCpuIncr(chrono_cpu_t * cc);


/*!
  CPU時刻 cc を最大n分解能でデクリメントする.
 */
extern void ChronoCpuDecr(chrono_cpu_t * cc);


/*!
  CPU時刻 cc に期間 c を加算する.
 */
extern bool ChronoCpuAdd(chrono_cpu_t * cc, chrono_t const * c);


/*!
  CPU時刻 cc に期間 (value, period) を加算する.
 */
extern bool ChronoCpuAddValue(chrono_cpu_t * cc, intmax_t value, chrono_period_t period);


/*!
  CPU時刻 cc1 - cc2 間の時間差(絶対値)を c に設定する.
*/
extern bool ChronoCpuDiff(chrono_cpu_t const * cc1, chrono_cpu_t const * cc2, chrono_t * c);

/*!
  CPU時刻 cc - 現在時刻までの時間差(絶対値)を c に設定する.
*/
extern bool ChronoCpuDiffNow(chrono_cpu_t const * cc, chrono_t * c);


/*!
  CPU時刻 cc1 が小さいと <0, cc1 が大きいと 0< を返す.
*/
extern int ChronoCpuComp(chrono_cpu_t const * cc1, chrono_cpu_t const * cc2);


/*!
  CPU時刻 cc を time_t に変換する.
*/
extern void ChronoCpuToTimeT(chrono_cpu_t const * cc, time_t * t);


#ifndef CHRONO_NO_TIMEVAL
/*!
  CPU時刻 cc を struct timespec に変換する.
*/
extern void ChronoCpuToTimeVal(chrono_cpu_t const * cc, struct timeval * tv);
#endif


#ifndef CHRONO_NO_TIMESPEC
/*!
  CPU時刻 cs を struct timespec に変換する.
*/
extern void ChronoCpuToTimeSpec(chrono_cpu_t const * cc, struct timespec * ts);
#endif


#ifndef CHRONO_NO_ANY_SLEEP
/*!
  CPU時刻 cc から期間 (value, period) 経過するまで待つ.
*/
extern int ChronoCpuSleepUntil(chrono_cpu_t const * cs, intmax_t value, chrono_period_t cp);
#endif

#endif //CHRONO_CPU_H
