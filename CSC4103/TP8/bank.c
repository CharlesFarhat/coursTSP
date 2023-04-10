#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if(argc != 3) {
        fprintf(stderr, "usage: %s name [balance]\n", argv[0]);
        return EXIT_FAILURE;
    }
    char*  name = argv[1];
    float  balance = 0;

    sem_t* sem = sem_open(name, O_CREAT, S_IRWXU, 1);

    sem_wait(sem);

    FILE* f = fopen(name, "r");

    if(f) {
        fread(&balance, sizeof(float), 1, f);
        fclose(f);
    }

    balance += atof(argv[2]);

    printf("%s has %f euros\n", name, balance);

    f = fopen(name, "w");
    fwrite(&balance, sizeof(float), 1, f);

    fclose(f);

    sem_post(sem);

    return EXIT_SUCCESS;
}