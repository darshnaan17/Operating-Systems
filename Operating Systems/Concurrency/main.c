#include "main.h"

/*

Darshan Vala


- My solution goes by looking at each scenerio until we hit the new line marker
- Once we finish with one thread we move onto the next
- until the end of the file is reached
*/
int main(){
    
    int i = 0; // used for the index
    char sign[THREAD_LEN], type;
    Thread threads[THREAD_LEN];

    sem_init(&lock_num, 0, 1);
    sem_init(&stopW, 0, 1); 
    sem_init(&stopR, 0, 1);

    FILE* input;input = fopen("scenarios.txt", "r"); // open the file
    if (input == NULL) {
      printf("\nError opening the file\n");
      exit(0);
    }


    do{ // reads in the input and sees if its w or r, if its neither we use that value as 0
        type = (char)fgetc(input);
        int d;
        if(type != '\n'){
            if(type == 'w' || type == 'r') 
            sign[i] = type;
            else sign[i] = '0'; 
            i++;
        }
        
        else{ 
            i = 0, d = 0;

            while(d < THREAD_LEN){ // this process increments through the thread and checks the sign
            printf("\nStarting Thread Sequence Below:\n");
                if(sign[d] != '0') printf("\n%c\n ", sign[d]);
                d++;
            }

            d = 0;
            while(d < THREAD_LEN){ // this process checks if the sign at that index is a w or r and creates the thread and then increments
                if(sign[d] != '0'){
                    if(sign[d] == 'w') pthread_create(&threads[d], NULL, threadW, NULL);
                    if(sign[d] == 'r') pthread_create(&threads[d], NULL, threadR, NULL);
                }
                d++;
            }

            d = 0;
            while(d < THREAD_LEN){ // this process joins the threads
                if(sign[d] != '0') pthread_join(threads[d], NULL);
                d++;
            }
        printf("\nThis thread is finished!\n");}
    } while(type != EOF); // end of file reaches, no more scenerios

    fclose(input);
    printf("\nEnd of the file reached\nExiting now\n");
    return 0;
}