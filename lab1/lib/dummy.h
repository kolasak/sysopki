#ifndef __DUMMY_H__


#include <time.h>


int dummy_run_clock_gettime(clockid_t clk_id);

int dummy_run_getrusage();

int dummy_run_times();

int dummy_run_clock();


#endif  /* __DUMMY_H__ */