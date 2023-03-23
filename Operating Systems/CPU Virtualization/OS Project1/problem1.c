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

int main(int argc, char *argv[]){

    char **p; // pointer to go through argv[] arguments

    //printf("%d\n", argc); // seeing how many command args there are for testing purposes

    if(argc <= 1){ //this is checking if there is only 1 or 0 arguments
        printf("There are no arguements in the command line besides ./a.out\n"); // only ./a.out is here
    }
    else{
        argv[argc] = NULL; // we do not want the 0th element since that is not the command we want
        p = argv + 1; //from element 1 we get the element
    }

    int cid = fork(); // create new process since we have command line arguements
    if(cid < 0){ // if fork failed, exit
        printf("In the Child Process");
        fprintf(stderr,"Fork failed");
        exit(1);
    }
    else if(cid == 0){ //child
        printf("I am in the child process and success");
        execvp(p[0], p);
    }
    else{ //parent
        // wait for child to finish
        sleep(1);
        waitpid(cid,0,0);
        printf("This is the Parent Process\n");
        printf("++++");
    }

    return 0;

    
}
    
    
    /*printf("Process ID (pid:%d)\n", (int)getpid());
    int cid = fork();
    if(cid < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(cid == 0){
        printf("Child Process ID (pid:%d)\n", (int) getpid());
    }
    else{
        sleep(1);
        waitpid(cid,0,0);
        printf("This the parent process of %d (pid:%d)\n", cid, (int) getpid());
    }
    return 0;
}
*/ 
