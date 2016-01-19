/*! @file
  chrono : Chronoの環境定義

  Copyright (C) 2017 Haruhiko Uchida
  The software is released under the MIT license.
  http://opensource.org/licenses/mit-license.php

  @mainpage

  @section 初めに
  Chrono は、OS等のシステムによって異なる時刻操作をカプセル化し、システムに依存しない時刻と期間のインタフェースを提供するC言語のモジュールです.

  最近のOSは、clock_gettime() 関数を持ちることができますが、一昔前では gettimeofday() を用いていました。
  これらの関数はデータ構造が異なっており、移植するために、マイクロ秒とナノ秒の変換を行う一手間が必要があります。
  また、OSのシステム時間の他に、モノトニック時間やCPU時間といった情報が取得できるようになりました。
  しかし、これらはすべて struct timespec という同じデータ構造を用いてしまうため、お互いに演算が出来てしまっていました。
  本来、異なる時間ソース間は、全く異なる時系列であるため、演算を行ってはいけません。
  そこで、Chrono は、clock_gettime() や gettimeofday() のデータ構造を統一的に扱い、異なる時間ソースの演算はすべて独立したデータ構造で扱う設計になりました。

  Chrono は C++11 で標準に追加された std::chrono ライブラリ(もしくは C++ boost ライブラリの boost::chrono) の設計を基にしています。
  std::chrono の方が、Chrono より洗練されているので、一度触っておくことをおすすめします。

  @section 導入
  Chrono は、以下の4つのデータ構造を用います。

  1. 期間 chrono_t
  2. システム時刻 chrono_sys_t
  3. モノトニック時刻 chrono_mno_t
  4. CPU時刻 chrono_cpu_t

  chrono_sys_t 、 chrono_mno_t 、 chrono_cpu_t は、関数の接頭語が ChronoSys 、 ChronoMno 、 ChronoCpu と異なるだけで、いずれも同じ動作の関数を提供しています。

  @subsection 期間
  期間を chrono_t で表すことができます。例えば、10分間を表す場合は、以下のようなコードになります。

  @code
  chrono_t c;

  // 10分間を設定
  c = ChronoInit(10, chrono_minutes);

  // 秒に変換して出力
  printf("%ld seconds.\n", ChronoGet(&c, chrono_seconds));
  @endcode

  @subsection システム時刻
  システム時刻を chrono_sys_t で表すことができます。
  システム時刻とは、システムが持つ時計に対応する値で、ユーザーが任意に値を調整することができる時刻のことです。
  そのため、時刻を過去に変更してしまうと、時間の逆行が発生します。

  @subsection モノトニック時刻
  モノトニック時刻を chrono_mno_t で表すことができます。
  モノトニック時刻とは、時間の逆行が発生しない（システムの起動からの累積）時間のことです。
  システム時刻は、逆行の可能性がありましたが、モノトニック時刻は逆行することがありません。
  便利ではありますが、モノトニック時刻は、時刻を相対的でしか判断できないという欠点があります。

  @subsection CPU時刻
  CPU時刻を chrono_cpu_t で表すことができます。
  CPU時刻は、プロセス実行に要したCPU利用時間の累計です。自然界の秒数とは相関しません。
*/

#ifndef CHRONO_CONFIG_H
#define CHRONO_CONFIG_H

//! time_t の最小サイズ
#define CHRONO_TIME_T_MIN INT64_MIN

//! time_t の最大サイズ
#define CHRONO_TIME_T_MAX INT64_MAX

//! struct timeval が使えない.
//#define CHRONO_NO_TIMEVAL

//! struct timespec が使えない.
//#define CHRONO_NO_TIMESPEC

//! gettimeofday() が使えない
//#define CHRONO_NO_GETTIMEOFDAY

//! clock_gettime() が使わない.
//#define CHRONO_NO_CLOCK_GETTIME

//! すべての sleep() が使えない.
//#define CHRONO_NO_ANY_SLEEP

//! usleep() が使えない.
//#define CHRONO_NO_USLEEP

//! nanosleep() が使えない.
//#define CHRONO_NO_NANOSLEEP

//! CLOCK_MONOTONIC が使用できない場合に、システムの Uptime 時間(秒)を利用しない.
#define CHRONO_MNO_NO_UPTIME

#endif  // CHRONO_CONFIG_H
