#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>

FILE * fp_track ;

char cwd[1024];

char commands[][30] = {"init","track","untrack","store","restore","versions"};

typedef enum 
{

    INITIALIZE ,
    TRACK      ,
    UNTRACK    ,
    STORE      ,
    RESTORE    ,
    VERSIONS

} TYPE;

/*-----for run------*/
int run_keep                    (int argc, char *argv[]);
/*------------------*/

/*-----for init-----*/
int  initialize                 ();
/*------------------*/

/*---------------for untrack-----------------*/
int  untrack                    (char target[], char track_path[]);

bool delete_specific_in_file    (char * check);
/*-------------------------------------------*/


/*----------------for track -----------------*/
int  track                      (char target[], char track_path[],int signal);

bool track_regular              (char target[], char track_path[], struct stat buf);

bool track_dir                  (char path[],char track_path[]);

bool track_deleted              (char target[],char track_path[]);

bool is_target_new              (char target[],char track_path[]);

bool is_file_in_track           (char * target, char * file_name);

bool append_to_file             (char * file_path, char * content);

bool edit_specific_in_file      ( char * content, char * check);
/*-------------------------------------------*/

/*---------------for store-------------------*/
int  store                      (char note[]);

bool check_store_available      ();

int  get_latest_version         ();

bool save_latest_version        (int version, char note[]);

bool store_file_init            (int version, char note[]);

bool update_track_files         ();

bool copy_file_to_store         (int version);

bool check_object_deleted       (char object[]);
/*--------------------------------------------*/

/*---------------for restore-------------------*/
int  restore                    (int version);

bool deleteFiles                (char path[]);

bool cover_file                 (int version);

void copyFile                   (char* sourcePath, char* destinationPath);

void copyDirectory              (char* sourcePath, char* destinationPath);

/*---------------------------------------------*/

/*---------------for versions------------------*/
int  versions                   (char path[]);

void print_version              (char number[] ,FILE *f);
/*---------------------------------------------*/

/*---------------for prerequirement------------*/
bool check_arg_count            (int com_num ,int argc);

bool check_prerequirement       (int com_num);

bool is_initialized();

bool is_tracked();

bool is_stored();
/*---------------------------------------------*/

/*------------------others--------------------*/
bool find_f_d                   (char * dirpath,char * check);

int find_com_num                (char command[]);

void print_keep                 ();

void print_bye                  ();

bool is_digit                   (char a[]);

bool is_char                    (char a[]);

int  cnt_char_in_string         (char a[], char check);

void print_file                 (char file_path[]);

char * read_a_line 				();

int list_dir                    (char * dirpath,char * check);
/*--------------------------------------------*/


int main                        (int argc, char *argv[])
{   
    if(run_keep(argc,argv) != 0)
        EXIT_FAILURE;

    return EXIT_SUCCESS;
}



int run_keep                    (int argc, char *argv[])
{
    int check, com_num;

    getcwd(cwd, sizeof(cwd));

    char track_path[100];

    sprintf(track_path,"%s/%s",cwd,".keep/tracking-files");

    if(argc == 1)
    {
        printf("Please Type the Command\n");
        exit(1);
    }

    com_num = find_com_num(argv[1]);


    if(!check_arg_count(com_num,argc)  || !check_prerequirement(com_num))
    {
        printf("Invalid Command\n");
        exit(1);
    }
        

    switch (com_num)
    {
        case INITIALIZE:
            check = initialize();
            break;
        case TRACK:
            check = track(argv[2],track_path,1);
            break;
        case UNTRACK:
            check = untrack(argv[2],track_path);
            break;
        case STORE:
            check = store(argv[2]);
            break;
        case RESTORE:
            check = restore(atoi(argv[2]));
            break;
        case VERSIONS:
            check = versions(".keep");
            break;
        default:
            printf("Invalid Command\n");
            exit(1);
            break;
    }

    return check;
}


