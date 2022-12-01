#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

/*
    SEMAPHORES IN C

    Copyright 2022, Vazquez Gutierrez Marcelo, All rights reserved.

    This programm creates an especified amount of childs, where each one must execute
    their code in certain order.

    Each childs gets asigned a number, and the execution of the code inside each child
    must be ascendent order, this means the first child must execute their code first,
    then the second child, then the third and so on.

    When the last child executes their code, the first one executes their code, making it a
    full cycle.

    In this implementation, each child just prints on screen a greeting and their assigned
    number, just so we can see that they are executed in the expected order.
*/

/* Prototypes for the functions */
void process(int semid,  int nsem);
void up (int semid, int nsem);
void down (int semid, int nsem);

int main(int argc, char const *argv[])
{

    key_t key;

    //The file pointed here MUST exist
    char path[] = "sem1.c";

    if((key = ftok(path, 'e')) == -1)
    {
        perror("ftok");
        exit(1);
    }

    int semid;

    //Create the given amount of semaphores
    if ((semid = semget(key, 5, 0666 | IPC_CREAT)) == -1) 
    {
        perror("semget");
        exit(1); 
    }

    int pid, i;

    // Create all the childs, one per semaphore
    for(i = 0; i < 5; ++i)
    {
        if((pid = fork()) == 0)
            process(semid, i);
    }

    return 0;
}

// This is the code every child executes.
void process(int semid, int nsem)
{
    int next;

    // If is the last child, target the first one
    if(nsem == 4)
        next = 0;
    else
        next = nsem + 1;
    
    while(1)
    {
        down(semid, nsem);
        printf("Hello from child %d!", nsem + 1);
        if(nsem == 4)
            printf("\n");
        up(semid, next);
    }
}

// This function unlocks the process
void up (int semid, int nsem)
{
    struct sembuf sb = {nsem, 1, 0};
    if (semop(semid, &sb, 1) == -1) 
    {
        perror("semctl at up");
        exit(1); 
    }
}

// This function locks the process
void down (int semid, int nsem)
{
    struct sembuf sb = {nsem, -1, 0};
    if (semop(semid, &sb, 1) == -1) 
    {
        perror("semctl at down");
        exit(1); 
    }
}

