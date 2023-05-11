#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000

void txt_to_bin(char *argv[]);
void bin_to_txt(char *argv[]);

void hex_to_char(int hex, char arr[],int idx);

int main(int argc, char *argv[]) {

    int state;

    if( strcmp("-r",argv[1]) == 0 )
    {
        if (argc != 4) 
            exit(1);
        else
            state = 0;
    }
    else
    {
        if (argc != 3) 
            exit(1);
        else
            state = 1;
    }

    if(state == 0)
        txt_to_bin(argv);
    else
        bin_to_txt(argv);

   
    return 0;
}


void txt_to_bin(char *argv[])
{
    FILE *fp,*output_file;
    char line[MAX_LINE_LENGTH];
    char num_of_order[10],hex_num[3],result[200],tempin [50],hex_str[2],* t;
    int result_hex[20],cnt,cnt_p,l_num=0,val,lc,l_length;

    fp = fopen(argv[2], "r");
    if (fp == NULL) {
        exit(1);
    }

    output_file = fopen(argv[3], "w");

    
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {

        l_length = strlen(line);

        if(l_length < 52)
            exit(1);


        for(int i = 0; i < 8; i++)
        {
            num_of_order[i] = line[i];
        }

        num_of_order[8] = '\0';

        val = (int) strtol(num_of_order, NULL, 16);

        if(val != l_num)
            exit(1);

        if(line[8] != ':')
            exit(1);
        
        for(int i = 9; i < 50; i+=5)
        {
            if(line[i] != ' ')
                exit(1);
        }

        lc = 0;
        for(int i = 10; i < 50; i++)
        {
            if(lc == 4)
            {
                lc = 0;
                continue;
            }
            if('0' > line[i] && '9' < line[i] && 'a' > line[i] && 'f' < line[i])
                exit(1);

            lc++;
        }

        if(line[50] != ' ')
            exit(1);
        
        cnt = 0;
    
        t = line;
        t += 10;

        for(int i = 0; i < 40; i++)
        {
            tempin[i] = *(t+i);
            cnt_p = i;
        }
        tempin[cnt_p] = '\0';

        t = tempin;
        t = strtok(t, " ");

        memset(result,0,sizeof(char)*200);

        if(t != NULL)
        {
            while (t != NULL)              
            {

                strcat(result,t);

                if(cnt == 7)
                    break;
                        
                t = strtok(NULL, " ");   

                cnt++;  
            }
        }

        result[(cnt+1)*4] = '\0';

        
        cnt = 0;

        for(int i = 0; i < strlen(result); i += 2)
        {
            if('0' > result[i] && '9' < result[i] && 'a' > result[i] && 'f' < result[i])
                exit(1);
            if('0' > result[i+1] && '9' < result[i+1] && 'a' > result[i+1] && 'f' < result[i+1])
                exit(1);

            hex_str[0] = result[i];
            hex_str[1] = result[i+1];

            val = (int) strtol(hex_str, NULL, 16);

            result_hex[cnt] = val;

            cnt++;

        }

        if(cnt != l_length-52)
            exit(1);

        for(int i = 0; i < cnt; i++)
        {
            fprintf(output_file,"%c", (char)result_hex[i]);
        }
        
        l_num += 0x10;
    }

    fclose(output_file);
    fclose(fp);

}

void bin_to_txt(char *argv[])
{
    FILE *fp, *output_file;
    fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        exit(1);
    }

    output_file = fopen(argv[2], "w");

    unsigned char buffer[16];
    char str[20];

    size_t bytesRead;
    int t,k=0,temp = 0;

    while ((bytesRead = fread(buffer, sizeof(unsigned char), 16, fp)) > 0) {
        t = 0;
        fprintf(output_file,"%08x: ", k);
        for (size_t i = 0; i < bytesRead; i++) {
            hex_to_char(buffer[i],str,i);
            fprintf(output_file,"%02x", buffer[i]);
            if(t % 2 !=0)
                fprintf(output_file," ");
            t++;
            temp = i;
        }

        str[temp+1] = '\0';
        
        t = 0;
        for (int i = 0; i < 16- bytesRead; i++)
        {
            fprintf(output_file,"  ");
            if(t % 2 !=0)
                fprintf(output_file," ");
            t++;
        }

        fprintf(output_file," %s",str);
        fprintf(output_file,"\n");

        k += 0x10;
    }

    fclose(fp);
}
void hex_to_char(int hex, char arr[], int idx)
{
    if(0x20 <= hex && hex <= 0x7e)
        arr[idx] = (char) hex ;
    else
        arr[idx] = '.';
}