int initialize                  ()
{

    char cwd[1024], * fullpath, * dirname = ".keep";
    char* fname1 = "tracking-files"; // 새 폴더 이름1
    char* fname2 = "latest-version"; // 새 폴더 이름2
    int status, result_of_search; 

    fullpath = (char *) malloc(sizeof(char) * 1000);
    
    
    if (getcwd(cwd, sizeof(cwd)) == NULL) 
    {
        perror("getcwd() error");
        return 1;
    }
    
    // result_of_search = list_dir(cwd,".keep");
    
    // if(result_of_search == 1)
    // {
    //     printf("[ERROR] Already initialized\n");
    //     return 1;
    // }
        
    
    sprintf(fullpath, "%s/%s", cwd, dirname); // 경로와 폴더 이름을 합쳐 전체 경로 생성
    status = mkdir(fullpath, 0777); // 폴더 생성

    
    if (status != 0) 
    {
        return 1;
    }

    free(fullpath);
    fullpath = (char *) malloc(sizeof(char) * 1000);

    // .keep 내부에 폴더 생성
    sprintf(fullpath, "%s/%s/%s", cwd, dirname,fname1); 

    FILE* file = fopen(fullpath, "w"); // 파일을 생성하고 파일 포인터를 반환
    if (file == NULL) 
    { 
        return 1;
    }

    fclose(file); // 파일 포인터 닫기
    free(fullpath);
    
    fullpath = (char *) malloc(sizeof(char) * 1000);

    // .keep 내부에 폴더 생성
    sprintf(fullpath, "%s/%s/%s", cwd, dirname,fname2); 
    FILE* file1 = fopen(fullpath, "w"); // 파일을 생성하고 파일 포인터를 반환
    fprintf(file,"0\n");

    free(fullpath);

    fullpath = (char *) malloc(sizeof(char) * 1000);


    if (status != 0) 
    {
        return 1;
    }

    fclose(file1); // 파일 포인터 닫기

    // sprintf(fullpath, "%s/%s/0", cwd, dirname); 
    // status = mkdir(fullpath, 0777); // 폴더 생성


    // if (status != 0) 
    // {
    //     return 1;
    // }


	// char* current_path = getenv("PATH");
    // char new_path[1024];
    // snprintf(new_path, sizeof(new_path), "%s:.", current_path);
    // setenv("PATH", new_path, 1);

    print_keep();
    printf("\n\t   Initialization Successful!\n\n");
    return 0;
}


int  untrack                    (char target[],char track_path[])
{

    if(is_file_in_track(target,track_path))
        delete_specific_in_file(target);
    else
    {
        printf("This file is not included in tracking-file\n");
        return 1;
    }
    
    
    
    return 0;
}

bool delete_specific_in_file    (char * check)
{
    FILE *f = fopen(".keep/tracking-files", "r");  // 파일 리스트를 읽기 모드로 열기

    if (f == NULL) 
    {
        printf("Impossible to open the file.\n");
        return false;
    }

    FILE *tempFile = fopen(".keep/temp", "w");  // 임시 파일 열기

    if (tempFile == NULL) 
    {
        printf("Impossible to open the temp file.\n");
        return false;
    }

    char line[1000],line_copy[1000];

    while (fgets(line, 1000, f) != NULL) 
    {
        strcpy(line_copy,line);
        char * p = strtok(line_copy," ");

        if (strncmp(p,check,strlen(check)) == 0) 
            continue;   
        else 
            fprintf(tempFile, "%s", line);
        
    }

    fclose(f);
    fclose(tempFile);

    remove(".keep/tracking-files");  // 기존 파일 삭제
    rename(".keep/temp", ".keep/tracking-files");  

    return true;
}


int  track                      (char target[], char track_path[],int signal)
{

    char * full_path;
    int result;
    bool check = false;
    struct stat buf;

    DIR * dir = opendir(cwd) ;

    if (dir == 0x0) // there is nothing in dir.
		return 1;

    full_path = (char *) malloc(sizeof(char) * (1024));
    sprintf(full_path,"%s/%s",cwd,target);
    result = stat(full_path, &buf);

    if (result == -1) 
    {
        perror("stat");

        return 1;
    }


    if(is_target_new(target,track_path) && signal == 1)
    {
        printf("Already Tracked File\n");
        return 1;
    }


    if(S_ISREG(buf.st_mode))
        track_regular(target,track_path,buf);
    else if(S_ISDIR(buf.st_mode))
        track_dir(target,track_path);

    free(full_path);
    closedir(dir);
    return 0;

}

bool track_regular              (char target[], char track_path[], struct stat buf)
{
    char content[100];

    if(is_file_in_track(target,track_path))
    {
        sprintf(content, "%s %s",target,ctime(&buf.st_mtime));
        edit_specific_in_file(content,target);
    }
        
    else
    {
        sprintf(content, "%s 0",target); 
        append_to_file(track_path,content);
    }

    return true;
}

