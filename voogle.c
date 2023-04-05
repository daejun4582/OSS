#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


enum ComType
{
	CORRES    ,
    PREFIX    ,
	NOT_CORRES,
	STRING    ,
	CHAPTER   ,
	BOOK      ,
	END
};


FILE * fp_niv ;

char * read_a_line  		();


int num_of_digit			(int num);

// functions related with command

void get_command			(char com_str[],char *com[],int *words_num);

void command_replace		(char str[]);

void command_tokenize		(char str[],char** a,int *word);

void command_preprocessing	(char* com[],int words_num);

void command_print_all		(char *com[], int words_num);

bool command_checking		(char* com[], int words_num);


// functions related with checking bible content

void content_tolower		(char s[]);

int identify_types			(char com[]);

bool check_types			(char s[], char* comms[], int words_num);

bool check_type1			(char s[], char com[]); //  token

bool check_type2			(char s[], char com[]); //  token*

bool check_type3			(char s[], char com[]); //  -token

bool check_type4			(char s[], char com[]); //  "token"

bool check_type5			(char s[], char com[]); //  chapter

bool check_type6			(char s[], char com[]); //  book


// functions related with printing 

void print_bible			(char *com[],int words_num);


/*
	example : abraham* isaac -easu " and " book:8 chapter:Mat ^D
*/


int main (int argc, char ** argv)
{

	int words_num;

	char* command_tokenes[8];

	char temp[1000], com_str[1000];

	get_command(com_str,command_tokenes,&words_num);

	command_preprocessing(command_tokenes,words_num);

	command_print_all(command_tokenes,words_num);

	print_bible(command_tokenes,words_num);

	return 0;
}


void get_command			(char com_str[], char *com[], int *words_num){

	while(true){

		printf("명령어를 입력해주세요 : ");

		fgets(com_str,1000,stdin);

		com_str[strlen(com_str)-1] = '\0';

		command_replace(com_str);

		command_tokenize(com_str, com, words_num);
	
		if ( !(command_checking(com, *words_num)) ) printf("\n[Warning] 지원하지 않는 명령어 형식입니다. 다시 입력해주세요. \n\n");
		
		else break;
	}
}

void command_replace		(char str[]){
	bool flag = false;

	for(int i = 0; i < strlen(str); i++){

		if(str[i] == ' ' && flag) str[i] = '#';

		else if(str[i] == '^' && !flag) str[i] = ' ';

		else if (str[i] == 'D' && !flag) str[i] = '$'; 

		else if(str[i] == '"' ) {

			if(flag) flag = false;

			else flag = true;

		}
	}
}

void command_tokenize		(char str[],char** a, int *word){

	int idx = 0;

	char *ptr = strtok(str, " ");    

	*(a+idx) = ptr;

	idx++;
	
	while (ptr != NULL)              
	{
		ptr = strtok(NULL, " ");     

		if(ptr == NULL) break;

		*(a+idx) = ptr;

		idx++;
	}
	*word = idx;

}

void command_print_all		(char *com[], int words_num){

	printf("\n입력된 명령어는 다음과 같습니다. : | ");

	for(int i = 0; i < words_num; i++){

		printf("%s | ",com[i]);

	}

	printf("\n");
}

void command_preprocessing	(char* com[], int words_num){

	for(int i = 0; i < words_num; i++){

		if(strstr(com[i],"\"") != NULL){

			for(int j = 0; j < strlen(com[i]); j++){

				if(com[i][j] == '#') com[i][j] = ' '; 
			}
		}
		else{

			if(strstr(com[i],":")!= NULL) continue;

			for(int j = 0; j < strlen(com[i]); j++){

				if((com[i][j]) >= 'A' && (com[i][j]) <= 'Z'){

					com[i][j] = tolower(com[i][j]);
				}
			}
		}
	}
}

bool command_checking		(char* com[], int words_num){

	char book_c[20], *ptr,token;
	int result;

	for(int i = 0; i < words_num; i++){

		if( strstr(com[i],"book") != NULL ){

			strcpy(book_c,com[i]);

			ptr = strtok(book_c,":");
			ptr = strtok(NULL,":");

			result = atoi(ptr);

			if(strlen(ptr) != num_of_digit(result) && result == 0) return false;
		}
	}

	return true;
}

int num_of_digit			(int num){

	int digit = 0;

	while(num != 0){

		num = num/10;

		digit++;

	}

	return digit;
}

void content_tolower		(char s[]){

	for(int i = 0; i < strlen(s); i++){

		if((s[i]) >= 'A' && (s[i]) <= 'Z'){

			s[i] = tolower(s[i]);

		}
	}
}

int identify_types			(char com[]){

	if(strstr(com,"chapter") != NULL) return CHAPTER;

	else if(strstr(com,"book") != NULL) return BOOK;

	else if(strstr(com,"\"") != NULL) return STRING;

	else if(strstr(com,"-") != NULL) return NOT_CORRES;

	else if(strstr(com,"*") != NULL) return PREFIX;

	else if(strstr("$",com) != NULL) return END;

	else return CORRES;
}

