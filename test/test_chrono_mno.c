#include "chrono.c"
#include "minunit.h"


mu_test_case(MinMax) {
    chrono_mno_t zero, min, max;
    ChronoMnoZero(&zero);
    ChronoMnoMin(&min);
    ChronoMnoMax(&max);
    mu_assert(ChronoMnoComp(&min, &zero) <= 0);
    mu_assert(ChronoMnoComp(&zero, &max) <= 0);
    mu_assert(ChronoMnoComp(&min,  &max) <  0);
}

mu_test_case(Now) {
    chrono_mno_t now, min, max;
    ChronoMnoNow(&now);
    ChronoMnoMin(&min);
    ChronoMnoMax(&max);
    mu_assert(ChronoMnoComp(&min, &now) <= 0);
    mu_assert(ChronoMnoComp(&now, &max) <= 0);
}

mu_test_case(DHMS) {
    chrono_mno_t cs;
    ChronoMnoDHMS(&cs, 1, 2, 3, 4);
    time_t t;
    ChronoMnoToTimeT(&cs, &t);
    struct tm * tm = gmtime(&t);
    mu_assert(tm->tm_yday == 1);
    mu_assert(tm->tm_hour == 2);
    mu_assert(tm->tm_min == 3);
    mu_assert(tm->tm_sec == 4);
}

mu_test_case(Incr) {
    chrono_mno_t cs1, cs2;
    ChronoMnoNow(&cs1);
    cs2 = cs1;
    ChronoMnoIncr(&cs1);
    mu_assert(ChronoMnoComp(&cs1, &cs2) > 0);
    ChronoMnoIncr(&cs2);
    mu_assert(ChronoMnoComp(&cs1, &cs2) == 0);
}

mu_test_case(Decr) {
    chrono_mno_t cs1, cs2;
    ChronoMnoNow(&cs1);
    cs2 = cs1;
    ChronoMnoDecr(&cs1);
    mu_assert(ChronoMnoComp(&cs1, &cs2) < 0);
    ChronoMnoDecr(&cs2);
    mu_assert(ChronoMnoComp(&cs1, &cs2) == 0);
}

mu_test_case(Add) {
    chrono_mno_t cs1, cs2;
    ChronoMnoNow(&cs1);
    cs2 = cs1;
    ChronoMnoAddValue(&cs1, 1, chrono_hours);
    mu_assert(ChronoMnoComp(&cs1, &cs2) > 0);
    ChronoMnoAddValue(&cs2, 60, chrono_minutes);
    mu_assert(ChronoMnoComp(&cs1, &cs2) == 0);
}

mu_test_case(Diff) {
}

mu_test_case(Comp) {
}

mu_test_case(ToTimeT) {
}

mu_test_case(ToTimeVal) {
}

mu_test_case(ToTimeSpec) {
}

int main()
{
    mu_run_test(MinMax);
    mu_run_test(Now);
    mu_run_test(DHMS);
    mu_run_test(Incr);
    mu_run_test(Decr);
    mu_run_test(Add);
    mu_run_test(Diff);
    mu_run_test(Comp);
    mu_run_test(ToTimeT);
    mu_run_test(ToTimeVal);
    mu_run_test(ToTimeSpec);
}