bool track_dir                  (char path[],char track_path[])
{   
    char full_path[200];
    char *target_path,*direct_path;
    sprintf(full_path,"%s/%s",cwd,path);

    DIR *d = opendir(full_path);

    if (d == 0x0) 
		return false;
    
    for (struct dirent * i = readdir(d) ; i != NULL ; i = readdir(d)) {
		if (i->d_type != DT_DIR && i->d_type != DT_REG)
			continue ;

		char * filepath = (char *) malloc(strlen(full_path) + 1 + strlen(i->d_name) + 1) ;
        char * direct_path = (char *) malloc(sizeof(char)*200) ;
		strcpy(filepath, full_path) ;
		strcpy(filepath + strlen(full_path), "/") ;
		strcpy(filepath + strlen(full_path) + 1, i->d_name) ;

        strcpy(direct_path,path);

		if (i->d_type == DT_DIR) {
			// printf("d ") ;
			// printf("%s\n", filepath) ;
            
			if (strcmp(i->d_name, ".") != 0 && strcmp(i->d_name, "..") != 0) 
            {
                strcat(direct_path,"/");
                strcat(direct_path,i->d_name);
                track_dir(direct_path,track_path);
                free(direct_path);
            }
				
		}
		else if (i->d_type == DT_REG) {
			struct stat st ;
            target_path = (char *) malloc(sizeof(char) * 1000);
            sprintf(target_path,"%s/%s",path,i->d_name);
			int check_stat = stat(filepath, &st) ;

            if(check_stat == -1)
            {
                printf("Error Occures at Track Dir\n");
            }
			// printf("r ") ;
			// printf("%s ", filepath) ;
			// printf("%d\n", (int) st.st_size) ;
            track_regular(target_path,track_path,st);
            free(target_path);
		}
		free(filepath) ;
	}

	closedir(d) ;

    return true;
}

bool track_deleted              (char target[],char track_path[])
{

    char * content;

    content = (char *) malloc(sizeof(char)*1000);

    if(is_file_in_track(target,track_path))
    {
        // printf("deleted check : %s -1 \n" ,target);
        sprintf(content, "%s -1\n",target);
        edit_specific_in_file(content,target);
    }
    else
    {
        printf("Unknown Error - track_deleted\n");
        return false;
    }

    free(content);   
    
    return true;
}

bool is_target_new              (char target[],char track_path[])
{
    bool check = false;
    fp_track = fopen(track_path, "r") ;

    if(fp_track == NULL)
        return 1;

    char * s = 0x0,* s_copy ;

	while ((s = read_a_line())) {
        s_copy = (char*) malloc(sizeof(char) * 1000);
        strcat(s_copy,s);
        char * p = strtok(s_copy," ");    
        if(strcmp(p,target) == 0)
        {
            if(p != NULL)
            {
                p = strtok(NULL," ");
                if( is_digit(p) == true)
                {
                    check = true;
                    break;
                }
            }
        }

		free(s) ;
        free(s_copy);
	}

    fclose(fp_track);

    return check;
}

bool is_file_in_track           (char * target, char * file_name)
{
    bool check = false;
    fp_track = fopen(file_name, "r") ;

    if(fp_track == NULL)
        return 1;

    char * s = 0x0,* s_copy ;

	while ((s = read_a_line())) {
        s_copy = (char*) malloc(sizeof(char) * 1000);
        strcat(s_copy,s);
        char * p = strtok(s_copy," ");    
        if(strncmp(p,target,strlen(target)) == 0)
        {
            check = true;
            break;
        }

		free(s) ;
        free(s_copy);
	}

    fclose(fp_track);

    return check;
}

bool append_to_file             (char * file_path, char * content)
{
    FILE * f = fopen(file_path,"a");

    if(f == NULL)
        return false;
    
    fprintf(f,"%s\n",content);
    fclose(f);

    return true;
}

