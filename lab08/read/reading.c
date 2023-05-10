#include <stdio.h>

void hex_to_char(int hex, char arr[],int idx);

int main() {
    FILE* fp = fopen("a.out", "rb");
    if (fp == NULL) {
        printf("Failed to open file");
        return 1;
    }

    unsigned char buffer[16];
    char str[20];

    size_t bytesRead;
    int t,k=0,temp = 0;

    while ((bytesRead = fread(buffer, sizeof(unsigned char), 16, fp)) > 0) {
        t = 0;
        printf("%08x: ", k);
        for (size_t i = 0; i < bytesRead; i++) {
            hex_to_char(buffer[i],str,i);
            printf("%02x", buffer[i]);
            if(t % 2 !=0)
                printf(" ");
            t++;
            temp = i;
        }

        str[temp+1] = '\0';
        
        t = 0;
        for (int i = 0; i < 16- bytesRead; i++)
        {
            printf("  ");
            if(t % 2 !=0)
                printf(" ");
            t++;
        }

        printf(" %s",str);
        printf("\n");

        k += 0x10;
    }

    fclose(fp);

    return 0;
}


void hex_to_char(int hex, char arr[], int idx)
{
    if(0x20 <= hex && hex <= 0x7e)
        arr[idx] = (char) hex ;
    else
        arr[idx] = '.';
}