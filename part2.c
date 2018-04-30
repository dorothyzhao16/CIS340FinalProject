#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

void disPerm(char file[]) {

	struct stat fileStat;
	lstat(file, &fileStat);
	//printf("File permissions: \t");
	printf((S_ISDIR(fileStat.st_mode)) ? "d":"-");
	printf((fileStat.st_mode & S_IRUSR) ? "r":"-");
	printf((fileStat.st_mode & S_IWUSR) ? "w":"-");
	printf((fileStat.st_mode & S_IXUSR) ? "x":"-");
	printf((fileStat.st_mode & S_IRGRP) ? "r":"-");
	printf((fileStat.st_mode & S_IWGRP) ? "w":"-");
	printf((fileStat.st_mode & S_IXGRP) ? "x":"-");
	printf((fileStat.st_mode & S_IROTH) ? "r":"-");
	printf((fileStat.st_mode & S_IWOTH) ? "w":"-");
	printf((fileStat.st_mode & S_IXOTH) ? "x":"-");

	printf("\t%ld",fileStat.st_nlink);
	//printf("\t%d",fileStat.st_uid);
	printf("\t%ld",fileStat.st_size);
	
	printf("\t%ld",fileStat.st_atime);
	printf("\t%s",file);
	printf("\n");
}

int main(int argc, char *argv[]) {

	
	//pointer for dir entry
	struct dirent *de;
	DIR *dr = opendir(argv[1]);

	if(dr == NULL) { //opendir returns null if it couldn't open dir
		printf("Could not open %s directory", argv[1]);
		return 0;
	}

	while((de = readdir(dr)) != NULL) {
		char f[50];
		strcpy(f,de->d_name);
		disPerm(f);
	}

	closedir(dr); //close directory		


	return 0;

	/*if(argc == 1) 
		disPerm(".");
	else {
		while(--argc) {
			printf("%s:\n", *++argv);
			disPerm(*argv);
		}
	}*/

}
