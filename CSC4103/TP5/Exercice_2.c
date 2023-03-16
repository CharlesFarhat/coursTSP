//
// Created by charl on 3/13/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void encode(uint8_t key, FILE* in, FILE* out) {
    uint8_t byte[256];
    int read = fread(&byte, 1, 256, in);
    do {
        for (int i = 0; i < read; ++i) {
            byte[i] = byte[i] ^ key;
        }
        fwrite(&byte, 1, read, out);
        read = fread(&byte, 1, 256, in);
    } while (read != 0);
}

int main(int argv, char** argc) {

    if (argv < 4) {
        fprintf(stderr, "ERROR : Please provide key, entry path, output path \n");
        return EXIT_FAILURE;
    }

    char* Key_raw = argc[1];
    char* entry_path = argc[2];
    char* output_path = argc[3];

    // Conversion de l'entier
    uint8_t key;
    key = atoi(Key_raw);
    fprintf(stderr, "encode %s with the key %d to %s \n", entry_path, key, output_path);

    FILE* in = fopen(entry_path, "r");
    if (in == NULL) {
        fprintf(stderr, "fopen : %s", strerror(errno));
        return EXIT_FAILURE;
    }

    FILE* out = fopen(output_path, "w");
    if (out == NULL) {
        fprintf(stderr, "fopen out: %s", strerror(errno));
        return EXIT_FAILURE;
    }

    encode(key, in, out);

    // Close streams
    fclose(in);
    fclose(out);

    return 0;
}