#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/random.h>


#define GENERATED_BYTES_COUNT 1024


int main()
{
	/**
	 * Load random values to dynamically allocated buffer.
	 *   I used trick with while(0) loop solely to reduce scope of variables.
	 */
	do
	{
		char *buffer;
		size_t length = GENERATED_BYTES_COUNT;
		int n;

		buffer = malloc(GENERATED_BYTES_COUNT * sizeof(*buffer));

		n = getrandom(buffer, length, 0);
		if (n == -1)
		{
			perror("getrandom");
			exit(EXIT_FAILURE);
		}

		puts("Generated bytes");
		write(STDOUT_FILENO, buffer, n);

		free(buffer);
	} while ( 0 );


	/**
	 * Or to statically allocated buffer
	 */
	do
	{
		char buffer[GENERATED_BYTES_COUNT];
		int n;

		n = getrandom(buffer, sizeof(buffer), 0);
		if (n == -1)
		{
			perror("getrandom");
			exit(EXIT_FAILURE);
		}

		puts("\nGenerated bytes");
		write(STDOUT_FILENO, buffer, n);
	} while ( 0 );
}