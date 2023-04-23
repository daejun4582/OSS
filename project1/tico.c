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
	"EQ"		,
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

bool run_tico					();

void memory_initialize			(memory mem1[]);

void print_memory				(memory mem1[]);

bool load_file_to_memory		(memory mem1[]);

int find_end_of_memory			(memory mem1[]);

int run_command				(memory mem1[], int * n, int end);

void save_command_in_memory		(char com[], memory *mem1);

int find_end_of_inst			(memory mem1[]);

bool is_command					(char com[]);

int  num_of_digit				(int num);

void eliminate					(char *str, char ch, char * result);


int main(void)
{
	run_tico();

    return 0;
}

bool run_tico()
{

	memory mem1[MEMORY_SIZE];

	int result = 0,idx = 0;

	printf("+-------------------------------------------------+\n");
	printf("|    _____                    ____     U  ___ u   |\n");
	printf("|   |_ \" _|       ___       U \"___|     \\/\"_ \\/   |\n");
	printf("|     | |        |_\"_|     \\| | u       | | | |   |\n");
	printf("|    /| |\\        | |       | |/__  .-,_| |_| |   |\n");
	printf("|   u |_|U      U/| |\\u      \\____|  \\_)-\\___/    |\n");
	printf("|   _// \\\\_  .-,_|___|_,-.  _// \\\\        \\\\      |\n");
	printf("|   (__) (__)  \\_)-' '-(_/  (__)(__)      (__)    |\n");
	printf("+-------------------------------------------------+\n\n");

	printf(">> hi this is tico\n");

	printf(">> [info] memory initializing...\n");

	memory_initialize(mem1);

	if (load_file_to_memory(mem1))
		printf(">> save data to memory succesfully !\n");


	while(true)
	{
		result = run_command(mem1,&idx,find_end_of_inst(mem1));
		
		if(result == -1)
			break;
	}

	printf(">> check the memory state\n\n");
	print_memory(mem1);
	
	printf("\n\n>> terminate the tico\n");
	printf(">> thank you\n");

	return true;

}

bool load_file_to_memory	(memory m1[])
{

	char fname[100];

	char form[] = ".txt";

	printf(">> type the name of file exclude extends ex : fil1 ) :  ");

	scanf("%s",fname);

	strcat(fname,form);

	printf(">> [info] start lodaing file \n");

	printf(">> [info] loading... \n");

	fp_asb = fopen(fname, "r") ;

	printf(">> [info] file loaded succesfully \n");

	char * s = 0x0 ;

	printf(">> [info] save data to memory \n");

	printf(">> [info] loading . . .\n");

	while ((s = read_a_line())) {

		save_command_in_memory(s,m1);

		free(s) ;
	}

	fclose(fp_asb) ;

	return true;

}

void memory_initialize		(memory mem1[])
{
	for(int i = 0; i < 256; i++)
	{

		mem1[i].type = VALUE;

		for(int j = 0; j < 3; j++)
		{
			mem1[i].value[j] = 0;
		}

	}
}

void print_memory			(memory mem1[])
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
	printf("+--------------------------------------------+\n");
}


int run_command			(memory mem1[], int* n, int end)
{
	int result = 0;
	int now = *n;

	switch (mem1[now].type)
	{
		case READ:
			printf(">> [info] READ start \n");
			printf(">> type the input data : ");

			scanf("%d",&(mem1[mem1[now].value[0]].value[0]));
			
			printf(">> save succesfully !\n");

			break;
		case WRITE:
			printf(">> [info] WRITE start \n");
			printf("-----------------------------------------------------\n");
			printf(">> output : %d\n", mem1[mem1[now].value[0]].value[0]);
			printf("-----------------------------------------------------\n");
			break;
		case ASSIGN:
			printf(">> [info] ASSIGN start \n");
			mem1[mem1[now].value[0]].value[0] = mem1[now].value[1];
			printf(">> save succesfully !\n");
			break;
		case MOVE:
			printf(">> [info] MOVE start \n");
			mem1[mem1[now].value[0]].value[0] = mem1[mem1[now].value[1]].value[0] ;
			printf(">> save succesfully !\n");
			break;
		case LOAD:
			printf(">> [info] LOAD start \n");
			// md ms
			// ms에 있는 값이 주소값 -> 그 주소값으로 넘어가서 사지고 있는 값을 md로 집어 넣는다.
			mem1[mem1[now].value[0]].value[0] = mem1[mem1[mem1[now].value[1]].value[0]].value[0];
 			break;
		case STORE:
			printf(">> [info] STORE start \n");
			//md ms
			// ms에 있는 값이 주소값 -> md의 값을 그 주소값에 다가 집어 넣는다.
			mem1[mem1[mem1[now].value[1]].value[0]].value[0] = mem1[mem1[now].value[0]].value[0];
			break;
		case ADD	:
			printf(">> [info] ADD start \n");
			mem1[mem1[now].value[0]].value[0] = mem1[mem1[now].value[1]].value[0] +  mem1[mem1[now].value[2]].value[0];
			break;
		case MINUS:
			printf(">> [info] MINUS start \n");
			mem1[mem1[now].value[0]].value[0] = mem1[mem1[now].value[1]].value[0] -  mem1[mem1[now].value[2]].value[0];
			break;
		case MULT:
			printf(">> [info] MULT start \n");
			mem1[mem1[now].value[0]].value[0] = mem1[mem1[now].value[1]].value[0] *  mem1[mem1[now].value[2]].value[0];
			break;
		case MOD:
			printf(">> [info] MOD start \n");
			mem1[mem1[now].value[0]].value[0] = mem1[mem1[now].value[1]].value[0] %  mem1[mem1[now].value[2]].value[0];
			break;
		case EQ	:
			printf(">> [info] EQ start \n");
			if( mem1[mem1[now].value[1]].value[0] == mem1[mem1[now].value[2]].value[0])
				mem1[mem1[now].value[0]].value[0] = 1;
			else
				mem1[mem1[now].value[0]].value[0] = 0;
			break;
		case LESS:
			printf(">> [info] LESS start \n");
			if( mem1[mem1[now].value[1]].value[0] < mem1[mem1[now].value[2]].value[0])
				mem1[mem1[now].value[0]].value[0] = 1;
			else
				mem1[mem1[now].value[0]].value[0] = 0;
			break;
		case JUMP:
			printf(">> [info] JUMP start \n");
			*n =  mem1[now].value[0]; 
			return 0;

			break;
		case JUMPIF:
			printf(">> [info] JUMPIF start \n");
			if(mem1[mem1[now].value[1]].value[0] != 0)
			{
				*n =  mem1[now].value[0]; 
				return 0;
			}
				
			break;
		case TERM:
			printf(">> [info] TERM start \n");
			return -1;
			break;
	
	default:
		printf(">> [ERROR] This command is in an unsupported format");
		return -1;
		break;
	}

	*n = (*n)+1;

	return 0;
}

void save_command_in_memory	(char com[], memory * mem1)
{

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
			{
				m1.type = i;
			}
				
		
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

int find_end_of_inst			(memory mem1[])
{	
	int idx = 0;

	while(true)
	{
		if(mem1[idx].type == VALUE)
			break;
		idx++;
	}
	return idx;
}

void eliminate				(char *str, char ch , char * result)
{

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

bool is_command				(char com[])
{
	// command that include upper class word

	for(int i = 'A'; i <= 'Z'; i++)
	{
		if(strchr(com,i) != NULL)
			return true;
	}

	return false;

}

int  num_of_digit			(int num)
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