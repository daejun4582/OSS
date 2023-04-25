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
	TERM	
} TYPE;

char type_s[16][10] = 
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

typedef struct { 
  TYPE operator;
  unsigned char operand [3];    
} Instructor;



typedef union { 
    signed char value; 
    Instructor inst; 
} Memory;

FILE * fp_asb ;

char * read_a_line  			();


bool run_tico					();


void memory_initialize			(Memory mem1[]);

int load_file_to_memory		(Memory mem1[], int * eoins);

int save_command_to_memory		(char com[], Memory mem1[], int * eoins);

int run_command				    (Memory mem1[], int * n);

void print_memory				(Memory mem1[], int eoins);


bool is_instruction				(char com[]);

void eliminate_char				(char *str, char ch, char * result);

int  num_of_digit				(int num);


void print_banner           	();

void print_bye					();


int main(void)
{
    run_tico();
    return 0;
}



bool run_tico					()
{
    Memory mem1[MEMORY_SIZE];

    int result = 0,idx = 0, eoins = 0, check = 0;

	char pm_b;

	print_banner();

	printf(">> HI, This is your TICO: Tiny Computer\n");

	printf("\n>> Do you prefer to print the Memory  State ? [ Y , N ] : ");

	scanf("%c",&pm_b);

	printf("\n>> [info] Memory Initializing...\n");

	memory_initialize(mem1);

	printf("\n>> [info] SUCCESS !\n\n");

	printf(">> [info] Start Lodaing File \n");

	check = load_file_to_memory(mem1,&eoins);

	if (check == 0)
		printf("\n>> [info] SUCCESS !\n\n");
	else 
	{	
		if(check == 1)
			printf("\n>> [ERROR] the range of operand is overflowed. \n\n");
		else if(check == 2)
			printf("\n>> [ERROR] the range of value is overflowed. \n\n");
		printf("\n\n>> terminate the tico\n");
		print_bye();
		return 0;
	}
    

    printf("\n>> Run the commands in memory \n\n");

	printf("\n>> Proccess of running\n");

	printf("-------------------------------------------\n");

	while(true)
	{
		result = run_command(mem1,&idx);
		
		if(result == -1)
			break;
	}
	printf("-------------------------------------------\n");
	
	if(pm_b == 'Y')
	{
		printf("\n>> check the memory state\n\n");
		print_memory(mem1,eoins);
	}
	
	printf("\n\n>>[info] terminate the TICO\n");
	print_bye();

    return 0;
}

void memory_initialize			(Memory mem1[])
{
    for(int i  = 0; i < 256; i++)
    {
        mem1[i].value = 0;
    }
}

int load_file_to_memory		(Memory mem1[], int * eoins)
{
    char fname[100];

	char form[] = ".txt";

	int result;

	printf("\n>> Type the name of file exclude extends (EX : fil1.txt -> file ) :  ");

	scanf("%s",fname);

	strcat(fname,form);

	printf("\n>> [info] Loading... \n");

	fp_asb = fopen(fname, "r") ;

	printf("\n>> [info] SUCCESS ! \n");

	char * s = 0x0 ;

	printf("\n>> [info] Save data to memory \n");

	printf("\n>> [info] Loading . . .\n");

	while ((s = read_a_line())) {
		
		result = save_command_to_memory(s,mem1,eoins);

		if(result != 0)
			return result;

		free(s) ;
	}

	fclose(fp_asb) ;

	return 0;

}

