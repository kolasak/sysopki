#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dummy.h"


/* Presents some time measuring methods provided by dummy library */
static void main_run()
{
	dummy_run_clock_gettime(CLOCK_REALTIME);
	dummy_run_clock_gettime(CLOCK_REALTIME);
	dummy_run_clock_gettime(CLOCK_REALTIME);
	dummy_run_clock_gettime(CLOCK_REALTIME);
	dummy_run_clock_gettime(CLOCK_REALTIME);
	puts("");

	dummy_run_clock_gettime(CLOCK_MONOTONIC);
	dummy_run_clock_gettime(CLOCK_MONOTONIC);
	dummy_run_clock_gettime(CLOCK_MONOTONIC);
	dummy_run_clock_gettime(CLOCK_MONOTONIC);
	dummy_run_clock_gettime(CLOCK_MONOTONIC);
	puts("");

	dummy_run_clock_gettime(CLOCK_MONOTONIC_RAW);
	dummy_run_clock_gettime(CLOCK_MONOTONIC_RAW);
	dummy_run_clock_gettime(CLOCK_MONOTONIC_RAW);
	dummy_run_clock_gettime(CLOCK_MONOTONIC_RAW);
	dummy_run_clock_gettime(CLOCK_MONOTONIC_RAW);
	puts("");

	dummy_run_clock_gettime(CLOCK_PROCESS_CPUTIME_ID);
	dummy_run_clock_gettime(CLOCK_PROCESS_CPUTIME_ID);
	dummy_run_clock_gettime(CLOCK_PROCESS_CPUTIME_ID);
	dummy_run_clock_gettime(CLOCK_PROCESS_CPUTIME_ID);
	dummy_run_clock_gettime(CLOCK_PROCESS_CPUTIME_ID);
	puts("");

	dummy_run_getrusage();
	dummy_run_getrusage();
	dummy_run_getrusage();
	dummy_run_getrusage();
	dummy_run_getrusage();
	puts("");

	dummy_run_times();
	dummy_run_times();
	dummy_run_times();
	dummy_run_times();
	dummy_run_times();
	puts("");

	dummy_run_clock();
	dummy_run_clock();
	dummy_run_clock();
	dummy_run_clock();
	dummy_run_clock();
	puts("");


	exit(EXIT_SUCCESS);
}



int main(int argc, char **argv)
{
	if (argc != 1)
	{
		printf("Usage\n"
			   "%s <arg1_name> <arg2_name> ...\n"
			   "  arg1_name - arg1_description\n"
			   "  arg2_name - arg2_description\n"
			   "  ...\n"
			   "\n"
			   "  But really this program takes no arguments at the moment\n",
			   argv[0]);

		exit(EXIT_FAILURE);
	}


	main_run();
}