#include "chrono.c"
#include "minunit.h"
#include <stdio.h>

mu_test_case(Get) {
    chrono_t c = ChronoInit(86400, chrono_seconds);
    mu_assert(ChronoGet(&c, chrono_days) == 1L);
    mu_assert(ChronoGet(&c, chrono_hours) == 1L * 24);
    mu_assert(ChronoGet(&c, chrono_minutes) == 1L * 24 * 60);
    mu_assert(ChronoGet(&c, chrono_seconds) == 1L * 24 * 60 * 60);
    mu_assert(ChronoGet(&c, chrono_milliseconds) == 1L * 24 * 60 * 60 * 1000);
    mu_assert(ChronoGet(&c, chrono_microseconds) == 1L * 24 * 60 * 60 * 1000 * 1000);
    mu_assert(ChronoGet(&c, chrono_nanoseconds) == 1L * 24 * 60 * 60 * 1000 * 1000 * 1000);

    c = ChronoInit(100, chrono_milliseconds);
    mu_assert(ChronoGet(&c, -20000) == 2000);

    c = ChronoInit(1, 20);
    mu_assert(ChronoGet(&c, -2000) == 40000);

    c = ChronoInit(1000, -100);
    mu_assert(ChronoGet(&c, 10) == 1);
}

mu_test_case(Add) {
    chrono_t c = ChronoInit(1, chrono_days);

    ChronoAddValue(&c, 10, chrono_hours);
    mu_assert(ChronoGet(&c, chrono_hours) == (24 + 10));

    ChronoAddValue(&c, 100, chrono_minutes);
    mu_assert(ChronoGet(&c, chrono_minutes) == (24 + 10) * 60 + 100);

    ChronoAddValue(&c, 1000, chrono_seconds);
    mu_assert(ChronoGet(&c, chrono_seconds) == ((24 + 10) * 60 + 100) * 60 + 1000);

    ChronoAddValue(&c, 100, chrono_milliseconds);
    mu_assert(ChronoGet(&c, chrono_milliseconds) == (((24L + 10) * 60 + 100) * 60 + 1000) * 1000 + 100);

    ChronoAddValue(&c, 1000, chrono_microseconds);
    mu_assert(ChronoGet(&c, chrono_milliseconds) == (((24L + 10) * 60 + 100) * 60 + 1000) * 1000 + 101);
}

mu_test_case(Sub) {
    chrono_t c = ChronoInit(1, chrono_days);

    ChronoSubValue(&c, 10, chrono_hours);
    mu_assert(ChronoGet(&c, chrono_hours) == (24 - 10));

    ChronoSubValue(&c, 100, chrono_minutes);
    mu_assert(ChronoGet(&c, chrono_minutes) == (24 - 10) * 60 - 100);

    ChronoSubValue(&c, 1000, chrono_seconds);
    mu_assert(ChronoGet(&c, chrono_seconds) == ((24 - 10) * 60 - 100) * 60 - 1000);

    ChronoSubValue(&c, 100, chrono_milliseconds);
    mu_assert(ChronoGet(&c, chrono_milliseconds) == (((24 - 10) * 60 - 100) * 60 - 1000) * 1000 - 100);

    ChronoSubValue(&c, 1000, chrono_microseconds);
    mu_assert(ChronoGet(&c, chrono_milliseconds) == (((24 - 10) * 60 - 100) * 60 - 1000) * 1000 - 101);
}

mu_test_case(ToTimeT) {
    chrono_t c = ChronoInit(12345, chrono_milliseconds);
    time_t t;
    ChronoToTimeT(&c, &t);
    mu_assert(t == 12);
}

mu_test_case(ToTimeVal) {
    chrono_t c = ChronoInit(12345, chrono_milliseconds);
    struct timeval tv;
    ChronoToTimeVal(&c, &tv);
    mu_assert(tv.tv_sec == 12);
    mu_assert(tv.tv_usec == 345000);
}

mu_test_case(ToTimeSpec) {
    chrono_t c = ChronoInit(12345, chrono_milliseconds);
    struct timespec tv;
    ChronoToTimeSpec(&c, &tv);
    mu_assert(tv.tv_sec = 12);
    mu_assert(tv.tv_nsec = 345000000);
}

int main()
{
    mu_run_test(Get);
    mu_run_test(Add);
    mu_run_test(Sub);
    mu_run_test(ToTimeT);
    mu_run_test(ToTimeVal);
    mu_run_test(ToTimeSpec);
}
