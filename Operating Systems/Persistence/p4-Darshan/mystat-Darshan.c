/*******************************
*    Darshan Vala              *
*    U39780959                 *
*                              *
********************************
*/

#include "util.h"

//Write your own version of the command line program  stat, which simply calls the 
//stat() system call on a given file or directory. Print out file size, number of blocks allocated, 
//reference (link) count, file permissions, and file inode.   



int main(int argc, char *argv[]){
	
	//makes sure the right amount of arguments are there

	if((argc == 1) || (argc >= 3)){
		printf("\nToo many or too little arguements entered, try again");
		return 0;
	}
	
	// if we have the right amount of arguments then we 
	// get info and store it in stats
	// there is error checking if an error comes up
	// if there are no errors then if its a regular file it prints its info
	// if there are no errors then if its a directory it prints its info

	else{
		S stats; 
    	if(stat(argv[1], &stats) == -1){
			printf("\nSomething went wrong"); return 0;
		}
		
		if(S_ISDIR(stats.st_mode)){
			char either[10];
			char directory[] = "directory";
			char file[] = "file";
			strcpy(either, directory);
			printf("\n%s Information of %s", either, argv[1]); //target file/directory
			printf("\n%s Size: %d Bytes", either, stats.st_size); //size in bytes
			printf("\nBlocks Allocated: %d", stats.st_blocks); //number of blocks
			printf("\nLinks: %d", stats.st_nlink); //number of links
			printf("\nPermissions: "); 
			Authorize(stats); //getting permissions after calling the declared function
			printf("\n%s Inode: %li", either, stats.st_ino);	//inode number
			return 0;
			} //if directory print the word "Directory" when displaying the information
		
		else if(S_ISREG(stats.st_mode)){
			char either[10];
			char directory[] = "directory";
			char file[] = "file";
			strcpy(either, file);
			printf("\n%s Information of %s\n", either, argv[1]); //target file/directory
			printf("%s Size: %d Bytes\n", either, stats.st_size); //size in bytes
			printf("Number of Blocks Allocated: %d\n", stats.st_blocks); //number of blocks
			printf("Number of Links: %d\n", stats.st_nlink); //number of links
			printf("File permissions: "); 
			Authorize(stats); //getting permissions after calling the declared function
			printf("%s Inode: %li\n\n", either, stats.st_ino);	//inode number
			return 0;
		} //if file print the word "File" when displaying the information	//displaying the proper information
    }	
}