
/*******************************
*    Darshan Vala              *
*    U39780959                 *
*                              *
********************************
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>



#define LIMIT 4096
#define SIZE 256
#define TRACE 100

// This header contains all the additional functions so that each program is less clustered



// useful variables and struct
typedef struct dirent D; 
typedef struct stat S; // info
int s;
off_t point;
off_t last;


// the function below goes through the directory and prints the name of all the files in the current one
// Used in myls-Darshan

void displayRecord(DIR * d){
    D * read = readdir(d);
	while((read != NULL) && (read = readdir(d))){
		printf("%s\n", read -> d_name);
	}
}

// the function below uses flags to get permisson for authorization to directory 
// It uses stats to do this
// a lot of warnings come from this... might have to change
// used in mystats-Darshan 

void Authorize(S s){
	printf( (S_ISDIR(s.st_mode)) ? "d" : "-"); 
    printf( (s.st_mode & S_IRUSR) ? "r" : "-");	
    printf( (s.st_mode & S_IWUSR) ? "w" : "-");
    printf( (s.st_mode & S_IXUSR) ? "x" : "-");
    printf( (s.st_mode & S_IRGRP) ? "r" : "-");	
    printf( (s.st_mode & S_IWGRP) ? "w" : "-");
    printf( (s.st_mode & S_IXGRP) ? "x" : "-");
    printf( (s.st_mode & S_IROTH) ? "r" : "-");	
    printf( (s.st_mode & S_IWOTH) ? "w" : "-");
    printf( (s.st_mode & S_IXOTH) ? "\nx" : "\n-");
}



// This is the information from stat, printing it out for user and right format
// it calls Authorize for the permissions
// Used in myls-Darshan

void dataPrint(S info){
    printf("\nInode number: %lu", info.st_ino);
    printf("\nLast Access: %s", ctime( &(info.st_atime)));
    printf("\nLast Modification: %s", ctime( &(info.st_mtime)));
    printf("\nLinks: %d", info.st_nlink);
    printf("\nDevice ID has file: %lu", info.st_dev);
    printf("\nPermissions: "); 
    Authorize(info);
    printf("\nFile Owner: %lu", info.st_uid);
    printf("\nBlocksize: %lu", info.st_nlink);
    printf("\nLast Status Change: %s", ctime( &(info.st_ctime)));
    printf("\nFile belongs to group: %lu", info.st_gid);
    printf("\nSize: %lu Bytes", info.st_size);
    printf("\nDevice ID: %lu ", info.st_rdev);
    printf("\nBlocks Allocated: %d ", info.st_blocks);

}

// This function reads and prints into a trace with the proper format
// it has error checking for if the root of the directory is null or has an error
// it then displays the info to the user

void fileDisplay(const char * list){
    DIR * noderoot = opendir(list);
    if((noderoot == -1) || (noderoot == NULL)) exit(0); // cant be return 0 because its void

    printf("\nOpening: %s", list);
    struct dirent * read; read = readdir(noderoot);

    do{
		printf("\n%s", read -> d_name);
        if((strcmp(read -> d_name, ".") != 0) && (strcmp(read -> d_name, "..") != 0) && (read -> d_type == DT_DIR)){
            char trace[TRACE] = {0};
            strcat(trace, list); strcat(trace, "/"); strcat(trace, read -> d_name);
            fileDisplay(trace);
        }
        read = readdir(noderoot);
    }while((noderoot != -1) || (noderoot != NULL)); closedir(noderoot);
}