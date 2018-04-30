#include <sys/types.h> 
#include <dirent.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 

void do_ls(char[]);
void dostat(char *);
void show_file_info (char *, struct stat *);
void mode_to_letters(int, char []);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);

main(int ac, char *av[])
{
	if(ac==1)
		do_ls(".");
	else
		while(--ac){
			printf("%s:\n", *++av);
			do_ls(*av);
		}
}
void do_ls(char dirname[]){
	DIR				*dir_ptr;
	struct dirent	*direntp;
	
	if ((dir_ptr = opendir(dirname))==NULL)
		fprintf(stderr,"ls-l: cannot open %s:\n", dirname);
	else {
		while ((direntp = readdir(dir_ptr))!=NULL)
			dostat(direntp->d_name);
		closedir(dir_ptr);
	}
}

void dostat(char *filename)
{
	struct stat info;
	if (stat(filename, &info)==-1)
		perror(filename);
	else
		show_file_info(filename, &info);
}

void show_file_info(char *filename, struct stat *info_p){
	char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void mode_to_letters();
	char modestr[11];
	char str1, str2;
    
    	scanf("%2s",str1);
    	str2 = "-l";

    	if (str1 == str2) {
		mode_to_letters(info_p->st_mode, modestr);
		printf( " %s "	,modestr);
		printf( " %4d "	,(int)info_p->st_nlink);
		printf( " %-8s "	,uid_to_name(info_p->st_uid));
		printf( " %-8s "	,gid_to_name(info_p->st_gid));
		printf( " %8ld "	,(long)info_p->st_size);
		printf( " %.12s "	,4+ctime(&info_p->st_mtime));
		printf( " %s\n "	,filename);
	}
}

void mode_to_letters(int mode, char str[]){
strcpy(str, "----------");
	if(S_ISDIR(mode)) str[0]='d';
	if(S_ISCHR(mode)) str[0]='c';
	if(S_ISBLK(mode)) str[0]='b';
	
	if (mode & S_IRUSR) str[1]='r';
	if (mode & S_IWUSR) str[2]='w';
	if (mode & S_IXUSR) str[3]='x';
	
	if (mode & S_IRGRP) str[4]='r';
	if (mode & S_IWGRP) str[5]='w';
	if (mode & S_IXGRP) str[6]='x';
	
	if (mode & S_IROTH) str[7]='r';
	if (mode & S_IWOTH) str[8]='w';
	if (mode & S_IXOTH) str[9]='x';
}

#include	<pwd.h>

char *uid_to_name(uid_t uid){
	struct passwd *getpwuid(), *pw_ptr;
	static char numstr[10];
	
	if((pw_ptr=getpwuid(uid))==NULL){
		sprintf(numstr,"%d",uid);
		return numstr;
	}
	else
			return pw_ptr->pw_name;
}
#include <grp.h>
char *gid_to_name(gid_t gid){
	struct group *getgrgid(), *grp_ptr;
	static char numstr[10];
	
	if((grp_ptr = getgrgid(gid)) == NULL){
		sprintf(numstr,"%d",gid);
		return numstr;
	} else
		return grp_ptr->gr_name;
}

/******************************************************
long *ptr = NULL;
    unsigned int count = 0;
    int num_files = 0; //holds number of files inside dir

    DIR *dp = NULL;
    char *curr_dir = NULL;
    struct dirent *dptr = NULL;

    dp = opendir((const char*)curr_dir); //open dir
    while(NULL != (dptr = readdir(dp))) { //start reading dir contents
	//don't count files beginning with "."
	if(dptr->d_name[0] != '.')
		num_files++;
    }

    closedir(dp);

    dp = NULL;
    dptr = NULL;
   
    //check if there is at least one file or folder inside curr working dir
    if (!num_files) {
	return 0;
    } else {
	//allocate mem to hold addr of names of contents in curr working dir
	ptr = malloc(num_files*8);
	if (NULL == ptr) {
		printf("\n Memory allocation failed.\n");
		return -1;
	} else { 
		//initialize memory by zeros
		memset(ptr, 0, num_files*8);
	}
    }

    //open dir 
    dp = opendir((const char*)curr_dir);
    if(NULL == dp) {
    	printf("\nError: Could not open working dir\n");
	free(ptr);
	return -1;
    }

    unsigned int j = 0;
    for(count = 0; NULL != (dptr = readdir(dp)); count ++) {
	if(dptr->d_name[0] != '.') {
		ptr[j] = (long)dptr->d_name;
		j++;
	}
    }

    //sorts names alphabetically using bubble sort
    for(count = 0; count < num_files - 1; count ++) {
	for(j = count + 1; j < (num_files); j++) {
		char *c = (char*)ptr[count];
		char *d = (char*)ptr[j];

		//checks if two numbers are from same set
		if( ((*c >= 'a') && (*d >= 'a')) || ((*c <= 'Z') && (*d <= 'Z')) ) {
			int i = 0;
			//if initial characters are same, continue comparing until difference
			if (*c == *d) {
				while(*(c+i) == *(d+i)) {
					i++;
				}
			}
			//checks if earlier stored value is alphabetically higher than next val
			if(*(c+i) > *(d+i)) {
				//if yes, then swap values
				long temp = 0;
				temp = ptr[count];
				ptr[count] = ptr[j];
				ptr[j] = temp;
			}
		} else { //if two beginning characters differ, then make same and compare
			int off1 = 0, off2 = 0;
			if(*c <= 'Z') {
				off1 = 32;
			}
			if (*d <= 'Z') {
				off2 = 32;
			}

			int i = 0;

			//after character set made same, check if beginning char are same
			if(*c + off1 == *d + off2) { //search until find some difference
				while(*(c + off1 + i) == *(d + off2 + i)) {
					i++;
				}
			}
		
			//after difference is found, swap if necessary
			if ((*c + off1 + i) > (*d + off2 + i)) {
				long temp = 0;
				temp = ptr[count];
				ptr[count] = ptr[j];
				ptr[j] = temp;
			}
		}
	     }
	}

	//start displaying on console
	for(count = 0; count < num_files; count++) {
		if(!access((const char*)ptr[count], X_OK)) {
			//int fd = -1;
			struct stat st;
		
			fd = open((char*)ptr[count], O_RDONLY, 0);
			if(-1 == fd) {
				printf("\n Opening file/Dir failed\n");
				free(ptr);
				return -1;
			}

			close(fd);

		//} else {
			printf("%s	",(char*)ptr[count]);
		}
	}
	printf("\n");

	free(ptr); //free allocated memory
	return 0; 

************************************************************/
				
}
