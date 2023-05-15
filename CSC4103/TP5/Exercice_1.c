#include <stdio.h>

void read_bank_ascii(FILE *file)
{
    char accounts[256];
    // fscanf returns nb of read characters or EOF flag
    // it reads line by line
    do
    {
        // Va lire ligne par ligne jusque EOF
        ret = fscanf(f, "%d", &val);
        if (ret == EOF || ret == 0)
            break;
        printf("read: %x / %d\n", val, val);
    } while (1);
}
void read_bank_binary(FILE *file)
{
    int val;
    // Va lire ligne par ligne jusque EOF
    while (fread(&val, sizeof(int), 1, f))
    {
        printf("read: %x / %d\n", val, val);
    }
}

int main()
{
    printf("Start CI5 : Files\n");

    FILE *file = fopen("fichier_ascii.dat", "r");
    if (file == NULL)
    {
        printf("No such file to open \n");
        return 0;
    }
    // read_bank_ascii(file);
    read_bank_binary(file);

    // Close file
    fclose(file);
    return 0;
}
