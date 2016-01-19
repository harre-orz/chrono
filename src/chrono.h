/*! @file
  Chrono : 期間（時間差）モジュール

  Copyright (C) 2017 Haruhiko Uchida
  The software is released under the MIT license.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef CHRONO_H
#define CHRONO_H

#include "chrono_config.h"
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifndef CHRONO_NO_TIMEVAL
#include <sys/time.h>
#endif

/*!
  時間倍率.
*/
typedef enum {
    chrono_days         = 86400,       //!< 日
    chrono_hours        = 3600,        //!< 時
    chrono_minutes      = 60,          //!< 分
    chrono_seconds      = 1,           //!< 秒
    chrono_milliseconds = -1000,       //!< ミリ秒
    chrono_microseconds = -1000000,    //!< マイクロ秒
    chrono_nanoseconds  = -1000000000, //!< ナノ秒
} chrono_period_t;


/*!
 期間.
 直接メンバを操作せずに、関数を使うこと

 情報を value と period に分けることで、有効数字を変化させている
 例えば value = 10, period = chrono_minutes の場合、10分として扱う
*/
typedef struct {
    intmax_t value;  //!< 数値
    chrono_period_t period;  //!< 倍率
} chrono_t;


/*!
  期間構造体を初期化する.
*/
#define ChronoInit(c, cp) (chrono_t){ (c), (cp) }


/*!
  期間 c を、時間倍率 period に変換して取得する.

  指定した倍率よりも小さい位は、切り捨てられる
*/
extern intmax_t ChronoGet(chrono_t const * c, chrono_period_t period);


/*!
  期間 c に期間 rhs を加算する.
*/
extern void ChronoAdd(chrono_t * c, chrono_t const * rhs);


/*!
  期間 c に期間 (value, period) を加算する.
*/
extern void ChronoAddValue(chrono_t * c, intmax_t value, chrono_period_t period);


/*!
  期間 c から期間 rhs を減算する.
 */
extern void ChronoSub(chrono_t * c, chrono_t const * rhs);


/*!
  期間 c  から期間 (value, period) を減算する.
*/
extern void ChronoSubValue(chrono_t * c, intmax_t value, chrono_period_t period);


/*!
  期間 c を time_t (秒の位)に変換する.
  秒より小さい位は、切り捨てられる
*/
extern void ChronoToTimeT(chrono_t const * c, time_t * t);


#ifndef CHRONO_NO_TIMEVAL
/*!
  期間 c を struct timeval に変換する.
  マイクロ秒より小さい位は、切り捨てられる
*/
extern void ChronoToTimeVal(chrono_t const * c, struct timeval * tv);
#endif


#ifndef CHRONO_NO_TIMESPEC
/*!
  期間 c を struct timespec に変換する.
  ナノ秒より小さい位は、切り捨てられる
*/
extern void ChronoToTimeSpec(chrono_t const * c, struct timespec * tv);
#endif


#ifndef CHRONO_NO_ANY_SLEEP
/*!
  期間 c だけ sleep する.
*/
extern int ChronoSleepFor(chrono_t const * c);


/*!
  期間 (value, period) だけ sleep する.
*/
extern int ChronoSleepForValue(intmax_t value, chrono_period_t period);
#endif

#endif // CHRONO_H
