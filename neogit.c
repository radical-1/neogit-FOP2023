#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <fnmatch.h>
typedef char* string;
#define make_string(n) (string)calloc(n, sizeof(char))
#define remove_string(str) free(str)
#define GLOBAL 1
#define LOCAL 0
int IS_GLOBAL = LOCAL;
int check_is_global(char*, int*);
// random number for hash
long long giveRandomNumber() {
    // Initialize random seed
    srand(time(NULL));
    // Generate a random number and assign it to the integer
    return (rand() % (100000000 - 10000000 + 1)) + 10000000;
}
// finding a file in a folder
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
// check if neogit is initialized already or not
string IS_INITED() {
    string neogit = make_string(300);
    getcwd(neogit, 300);
    int length = strlen(neogit);
    for (int i = length; i >= 0; i--) {
        if (neogit[i] == '/') {
            if (find_file(neogit, ".neogit")) {
                return neogit;
            }
            neogit[i] = '\0';
        }
    }
    return NULL;
}
string WilCard_check(string address, string pattern) 
{
    DIR *dir;
    struct dirent *entry;
    int result;

    dir = opendir(address);

    if (dir != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            result = fnmatch(pattern, entry->d_name, 0);

        if (result == 0) {
            printf("Found file: %s\n", entry->d_name);
            }
        }
    }
    
}
int check_is_dir(string name) 
{
    DIR* dir = opendir(name);
    /* If the directory opened successfully, then it's a directory. */
    if (dir != NULL) {
        return 1;
    }
    return 0;
}
// base setting
void information(char* input[])
{
    if (IS_GLOBAL == LOCAL) {
        if (!IS_INITED()) {
            printf(".neogit not found in current working directory. Please run `neogit init` first\n");
            return;
        }
    }
    int MOD;
    int EMAIL = 1;
    int NAME = 0;
    if (strcmp(input[0], "user.name") == 0)
        MOD = NAME;
    else if (strcmp(input[0], "user.email") == 0)
        MOD = EMAIL;
    else {
        printf("INVALID INPUTS!\n");
        return;
    }
    if (IS_GLOBAL == GLOBAL) {
        FILE* GLOB = fopen("GLOBAL_info.txt", "r+");
        if (MOD == NAME) {
            fprintf(GLOB, "user.name = %s", input[1]);
        }
        else if (MOD == EMAIL) {
            char ch[200];
            fgets(ch, 200, GLOB);
            fprintf(GLOB, "user.email = %s", input[1]);
        }
        fclose(GLOB);
    }
    char local_info[500];
    sprintf(local_info, "%s/.neogit/LOCAL_info.txt", IS_INITED());
    FILE* LOC  = fopen(local_info, "r+");
    if (MOD == NAME) {
            fprintf(LOC, "user.name = %s", input[1]);
        }
        else if (MOD == EMAIL) {
            char ch[200];
            fgets(ch, 200, LOC);
            fprintf(LOC, "user.email = %s", input[1]);
        }
        fclose(LOC);
}
void ALIAS(char* input[])
{
    printf ("%s = %s\n", input[0], input[1]);
    if(IS_GLOBAL == LOCAL) {
        if (IS_INITED() == NULL) {
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
    if (IS_INITED()) {
        char local_alias[300];
        sprintf(local_alias, "%s/.neogit/LOCAL_alias.txt", IS_INITED());
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
    char unstage[200];
    make_branch("master", neogitDir);
    sprintf(repository,"%s/.LOCAL_REPOSITORY",neogitDir);
    sprintf(stage,"%s/.STAGING_AREA",neogitDir);
    sprintf(commit,"%s/.COMMITS",neogitDir);
    sprintf(unstage, "%s/.UNSTAGED", neogitDir);
    mkdir(commit, 0775);
    mkdir(repository, 0775);
    mkdir(stage, 0775);
    mkdir(unstage, 0775);
    // using slash for unix path
}
void init() {
    char cwd[200];
    getcwd(cwd, sizeof(cwd));
    if (IS_INITED()) {
        printf("noegit is already initialized in this folder!\n");
        return;
    }
    string neoGitDir = make_string(1000);
    sprintf(neoGitDir, "%s/.neogit", cwd);
    mkdir(neoGitDir, 0775);
    Open_dirctories_for_init(neoGitDir);
    char local_info[500];
    sprintf(local_info, "%s/.neogit/LOCAL_info.txt", IS_INITED());
    FILE* LOCAL_INFO = fopen(local_info, "w");
    char add_info[500];
    sprintf(add_info, "%s/.neogit/.ADD_INFO", IS_INITED());
    FILE* ADD_INFO = fopen(add_info, "w");
    fprintf(LOCAL_INFO, "user.name =\nuser.email =\n");
    fclose(LOCAL_INFO);
    fclose(ADD_INFO);
    printf("Initialized empty Git repository in %s\n", neoGitDir);
}

void ADD_TXT(char sourceAddress[], char destinationAddress[])
{
    FILE *source, *destination;
    char buffer[1024];
    size_t bytes_read;

    source = fopen(sourceAddress, "r");
    if (source == NULL) {
        printf("Error: Could not open source file\n");
        exit(1);
    }

    destination = fopen(destinationAddress, "w");
    if (destination == NULL) {
        printf("Error: Could not open destination file\n");
        fclose(source);
        exit(1);
    }

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        fwrite(buffer, 1, bytes_read, destination);
    }

    fclose(source);
    fclose(destination);

    printf("File copied successfully\n");
}
void ADD_PLUS(const char* source, const char* destination) 
{
    DIR *dir = opendir(source);
    if (dir == NULL) {
        perror("Unable to open the directory");
        return;
    }

    mkdir(destination, 0777);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char source_path[1024];
        char destination_path[1024];

        snprintf(source_path, sizeof(source_path), "%s/%s", source, entry->d_name);
        snprintf(destination_path, sizeof(destination_path), "%s/%s", destination, entry->d_name);

        if (entry->d_type == DT_DIR) {
            ADD_PLUS(source_path, destination_path);
        } else {
            
            ADD_TXT(source_path, destination_path);
        }
    }

    closedir(dir);

}
void ADD_FUNC(string input)
{
    char cwd[200];
    getcwd(cwd, 200);
    if (find_file(cwd, input) == 0) {
        printf("No such a file or directory!\n");
        return;
    }
    if (check_is_dir(input)) {
        char adrs[500];
        sprintf(adrs, "%s/%s", cwd, input);
        char dest[500];
        sprintf(dest, "%s/.neogit/.STAGING_AREA/%s", IS_INITED(), input);
        ADD_PLUS(adrs, dest);
    } 
    else {
        char source[300];
        char destination[300];
        sprintf(source , "%s", input);
        sprintf(destination, "%s/.neogit/.STAGING_AREA/%s", IS_INITED(), input);
        ADD_TXT(source, destination);
    }
}

void show_in_add(int depth, int cur, string source, string destination)
{
    if(depth == cur) return;
    DIR *dir = opendir(source);
    if (dir == NULL) {
        perror("Unable to open the directory");
        return;
    }

    DIR* dir_dest = opendir(destination);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".neogit") == 0) {
            continue;
        }

        char source_path[1024];
        char destination_path[1024];

        snprintf(source_path, sizeof(source_path), "%s/%s", source, entry->d_name);
        snprintf(destination_path, sizeof(destination_path), "%s/%s", destination, entry->d_name);

        if (entry->d_type == DT_DIR) {
            if(find_file(destination, entry->d_name) == 1) {
                printf("%s -> STAGED\n", entry->d_name);
                show_in_add(depth , cur + 1, source_path, destination_path);
            } else {
                printf("%s -> UNSTAGED\n", entry->d_name);
                show_in_add(depth, cur + 1, source_path, destination);
            }
        } else {
            if(find_file(destination, entry->d_name) == 1)
                printf("%s -> STAGED\n", entry->d_name);
            else
                printf("%s -> UNSTAGED\n", entry->d_name);
            
        }
    }

    closedir(dir);
}
//reset part
void RESET_FUNC(string input)
{
    string path = make_string(500);
    sprintf(path, "%s/.neogit/.STAGING_AREA", IS_INITED());
    if(find_file(path, input) == 0) {
        printf("there is no such a file(or directory) in staging area!\n");
        return;
    } remove_string(path);
    if (check_is_dir(input)) {
        char adrs[500];
        sprintf(adrs, "%s/.neogit/.STAGING_AREA/%s", IS_INITED(), input);
        char dest[500];
        sprintf(dest, "%s/.neogit/.UNSTAGED/%s", IS_INITED(), input);
        ADD_PLUS(adrs, dest);
    } 
    else {
        char source[300];
        char destination[300];
        sprintf(source , "%s/.neogit/.STAGING_AREA/%s", IS_INITED(), input);
        sprintf(destination, "%s/.neogit/.UNSTAGED/%s", IS_INITED(), input);
        ADD_TXT(source, destination);
    }

    printf("File successfully moved.\n");
}

