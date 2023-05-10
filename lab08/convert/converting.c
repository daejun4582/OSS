#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s hex_file\n", argv[0]);
        return 1;
    }
    
    char *hex_file = argv[1];
    FILE *fp = fopen(hex_file, "r");
    if (fp == NULL) {
        printf("Failed to open file: %s\n", hex_file);
        return 1;
    }
    
    char *out_file = "output";
    FILE *out_fp = fopen(out_file, "wb");
    if (out_fp == NULL) {
        printf("Failed to create output file: %s\n", out_file);
        return 1;
    }
    
    char line[1000];

    while (fgets(line, sizeof(line), fp)) {
        char *tok = strtok(line, " :");
        unsigned char buf[100];
        size_t i = 0;
        while ((tok = strtok(NULL, " ")) != NULL) {
            buf[i++] = strtol(tok, NULL, 16);
        }
        
        fwrite(buf, sizeof(unsigned char), i, out_fp);
    }

    
    
    fclose(fp);
    fclose(out_fp);
    
    return 0;
}