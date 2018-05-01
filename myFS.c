#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include <sys/types.h> 
#include <dirent.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <pwd.h> 
#include <grp.h> 
#include <time.h>  
 
 
int main(void) { 

   char *curr_dir = NULL; 
   DIR *dp = NULL; 
   struct dirent *dptr = NULL; 
   unsigned int count = 0; 
   long *ptr = NULL; 
 
   curr_dir = getenv("PWD"); //gets variable PWD to get current working dir
   if(NULL == curr_dir) { 
       printf("\n ERROR : Could not get the working directory\n"); 
       return -1; 
   } 
 
   int numFiles = 0; //holds number of files inside dir
   
   dp = opendir((const char*)curr_dir); //opens the dir  
    
   while(NULL != (dptr = readdir(dp))) { //starts reading dir contents  
       if(dptr->d_name[0] != '.') //don't count "." files
       numFiles++; 
   } 

   closedir(dp); //closes dir after counting number of files inside current dir

   dp = NULL; //restores values to use again later
   dptr = NULL; 
 
   if(!numFiles) { //checks if at least one file inside current dir
       return 0; 
   } 
   else { 
       ptr = malloc(numFiles*8); //allocates memory to hold addr of content names
       if(NULL == ptr) { 
           printf("\n Memory allocation has failed.\n"); 
           return -1; 
       } 
       else { 
           memset(ptr,0,numFiles*8); //initializes memory by zeroes
       } 
   }  
  
   dp = opendir((const char*)curr_dir); //opens dir    
   if(NULL == dp) { 
       printf("\n Error: Couldn't open working directory.\n"); 
       free(ptr); 
       return -1; 
   } 
   
   unsigned int j = 0; 
   for(count = 0; NULL != (dptr = readdir(dp)); count++) { //read contents of dir  
       if(dptr->d_name[0] != '.') { 
          ptr[j] = (long)dptr->d_name; 
          j++;  
       } 
   } 
 
   for(count = 0; count< numFiles-1;count++) { //alphabetize here using bubble sort
       for(j=count+1; j< (numFiles);j++) {
           char *c = (char*)ptr[count]; 
           char *d = (char*)ptr[j]; 
            
           if(((*c >= 'a') && (*d >= 'a')) || ((*c <='Z') && (*d <='Z'))) //checks if 2 characters from same set
           { 
               int i = 0; 
 
               if(*c == *d) { //if first characters are same, continue until difference
                   while(*(c+i)==*(d+i)) { 
                       i++; 
                   } 
               } 

               if(*(c+i) > *(d+i)) { //checks if one value is alphabetically higher than other 
                   long temp = 0; //swap if one is indeed higher

                   temp = ptr[count]; 
                   ptr[count] = ptr[j]; 
                   ptr[j] = temp; 
               } 
 
           } else { //make two beginning characters same and compare
               int off1=0, off2=0; 
               if(*c <= 'Z') { 
                   off1 = 32; 
               } 
               if(*d <= 'Z') { 
                   off2 = 32; 
               } 
 
               int i = 0; 

               if(*c+ off1 == *d + off2) { //check if beginning characters are same, continue until difference
                   while(*(c+off1+i)==*(d+off2+i)) {
                       i++; 
                   } 
               } 

               if((*c + off1+i) > (*d + off2+i)) { //swap again for alphabetically higher
                   long temp = 0; 
                   temp = ptr[count]; 
                   ptr[count] = ptr[j]; 
                   ptr[j] = temp; 
               } 
           } 
       } 
    } 

   for(count = 0; count< numFiles; count++) { //display 
       int fd = -1; 
       struct stat st; 
 
       fd = open((char*)ptr[count], O_RDONLY, 0); 
       if(-1 == fd) { 
           printf("\n Opening file or directory failed.\n"); 
           free(ptr); 
           return -1; 
       } 
 
      if(fstat(fd, &st)) { //call fstat to obtain stat info about file
          printf("\n Fstat() failed\n"); //if fstat() fails
          close(fd); 
          free(ptr); 
          return -1; 
      } 
  
      if(S_ISDIR(st.st_mode)) { //checks if it is a directory
      	  printf("d"); 
      } else {     
          printf("-"); 
      } 

      mode_t permission = st.st_mode & S_IRWXU; //checks owner permission
      if(permission & S_IRUSR) { 
          printf("r"); 
      } else { 
          printf("-"); 
      } 
 
      if(permission & S_IWUSR) { 
          printf("w"); 
      } else { 
          printf("-"); 
      } 
 
      if(permission & S_IXUSR) { 
          printf("x"); 
      } else { 
          printf("-"); 
      } 
 

      permission = st.st_mode & S_IRWXG; //checks group permission
      if(permission & S_IRGRP) { 
          printf("r"); 
      } else { 
          printf("-"); 
      } 
 
      if(permission & S_IWGRP) { 
          printf("w"); 
      } else { 
          printf("-"); 
      } 
 
      if(permission & S_IXGRP) { 
          printf("x"); 
      } else { 
          printf("-"); 
      } 
 

      permission = st.st_mode & S_IRWXO; //checks others permission
      if(permission & S_IROTH) { 
          printf("r"); 
      } else { 
          printf("-"); 
      } 
 
      if(permission & S_IWOTH) { 
          printf("w"); 
      } else { 
          printf("-"); 
      } 
 
      if(permission & S_IXOTH) { 
          printf("x"); 
      } else { 
          printf("-"); 
      } 
  
      printf("%d\t", (int)st.st_nlink); //prints number of links
 
      struct passwd *pt = getpwuid(st.st_uid); //gets user name
      printf("%s\t",pt->pw_name); 
 
      struct group *p = getgrgid(st.st_gid); //gets group name
      printf("%s\t",p->gr_name); 

      printf("%lld\t",(long long) st.st_size); //gets file size

      char date_time[100]; //gets date and time here
      memset(date_time,0,sizeof(date_time)); 
      strncpy(date_time, ctime(&st.st_ctime), sizeof(date_time)); 

      int c = 0; 
      while(date_time[c] != '\0') { 
          if(date_time[c] == '\n') 
              date_time[c] = '\0'; 
          c++; 
      } 

      printf("%s   ", date_time); 
 
      printf("%s\n",(char*)ptr[count]); 
       
      close(fd); 
   } 

   free(ptr); //frees allocated memory
   return 0; 
}