void REDO_FUNC()
{
    struct dirent *stage;
    string name = make_string(500);
    sprintf(name, "%s/.neogit/.UNSTAGED", IS_INITED());
    DIR* dp = opendir(name);
    while((stage = readdir(dp)) != NULL) {
        if(strcmp(stage->d_name, ".") == 0 || strcmp(stage->d_name, "..") == 0) continue;
        if (check_is_dir(stage->d_name)) {
            char adrs[500];
            sprintf(adrs, "%s/.neogit/.UNSTAGED/%s", IS_INITED(), stage->d_name);
            char dest[500];
            sprintf(dest, "%s/.neogit/.STAGING_AREA/%s", IS_INITED(), stage->d_name);
            ADD_PLUS(adrs, dest);
        } 
        else {
            char source[300];
            char destination[300];
            sprintf(source , "%s/.neogit/.UNSTAGED/%s", IS_INITED(), stage->d_name);
            sprintf(destination, "%s/.neogit/.STAGING_AREA/%s", IS_INITED(), stage->d_name);
            ADD_TXT(source, destination);
        }
        char command[500];
        sprintf(command, "rm -r %s/.neogit/.UNSTAGED/%s", IS_INITED(), stage->d_name);
        system(command);
    }

}

void UNDO_FUNC()
{
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char fileAddress[500];
    sprintf(fileAddress, "%s/.neogit/.ADD_INFO", IS_INITED());
    file = fopen(fileAddress, "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Move the file pointer to the end of the file
    fseek(file, 0, SEEK_END);

    // Get the size of the file
    long fileSize = ftell(file);

    // Move the file pointer to the second-to-last newline character
    fseek(file, -2 * sizeof(char), SEEK_CUR);

    // Read the last line of the file
    while (fgetc(file) != '\n') {
        if (feof(file)) {
            break;
        }
        fseek(file, -2 * sizeof(char), SEEK_CUR);
    }

    // Read the last line of the file
    read = getline(&line, &len, file);
    if (read == -1) {
        perror("getline");
        exit(EXIT_FAILURE);
    }
    int point = 0;
    while (point < strlen(line) - 2) {
        char word[500];
        sscanf(line + point, "%s", word);
        point += strlen(word);
        RESET_FUNC(word);
        char command[550];
        sprintf(command, "rm -r %s/.neogit/.STAGING_AREA/%s", IS_INITED(), word);
        system(command);
    }
    free(line);
    rewind(file);
    char input[300];
    char total[2000];
    int index = 0;
    while(fgets(input, 300, file) != NULL) {
        sprintf(total + index, "%s", input);
        index += strlen(input);
    }
    for (int i = strlen(total) - 2;;  i--) {
        if(total[i] == '\n') {
            total[i + 1] = '\0';
            break;
        }
    }
    // Close the file
    fclose(file);
    printf("%s", total);
    FILE* new_file = fopen(fileAddress, "w");
    fputs(total, new_file);
    fclose(new_file);
}

int main(int argc, char *argv[])
{    
    int crt_arg = 1; //current argument
    if (strcmp(argv[crt_arg], "init") == 0) {
        init();
        return 0;
    }
    else if (strcmp(argv[crt_arg], "config") == 0) {
        crt_arg++;
        check_is_global(argv[crt_arg], &crt_arg);
        if (strncmp(argv[crt_arg], "alias", 5) == 0) {
            ALIAS(&argv[crt_arg]);
        }
        else information(&argv[crt_arg]);
        return 0;
    }
    if(IS_INITED() == NULL) {
        printf("please initilize neogit first!\n");
        return 0;
    }
    if(strcmp(argv[crt_arg], "add") == 0) {
        crt_arg++;
        char addfile[500];
        sprintf(addfile, "%s/.neogit/.ADD_INFO", IS_INITED());
        FILE* add = fopen(addfile, "a");
        if(strcmp(argv[crt_arg], "-f") == 0) {
            for (int i = crt_arg + 1; i < argc; i++) {
                ADD_FUNC(argv[i]);
                fprintf(add, "%s ", argv[i]);
            }
            fprintf(add, "\n");
        }
        else if(strcmp(argv[crt_arg], "-n") == 0) {
            crt_arg++;
            int depth;
            sscanf(argv[crt_arg], "%d", &depth);
            char cwd[500];
            getcwd(cwd, 500);
            char stage[500];
            sprintf(stage, "%s/.neogit/.STAGING_AREA", IS_INITED());
            show_in_add(depth, 0, cwd, stage);
        }
        else if(strcmp(argv[crt_arg], "-redo") == 0) {
            //fill this part after completing reset part
            REDO_FUNC();
        }
        else {
            ADD_FUNC(argv[crt_arg]);
            fprintf(add, "%s\n", argv[crt_arg]);
        }
        fclose(add);

    }
    else if (strcmp(argv[crt_arg], "reset") == 0) {
        crt_arg++;
        if(strcmp(argv[crt_arg], "-undo") == 0) {
            UNDO_FUNC();
        }
        else if(strcmp(argv[crt_arg], "-f") == 0) {
            for (int i = crt_arg + 1; i < argc; i++) {
                RESET_FUNC(argv[i]);
                char command[200];
                sprintf(command, "rm -r %s/.neogit/.STAGING_AREA/%s", IS_INITED(), argv[i]);
                system(command);
            }
        } else {
            RESET_FUNC(argv[crt_arg]);
            char command[200];
            sprintf(command, "rm -r %s/.neogit/.STAGING_AREA/%s", IS_INITED(), argv[crt_arg]);
            system(command);
        }
    }
    else {
        string input = make_string(300);
    }
    printf("end of our project!\n");
    return 0;
}
int check_is_global(char argv[], int* crt_arg)
{
    if(strcmp(argv, "-global") == 0) {
        IS_GLOBAL = GLOBAL;
        (*crt_arg)++;
        return 1;
    }
    return 0;
}