int save_command_to_memory		(char com[], Memory mem1[], int * eoins)
{
    char * address, * val_s, result[10];

	int digit,val,idx = 0;

	unsigned char add_address,res;

	bool check_com = is_instruction(com);

	Memory m1;

	for (int i = 0; i < 3; i++)
        m1.inst.operand[i] = 0;


	address = strtok(com, ":");

	add_address = atoi(address);

	digit = num_of_digit(add_address);
	
	address = address+(digit+1);

	if(check_com){
        *eoins = *eoins +1;

		address = strtok(address, " ");

		for(int i = 0; i < 16; i++)
		{
			if(strcmp(address,type_s[i])==0)
			{
				m1.inst.operator = i;
			}
				
		}

		while(true){
			address = strtok(NULL, " ");   
			if(address == NULL)
				break;
			
			val = atoi(address);

			if(0 > val || val > 255)
				return 1;

			m1.inst.operand[idx] = val;

			idx ++;
			
		}  

	}
	else{

		address = strtok(NULL, " ");   

		if(address != NULL)
		{
			eliminate_char(address,'"',result);
			
			val = atoi(result);

			if(-128 > val || val > 127)
				return 2;

			m1.value = val;
		}
        else
        {
            m1.value = 0;
        }

	}

	mem1[add_address] = m1;


	return 0;
}

