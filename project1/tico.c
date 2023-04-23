#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MEMORY_SIZE 256

typedef enum 
{
	READ	,    	
    WRITE	,    	
	ASSIGN	,	
	MOVE	,    	
	LOAD	,   	
	STORE	,   	
	ADD		,	
	MINUS	,		
	MULT	,	
	MOD		,	
	EQ		,	
	LESS	,	
	JUMP	,	
	JUMPIF	,	
	TERM	,	
	VALUE
} INST_TYPE;

char commands[16][10] = 
{
	"READ"		,    
    "WRITE"		,    
	"ASSIGN"	,
	"MOVE"		,    
	"LOAD"		,   
	"STORE"		,   
	"ADD"		,
	"MINUS"		,	
	"MULT"		,
	"MOD"		,
	"EQ	"		,
	"LESS"		,
	"JUMP"		,
	"JUMPIF"	,
	"TERM"		,
	"VALUE"		,
};

typedef struct{
	INST_TYPE 	type;
	int 		value[3];
} memory;



FILE * fp_asb ;

char * read_a_line  			();

void memory_initialize(memory mem1[]);

void print_memory(memory mem1[]);

void save_command_in_memory	(char com[], memory *mem1);

bool is_command					(char com[]);

int check_type_of_command		(char com);

int num_of_digit				(int num);

void eliminate				(char *str, char ch, char * result);

bool load_file_to_memory(memory mem1[]);

bool run_tico();


int main(void){

	run_tico();

    return 0;
}

bool run_tico(){

	memory mem1[MEMORY_SIZE];

	memory_initialize(mem1);

	load_file_to_memory(mem1);

	print_memory(mem1);

}

bool load_file_to_memory(memory m1[]){

	char fname[100];

	char form[] = ".txt";

	printf("type the name of file : ");

	scanf("%s",fname);

	strcat(fname,form);

	fp_asb = fopen(fname, "r") ;

	char * s = 0x0 ;

	while ((s = read_a_line())) {
		save_command_in_memory(s,m1);
		free(s) ;
	}

	fclose(fp_asb) ;

	return true;

}

void memory_initialize(memory mem1[]){
	for(int i = 0; i < 256; i++)
	{
		mem1[i].type = VALUE;

		for(int j = 0; j < 3; j++)
		{
			mem1[i].value[j] = 0;
		}

	}
}

void print_memory(memory mem1[])
{
	int dinum ;

	printf("+--------------------------------------------+\n");
	printf("|                MEMORY  INFO                |\n");
	printf("+--------------------------------------------+\n");
	printf("|  NUM  | INSTRUC | VALUE1 | VALUE2 | VLAUE3 |\n");
	printf("+--------------------------------------------+\n");
	for(int i = 0; i < 30; i++)
	{
		dinum = num_of_digit(i)	;
		if(dinum == 1)
			printf("| %3d   |  %-6s |",i,commands[mem1[i].type]);
		else if(dinum == 2)
			printf("|   %2d  |  %-6s |",i,commands[mem1[i].type]);
		else
			printf("|  %3d  |  %-6s |",i,commands[mem1[i].type]);
		for(int j = 0; j < 3; j++)
		{
			printf("   %2d   |",mem1[i].value[j]);
		}
		
		printf("\n");

	}
	printf("+--------------------------------------------+");
}

void save_command_in_memory	(char com[], memory * mem1){

	char * address, * val_s, result[10];

	int add_address,digit,val,idx = 0;

	bool check_com = is_command(com);

	memory m1;

	m1.type = VALUE;
	
	for(int i = 0 ; i < 3; i++)
		m1.value[i] = 0;


	address = strtok(com, ":");

	add_address = atoi(address);

	digit = num_of_digit(add_address);
	
	address = address+(digit+1);

	if(check_com){

		address = strtok(address, " ");

		for(int i = 0; i < 16; i++)
		{
			if(strcmp(address,commands[i])==0)
			
			m1.type = i;
		}

		while(true){
			address = strtok(NULL, " ");   
			if(address == NULL)
				break;
			
			m1.value[idx] = atoi(address);

			idx ++;
			
		}  


	}
	else{

		address = strtok(NULL, " ");   



		if(address != NULL)
		{
			eliminate(address,'"',result);
			m1.value[0] = atoi(result);
		}


	}

	mem1[add_address] = m1;

}

void eliminate(char *str, char ch , char * result){

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

bool is_command(char com[]){
	// command that include upper class word

	for(int i = 'A'; i <= 'Z'; i++)
	{
		if(strchr(com,i) != NULL)
			return true;
	}

	return false;

}

int num_of_digit				(int num)
{	
	if(num == 0) return 1;

	int cnt = 0;
	while(num != 0)
	{
		num /= 10;
		cnt++;
	}

	return cnt;
}

char * read_a_line 			()
{
	static char buf[BUFSIZ] ;
	static int buf_n = 0 ;
	static int curr = 0 ;

	if (feof(fp_asb) && curr == buf_n - 1)
		return 0x0 ;

	char * s = 0x0 ;
	size_t s_len = 0 ;
	do {
		int end = curr ;
		while (!(end >= buf_n || !iscntrl(buf[end]))) {
			end++ ;
		}
		if (curr < end && s != 0x0) {
			curr = end ;
			break ;
		}
		curr = end ;
		while (!(end >= buf_n || iscntrl(buf[end]))) {
			end++ ;
		}
		if (curr < end) {
			if (s == 0x0) {
				s = strndup(buf + curr, end - curr) ;
				s_len = end - curr ;
			}
			else {
				s = realloc(s, s_len + end - curr + 1) ;
				s = strncat(s, buf + curr, end - curr) ;
				s_len = s_len + end - curr ;
			}
		}
		if (end < buf_n) {
			curr = end + 1 ;
			break ;
		}

		buf_n = fread(buf, 1, sizeof(buf), fp_asb) ;
		curr = 0 ;
	} while (buf_n > 0) ;
	return s ;
}