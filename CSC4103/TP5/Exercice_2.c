//
// Created by charl on 3/13/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void encode(uint8_t key, FILE *in, FILE *out)
{
    uint8_t byte[256];
    // lit n < 256 bytes
    while ((n = fread(buf, 1, 256, in)))
    {
        for (i = 0; i < n; i++)
            buf[i] = buf[i] ^ key;

        if (!fwrite(buf, 1, n, out))
        {
            fprintf(stderr, "fwrite: %s\n", strerror(errno));
            return;
        }
    }
    /* we cannot directly know if n = 0 because of an error */
    if (feof(in))
        return;
    else
    {
        fprintf(stderr, "fread: %s\n", strerror(errno));
        return;
    }
}

int main(int argv, char **argc)
{

    if (argv < 4)
    {
        // ecrit sur la sortie de l'erreur standard
        fprintf(stderr, "ERROR : Please provide key, entry path, output path \n");
        return EXIT_FAILURE;
    }

    char *Key_raw = argc[1];
    char *entry_path = argc[2];
    char *output_path = argc[3];

    // Conversion de l'entier
    uint8_t key;
    key = atoi(Key_raw);
    fprintf(stderr, "encode %s with the key %d to %s \n", entry_path, key, output_path);

    FILE *in = fopen(entry_path, "r");
    if (in == NULL)
    {
        fprintf(stderr, "fopen : %s", strerror(errno));
        return EXIT_FAILURE;
    }

    FILE *out = fopen(output_path, "w");
    if (out == NULL)
    {
        fprintf(stderr, "fopen out: %s", strerror(errno));
        return EXIT_FAILURE;
    }

    encode(key, in, out);

    // Close streams
    fclose(in);
    fclose(out);

    return 0;
}