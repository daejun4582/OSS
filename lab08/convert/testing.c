#include <stdio.h>
#include <stdlib.h>

int main() {
    char hex_str[] = "cf";
    int val = (int) strtol(hex_str, NULL, 16);
    printf("%d\n", val);
    return 0;
}