bool edit_specific_in_file      (char * content, char * check)
{
    FILE *f = fopen(".keep/tracking-files", "r");  // 파일 리스트를 읽기 모드로 열기

    if (f == NULL) 
    {
        printf("Impossible to open file.");
        return false;
    }

    FILE *tempFile = fopen(".keep/temp", "w");  // 임시 파일 열기

    if (tempFile == NULL) 
    {
        printf("Impossible to open temp file.");
        return false;
    }

    char line[1000],line_copy[1000];

    while (fgets(line, 1000, f) != NULL) 
    {
        strcpy(line_copy,line);
        char * p = strtok(line_copy," ");

        if (strncmp(p,check,strlen(check)) == 0) 
        {
            fprintf(tempFile, "%s", content);
        } 
        else 
            fprintf(tempFile, "%s", line);
    }

    fclose(f);
    fclose(tempFile);

    // print_file(".keep/temp");

    remove(".keep/tracking-files");  // 기존 파일 삭제
    rename(".keep/temp", ".keep/tracking-files");  

    return true;
}


int  store                      (char note[])
{
    if(check_store_available())
    {
        int version = get_latest_version();
        if(!save_latest_version(version,note))
            return false;
        update_track_files();
        store_file_init(version,note);
        copy_file_to_store(version);
        printf("stored as version %d\n",version);
    }
    else 
    {
        printf("Nothing to update\n");
        return 1;
    }
    return 0;
}

bool check_store_available      ()
{
    FILE *f = fopen(".keep/tracking-files", "r");  // 파일 리스트를 읽기 모드로 열기
    struct stat buf;

    if (f == NULL) 
    {
        printf("Impossible to open file.");
        return false;
    }    

    char line[1000];
    char *file_path, *p, *modified_time, *line_copy;
    bool check = false,is_deleted=false;
    int result,line_cnt=0,not_mod = 0;
    

    while (fgets(line, 1000, f) != NULL) 
    {
        p = strtok(line," ");

        file_path = (char*)malloc(sizeof(char) * 1000);
        sprintf(file_path,"%s/%s",cwd,p);

        result = stat(file_path, &buf);

        if (result == -1) 
        {
            is_deleted = true;
        }

        p += strlen(p)+1;
        modified_time = (char*)malloc(sizeof(char) * 1000);
        if(!is_deleted)
            sprintf(modified_time,"%s",ctime(&buf.st_mtime));
        else
            sprintf(modified_time,"-1");
        

        // printf("modi : %s p: %s\n",modified_time,p);

        if(strcmp(p,modified_time) != 0)
            return true;
        free(file_path);
        free(modified_time);
    }
    fclose(f);
    return false;
}

int  get_latest_version         ()
{
    int version = 0;
    FILE *f = fopen(".keep/latest-version","r");

    if (f == NULL) 
    {
        printf("File is not available to open.\n");
        return false;
    }
    
    char line[1000], *p;
    
    while (fgets(line, 1000, f) != NULL) 
    {
        version  = atoi(line);
    }

    fclose(f);
    return version+1;
}

bool save_latest_version        (int version, char note[])
{
    FILE *f = fopen(".keep/latest-version","w");

    if (f == NULL) 
    {
        printf("File is not available to open.\n");
        return false;
    }

    fprintf(f,"%d\n",version);

    fclose(f);

    return true;
}

bool store_file_init            (int version, char note[])
{
    FILE * f;

    char file_path[1000], * elements;
    
    int status;

    sprintf(file_path,".keep/%d",version);

    status = mkdir(file_path, 0777); 

    if (status != 0) 
        return false;
    
    elements = (char*) malloc(sizeof(char) * 1000);

    sprintf(elements,"%s/target",file_path);

    status = mkdir(elements, 0777); 

    if (status != 0) 
        return false;
    
    free(elements);
    
    elements = (char*) malloc(sizeof(char) * 1000);

    sprintf(elements,"%s/note",file_path);

    f = fopen(elements,"w");

    fprintf(f,"%s\n",note);

    if(f == NULL)
        return false;
    
    fclose(f);
    free(elements);

    elements = (char*) malloc(sizeof(char) * 1000);

    sprintf(elements,"%s/tracking-files",file_path);  

    char command[100];
    sprintf(command, "cp %s %s", ".keep/tracking-files", elements);
    system(command);

    free(elements);

    return true;
}

bool update_track_files         ()
{
    FILE *f = fopen(".keep/tracking-files","r");

    struct stat buf;

    char track_path[1000];

    int result;

    sprintf(track_path,"%s/%s",cwd,".keep/tracking-files");

    if (f == NULL) {
        printf("파일을 열 수 없습니다.");
        return false;
    }

    char line[1000],* p;

    while (fgets(line, 1000, f) != NULL) 
    {
        p = strtok(line," ");

        result = stat(p,&buf);

        if(result == -1)
            track_deleted(p,track_path);
        else
            track(p,track_path,2);
    }

    
    fclose(f);

    return true;
}

