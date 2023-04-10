#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Attention fournir l'argument !");
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]); // use to convert char to int

    printf("n=%d", n);

    sem_t* semaphore = sem_open("/CLE", O_CREAT, S_IRWXU, 4);

    // on supprime le sémaphore si il existe déjà
    sem_unlink("/CLE");

    // Attention ici on créer 2^n processus !
    for(int i=0; i<n; i++)
        fork();

    // P
    sem_wait(semaphore);
    printf("[%d] start a job \n", getpid());
    sleep(1);
    printf("[%d] stop a job \n", getpid());
    // V
    sem_post(semaphore);

    wait(NULL);

    return EXIT_SUCCESS;
}