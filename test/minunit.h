#ifndef MINUNIT_H
#define MINUNIT_H

#include <stdio.h>

#define mu_failed(file, line, expr) printf("%s:%u: failed assertion `%s'\n", file,line, expr)
#define mu_tested(test, passed) printf("Test: %-10s ... %s\n", test, (passed) ? "passed" : "FAILED")
#define mu_assert(expr) do { mu_nassert++; if(!(expr)) { ++mu_nfail; mu_failed(__FILE__, __LINE__, #expr); return;} } while(0)
#define mu_test_case(test) static void  mu_test_case_ ## test()
#define mu_run_test(test) do { int save = mu_nfail; ++mu_ntest; mu_test_case_ ## test(); mu_tested(#test, (save == mu_nfail)); } while(0)
#define mu_show_failures() do { printf("### Failed %d of %d asserts (%d tests).\n", mu_nfail, mu_nassert, mu_ntest); } while(0)

int mu_nfail;
int mu_ntest;
int mu_nassert;

#endif
