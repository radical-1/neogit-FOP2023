#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#endif
#define GLOBAL 1
#define LOCAL 0
int IS_GLOBAL = LOCAL;

// base setting
void config(char* input[])
{
    char cwd[100];
    getcwd(cwd, 100);
    char mem[100];
    strcpy(mem, "D:\\projectFOP\\user\\");
    strcat(mem, cwd);
    strcat(mem, ".txt");
    FILE* memory = fopen(mem , "w");
    if(strcmp(input[0], "user.name") == 0) {
        char filewrite[100];
        strcpy(filewrite, "username : ");
        strcat(filewrite, input[1]);
        strcat(filewrite, "\n");
        fputs(filewrite, memory);
        if (IS_GLOBAL == GLOBAL) {
            FILE* global = fopen("D:\\projectFOP\\user\\global.txt", "w");
            if(global == NULL) printf("failed to open file!\n");
            fputs(filewrite, global);
            fclose(global);
        }
        printf("usser.name completelly get setted!\n");
    }
    if(strcmp(input[0], "user.email") == 0) {
        char filewrite[100];
        strcpy(filewrite, "user email : ");
        strcat(filewrite, input[1]);
        strcat(filewrite, "\n");
        fputs(filewrite, memory);
        if (IS_GLOBAL == GLOBAL) {
            FILE* global = fopen("D:\\projectFOP\\user\\global.txt", "w");
            if(global == NULL) printf("failed to open file!\n");
            fputs(filewrite, global);
            fclose(global);
        }
        printf("user.email completelly get setted!\n");
    }
    fclose(memory);
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
//this part will be completed soon
void make_branch(char branchName[], char neogitDir[])
{
    char branchAddress[100];
    strcpy(branchAddress, neogitDir);
    #ifdef _WIN32
            strcat(branchAddress, "\\.");
    #else
            strcpy(branchAddress, "/.");
    #endif
    strcat(branchAddress, branchName);
    #ifdef _WIN32
        mkdir(branchAddress);
        SetFileAttributes(branchAddress, FILE_ATTRIBUTE_HIDDEN);
    #else
        mkdir(branchAddress, 666);
    #endif
}
// think about what is needed to add in .neogit directory when you are making it
void Open_dirctories_for_init(DIR* Repo, char neogitDir[]) 
{
    char commit[100];
    char stage[100];
    char Repository[100];
    strcpy(commit, neogitDir);
    strcpy(stage, neogitDir);
    strcpy(Repository, neogitDir);
    make_branch("master", neogitDir);
    #ifdef _WIN32
            strcat(commit, "\\.commits");
            strcat(stage, "\\.stage");
            strcat(Repository, "\\Local_Repository");
            mkdir(commit);
            mkdir(stage);
            mkdir(Repository);
            SetFileAttributes(commit, FILE_ATTRIBUTE_HIDDEN);
            SetFileAttributes(stage, FILE_ATTRIBUTE_HIDDEN);
    #else
            strcat(commit, "/.commits");
            strcat(stage, "/.stage");
            strcat(Repository, "/Local_Repository");
            mkdir(commit, 666);
            mkdir(stage, 666);
            mkdir(Repository, 666);
            // using slash for unix path

    #endif
}
void init() {
    char currentDirectory[100];
    getcwd(currentDirectory, sizeof(currentDirectory));
    int exist_neogit = find_file(currentDirectory, ".neogit");
    if (exist_neogit) {
        printf("noegit is already initialized in this folder!\n");
        return;
    }
    char neoGitDir[100];
    strcpy(neoGitDir, currentDirectory);
#ifdef _WIN32
    strcat(neoGitDir, "\\.neogit"); // Use backslash for Windows paths
    mkdir(neoGitDir);
    SetFileAttributes(neoGitDir, FILE_ATTRIBUTE_HIDDEN);
#else
    strcat(neoGitDir, "/.neogit"); // Use forward slash for Unix paths
    mkdir(neoGitDir, 666);
#endif
    DIR *Repo = opendir(neoGitDir);
    Open_dirctories_for_init(Repo, neoGitDir);
    printf("Initialized empty Git repository in %s\n", neoGitDir);
}
int check_is_global(char*, int*);
int main(int argc, char *argv[])
{
    
    int crt_arg = 1; //current argument
    if (strcmp(argv[crt_arg], "init") == 0) {
        init();
        crt_arg++;
    }
    if (strcmp(argv[crt_arg], "config") == 0) {
        crt_arg++;
        check_is_global(argv[crt_arg], &crt_arg);
        config(&argv[crt_arg]);
        
    }
    printf("end of our project!\n");
    return 0;

}
int check_is_global(char* input, int* crt_arg)
{
    if(strcmp(input, "-global") == 0) {
        IS_GLOBAL = GLOBAL;
        (*crt_arg)++;
        return 1;
    }
    return 0;
}