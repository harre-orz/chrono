/*! @file
 Chrono : モノトニック時間モジュール

 Copyright (C) 2017 Haruhiko Uchida
 The software is released under the MIT license.
 http://opensource.org/licenses/mit-license.php
*/

#ifndef CHRONO_MNO_H
#define CHRONO_MNO_H

#include "chrono.h"

/*!
  モノトニック時刻.
 */
#if !defined(CHRONO_NO_CLOCK_GETTIME)
  typedef struct {
    struct timespec time_point;
  } chrono_mno_t;
#elif !defined(CHRONO_MNO_NO_UPTIME)
  typedef struct {
    time_t time_point;
  } chrono_mno_t;
#else
# error "Disabled ChronoMno"
#endif


/*!
  モノトニック時刻 0 を cm に設定する.
 */
extern void ChronoMnoZero(chrono_mno_t * cm);


/*!
  最小のモノトニック時刻を cm に設定する.
 */
extern void ChronoMnoMin(chrono_mno_t * cm);


/*!
  最大のモノトニック時刻を cm に設定する.
 */
extern void ChronoMnoMax(chrono_mno_t * cm);


/*!
  現在のモノトニック時刻を cm に設定する.
*/
extern bool ChronoMnoNow(chrono_mno_t * cm);


/*!
  (days, hours, minutes, seconds) をモノトニック時刻 cs に設定する.
 */
extern bool ChronoMnoDHMS(chrono_mno_t * cm, int days, int hours, int minutes, int seconds);


/*!
  モノトニック時刻 cm を最大分解能でインクリメントする.
 */
extern void ChronoMnoIncr(chrono_mno_t * cm);


/*!
  モノトニック時刻 cm を最大分解能でデクリメントする.
 */
extern void ChronoMnoDecr(chrono_mno_t * cm);


/*!
  モノトニック時刻 cm に期間 c を加算する.
 */
extern bool ChronoMnoAdd(chrono_mno_t * cm, chrono_t const * c);


/*!
  モノトニック時刻 cm に期間 (value, period) を加算する.
*/
extern bool ChronoMnoAddValue(chrono_mno_t * cm, intmax_t value, chrono_period_t period);


/*!
  モノトニック時刻 cm1 - cm2 間の時間差(絶対値)を c に設定する.
*/
extern bool ChronoMnoDiff(chrono_mno_t const * cm1, chrono_mno_t const * cm2, chrono_t * c);


/*!
  モノトニック時刻 cm - 現在時刻までの時間差(絶対値)を c に設定する.
*/
extern bool ChronoMnoDiffNow(chrono_mno_t const * cm, chrono_t * c);


/*!
  モノトニック時刻 cm1 が小さいと <0, cs1 が大きいと 0< を返す.
*/
extern int ChronoMnoComp(chrono_mno_t const * cm1, chrono_mno_t const * cm2);


/*!
  モノトニック時刻 cm を time_t に変換する.
*/
extern void ChronoMnoToTimeT(chrono_mno_t const * cm, time_t * t);


#ifndef CHRONO_NO_TIMEVAL
/*!
  モノトニック時刻 cm を struct timeval に変換する.
*/
extern void ChronoMnoToTimeVal(chrono_mno_t const * cm, struct timeval * tv);
#endif


#ifndef CHRONO_NO_TIMESPEC
/*!
  モノトニック時刻 cm を struct timespec に変換する.
*/
extern void ChronoMnoToTimeSpec(chrono_mno_t const * cm, struct timespec * ts);
#endif


#ifndef CHRONO_NO_ANY_SLEEP
/*!
  モノトニック時刻 cm から期間 (value, period) 経過するまで待つ.
*/
extern int ChronoMnoSleepUntil(chrono_mno_t const * cm, intmax_t value, chrono_period_t period);
#endif

#endif //CHRONO_MNO_H
