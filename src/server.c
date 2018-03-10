#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

#define BUFFER_SIZE 1024

static key_t key = 8082;
static int shm_id;
static char *shm_addr;

void sig_handler(int signum)
{
    printf("Handling SIGNAL: %d !\n", signum);
    printf("Read data from shared memory ...\n");
    printf("data = [%s]\n", shm_addr);
}

void SIGINT_handler(signum)
{
    /* Clean up. */
    shmdt(shm_addr);
    close(shm_id);
    close(key);
    exit(0);
}

int main(void)
{
    char *shm_addr;
    pid_t pid = getpid();
    printf("Start share memory server at pid %lu\n", (unsigned long)pid);

    signal(SIGUSR1, sig_handler);
    signal(SIGINT, SIGINT_handler);

    shm_id = shmget(key, BUFFER_SIZE, IPC_CREAT | IPC_EXCL | 0666);
    if (shm_id < 0)
    {
        perror("shmget");
        exit(1);
    }

    /* attach */
    shm_addr = shmat(shm_id, (void *)0, SHM_RDONLY);
    if (shm_addr < 0)
    {
        perror("shmat");
        exit(1);
    }

    while (1)
    {
        // Busy waiting.
        sleep(1);
    }
    return 0;
}