#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <ftw.h>



static int file_visit(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf);


int main(int argc, char **argv)
{
	printf("TEST TEST TEST'%*c'TEST TEST TEST\n", 5, ' ');
	nftw(".", file_visit, 10, FTW_CHDIR | FTW_PHYS);

	return 0;
}


int file_visit(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
	char *file_name =  &fpath[ftwbuf->base];

	
	
	printf("%*c", 2 * ftwbuf->level, ' ');
	
	
	switch (typeflag)
	{
		case FTW_D:
			printf("\n DIR: %s - uid: %d\n", file_name, sb->st_uid);
			break;

			/* ... */

		case FTW_F:
		default:
			printf("FILE: %s - size: %ld\n", file_name, sb->st_size);
			break;
	}

	return 0;
}