bool copy_file_to_store         (int version)
{
    FILE *f = fopen(".keep/tracking-files","r");

    char track_path[1000];

    sprintf(track_path,"%s/%s",cwd,".keep/tracking-files");

    if (f == NULL) {
        printf("파일을 열 수 없습니다.");
        return false;
    }

    char line[1000],* p,* save_path,* track_folder,* line_copy, command[100], * check_line_copy;
    int cnt, status, result;
    struct stat buf;

    while (fgets(line, 1000, f) != NULL) 
    {
        check_line_copy = (char*) malloc(sizeof(char)*1000);
        strcat(check_line_copy,line);

        if(check_object_deleted(check_line_copy))
        {
            free(check_line_copy);
            continue;
        }
            

        p = strtok(line," ");
        cnt = cnt_char_in_string(p,'/');

        line_copy = (char*) malloc(sizeof(char)*1000);
        strcpy(line_copy,p);

        track_folder = (char*) malloc(sizeof(char)*1000);
        save_path = (char*) malloc(sizeof(char)*1000);

        
        sprintf(save_path,"%s/%d/target/",".keep",version);

        if(cnt != 0)
        {
            for(int i = 0; i <cnt; i++)
            {
                if(i == 0)
                {
                    sprintf(track_folder,"%s",p);

                    p = strtok(track_folder,"/");

                    strcat(save_path,p);

                    result = stat(save_path, &buf);

                    // printf("result : %d , path : %s\n",result,save_path);

                    if(result != -1)
                        continue;

                    status = mkdir(save_path, 0777); 

                    if (status != 0)
                    {
                        printf("fail1\n");
                        return false;
                    }
                        

                }
                else
                {
                    p = strtok(NULL,"/");
                    // printf("%s ",p);

                    
                    strcat(save_path,"/");

                    strcat(save_path,p);

                    result = stat(save_path, &buf);

                    // printf("result : %d , path : %s\n",result,save_path);

                    if(result != -1)
                        continue;

                    status = mkdir(save_path, 0777); 

                    if (status != 0) 
                    {
                        printf("fail2\n");
                        return false;
                    }
                }

                
            }

            p = strtok(NULL,"/");
            
        }

        // printf("cp /%s %s\n",line_copy,save_path);

        sprintf(command, "cp %s %s", line_copy,save_path);
        system(command);
        
        free(track_folder);
    }

    
    fclose(f);

    return true;
}

bool check_object_deleted       (char object[])
{
    char *p = strtok(object," ");

    if(p == NULL)
        return false;
    
    p = strtok(NULL," ");

    if(p == NULL)
        return false;

    int result = atoi(p);

    if(result == -1)
        return true;
    return false;
}


int  restore                    (int version)
{   

    if(check_store_available())
    {
        printf("Refuse the Restore command.\n");
        return 1;
    }

    char *path,*dest_path;

    path = (char*) malloc(sizeof(char) * 1000);

    sprintf(path,"%s",cwd);

    if(!deleteFiles(path))
    {
        printf("Unknown Error in Deleting Files\n");
        return 1;
    }

    free(path);

    path = (char*) malloc(sizeof(char) * 1000);
    dest_path = (char*) malloc(sizeof(char) * 1000);

    sprintf(path,"%s/.keep/%d/target",cwd,version);
    sprintf(dest_path,"%s",cwd);

    copyDirectory(path,dest_path);

    if(cover_file(version) == false)
    {
        printf("Error in cover file\n");
        exit(1);
    }

    printf("restored as version %d\n",version);
    return 0;
}

bool cover_file                 (int version)
{
    char * vf_path;

    vf_path = (char *) malloc (sizeof(char) * 1000);

    sprintf(vf_path,".keep/%d/tracking-files",version);

    FILE *f = fopen(".keep/tracking-files", "w");
    FILE *vf = fopen(vf_path, "r");  

    if (f == NULL) 
    {
        printf("Impossible to open file.");
        return false;
    }

    if (vf == NULL) 
    {
        printf("Impossible to open file.");
        return false;
    }

    char line[1000],line_copy[1000];

    while (fgets(line, 1000, vf) != NULL) 
    {
        fprintf(f,"%s",line);
    }

    fclose(f);
    fclose(vf);

    return true;
}

