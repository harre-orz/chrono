/*! @file
  Chrono : システム時間モジュール

  Copyright (C) 2017 Haruhiko Uchida
  The software is released under the MIT license.
  http://opensource.org/licenses/mit-license.php
 */

#ifndef CHRONO_SYS_H
#define CHRONO_SYS_H

#include "chrono.h"

/*!
  システム時刻.
 */
#if !defined(CHRONO_NO_CLOCK_GETTIME)
# if !defined(CLOCK_REALTIME)
#  error "Undeclared CLOCK_REALTIME"
# endif
  typedef struct {
      struct timespec time_point;
  } chrono_sys_t;
#elif !defined(CHRONO_NO_GETTIMEOFDAY)
# include <sys/time.h>
  typedef struct {
      struct timeval time_point;
  } chrono_sys_t;
#else
  typedef struct {
    time_t time_point;
  } chrono_sys_t;
#endif


/*!
  システム時刻 0 を cs に設定する.
 */
extern void ChronoSysZero(chrono_sys_t * cs);


/*!
  最小のシステム時刻を cs に設定する.
 */
extern void ChronoSysMin(chrono_sys_t * cs);


/*!
  最大のシステム時刻を cs に設定する.
 */
extern void ChronoSysMax(chrono_sys_t * cs);


/*!
  現在のシステム時刻を cs に設定する.
 */
extern bool ChronoSysNow(chrono_sys_t * cs);


/*!
  (days, hours, minutes, seconds) をシステム時刻 cs に設定する.
 */
extern bool ChronoSysDHMS(chrono_sys_t * cs, int days, int hours, int minutes, int seconds);


/*!
  システム時刻 cs を最大分解でインクリメントする.
 */
extern void ChronoSysIncr(chrono_sys_t * cs);


/*!
  システム時刻 cs を最大分解能でデクリメントする.
 */
extern void ChronoSysDecr(chrono_sys_t * cs);


/*!
  システム時刻 cs に期間 c を加算する.
 */
extern bool ChronoSysAdd(chrono_sys_t * cs, chrono_t const * c);


/*!
  システム時刻 cs に期間 (value, period) を加算する.
 */
extern bool ChronoSysAddValue(chrono_sys_t * cs, intmax_t value, chrono_period_t period);


/*!
  システム時刻 cs1 - cs2 間の時間差(絶対値)を c に設定する.
*/
extern bool ChronoSysDiff(chrono_sys_t const * cs1, chrono_sys_t const * cs2, chrono_t * c);


/*!
  システム時刻 cs - 現在時刻までの時間差(絶対値)を c に設定する.
*/
extern bool ChronoSysDiffNow(chrono_sys_t const * cs, chrono_t * c);


/*!
  システム時刻 cs1 が小さいと <0, cs1 が大きいと 0< を返す.
*/
extern int ChronoSysComp(chrono_sys_t const * cs1, chrono_sys_t const * cs2);


/*!
  システム時刻 cs を time_t に変換する.
*/
extern void ChronoSysToTimeT(chrono_sys_t const * cs, time_t * t);


#ifndef CHRONO_NO_TIMEVAL
/*!
  システム時刻 cs を struct timeval に変換する.
*/
extern void ChronoSysToTimeVal(chrono_sys_t const * cs, struct timeval * tv);
#endif


#ifndef CHRONO_NO_TIMESPEC
/*!
  システム時刻 cs を struct timespec に変換する.
*/
extern void ChronoSysToTimeSpec(chrono_sys_t const * cs, struct timespec * ts);
#endif


#ifndef CHRONO_NO_ANY_SLEEP
/*!
  システム時刻 cm から期間 (value, period) 経過するまで待つ.
*/
extern int ChronoSysSleepUntil(chrono_sys_t const * cs, intmax_t value, chrono_period_t period);
#endif

#endif // CHRONO_SYS_H
