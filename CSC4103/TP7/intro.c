#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid1, pid2, pid3, pid4;

    pid1 = getpid();
    printf("Je suis le processus P1 avec PID=%d et PPID=%d\n", pid1, getppid());

    if ((pid2 = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    // Si le pid est égal à 0 on est dans le processsus fils
    else if (pid2 == 0) {
        pid2 = getpid();
        printf("Je suis le processus P2 avec PID=%d et PPID=%d\n", pid2, getppid());
        sleep(1);
        if ((pid3 = fork()) == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid3 == 0) {
            pid3 = getpid();
            printf("Je suis le processus P3 avec PID=%d et PPID=%d\n", pid3, getppid());
            sleep(1);
            if ((pid4 = fork()) == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (pid4 == 0) {
                pid4 = getpid();
                printf("Je suis le processus P4 avec PID=%d et PPID=%d\n", pid4, getppid());
                execlp("ps", "ps", "f", NULL);
                exit(EXIT_SUCCESS);
            } else {
                wait(NULL);
                printf("Je suis le processus P3 avec PID=%d et PPID=%d, j'ai créé P4\n", pid3, getppid());
                printf("Je suis le processus P3 avec PID=%d et PPID=%d, je me termine\n", pid3, getppid());
                exit(EXIT_SUCCESS);
            }
        } else {
            wait(NULL);
            printf("Je suis le processus P2 avec PID=%d et PPID=%d, j'ai créé P3\n", pid2, getppid());
            printf("Je suis le processus P2 avec PID=%d et PPID=%d, je me termine\n", pid2, getppid());
            exit(EXIT_SUCCESS);
        }
    }
    // Si le PID est supérieur à 0 ça veut dire qu'on est dans le processus père
    else if (pid2 > 0) {
        wait(NULL);
        printf("Je suis le processus P1 avec PID=%d et PPID=%d, j'ai créé P2\n", pid1, getppid());
        printf("Je suis le processus P1 avec PID=%d et PPID=%d, je me termine\n", pid1, getppid());
    }

    return EXIT_SUCCESS;
}