int run_command				(Memory mem1[], int* n)
{
	int result = 0,input;
	int now = *n;

    unsigned char opern[3];

    for(int i = 0; i < 3; i++)
    {
        opern[i] = mem1[now].inst.operand[i];
    }

	switch (mem1[now].inst.operator)
	{
		case READ:
			// printf(">> [info] READ start \n");
			
			printf("INPUT  : ");

			scanf("%d",&(input));

			while(true)
			{
				if( -128 <= input && input <= 127)
					break;

				printf("\n[ERROR] Input value is overflowed. Try it agin.(range : -128 <= x <=127) \n\n");

				printf("INPUT  : ");

				scanf("%d",&(input));
			}


			mem1[opern[0]].value = input;
			// printf(">> save succesfully !\n");

			break;
		case WRITE:
			// printf(">> [info] WRITE start \n");
			// printf("-----------------------------------------------------\n");
            printf("OUTPUT : %d\n", mem1[opern[0]].value);
			// printf("-----------------------------------------------------\n");
			break;
		case ASSIGN:
			// printf(">> [info] ASSIGN start \n");
            mem1[opern[0]].value = opern[1];
			// printf(">> save succesfully !\n");
			break;
		case MOVE:
			// printf(">> [info] MOVE start \n");
            mem1[opern[0]].value = mem1[opern[1]].value ;
			// printf(">> save succesfully !\n");
			break;
		case LOAD:
			// printf(">> [info] LOAD start \n");
			// md ms
			// ms에 있는 값이 주소값 -> 그 주소값으로 넘어가서 사지고 있는 값을 md로 집어 넣는다.
            mem1[opern[0]].value = mem1[mem1[opern[1]].value].value;
 			break;
		case STORE:
			// printf(">> [info] STORE start \n");
			//md ms
			// ms에 있는 값이 주소값 -> md의 값을 그 주소값에 다가 집어 넣는다.
            mem1[opern[1]].value = mem1[mem1[opern[0]].value].value;
			break;
		case ADD	:
			// printf(">> [info] ADD start \n");
			mem1[opern[0]].value = mem1[opern[1]].value + mem1[opern[2]].value;
			break;
		case MINUS:
			// printf(">> [info] MINUS start \n");
			mem1[opern[0]].value = mem1[opern[1]].value - mem1[opern[2]].value;
			break;
		case MULT:
			// printf(">> [info] MULT start \n");
			mem1[opern[0]].value = mem1[opern[1]].value * mem1[opern[2]].value;
			break;
		case MOD:
			// printf(">> [info] MOD start \n");
			mem1[opern[0]].value = mem1[opern[1]].value % mem1[opern[2]].value;
			break;
		case EQ	:
			// printf(">> [info] EQ start \n");
            if( mem1[opern[1]].value == mem1[opern[2]].value)
				mem1[opern[0]].value = 1;
			else
				mem1[opern[0]].value = 0;

			break;
		case LESS:
			// printf(">> [info] LESS start \n");
            if( mem1[opern[1]].value < mem1[opern[2]].value)
				mem1[opern[0]].value = 1;
			else
				mem1[opern[0]].value = 0;
			break;
		case JUMP:
			// printf(">> [info] JUMP start \n");
			*n = opern[0];
			return 0;

			break;
		case JUMPIF:
			// printf(">> [info] JUMPIF start \n");
			if(mem1[opern[1]].value != 0)
			{
				*n =  opern[0]; 
				return 0;
			}
				
			break;
		case TERM:
			// printf(">> [info] TERM start \n");
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

void print_memory			(Memory mem1[],int eoins)
{
	int idx = 0 ,dinum;
    char types[][30] = {"INSTRUCTION","VALUE"};

	printf("+-----------------------------------------------------------------------+\n");
	printf("|                             MEMORY  INFO                              |\n");
	printf("+-----------------------------------------------------------------------+\n");
	printf("|  NUM  |      TYPE      | OPERATOR | OPERAND 1 | OPERAND 2 | OPERAND 3 |\n");
	printf("+-----------------------------------------------------------------------+\n");


	for(int i = 0; i < eoins-1; i++)
	{
		dinum = num_of_digit(idx);
		if(dinum == 1)
			printf("| %3d   |  %-12s  |  %-6s  |",idx,types[0],type_s[mem1[i].inst.operator]);
		else if(dinum == 2)
			printf("|   %2d  |  %-12s  |  %-6s  |",idx,types[0],type_s[mem1[i].inst.operator]);
		else
			printf("|  %3d  |  %-12s  |  %-6s  |",idx,types[0],type_s[mem1[i].inst.operator]);
		for(int j = 0; j < 3; j++)
		{
			if(mem1[i].inst.operand[j] != 0)
				printf("   %4d    |",mem1[i].inst.operand[j]);
			else
				printf("           |");
		}
		
		printf("\n");
		
        idx++;
	}

    printf("+-----------------------------------------------------------------------+\n");
	printf("|  NUM  |      TYPE      |                     VALUE                    |\n");
	printf("+-----------------------------------------------------------------------+\n");
    for(int i = eoins ; i < 256; i++)
    {
        dinum = num_of_digit(idx);

        if(dinum == 1)
			printf("|  %3d    |     %-7s   |",idx,types[1]);
		else if(dinum == 2)
			printf("|   %2d   |     %-7s   |",idx,types[1]);
		else
			printf("|  %3d   |     %-7s   |",idx,types[1]);
        
        printf("   %20d                       |\n",mem1[i].value);
        
        idx++;
    }
	printf("+-----------------------------------------------------------------------+\n");
}

bool is_instruction				(char com[])
{
	// command that include upper class word

	for(int i = 'A'; i <= 'Z'; i++)
	{
		if(strchr(com,i) != NULL)
			return true;
	}

	return false;

}

void eliminate_char				(char *str, char ch, char * result)
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

void print_banner           ()
{
	printf("+-------------------------------------------------+\n");
	printf("|    _____                    ____     U  ___ u   |\n");
	printf("|   |_ \" _|       ___       U \"___|     \\/\"_ \\/   |\n");
	printf("|     | |        |_\"_|     \\| | u       | | | |   |\n");
	printf("|    /| |\\        | |       | |/__  .-,_| |_| |   |\n");
	printf("|   u |_|U      U/| |\\u      \\____|  \\_)-\\___/    |\n");
	printf("|   _// \\\\_  .-,_|___|_,-.  _// \\\\        \\\\      |\n");
	printf("|   (__) (__)  \\_)-' '-(_/  (__)(__)      (__)    |\n");
	printf("+-------------------------------------------------+\n\n");
}

void print_bye				()
{

printf("     +-----------------------------------+\n");
printf("     |      ____     __   __ U _____ u   |\n");
printf("     |   U | __\")u   \\ \\ / / \\| ___\"|/   |\n");
printf("     |    \\|  _ \\/    \\ V /   |  _|\"     |\n");
printf("     |     | |_) |   U_|\"|_u  | \\|___    |\n");
printf("     |     |____/      |_|    |_____|    |\n");
printf("     |    _|| \\\\_  .-,//|(_   <<   >>    |\n");
printf("     |   (__) (__)  \\_) (__) (__) (__)   |\n");
printf("     +-----------------------------------+\n\n");

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
