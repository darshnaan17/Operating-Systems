/*******************************
*    Darshan Vala              *
*    U39780959                 *
*                              *
********************************
*/

#include "util.h"

//Write a program that prints out the last few lines of a file. The program should be 
//efficient, in that it seeks to near the end of the file, reads in a block of data, and then goes 
//back until it finds the requested number of lines; at this point, it should print out those lines 
//from the beginning to the end of the file. To invoke the program, one should type: mytail 
//-n file, where n is the number of lines at the end of the file to print.  

int main(int argc, char *argv[]){

    // declaring variables for later use

    FILE * file;
    int need, stream, count, x = 0;
    char * trace;
    char figure, quantity[LIMIT];
    struct stat S;
    long done = 0;

    // error checking by seeing if we have the appropriate amount of arguments

    if((argc <= 2) || (argc >= 4)){
        printf("Too many or too little arguments, try again"); return 0;
    }
    
    // removing the -

    memmove(argv[1], argv[1] + 1, strlen(argv[1]));
    
    trace = argv[2];
    need = atoi(argv[1]);

    // checking if we have an error

    if((stat(trace, &S) == NULL) || (stat(trace, &S) == -1)){
        printf("\nThis does not exist, please try again with other format"); return 0;
    }

    // open the file to read and use fseek to get the end of the file position
    // the rest of the code reads in the lines requested and prints out 
    // lines of the file from the beginning to the end

    file = fopen(trace, "r"); fseek(file, 0, SEEK_END);
    off_t hold_point = point;
    point = ftell(file);
    last = point;

    for(hold_point; hold_point >= 0; hold_point--){
        figure = fgetc(file);
        if(((last - hold_point) > 1) && (figure == '\n')){
            done++;
            if(done == need) break;
        }
        point--; fseek(file, point, SEEK_SET);
    }
    
    stream = fileno(file); lseek(stream, (point + 1), SEEK_SET);

    do{
        while(x < count){
            printf("\n%c", quantity[x]);
            x++;

            if( x >= count) break; // just in case
        }
        printf("\n");
    }while((count = read(stream, quantity, 4096)) > 0);

    fclose(file); return 0;
}