bool check_type1			(char s[], char com[]){

	int result;

	char s_copy[1000];

	strcpy(s_copy,s);
	
	s_copy[strlen(s_copy)] = '\0';

	content_tolower(s_copy);

	char *ptr = strtok(s_copy, " ");    
	
	while (ptr != NULL)              
	{	
		if(strcmp(com,ptr)==0) return true;

		ptr = strtok(NULL, " ");    
	}

	return false;
}

bool check_type2			(char s[], char com[]){

	char com_copy[100];
	char s_copy[1000];
	int com_len = strlen(com);
	
	strncpy(com_copy,com,com_len-1);
	strcpy(s_copy,s);

	com_copy[com_len-1] = '\0';
	s_copy[strlen(s_copy)] = '\0';

	content_tolower(s_copy);

	char *ptr = strtok(s_copy, " ");    
	
	while (ptr != NULL)              
	{

		if(strstr(ptr,com_copy) != NULL) return true;

		ptr = strtok(NULL, " ");     
		
	}

	return false;

}

bool check_type3			(char s[], char com[]){

	char com_copy[100], s_copy[1000];

	int com_len = strlen(com), idx = 0;

	strcpy(s_copy,s);

	s_copy[strlen(s_copy)] = '\0';

	for(int i = 0; i < com_len ; i++){

		if(com[i] != '-') {

			com_copy[idx] = com[i]; 

			idx++;
		}
		
	}

	com_copy[idx] = '\0';

	content_tolower(s_copy);

	char *ptr = strtok(s_copy, " ");    //첫번째 strtok 사용.
	
	while (ptr != NULL)              //ptr이 NULL일때까지 (= strtok 함수가 NULL을 반환할때까지)
	{
		if(strcmp(ptr,com_copy) == 0){

			return false;
		}
		 
		ptr = strtok(NULL, " ");     //자른 문자 다음부터 구분자 또 찾기
		
	}

	return true;
}

bool check_type4			(char s[], char com[]){

	char a[100];

	int com_len = strlen(com), idx = 0;

	for(int i = 0; i < com_len ; i++){

		if(com[i] != '\"') {

			a[idx] = com[i]; 

			idx++;
		}
		
	}
	a[idx] = '\0';

	if(strstr(s,a) != NULL) return true;

	return false;
}

bool check_type5			(char s[], char com[]){

	char com_copy[100], s_copy[1000];

	strcpy(s_copy,s);

	s_copy[strlen(s_copy)] = '\0';

	char *ptr_s = strtok(s_copy, " "); 

	strcpy(com_copy,com);

	com_copy[strlen(com_copy)] = '\0';

	char *ptr_c = strtok(com_copy,":");

	ptr_c = strtok(NULL," ");

	if(strcmp(ptr_s,ptr_c) == 0) return true;

	return false;
}

bool check_type6			(char s[], char com[]){


	char s_copy[1000], com_copy[100];;

	strcpy(s_copy,s);

	s_copy[strlen(s_copy)] = '\0';

	char *ptr1 = strtok(s_copy, " ");   

	ptr1 = strtok(NULL," ");

	ptr1 = strtok(ptr1,":");

	strcpy(com_copy,com);

	com_copy[strlen(com_copy)] = '\0';
	
	char *ptr2 = strtok(com_copy,":");

	ptr2 = strtok(NULL," ");

	if(strcmp(ptr1,ptr2) == 0) return true;

	return false;
}

bool check_types			(char s[], char* comms[], int word_num){

	int type;
	int result;
	bool flag;

	for(int i = 0; i < word_num; i++){

		flag = false;

		type = identify_types(comms[i]);

		switch (type)
		{
		case CORRES:
			if(!check_type1(s,comms[i])) return false;
			break;

		case PREFIX:
			if(!check_type2(s,comms[i])) return false;
			break;

		case NOT_CORRES:
			if(!check_type3(s,comms[i])) return false;
			break;

		case STRING:
			if(!check_type4(s,comms[i])) return false;
			break;

		case CHAPTER:
			if(!check_type5(s,comms[i])) return false;
			break;

		case BOOK:
			if(!check_type6(s,comms[i])) return false;
			break;

		case END:
			flag = true;
		default:
			break;
		}
		if(flag) break;
	}

	return true;

}

void print_bible			(char *com[],int words_num){

	int cnt = 0;

	printf("\n일치한 구절을 출력하겠습니다. \n\n");

	fp_niv = fopen("NIV.txt", "r") ;

	char * s = 0x0 ;     
	
	while ((s = read_a_line())) {
		
		if(check_types(s,com, words_num)) {

			cnt++;

			printf("[%d] %s\n",cnt,s);
		}

		free(s) ;
	}

	printf("\n-------------------------------------\n");
	printf("  총 \"%d\" 개의 구절이 탐색되었습니다. \n",cnt);
	printf("-------------------------------------\n");

	fclose(fp_niv);

}

char * read_a_line 			()
{
	static char buf[BUFSIZ] ;
	static int buf_n = 0 ;
	static int curr = 0 ;

	if (feof(fp_niv) && curr == buf_n - 1)
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

		buf_n = fread(buf, 1, sizeof(buf), fp_niv) ;
		curr = 0 ;
	} while (buf_n > 0) ;
	return s ;
}