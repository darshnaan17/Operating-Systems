#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
//#include "readerwriter.h"
#define Thread pthread_t
#define Semaphore sem_t
#define THREAD_LEN 8
#define MAX_RAN_NUM 10000

Semaphore stopR; // stops reads
Semaphore stopW; //stops writes
Semaphore lock_num; //lock counter
int countR = 0; // read counter

/*
This function utilizes a random num generator that sets x equal to y*z to find a value to return to threadR and threadW functions
after they stop/lock
*/
void read_and_write(){
    int x = 0, y = 0, z = 0;
    int ran = rand() % MAX_RAN_NUM;
    do{
        do{
            x = y * z;
            z++;
        }while(z < ran);
        y++;
    }while(y < ran);
    
    return;
}
/*
This thread is for reading and is a pointer because it uses the value. If there is a read then we stop write from getting in until
we progress in the thread. If there isnt a read coming up then we say we are taking the last read and we lock and say the read is done until 
we progress further.
*/
void* threadR() {
    sem_wait(&stopR);
    sem_wait(&lock_num);countR++;
    printf("\n'Read' is in\nOther 'Read' threads can get in :)\n");

    if(countR == 1){
        sem_wait(&stopW);
    }

    sem_post(&lock_num);
    sem_post(&stopR);
    read_and_write();
    sem_wait(&lock_num);
    countR--;

    if (countR == 0) {
        sem_post(&stopW);
        printf("\nLast 'Read' is in\nPrepare for 'Write if any\n");
    }
    sem_post(&lock_num);
    printf("\n'Read' is all done\n"); 

    return NULL;
}

/*
This thread is for writing and is a pointer because it uses the value. If there is a write then we stop read from getting in until
we progress in the thread. If there isnt a write coming up then we say we are taking the last write and we lock and say the write is done until 
we progress further.
*/
void* threadW() {
   
    sem_wait(&stopR);
    sem_wait(&stopW);
    printf("\n'Write is in\nOthers can not get in :D\n");

    read_and_write();

    sem_post(&stopW);
    sem_post(&stopR);
    printf("\n'Write' is all done\n");

    return NULL;
} 