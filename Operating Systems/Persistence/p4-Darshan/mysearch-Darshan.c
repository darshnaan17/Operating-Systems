/*******************************
*    Darshan Vala              *
*    U39780959                 *
*                              *
********************************
*/

#include "util.h"

//Write a program that prints out the names of each file and directory in 
//the file system tree, starting at a given point in the tree.  For example, when run without 
//arguments,  the  program  should  start  with  the  current  working  directory  and  print  its 
//contents, as well as the contents of any sub-directories, etc., until the entire tree, root at the 
//CWD, is printed. If given a single argument (of a directory name), use that as the root of 
//the tree instead.   


int main(int argc, char* argv[]){
	
    // error checking if the amount of arguments is acceptable

    if((argc != 1) || (argc != 2)){
	    printf("Incorrect format, please try again"); return 0;	
	}
	
    // if there are 2 arguments then we declare trace for the path
    // and then we concatinize the proper form and call fileDisplay

    else if(argc == 2){
		char trace[TRACE] = {0};
        strcat(trace, ".");  strcat(trace, "/"); strcat(trace, argv[1]);
		fileDisplay(trace);
	}
    
    // if there is 1 argument then we concatinize the proper form and call fileDisplay

    else if(argc == 1) fileDisplay(".");
    return 0;
}