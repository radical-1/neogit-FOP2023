#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
typedef char* string;
#define make_string(n) (string)calloc(n, sizeof(char))
#define remove_string(str) free(str)
#define GLOBAL 1
#define LOCAL 0
int IS_GLOBAL = LOCAL;
// random number for hash
long long giveRandomNumber() {
    // Initialize random seed
    srand(time(NULL));
    // Generate a random number and assign it to the integer
    return (rand() % (100000000 - 10000000 + 1)) + 10000000;
}
int find_file(const char *directory, const char *search_name) {
    struct dirent *entry;
    DIR *dp;

    // Open the directory
    dp = opendir(directory);
    if (dp == NULL) {
        perror("opendir");
        return -1;
    }

    // Read the directory entries
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, search_name) == 0) {
            // Found the file
            closedir(dp);
            return 1;
        }
    }

    // Close the directory stream
    closedir(dp);
    return 0;
}
// base setting
void information(char* input[])
{
    char cwd[100];
    getcwd(cwd, 100);
    char mem[100];

    
}
void ALIAS(char* input[])
{
    printf ("%s = %s\n", input[0], input[1]);
    char cwd[200];
    getcwd(cwd, sizeof(cwd));
    if(IS_GLOBAL == LOCAL) {
        if (!find_file(cwd, ".neogit")) {
            printf(".neogit not found in current working directory. Please run `neogit init` first\n");
            return;
        }
    }
    char alias[200];
    char command[200];
    sscanf(input[0], "alias.%s", alias);
    strcpy(command, input[1]);
    if(IS_GLOBAL == GLOBAL) {
        FILE* GLOB = fopen("GLOBAL_alias.txt", "a");
        fprintf(GLOB, "%s = %s\n", alias, command);
        fclose(GLOB);       
    }
    if (find_file(cwd, ".neogit")) {
        char local_alias[300];
        sprintf(local_alias, "%s/.neogit/LOCAL_alias.txt", cwd);
        FILE* LOC = fopen(local_alias, "a");
        fprintf(LOC, "%s = %s\n", alias, command);
        fclose(LOC);
    }
}
//this part will be completed soon
void make_branch(char branchName[], char neogitDir[])
{
    char branchAddress[100];
    sprintf(branchAddress, "%s/.%s", neogitDir, branchName);
    mkdir(branchAddress, 0775);
}
// think about what is needed to add in .neogit directory when you are making it
void Open_dirctories_for_init(char neogitDir[]) 
{
    char commit[200];
    char repository[200];
    char stage[200];
    make_branch("master", neogitDir);
    sprintf(repository,"%s/.LOCAL_REPOSITORY",neogitDir);
    sprintf(stage,"%s/.STAGING_AREA",neogitDir);
    sprintf(commit,"%s/.COMMITS",neogitDir);
    mkdir(commit, 0775);
    mkdir(repository, 0775);
    mkdir(stage, 0775);
    // using slash for unix path
}
void init() {
    char currentDirectory[200];
    getcwd(currentDirectory, sizeof(currentDirectory));
    int exist_neogit = find_file(currentDirectory, ".neogit");
    if (exist_neogit) {
        printf("noegit is already initialized in this folder!\n");
        return;
    }
    char neoGitDir[200];
    sprintf(neoGitDir, "%s/.neogit", currentDirectory);
    mkdir(neoGitDir, 0775);
    Open_dirctories_for_init(neoGitDir);
    printf("Initialized empty Git repository in %s\n", neoGitDir);
}
int check_is_global(char*, int*);
int main(int argc, char *argv[])
{    
    int crt_arg = 1; //current argument
    FILE* GLOBAL_ALIAS = fopen("GLOBAL_alias.txt", "r");
    if (GLOBAL_ALIAS) {
        string tmp = make_string(200);
        while(fgets(tmp, sizeof(tmp), GLOBAL_ALIAS)) {
            char alias[200];
            sscanf(tmp, "%s", alias);
            if (strcmp(alias, argv[crt_arg]) == 0) {
                tmp += (strlen(alias) + 3);
                int idx = 0;
                while (strlen(argv[crt_arg + idx + 1])) {
                    ssscanf(tmp, "%s", argv[crt_arg + idx]);
                    tmp += (strlen(argv[crt_arg + idx]) + 1);
                    idx++;
                }
                break;
            }
        }
        remove_string(tmp);
    }
    if (strcmp(argv[crt_arg], "init") == 0) {
        init();
        crt_arg++;
    }
    if (strcmp(argv[crt_arg], "config") == 0) {
        crt_arg++;
        check_is_global(argv[crt_arg], &crt_arg);
        printf("%s\n", argv[crt_arg]);
        if (strncmp(argv[crt_arg], "alias", 5) == 0) {
            ALIAS(&argv[crt_arg]);
        }
        else information(&argv[crt_arg]);
        
    }
    printf("end of our project!\n");
    return 0;
}
int check_is_global(char input[], int* crt_arg)
{
    if(strcmp(input, "-global") == 0) {
        IS_GLOBAL = GLOBAL;
        (*crt_arg)++;
        return 1;
    }
    return 0;
}
