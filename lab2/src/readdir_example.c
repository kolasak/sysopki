#include <stdio.h>
#include <stdlib.h>

#include <dirent.h>



int main()
{
	DIR *dirp;
	struct dirent *dirent_buf;


	dirp = opendir(".");
	if (dirp == NULL)
	{
		perror("opendir");
		exit(EXIT_FAILURE);
	}


	while ((dirent_buf = readdir(dirp)))
	{
		switch (dirent_buf->d_type)
		{
			case DT_DIR:
				printf("  DIR: %s\n", dirent_buf->d_name);
				break;

				/* ... */

			case DT_REG:
			default:
				printf("OTHER: %s\n", dirent_buf->d_name);
				break;
		}
	}

	return 0;
}