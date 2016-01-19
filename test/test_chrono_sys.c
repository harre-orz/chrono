#include "chrono.c"
#include "minunit.h"

mu_test_case(Comp) {
    chrono_sys_t a;
    ChronoSysZero(&a);
    mu_assert(ChronoSysComp(&a, &a) == 0);
}

mu_test_case(MinMax) {
    chrono_sys_t zero, min, max;
    ChronoSysZero(&zero);
    ChronoSysMin(&min);
    ChronoSysMax(&max);
    mu_assert(ChronoSysComp(&min, &zero) <= 0);
    mu_assert(ChronoSysComp(&zero, &max) <= 0);
    mu_assert(ChronoSysComp(&min,  &max) <  0);
}

mu_test_case(Now) {
    chrono_sys_t now, min, max;
    ChronoSysNow(&now);
    ChronoSysMin(&min);
    ChronoSysMax(&max);
    mu_assert(ChronoSysComp(&min, &now) <= 0);
    mu_assert(ChronoSysComp(&now, &max) <= 0);
}

mu_test_case(DHMS) {
    chrono_sys_t cs;
    ChronoSysDHMS(&cs, 1, 2, 3, 4);
    time_t t;
    ChronoSysToTimeT(&cs, &t);
    struct tm * tm = gmtime(&t);
    mu_assert(tm->tm_yday == 1);
    mu_assert(tm->tm_hour == 2);
    mu_assert(tm->tm_min == 3);
    mu_assert(tm->tm_sec == 4);
}

mu_test_case(Incr) {
    chrono_sys_t cs1, cs2;
    ChronoSysNow(&cs1);
    cs2 = cs1;
    ChronoSysIncr(&cs1);
    mu_assert(ChronoSysComp(&cs1, &cs2) > 0);
    ChronoSysIncr(&cs2);
    mu_assert(ChronoSysComp(&cs1, &cs2) == 0);
}

mu_test_case(Decr) {
    chrono_sys_t cs1, cs2;
    ChronoSysNow(&cs1);
    cs2 = cs1;
    ChronoSysDecr(&cs1);
    mu_assert(ChronoSysComp(&cs1, &cs2) < 0);
    ChronoSysDecr(&cs2);
    mu_assert(ChronoSysComp(&cs1, &cs2) == 0);
}

mu_test_case(Add) {
    chrono_sys_t cs1, cs2;
    ChronoSysNow(&cs1);
    cs2 = cs1;

    ChronoSysAddValue(&cs1, 1, chrono_hours);
    mu_assert(ChronoSysComp(&cs1, &cs2) > 0);

    ChronoSysAddValue(&cs2, 60, chrono_minutes);
    mu_assert(ChronoSysComp(&cs1, &cs2) == 0);

    ChronoSysAddValue(&cs1, -1, chrono_hours);
    mu_assert(ChronoSysComp(&cs1, &cs2) < 0);

    ChronoSysAddValue(&cs2, -60, chrono_minutes);
    mu_assert(ChronoSysComp(&cs1, &cs2) == 0);
}

mu_test_case(AddMicro) {
    chrono_sys_t cs1, cs2;
    ChronoSysNow(&cs1);
    cs2 = cs1;

    ChronoSysAddValue(&cs1, 1, chrono_microseconds);
    mu_assert(ChronoSysComp(&cs1, &cs2) > 0);

    ChronoSysAddValue(&cs2, 1, chrono_microseconds);
    mu_assert(ChronoSysComp(&cs1, &cs2) == 0);

    ChronoSysAddValue(&cs1, -1, chrono_microseconds);
    mu_assert(ChronoSysComp(&cs1, &cs2) < 0);

    ChronoSysAddValue(&cs2, -1, chrono_microseconds);
    mu_assert(ChronoSysComp(&cs1, &cs2) == 0);
}

mu_test_case(AddNano) {
    chrono_sys_t cs1, cs2;
    ChronoSysNow(&cs1);
    cs2 = cs1;

    ChronoSysAddValue(&cs1, 1, chrono_nanoseconds);
    mu_assert(ChronoSysComp(&cs1, &cs2) > 0);

    ChronoSysAddValue(&cs2, 1, chrono_nanoseconds);
    mu_assert(ChronoSysComp(&cs1, &cs2) == 0);

    ChronoSysAddValue(&cs1, -1, chrono_nanoseconds);
    mu_assert(ChronoSysComp(&cs1, &cs2) < 0);

    ChronoSysAddValue(&cs2, -1, chrono_nanoseconds);
    mu_assert(ChronoSysComp(&cs1, &cs2) == 0);
}

mu_test_case(Diff) {
    chrono_sys_t zero, min, max, now;
    ChronoSysZero(&zero);
    ChronoSysMin(&min);
    ChronoSysMax(&max);
    ChronoSysNow(&now);

    chrono_t diff;
    ChronoSysDiff(&zero, &zero, &diff);
    mu_assert(ChronoGet(&diff, chrono_nanoseconds) == 0);

    ChronoSysDiff(&zero, &min, &diff);
    mu_assert(ChronoGet(&diff, chrono_seconds) > 0);

    ChronoSysDiff(&zero, &max, &diff);
    mu_assert(ChronoGet(&diff, chrono_seconds) > 0);

    ChronoSysDiff(&min, &max, &diff);
    mu_assert(ChronoGet(&diff, chrono_seconds) > 0);
}

mu_test_case(ToTimeT) {
}

mu_test_case(ToTimeVal) {
}

mu_test_case(ToTimeSpec) {
}

int main()
{
    mu_run_test(Comp);
    mu_run_test(MinMax);
    mu_run_test(Now);
    mu_run_test(DHMS);
    mu_run_test(Incr);
    mu_run_test(Decr);
    mu_run_test(Add);
#if !defined(CHRONO_NO_TIMEVAL)
    mu_run_test(AddMicro);
#endif
#if !defined(CHRONO_NO_TIMESPEC)
    mu_run_test(AddNano);
#endif
    mu_run_test(Diff);
    mu_run_test(ToTimeT);
    mu_run_test(ToTimeVal);
    mu_run_test(ToTimeSpec);
}
