#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 16

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <hex_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    FILE *fp;
    if ((fp = fopen(argv[1], "rb")) == NULL) {
        printf("Error opening file: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    
    unsigned char buffer[MAX_LINE_LENGTH];
    size_t bytes_read;
    size_t offset = 0;
    int t;

    while ((bytes_read = fread(buffer, 1, MAX_LINE_LENGTH, fp)) > 0) {
        printf("%08zx: ", offset);
        t = 0;    
        for (size_t i = 0; i < bytes_read; i++) {
            printf("%02x", buffer[i]);
            if(t % 2 !=0)
                printf(" ");
            t++;
        }
        for (size_t i = bytes_read; i < MAX_LINE_LENGTH; i++) {
            printf("   ");
        }
        printf(" ");
        for (size_t i = 0; i < bytes_read; i++) {
            printf("%c", (buffer[i] >= 32 && buffer[i] <= 126) ? buffer[i] : '.');
        }
        printf("\n");
        offset += bytes_read;
    }
    
    fclose(fp);
    return 0;
}