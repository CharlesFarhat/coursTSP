#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int var = 42;

int main() {
    printf("Lancement du TP8 : Appels système\n");


    // Gestion du fichier pour l'ouverture
    FILE* sync;

    pid_t pid;
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        /* processus fils */
        printf("[PID : %d] Hhh hhh non, non \n", pid);
        printf("Instruction B1 \n");
        var = 666;
        printf(" [PID : %d] Variable globale var : %d, address de var : %p \n", pid, var, &var);
        sync = fopen("sync", "w");
        fclose(sync);
    }
    else {
        /* Processus père */
        printf("[PID : %d] Je suis ton père \n", pid);
        do {
            sync = fopen("sync", "r");
        } while (sync == NULL);
        printf("Instruction B2 \n");
        printf(" [PID : %d] Variable globale var : %d, address de var : %p \n", pid, var, &var);
        unlink("sync");
    }

    return 0;
}

/*
 * On peut observer que var a la même adresse dans le père en le fils. Pourtant, cette variable a deux valeur différentes, alors que les deux affichages ont bien lieu après la modification de var :
        - dans le père, on s'assure que l'affichage a lieu après l'affectation à 666 puisque le père attend la création du fichier, laquelle n'a lieu qu'après l'affectation dans le fils;
        - dans le fils, on s'assure que l'affichage a lieu après la modification simplement parce que la séquence de code du fils commence par modifier var avant d'afficher la valeur.
Cet exercice vous montre donc que la même adresse virtuelle dans le père et dans le fils est bien associé à une adresse physique différente.
 */
