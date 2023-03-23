/*******************************
*    Darshan Vala              *
*    U39780959                 *
*                              *
********************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h> // needed for parsing the input
#include <sys/types.h>

int main(){
    int pipe1[2];
    char * p1[100];
    char * p2[100];
    char * input_cmd = (char*) malloc (100 * sizeof(char));

    // recieve the commands from input 
    printf("Enter two commands seperated by '|' ");
    scanf("%[^\n]%*c", input_cmd); // recieves all the input until you press enter

    char * parse = strtok(input_cmd, " "); // for spaces
    int count1 = 1; // counting for first command
    do{
        if(parse[0] != '-'){
            p1[0] = parse;
        }
        else{
            p1[count1] = parse; count1++;
        }
            parse = strtok(NULL, " ");
    }while(parse[0] != '|'); // this takes in the first command since it stops at the '|'


    //same thing for p2  except while condition is changed till end of input so NULL
    parse = strtok(NULL, " "); // need this for the next while loop
    int count2 = 1; // counting for second command
    do{
        if(parse[0] != '-'){
            p2[0] = parse;
        }
        else{
            p2[count2] = parse; count2++;
        }
            parse = strtok(NULL, " ");
    }while(parse != NULL);


    // now that we have both commands parsed we create a pipe
    if(/*pipe(pipe1) < 0 || */pipe(pipe1) == -1){ //might be repeatative
        printf("Pipe has failed"); exit(0);
    }

    // pipe is successful and now we fork and make the processes
    int process = fork(); 
    // starting with child process

    if(process == 0){
        close(pipe1[0]);
        p1[count1] = NULL;
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[1]);
        execvp(p1[0], p1);
    }

    process = fork(); // new process 

    // creates new child, do the same thing for other param

    if(process == 0){
        close(pipe1[1]);
        dup2(pipe1[0], STDIN_FILENO);
        p2[count2] = NULL;
        close(pipe1[0]);
        execvp(p2[0], p2);
    }

    // since we're in the parent process we must wait for child to finish
    //process = wait(NULL);
    while((process = wait(NULL)) > 0); // parent is waiting

    printf("++++\n");
    return 0;
}