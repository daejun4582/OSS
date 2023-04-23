#include <stdio.h>
#include <string.h>


void eliminate(char *str, char ch, char * result);


int main(void){

    char a[] = "h\"el\"lo";


    char result[100];

    eliminate(a,'"',result);

    printf("%s",result);


    return 0;
}

void eliminate(char *str, char ch, char * result){

    char save[100];

    int idx = 0;

    for(; *str != '\0'; str++)
    {
        if(*str != ch)
        {
            result[idx] = *str;
            idx++;
        }
    }

    result[idx] = '\0';

}