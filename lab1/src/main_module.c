#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <dlfcn.h>



int (*dummy_run_clock_gettime)(clockid_t);

int (*dummy_run_getrusage)();

int (*dummy_run_times)();

int (*dummy_run_clock)();



static void load_dynamic_dummy_library(void **handle)
{
	char *error;


	handle = dlopen("./libdummy_module.so", RTLD_LAZY);
	if (!handle)
	{
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}


	dlerror();    /* Clear any existing error */

	dummy_run_clock_gettime = (int (*)(clockid_t)) dlsym(handle, "dummy_run_clock_gettime");  /* One way to do it */
	dummy_run_getrusage = (int (*)()) dlsym(handle, "dummy_run_getrusage");
	*(void **) (&dummy_run_times) = dlsym(handle, "dummy_run_times"); 	/* Other way */
	*(void **) (&dummy_run_clock) = dlsym(handle, "dummy_run_clock");

	/* Check if all functions loaded successfully. */
	error = dlerror();
	if (error != NULL)
	{
		fprintf(stderr, "%s\n", error);
		exit(EXIT_FAILURE);
	}
}


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

	void *dummy_library_handle;
	load_dynamic_dummy_library(&dummy_library_handle);


	main_run();

	/* Don't really need to dlclose, since it will be closed on exit anyway. Not sure if this is good practice tho */
//	dlclose(dummy_library_handle);
}