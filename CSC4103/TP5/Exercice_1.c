#include <stdio.h>





void read_bank_ascii(FILE* file) {
    char accounts[256];
    // fscanf returns nb of read characters or EOF flag
    while(fscanf(file, "%s", accounts) != EOF) {
        printf("%s\n", accounts);
    }
}

void read_bank_binary(FILE* file) {
    int balance = 0;
    while(fscanf(file, "%d", &balance) != EOF) {
        printf("%d\n", balance);
    }
}


int main() {
    printf("Start CI5 : Files\n");

    FILE* file = fopen("fichier_ascii.dat", "r");
    if (file == NULL) {
        printf("No such file to open \n");
        return 0;
    }
    //read_bank_ascii(file);
    read_bank_binary(file);


    // Close file
    fclose(file);
    return 0;
}