bool deleteFiles                (char path[]) 
{
    struct dirent* entry;
    DIR* dir = opendir(path);

    if (dir == NULL) {
        printf("디렉토리를 열 수 없습니다: %s\n", path);
        return false;
    }

    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".keep") == 0) 
            continue;  // ".","..",".keep" 파일은 제외

        char filePath[256];
        snprintf(filePath, sizeof(filePath), "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR) 
        {
            deleteFiles(filePath);  // 하위 디렉토리의 파일들도 삭제
            rmdir(filePath);  // 빈 폴더 삭제
        } 
        else 
        {
            remove(filePath);  // 파일 삭제
        }
    }

    closedir(dir);

    return true;
}

void copyFile                   (char* sourcePath, char* destinationPath) 
{
    FILE* sourceFile = fopen(sourcePath, "rb");
    FILE* destinationFile = fopen(destinationPath, "wb");

    if (sourceFile == NULL || destinationFile == NULL) 
    {
        printf("파일을 열 수 없습니다.\n");
        return;
    }

    int bufferSize = 1024;
    char buffer[bufferSize];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, bufferSize, sourceFile)) > 0) 
    {
        fwrite(buffer, 1, bytesRead, destinationFile);
    }

    fclose(sourceFile);
    fclose(destinationFile);
}

