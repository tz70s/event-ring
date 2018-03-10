#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Not enough arguments!\n");
        exit(1);
    }

    key_t key = 8082;
    int shm_id;
    char *shm_addr;

    shm_id = shmget(key, BUFFER_SIZE, 0666);
    if (shm_id < 0)
    {
        perror("shmget");
        exit(1);
    }

    /* attach */
    shm_addr = shmat(shm_id, (void *)0, 0);
    if (shm_addr < 0)
    {
        perror("shmat");
        exit(1);
    }

    printf("Copy data into shm_addr. Data: %s\n", argv[2]);
    sprintf(shm_addr, "%s", "Hello World!\n");

    kill(atoi(argv[1]), SIGUSR1);

    shmdt(shm_addr);
    close(shm_id);
    close(key);
    return 0;
}