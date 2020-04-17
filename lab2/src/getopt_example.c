#include "getopt_example.h"

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>



static void op_start_run();

static void op_stop_run();

static void op_restart_run();

static void op_load_run(const char *filename);

static void usage(char *progname);



int main(int argc, char **argv)
{
	int operations_count_check = 0;
	enum operation operation = op_blank;
	char *input_filename = NULL;

	int short_option;
	int option_index;

	struct option long_options[] = {
			{"help",       no_argument,       0,          'h'},
			{"start",      no_argument,       &operation, op_start},   // These
			{"stop",       no_argument,       &operation, op_stop},    // options are
			{"restart",    no_argument,       &operation, op_restart}, // mutually
			{"load",       no_argument,       &operation, op_load},    // exclusive
			{"input-file", required_argument, 0,          'i'},
			{0, 0,                            0,          0}
	};



	while ( 1 )
	{
		short_option = getopt_long(argc, argv, "+hi:",
								   long_options, &option_index);

		if (short_option == -1)
			break;

		switch (short_option)
		{
			case 0:    /* start, stop, restart, load */
				if (operations_count_check > 0)
				{
					fprintf(stderr, "--start, --stop, --restart and --load options are mutually exclusive\n\n");
					usage(argv[0]);
					exit(EXIT_FAILURE);
				}
				operations_count_check++;
				break;

			case 'i':    /* input-file */
				input_filename = optarg;
				break;

			case 'h':    /* help */
			case '?':    /* unrecognized option */
			case ':':    /* required argument not provided */
			default:
				if (short_option == ':')
					fprintf(stderr, "%s option requires argument\n\n", argv[optind - 1]);
				usage(argv[0]);
				exit(EXIT_FAILURE);
		}

	}



	switch (operation)
	{
		case op_start:
			op_start_run();
			break;
		case op_stop:
			op_stop_run();
			break;
		case op_restart:
			op_restart_run();
			break;
		case op_load:
			if (input_filename == NULL)
			{
				fprintf(stderr, "Provide --input-file for --load option\n\n");
				usage(argv[0]);
				exit(EXIT_FAILURE);
			}
			op_load_run(input_filename);
			break;
		default:
			fprintf(stderr, "choose one of --start, --stop, --restart or --load options\n\n");\
            usage(argv[0]);
			exit(EXIT_FAILURE);
	}



	exit(EXIT_SUCCESS);
}



static void op_start_run()
{
	printf("Running dummy start function\n");
}


static void op_stop_run()
{
	printf("Running dummy stop function\n");
}


static void op_restart_run()
{
	printf("Running dummy restart function\n");
}


static void op_load_run(const char *filename)
{
	printf("Running dummy load function on %s file\n", filename);
}


static void usage(char *progname)
{
	printf("Usage: %s --start|--stop|--restart|--load [-i input_file]\n"
		   "\n"
		   "Options:\n"
		   "  -h, --help                 show help\n"
		   "\n"
		   "  Operations:\n"
		   "    --start                  run dummy start function\n"
		   "    --stop                   run dummy stop function\n"
		   "    --restart                run dummy restart function\n"
		   "    --load                   run dummy load function on --input-file\n"
		   "\n"
		   "  --input-file filename    input file for --load operation\n"
		   "\n", progname ? progname : "program_name");
}