void copyDirectory              (char* sourcePath, char* destinationPath) 
{
    DIR* sourceDir = opendir(sourcePath);

    if (sourceDir == NULL) {
        printf("디렉토리를 열 수 없습니다.\n");
        return;
    }

    struct dirent* entry;
    char sourceFilePath[256];
    char destinationFilePath[256];

    while ((entry = readdir(sourceDir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
            continue;

        snprintf(sourceFilePath, sizeof(sourceFilePath), "%s/%s", sourcePath, entry->d_name);
        snprintf(destinationFilePath, sizeof(destinationFilePath), "%s/%s", destinationPath, entry->d_name);

        if (entry->d_type == DT_DIR) 
        {
            DIR* destinationDir = opendir(destinationFilePath);

            if (destinationDir == NULL) 
            {
                mkdir(destinationFilePath, 0755); // 폴더 생성
            } 
            
            else 
            {
                closedir(destinationDir);
            }

            copyDirectory(sourceFilePath, destinationFilePath); // 하위 폴더 복사
        } 

        else 
        {
            copyFile(sourceFilePath, destinationFilePath); // 파일 복사
        }
    }

    closedir(sourceDir);
}


int  versions                    (char path[])
{
    DIR * dir = opendir(path) ;

    bool check = false;

	if (dir == 0x0)
		return 0;

	for (struct dirent * i = readdir(dir) ; i != NULL ; i = readdir(dir)) {
		if (i->d_type != DT_DIR && i->d_type != DT_REG)
			continue ;

		char * filepath = (char *) malloc(strlen(path) + 1 + strlen(i->d_name) + 1) ;
		strcpy(filepath, path) ;
		strcpy(filepath + strlen(path), "/") ;
		strcpy(filepath + strlen(path) + 1, i->d_name) ;

		if (i->d_type == DT_DIR) {
			if (strcmp(i->d_name, ".") != 0 && strcmp(i->d_name, "..") != 0)
            {
                if(!is_char(i->d_name))
                {
                    check = true;
                    char * temp;

                    temp = (char*) malloc(sizeof(char) * 1000);
                    sprintf(temp,"%s/note",filepath);

                    FILE * f = fopen(temp,"r");

                    if(f == NULL)
                    {
                        printf("There is no file\n");
                        return false;
                    }

                    print_version(i->d_name,f);

                    fclose(f);
                    free(temp);
                }
            }
				
		}
		free(filepath) ;
	}

	closedir(dir) ;

    if(check == false)
    {
        printf("There is no stored version.\n");
        return false;
    }

    return true;
}

void print_version              (char number[],FILE *f)
{
    char line[1000],number_line[200],note_line[1000],index_line[1000];


    fgets(line, 1000, f);

    sprintf(index_line,"|  VERSION  |");
    int note_len,number_len;

    note_len = strlen(line);
    number_len = strlen(number);

    line[note_len-1] ='\0';

    strcat(number_line,"|");

    for(int i = 1; i <= 5 - number_len + 1 ; i++)
        strcat(number_line," ");
    
    strcat(number_line, number);

    for(int i = 1; i <= 6-number_len; i++)    
        strcat(number_line," ");
    strcat(number_line,"| ");

    strcat(number_line,line);
    strcat(number_line,"  |");

    for(int i = 1; i <= note_len/2-2 + 1; i++)
        strcat(index_line," ");
    strcat(index_line,"NOTE");
    for(int i = 1; i <= note_len/2-2 + 1; i++)
        strcat(index_line," ");
    strcat(index_line,"|");

    sprintf(note_line,"*");
    for(int i = 0; i < strlen(index_line) -2; i++)
        strcat(note_line,"-");
    strcat(note_line,"*");

    printf("%s\n",note_line);
    printf("%s\n",index_line);
    printf("%s\n",note_line);
    printf("%s\n",number_line);
    printf("%s\n",note_line);
        
}


bool check_arg_count            (int com_num,int argc)
{

    switch (com_num)
    {
        case INITIALIZE:
            if(argc != 2)
                return false;
            break;
        case TRACK:
            if(argc != 3)
                return false;
            break;
        case UNTRACK:
            if(argc != 3)
                return false;
            break;
        case STORE:
            if(argc != 3)
                return false;
            break;
        case RESTORE:
            if(argc != 3)
                return false;
            break;
        case VERSIONS:
            if(argc != 2)
                return false;
            break;
        default:
            printf("Invalid Command\n");
            exit(1);
            break;
    }

    return true;
}

bool check_prerequirement       (int com_num)
{
    if(com_num == INITIALIZE && is_initialized())
    {
        printf("Already Initialized.\n");
        exit(1);
    }
        

    if(com_num > 0)
    {
        if(is_initialized() == false)
        {
            printf("[Requriements] please Do \"Initialize\" First.\n");
            return false;
        }

        if(com_num > 1)
        {
            if(is_tracked() == false)
            {
                printf("[Requriements] please Do \"Track\" First.\n");
                return false;
            }

            if(com_num > 3 && is_stored() == false)
            {
                printf("[Requriements] please Do \"Store\" First.\n");
                return false;
            }
        }
    }

    return true;
}

bool is_initialized()
{   
    char keep_path[1000];
    sprintf(keep_path,"%s/.keep",cwd);
    
    if(find_f_d(cwd,".keep") && find_f_d(keep_path,"tracking-files") &&  find_f_d(keep_path,"latest-version"))
        return true;
    return false;
}

bool is_tracked()
{
    FILE *f;
    int cnt = 0;
    f = fopen(".keep/tracking-files","r");

    if(f == NULL)
        return false;
    
    char line[1000];
    
    while (fgets(line, 1000, f) != NULL) 
        cnt++;

    fclose(f);

    if(cnt == 0)
        return false;

    return true;
}

bool is_stored()
{
    if(get_latest_version() == 0)
        return false;

    return true;
}


bool find_f_d                   (char * dirpath,char * check)
{
    DIR * dir = opendir(dirpath) ;

    if(strcmp(dirpath,check) == 0)
        return true;

	if (dir == 0x0)
		return false;

	for (struct dirent * i = readdir(dir) ; i != NULL ; i = readdir(dir)) {
		if (i->d_type != DT_DIR && i->d_type != DT_REG)
			continue ;

		char * filepath = (char *) malloc(strlen(dirpath) + 1 + strlen(i->d_name) + 1) ;
		strcpy(filepath, dirpath) ;
		strcpy(filepath + strlen(dirpath), "/") ;
		strcpy(filepath + strlen(dirpath) + 1, i->d_name) ;

		if (i->d_type == DT_DIR) {
            if(strcmp(i->d_name,check) == 0)
                return true;

			if (strcmp(i->d_name, ".") != 0 && strcmp(i->d_name, "..") != 0) 
				list_dir(filepath,check) ;
		}
		else if (i->d_type == DT_REG) {
			struct stat st ;
			stat(filepath, &st) ;
			// printf("r ") ;
			// printf("%s ", filepath) ;
			// printf("%d\n", (int) st.st_size) ;
            if(strcmp(i->d_name,check) == 0)
                return true;
		}
		free(filepath) ;
	}

	closedir(dir) ;

    return false;
}

int find_com_num                (char command[])
{
    int  com_num = -1;

    for(int i = 0; i < 6; i++)
    {
        if(strcmp(commands[i],command) == 0)
        {
            com_num = i;
            break;
        }
    }

    
    return com_num;
}

void print_keep                 ()
{
    printf("*------------------------------------------------*\n");
    printf("|   .-. .-')      ('-.      ('-.      _ (`-.     |\n");
    printf("|   \\  ( OO )   _(  OO)   _(  OO)    ( (OO  )    |\n");
    printf("|   ,--. ,--.  (,------. (,------.  _.`     \\    |\n");
    printf("|   |  .'   /   |  .---'  |  .---' (__...--''    |\n");
    printf("|   |      /,   |  |      |  |      |  /  | |    |\n");
    printf("|   |     ' _) (|  '--.  (|  '--.   |  |_.' |    |\n");
    printf("|   |  .   \\    |  .--'   |  .--'   |  .___.'    |\n");
    printf("|   |  |\\   \\   |  `---.  |  `---.  |  |         |\n");
    printf("|   `--' '--'   `------'  `------'  `--'         |\n");
    printf("*------------------------------------------------*\n");                        
}

void print_bye                  ()
{
    printf("*--------------------------------*\n");
    printf("|      ___    __   __    ___     |\n");
    printf("|     | _ )   \\ \\ / /   | __|    |\n");
    printf("|     | _ \\    \\ V /    | _|     |\n");
    printf("|     |___/    _|_|_    |___|    |\n");
    printf("|   _|\"\"\"\"\"| _| \"\"\" | _|\"\"\"\"\"|   |\n");
    printf("|   \"`-0-0-' \"`-0-0-' \"`-0-0-'   |\n");
    printf("*--------------------------------*\n");
}

bool is_digit                   (char a[])
{
    for(int i = 0; i < strlen(a); i++)
    {
        for(int j = '0'; j <= '9'; j++)
        {
            if(a[i] == j)
                return true;
        }
    }

    return false;
}

bool is_char                    (char a[])
{

    for(int i = 0; i < strlen(a); i++)
    {
        for(int j = 'a'; j <= 'z'; j++)
        {
            if(a[i] == j)
            {
                return true;
            }
                
        }
    }

    return false;
}

void print_file                 (char file_path[])
{
    FILE *f = fopen(file_path, "r");  // 파일 리스트를 읽기 모드로 열기

    if (f == NULL) 
    {
        printf("파일을 열 수 없습니다.");
        return ;
    }

    char line[1000];

    while (fgets(line, 1000, f) != NULL) 
    {
        printf("%s\n",line);
    }
}

int list_dir                    (char * dirpath,char * check)
{
	DIR * dir = opendir(dirpath) ;

	if (dir == 0x0)
		return 1;

	for (struct dirent * i = readdir(dir) ; i != NULL ; i = readdir(dir)) {
		if (i->d_type != DT_DIR && i->d_type != DT_REG)
			continue ;

		char * filepath = (char *) malloc(strlen(dirpath) + 1 + strlen(i->d_name) + 1) ;
		strcpy(filepath, dirpath) ;
		strcpy(filepath + strlen(dirpath), "/") ;
		strcpy(filepath + strlen(dirpath) + 1, i->d_name) ;

		if (i->d_type == DT_DIR) {
			// printf("d ") ;
			// printf("%s %s\n", i->d_name,check) ;
            if(strcmp(i->d_name,check) == 0)
                return 0;

			if (strcmp(i->d_name, ".") != 0 && strcmp(i->d_name, "..") != 0) 
				list_dir(filepath,check) ;
		}
		else if (i->d_type == DT_REG) {
			struct stat st ;
			stat(filepath, &st) ;
			// printf("r ") ;
			// printf("%s ", filepath) ;
			// printf("%d\n", (int) st.st_size) ;
		}
		free(filepath) ;
	}

	closedir(dir) ;

    return 1;
}

char * read_a_line 				()
{
	static char buf[BUFSIZ] ;
	static int buf_n = 0 ;
	static int curr = 0 ;

	if (feof(fp_track) && curr == buf_n - 1)
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

		buf_n = fread(buf, 1, sizeof(buf), fp_track) ;
		curr = 0 ;
	} while (buf_n > 0) ;
	return s ;
}

int cnt_char_in_string          (char a[], char check)
{   
    int cnt = 0;

    for(int i = 0; i < strlen(a); i++)
    {
        if(a[i] == check)
            cnt++;
    }

    return cnt;
}