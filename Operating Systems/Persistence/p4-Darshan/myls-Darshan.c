/*******************************
*    Darshan Vala              *
*    U39780959                 *
*                              *
********************************
*/

#include "util.h"

/* Write a program that lists files in the given directory. When called without any 
   arguments, the program should just print the file names. When invoked with the -l flag, the 
   program should print out information about each file, such as the owner, group, 
   permissions,  and  other  information  obtained  from  the  stat()  system  call.  The  program 
   should take one additional argument, which is the directory to read, e.g., myls -l directory. 
   If no directory is given, the program should just use the current working directory.   
*/



int main(int argc, char *argv[]){
	
	// if there is one argument then it opens the directory and calls displayRecords
	// to show the name of all the files in the current directory

	if(argc == 1){
		DIR * here = opendir("."); 
        displayRecord(here);return 0; 
	}

	// there is too many arguments if there is more then 3

    else if(argc >= 4){
		printf("\nToo many arguments, please use the valid format");return 0;
	}

	// this is to check if there really is just 2 arguments and if it has -l
	// if it has 2 then we store the path in a trace and use a . for the directory we are in
	// we open and read and get all the info from the current file and print it

	else if(argc >= 2 && strcmp(argv[1], "-l") == 0){
		if(argc == 2){ 
			
			char trace[SIZE];strcpy(trace, ".");

			DIR * here = opendir(trace);
			D * read = readdir(here); 

			while((read != NULL) && (read = readdir(here))){
        		printf("\nInformation in file: %s", read -> d_name);
				S stats;
				dataPrint(stats);
			}
		}

		// If we have 3 arguments then we set up a trace and use S_ISREG if its a regular file
		// or we use S_ISDIR if its a directory
		// there is some error checking if the format is right or if an error is coming

        else if(argc == 3){

			char trace[SIZE];
			strcpy(trace, argv[2]); S tracestats; 

			if(stat(trace, &tracestats) == -1){
				printf("error in file or directory, or the trace does not exist\n\n");return 0;
			}

			if(S_ISREG(tracestats.st_mode)){
				printf("\n**Normal** File information of: %s\n", trace); dataPrint(tracestats);
			}

			else if(S_ISDIR(tracestats.st_mode)){
				printf("\nDisplaying Directory ");
				DIR * here = opendir(trace);
				D * read = readdir(here);

				do{
        			printf("\nFile information of: ./%s\n", read -> d_name);
					S tracestats;
					stat(read -> d_name, &tracestats); dataPrint(tracestats);
				}while((read != NULL) && (read = readdir(here)));
			}
		}
	}
	else{
		printf("\nPlease check your format and try again");return 0;
	}
	return 0;
}