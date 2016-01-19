#include "chrono.c"
#include "minunit.h"

mu_test_case(MinMax) {
    chrono_cpu_t zero, min, max;
    ChronoCpuZero(&zero);
    ChronoCpuMin(&min);
    ChronoCpuMax(&max);
    mu_assert(ChronoCpuComp(&min, &zero) <= 0);
    mu_assert(ChronoCpuComp(&zero, &max) <= 0);
    mu_assert(ChronoCpuComp(&min,  &max) <  0);
}

mu_test_case(Now) {
    chrono_cpu_t now, min, max;
    ChronoCpuNow(&now);
    ChronoCpuMin(&min);
    ChronoCpuMax(&max);
    mu_assert(ChronoCpuComp(&min, &now) <= 0);
    mu_assert(ChronoCpuComp(&now, &max) <= 0);
}

mu_test_case(DHMS) {
    chrono_cpu_t cs;
    ChronoCpuDHMS(&cs, 1, 2, 3, 4);
    time_t t;
    ChronoCpuToTimeT(&cs, &t);
    struct tm * tm = gmtime(&t);
    mu_assert(tm->tm_yday == 1);
    mu_assert(tm->tm_hour == 2);
    mu_assert(tm->tm_min == 3);
    mu_assert(tm->tm_sec == 4);
}

mu_test_case(Incr) {
    chrono_cpu_t cs1, cs2;
    ChronoCpuNow(&cs1);
    cs2 = cs1;
    ChronoCpuIncr(&cs1);
    mu_assert(ChronoCpuComp(&cs1, &cs2) > 0);
    ChronoCpuIncr(&cs2);
    mu_assert(ChronoCpuComp(&cs1, &cs2) == 0);
}

mu_test_case(Decr) {
    chrono_cpu_t cs1, cs2;
    ChronoCpuNow(&cs1);
    cs2 = cs1;
    ChronoCpuDecr(&cs1);
    mu_assert(ChronoCpuComp(&cs1, &cs2) < 0);
    ChronoCpuDecr(&cs2);
    mu_assert(ChronoCpuComp(&cs1, &cs2) == 0);
}

mu_test_case(Add) {
    chrono_cpu_t cs1, cs2;
    ChronoCpuNow(&cs1);
    cs2 = cs1;
    ChronoCpuAddValue(&cs1, 1, chrono_hours);
    mu_assert(ChronoCpuComp(&cs1, &cs2) > 0);
    ChronoCpuAddValue(&cs2, 60, chrono_minutes);
    mu_assert(ChronoCpuComp(&cs1, &cs2) == 0);
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
