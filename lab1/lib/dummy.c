/**
 * Dummy library.
 *   Performs assembly nop operation specified number of times.
 *   Time spent performing this task is measured and displayed.
 *   This library tries to show some time measuring methods available on UNIX system
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/times.h>

#include "dummy.h"


#define OP_CNT 1e8


/* Burns cycles unless gcc intervenes */
static void operations()
{
	for (int i = 0; i < OP_CNT; i++)
			asm("nop");
}


static void timespec_diff(struct timespec *a, struct timespec *b, struct timespec *result)
{
	result->tv_sec = a->tv_sec - b->tv_sec;
	result->tv_nsec = a->tv_nsec - b->tv_nsec;
	if (result->tv_nsec < 0)
	{
		--result->tv_sec;
		result->tv_nsec += 1000000000;
	}
}



int dummy_run_clock_gettime(clockid_t clk_id)
{
	struct timespec ts_start;
	struct timespec ts_end;
	struct timespec ts_diff;


	clock_gettime(clk_id, &ts_start);
	operations();
	clock_gettime(clk_id, &ts_end);


	timespec_diff(&ts_end, &ts_start, &ts_diff);
	printf("clock_gettime():    operation time: %ld.%09ld\n", ts_diff.tv_sec, ts_diff.tv_nsec);

	return 0;
}


int dummy_run_getrusage()
{
	struct rusage usage_start;
	struct rusage usage_end;
	struct timeval user_tv_diff;
	struct timeval system_tv_diff;


	getrusage(RUSAGE_SELF, &usage_start);
	operations();
	getrusage(RUSAGE_SELF, &usage_end);


	timersub(&usage_end.ru_utime, &usage_start.ru_utime, &user_tv_diff);
	timersub(&usage_end.ru_stime, &usage_start.ru_stime, &system_tv_diff);
	printf("    getrusage():   user operation time: %ld.%06ld\n", user_tv_diff.tv_sec, user_tv_diff.tv_usec);
	printf("    getrusage(): system operation time: %ld.%06ld\n", system_tv_diff.tv_sec, system_tv_diff.tv_usec);

	return 0;
}


int dummy_run_times()
{
	struct tms tms_start;
	struct tms tms_end;
	clock_t real_clock_start, real_clock_end, real_clock_diff;
	clock_t user_clock_diff;
	clock_t system_clock_diff;

	long clock_ticks_per_sec;


	real_clock_start = times(&tms_start);
	operations();
	real_clock_end = times(&tms_end);


	real_clock_diff = real_clock_end - real_clock_start;
	user_clock_diff = tms_end.tms_utime - tms_start.tms_utime;
	system_clock_diff = tms_end.tms_stime - tms_start.tms_stime;

	clock_ticks_per_sec = sysconf(_SC_CLK_TCK);
	printf("times():   real clock ticks: %ld (clock_ticks_per_sec: %ld)\n", real_clock_diff, clock_ticks_per_sec);
	printf("times():   user clock ticks: %ld\n", user_clock_diff);
	printf("times(): system clock ticks: %ld\n", system_clock_diff);

	return 0;
}


int dummy_run_clock()
{
	clock_t clock_start;
	clock_t clock_end;
	clock_t clock_diff;


	clock_start = clock();
	operations();
	clock_end = clock();


	clock_diff = clock_end - clock_start;
	printf("clock():             clocks: %ld (CLOCKS_PER_SEC = %ld)\n", clock_diff, CLOCKS_PER_SEC);

	return 0;
}




