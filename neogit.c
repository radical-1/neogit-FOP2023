#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <time.h>
#include <fnmatch.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>
#include <libavformat/avformat.h>


#ifndef __USE_MISC
#define __USE_MISC /* for dirfd() */
#endif

#ifndef DT_DIR
#define DT_DIR 4 // Replace 4 with the appropriate value for your system
#endif

//define for colors 
#define ANSI_RESET   "\x1b[0m"
#define ANSI_ACK   "\x1b[30m"
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_WHITE   "\x1b[37m"

#define ANSI_BOLD    "\x1b[1m"
#define ANSI_FAINT   "\x1b[2m"
#define ANSI_ITALIC  "\x1b[3m"
#define ANSI_UNDERLINE "\x1b[4m"
#define ANSI_BLINK   "\x1b[5m"
#define ANSI_INVERT  "\x1b[7m"

#define ANSI_BACK_BLACK   "\x1b[40m"
#define ANSI_BACK_RED     "\x1b[41m"
#define ANSI_BACK_GREEN   "\x1b[42m"
#define ANSI_BACK_YELLOW  "\x1b[43m"
#define ANSI_BACK_BLUE    "\x1b[44m"
#define ANSI_BACK_MAGENTA "\x1b[45m"
#define ANSI_BACK_CYAN    "\x1b[46m"
#define ANSI_BACK_WHITE   "\x1b[47m"


typedef char* string;
#define make_string(n) (string)calloc(n, sizeof(char))
#define remove_string(str) free(str)
typedef unsigned int Uint;
#define MAX_LENGTH_STRING 1024
#define MAX_LINE_LENGTH 1024
#define GLOBAL true
#define LOCAL false
#define MAX 10

struct Stack {
    int top;
    char items[MAX];
};

void make_empty_directories(string  src, string dest, string name); 
void base_empty_directories(string source, string destination);
void DECISION_FOR_COMMIT(string);
Uint find_head();
Uint find_commit(int);
string find_name();
string find_email();
string find_branch();

void remove_directory(const char* path) {
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r;

    struct dirent *ent;
    struct stat statbuf;

    if (!d) {
        perror("opendir");
        return;
    }

    while ((ent = readdir(d)) != NULL) {
        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) {
            continue;
        }

        char *new_path = (char*)malloc(path_len + strlen(ent->d_name) + 2);
        sprintf(new_path, "%s/%s", path, ent->d_name);

        if (!stat(new_path, &statbuf)) {
            if (S_ISDIR(statbuf.st_mode)) {
                remove_directory(new_path);
            } else {
                r = remove(new_path);
                if (r != 0) {
                    perror("remove");
                }
            }
        }

        free(new_path);
    }

    closedir(d);

    r = rmdir(path);
    if (r != 0) {
        perror("rmdir");
    }
}

string find_file_path(string bigger, string smaller) {
    return &bigger[strlen(smaller)];
}
const string CONFIG_ADDRESS()
{
    return "/home/radical-1/CONFIG";
}
// random number for hash
Uint giveRandomNumber() {
    // Initialize random seed
    srand(time(NULL));
    // Generate a random number and assign it to the integer
    return (rand() % (100000000 - 10000000 + 1)) + 10000000;
}
// finding a file in a folder
int find_file(const char *directory, const char *search_name) {
    struct stat st;

    char path[MAX_LENGTH_STRING];
    sprintf(path, "%s/%s", directory, search_name);

    if (stat(path, &st) == 0) {
        if (S_ISREG(st.st_mode)) {
           
            return 1;
        } else if (S_ISDIR(st.st_mode)) {
         
            return 1;
        } else {
        
        }
    } else {
        
        return 0;
    }
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


int is_text_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    char buffer[1024];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            if (!isprint(buffer[i]) && !isspace(buffer[i])) {
                fclose(file);
                return 0;
            }
        }
    }

    fclose(file);
    return 1;
}

bool compare_files(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "rb");
    FILE *f2 = fopen(file2, "rb");

    if (f1 == NULL || f2 == NULL) {
        if (f1 != NULL) fclose(f1);
        if (f2 != NULL) fclose(f2);
        return false;
    }

    char ch1, ch2;
    bool equal = true;
    int line = 1, pos = 1;

    while (equal) {
        ch1 = fgetc(f1);
        ch2 = fgetc(f2);
        if (ch1 == EOF && ch2 == EOF) break;
        if (ch1 == EOF || ch2 == EOF) {
            equal = false;
            break;
        }
        if (ch1 != ch2) {
            equal = false;
            break;
        }

        if (ch1 == '\n') {
            line++;
            pos = 1;
        } else {
            pos++;
        }
    }

    fclose(f1);
    fclose(f2);

    // if (equal) {
    //     printf("Files are identical. Line %d, position %d\n", line, pos);
    // } else {
    //     printf("Files differ. Line %d, position %d\n", line, pos);
    // }

    return equal;
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

string name_project()
{
    string name = make_string(MAX_LENGTH_STRING);
    string inited = IS_INITED();
    for(int i = strlen(inited) - 2; i >= 0; i--) {
        if(inited[i] == '/') {
            return &inited[i + 1];
            break;
        }
    }
}


void information(char model[], char input[], string project)
{
    
    int MOD;
    int EMAIL = 1;
    int NAME = 0;
    if (strcmp(model, "user.name") == 0)
        MOD = NAME;
    else if (strcmp(model, "user.email") == 0)
        MOD = EMAIL;
    else {
        printf("INVALID INPUTS!\n");
        return;
    }

    char info[MAX_LENGTH_STRING];
    char temp_file[MAX_LENGTH_STRING];
    sprintf(info, "%s/LOCAL_INFO/%s", CONFIG_ADDRESS(), project);
    sprintf(temp_file, "%s/LOCAL_INFO/temp.txt", CONFIG_ADDRESS());

    FILE* INFO = fopen(info, "r");
    if(INFO == NULL) {
        printf("%s\n", info);
        return;
    }
    FILE* TEMP = fopen(temp_file, "w");

    if (MOD == NAME) {
        for (int i = 0; i < 4; i++) {
            char line[MAX_LENGTH_STRING];
            fgets(line, MAX_LENGTH_STRING, INFO);
            if(i == 0) {
                char temp_line[MAX_LENGTH_STRING];
                sprintf(temp_line, "user.name : %s\n", input);
                fputs(temp_line, TEMP);
            } else {
                fputs(line, TEMP);
            }
        }
    }
    else if(MOD == EMAIL) {
        for (int i = 0; i < 4; i++) {
            char line[MAX_LENGTH_STRING];
            fgets(line, MAX_LENGTH_STRING, INFO);
            if(i == 1) {
                char temp_line[MAX_LENGTH_STRING];
                sprintf(temp_line, "user.email : %s\n", input);
                fputs(temp_line, TEMP);
            } else {
                fputs(line, TEMP);
            }
        }
    }

    fclose(INFO);
    fclose(TEMP);


    remove(info);
    rename(temp_file, info);

}

void global_information(char model[], char input[])
{
    char infoes[MAX_LENGTH_STRING];
    sprintf(infoes, "%s/LOCAL_INFO", CONFIG_ADDRESS());
    DIR* INFOES = opendir(infoes);

    struct dirent* entry;

    while((entry = readdir(INFOES)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        information(model, input, entry->d_name);
    }
}




void ALIAS(char input[], char command[], string project)
{
    char alias[MAX_LENGTH_STRING];
    sscanf(input, "alias.%s", alias);

    char new_alias[2 * MAX_LENGTH_STRING];
    sprintf(new_alias, "%s/LOCAL_ALIAS/%s/%s.txt", CONFIG_ADDRESS(), project, alias);

    FILE *NEW_ALIAS = fopen(new_alias, "w");

    fputs(command, NEW_ALIAS);

    fclose(NEW_ALIAS);  
}
void GLOBAL_ALIAS(char input[], char command[])
{
    char aliases[MAX_LENGTH_STRING];
    sprintf(aliases, "%s/LOCAL_ALIAS", CONFIG_ADDRESS());
    DIR* ALIASES = opendir(aliases);

    struct dirent* entry;

    while((entry = readdir(ALIASES)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        ALIAS(input, command, entry->d_name);
    }

    closedir(ALIASES);
}
void RUN_ALIAS(char command[])
{

    strcat(command, ".txt");
    char alias_address[MAX_LENGTH_STRING];
    sprintf(alias_address, "%s/LOCAL_ALIAS/%s", CONFIG_ADDRESS(), name_project());

    DIR*  DIRECTORY = opendir(alias_address);
    struct dirent* entry;

    while((entry = readdir(DIRECTORY)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        if(strcmp(entry->d_name , command) == 0) {
            char input_to_system[MAX_LENGTH_STRING];
            char alias_command[MAX_LENGTH_STRING];
            sprintf(alias_command, "%s/%s", alias_address, command);
            FILE* ALIAS_COMMAND = fopen(alias_command, "r");
            fscanf(ALIAS_COMMAND, "%[^\n]", input_to_system);
            system(input_to_system);
            fclose(ALIAS_COMMAND);
            return;
        }

    }
    perror("INVALID INPUTS!");
    return;
}



void make_branch(char branchName[], Uint head)
{
    char branchAddress[100];
    sprintf(branchAddress, "%s/.neogit/.BRANCHES/%s", IS_INITED(), branchName);
    mkdir(branchAddress, 0755);
    char head_commit[MAX_LENGTH_STRING];
    char commits[MAX_LENGTH_STRING];
    sprintf(head_commit, "%s/HEAD_COMMIT.txt", branchAddress);
    sprintf(commits, "%s/COMMITS", branchAddress);
    FILE* HEAD_COMMIT = fopen(head_commit, "w");
    char local_info[MAX_LENGTH_STRING];
    sprintf(local_info, "%s/LOCAL_INFO/%s.txt", CONFIG_ADDRESS(), name_project());

    Uint prev = 0;
  
    fprintf(HEAD_COMMIT, "HEAD COMMIT ID : %X", head);
    fclose(HEAD_COMMIT);
    mkdir(commits, 0755);
    char first_commit[MAX_LENGTH_STRING];
    sprintf(first_commit, "%s/%X.txt", commits, head);
    char tree[MAX_LENGTH_STRING];
    sprintf(tree, "%s/.neogit/.TREE.txt", IS_INITED());
    FILE* TREE = fopen(tree, "r");
    if (TREE) {
        char line[MAX_LENGTH_STRING];
        while(fgets(line, MAX_LENGTH_STRING, TREE) != NULL) {
            Uint temp_head;
            Uint temp_prev;
            sscanf(line, "prev_commit->%X commit_id->%X", &temp_prev, &temp_head);
            if(temp_head == head) {
                prev = temp_prev;
                break;
            }
        }
        fclose(TREE);
    }
    FILE* FIRST = fopen(first_commit, "w");
    fprintf(FIRST, "prev commit : %X\n", prev);
    fclose(FIRST);   
}


void Open_dirctories_for_init(char neogitDir[])  
{
    char repository[MAX_LENGTH_STRING];
    char stage[MAX_LENGTH_STRING];
    char unstage[MAX_LENGTH_STRING];
    char branches[MAX_LENGTH_STRING];
    char commit[MAX_LENGTH_STRING];
    char commit_info[MAX_LENGTH_STRING];
    char commit_set[MAX_LENGTH_STRING];
    char add_info[MAX_LENGTH_STRING];
    char alias[MAX_LENGTH_STRING];
    char repo_on_commit[MAX_LENGTH_STRING];
    char tags[MAX_LENGTH_STRING];
    char stashs[MAX_LENGTH_STRING];
    char applied_hooks[MAX_LENGTH_STRING];


    sprintf(repository,"%s/.LOCAL_REPOSITORY",neogitDir);
    sprintf(stage,"%s/.STAGING_AREA",neogitDir);
    sprintf(unstage, "%s/.UNSTAGED", neogitDir);
    sprintf(branches, "%s/.BRANCHES", neogitDir);
    sprintf(commit,"%s/.COMMITS",neogitDir);
    sprintf(commit_info, "%s/.COMMIT_INFO", neogitDir);
    sprintf(commit_set, "%s/.COMMIT_SET", neogitDir);
    sprintf(add_info, "%s/.ADD_INFO", neogitDir);
    sprintf(alias, "%s/LOCAL_ALIAS/%s", CONFIG_ADDRESS(), name_project());
    sprintf(repo_on_commit, "%s/.REPO_ON_COMMIT", neogitDir);
    sprintf(tags, "%s/.TAGS", neogitDir);
    sprintf(stashs, "%s/.STASHS", neogitDir);
    sprintf(applied_hooks, "%s/.HOOKS", neogitDir);


    mkdir(repository, 0755);
    mkdir(stage, 0755);
    mkdir(unstage, 0755);
    mkdir(branches, 0755);
    mkdir(commit, 0755);
    mkdir(commit_info, 0755);
    mkdir(commit_set, 0755);
    mkdir(add_info, 0755);
    mkdir(alias, 0755);
    mkdir(repo_on_commit, 0755);
    mkdir(tags, 0755);
    mkdir(stashs, 0755);
    mkdir(applied_hooks, 0755);

}
void init() {
    char cwd[200];
    getcwd(cwd, sizeof(cwd));
    if (IS_INITED()) {
        printf("noegit is already initialized in this folder!\n");
        return;
    }


    string neoGitDir = make_string(2 * MAX_LENGTH_STRING);
    sprintf(neoGitDir, "%s/.neogit", cwd);
    mkdir(neoGitDir, 0755);
    Open_dirctories_for_init(neoGitDir);


    char local_info[MAX_LENGTH_STRING];
    sprintf(local_info, "%s/LOCAL_INFO/%s.txt", CONFIG_ADDRESS(), name_project());
    FILE* LOCAL_INFO = fopen(local_info, "w");

    char global_info[MAX_LENGTH_STRING];
    sprintf(global_info, "%s/GLOBAL_info.txt",  CONFIG_ADDRESS());
    FILE* GLOBAL_INFO = fopen(global_info, "r");


    if (GLOBAL_INFO == NULL){
        printf("error opening");
        exit(1);
    }

    for(int i = 0; i < 4; i++) {
        char line[MAX_LENGTH_STRING];
        fgets(line, MAX_LENGTH_STRING, GLOBAL_INFO);
        fputs(line, LOCAL_INFO);
    }

    char is_okay_commit[MAX_LENGTH_STRING];
    sprintf(is_okay_commit, "%s/IS_OKAY_COMMIT.txt", neoGitDir);
    FILE* IS_OKAY_COMMIT = fopen(is_okay_commit, "w");
    fprintf(IS_OKAY_COMMIT, "YES");

    char is_okay_checkout[MAX_LENGTH_STRING];
    sprintf(is_okay_checkout, "%s/IS_OKAY_CHECKOUT.txt", neoGitDir);
    FILE* IS_OKAY_CHECKOUT = fopen(is_okay_checkout, "w");
    fprintf(IS_OKAY_CHECKOUT, "YES");
    fclose(GLOBAL_INFO);
    fclose(LOCAL_INFO);
    fclose(IS_OKAY_COMMIT);
    fclose(IS_OKAY_CHECKOUT);


    make_branch("master", 0);


    printf(ANSI_BACK_BLACK"Initialized empty Git repository in %s"ANSI_RESET"\n", neoGitDir);
}



void COPY_FILE(char src[], char dest[], char name[])
{
 

    char src_file[MAX_LENGTH_STRING];
    sprintf(src_file, "%s/%s", src, name);

    char dst_file[MAX_LENGTH_STRING];
    sprintf(dst_file, "%s/%s", dest, name);

    // Open the source file
    int src_fd = open(src_file, O_RDONLY);
    if (src_fd == -1) {
        perror("Error opening source file");
        return;
    }

    // Get the size of the source file
    struct stat src_stat;
    if (fstat(src_fd, &src_stat) == -1) {
        perror("Error getting source file size");
        close(src_fd);
        return;
    }

    // Open the destination file
    int dst_fd = open(dst_file, O_WRONLY | O_CREAT, src_stat.st_mode);
    if (dst_fd == -1) {
        perror("Error opening destination file");
        close(src_fd);
        return;
    }

    // Copy the source file to the destination file
    off_t offset = 0;
    ssize_t bytes_copied;
    while ((bytes_copied = sendfile(dst_fd, src_fd, &offset, src_stat.st_size)) > 0) {
        // Continue copying until all bytes are copied
    }

    // Check for errors during the copy operation
    if (bytes_copied == -1) {
        perror("Error copying file");
        close(src_fd);
        close(dst_fd);
        return;
    }

    // Close the file descriptors
    close(src_fd);
    close(dst_fd);

}
void COPY_DIR(string src, string dest, char name[]) 
{
    char source[MAX_LENGTH_STRING];
    sprintf(source, "%s/%s", src, name);

    char destination[MAX_LENGTH_STRING];
    sprintf(destination, "%s/%s", dest, name);

    DIR *directory = opendir(destination);
    if (directory == NULL) {
        mkdir(destination, 0755);
    }
    closedir(directory);

    DIR *dir = opendir(source);
    if (dir == NULL) {
        perror("failed to open");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".neogit") == 0) {
            continue;
        }

        if (entry->d_type == DT_DIR) {
            
            COPY_DIR(source, destination, entry->d_name);
        } else {
            
            COPY_FILE(source, destination, entry->d_name);
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
    char check[MAX_LENGTH_STRING];
    sprintf(check, "%s/%s", cwd, input);
    if (check_is_dir(check)) {
        char dest[MAX_LENGTH_STRING];
        sprintf(dest, "%s/.neogit/.STAGING_AREA%s", IS_INITED(), find_file_path(cwd, IS_INITED()));
        COPY_DIR(cwd, dest, input);
    } 
    else {
        char destination[MAX_LENGTH_STRING];
        sprintf(destination, "%s/.neogit/.STAGING_AREA%s", IS_INITED(), find_file_path(cwd, IS_INITED()));

        COPY_FILE(cwd, destination, input);
    }

    char is_okay_checkout[MAX_LENGTH_STRING];
    sprintf(is_okay_checkout, "%s/.neogit/IS_OKAY_CHECKOUT.txt", IS_INITED());
    FILE* IS_OKAY = fopen(is_okay_checkout, "w");
    fprintf(IS_OKAY, "NO\n");
    fclose(IS_OKAY);
}
void ADD_LIST(char list[][MAX_LENGTH_STRING], int number)
{
    char add_info[MAX_LENGTH_STRING];
    sprintf(add_info, "%s/.neogit/.ADD_INFO", IS_INITED());
    DIR* ADD_INFO = opendir(add_info);
    if (ADD_INFO == NULL) {
        perror("failed to open this folder!");
        return;
    }
    struct dirent* entry;
    int num_name = 0;
    while((entry = readdir(ADD_INFO)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        num_name++;
    }
    closedir(ADD_INFO);
    num_name++;
    char add_txt[2 * MAX_LENGTH_STRING];
    sprintf(add_txt, "%s/add%d.txt", add_info, num_name);
    FILE* ADD_TXT = fopen(add_txt, "w");
    if(!ADD_TXT) {perror("fopen failed in showCommitInfo()"); return;}
    for(int i = 0; i < number; i++) {
        fprintf(ADD_TXT, "%s\n", list[i]);
    }

    fclose(ADD_TXT);
}
void show_in_add(int depth, int cur, string source, string destination)
{
    
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
            printf(ANSI_BACK_BLACK"parent -> %s"ANSI_RESET"\n", entry->d_name);             
            show_in_add(depth , cur + 1, source_path, destination_path);
            
        } else {
            if(find_file(destination, entry->d_name) == 1)
                printf(ANSI_CYAN"file : %s -- status : STAGED"ANSI_RESET"\n", entry->d_name);
            else
                printf(ANSI_YELLOW"file : %s -- status : UNSTAGED"ANSI_RESET"\n", entry->d_name);
            
        }
    }

    closedir(dir);
}



void RESET_FUNC(char input[])
{

    char source[300];
    char destination[300];
    sprintf(source , "%s/.neogit/.STAGING_AREA", IS_INITED());
    sprintf(destination, "%s/.neogit/.UNSTAGED", IS_INITED());

    char cwd[MAX_LENGTH_STRING];
    getcwd(cwd, MAX_LENGTH_STRING);
    char check[MAX_LENGTH_STRING];
    sprintf(check, "%s/%s", source, input);


    if (check_is_dir(check)) {
        COPY_DIR(source, destination, input);
        char rm[MAX_LENGTH_STRING];
        sprintf(rm, "rm -rf %s/%s", source, input);
        system(rm);
    } 
    else {
        COPY_FILE(source, destination, input);
        char rm[MAX_LENGTH_STRING];
        sprintf(rm, "%s/%s", source, input);
        remove(rm);
    }

    printf("File successfully moved.\n");
}

void REDO_FUNC()
{
    char cwd[MAX_LENGTH_STRING];
    getcwd(cwd, MAX_LENGTH_STRING);

    struct dirent *entry;
    string unstage = make_string(500);
    sprintf(unstage, "%s/.neogit/.UNSTAGED", IS_INITED());
    DIR* dp = opendir(unstage);

    char stage[MAX_LENGTH_STRING];
    sprintf(stage, "%s/.neogit/.STAGING_AREA", IS_INITED());


    while((entry = readdir(dp)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        if (check_is_dir(entry->d_name)) {
            COPY_DIR(unstage, stage, entry->d_name);
            char command[500];
            sprintf(command, "rm -rf %s/.neogit/.UNSTAGED/%s", IS_INITED(), entry->d_name);
            system(command);
        } 
        else {
            COPY_FILE(unstage, stage, entry->d_name);
            char command[500];
            sprintf(command, "%s/.neogit/.UNSTAGED/%s", IS_INITED(), entry->d_name);
            remove(command);
        }
    }

}

void UNDO_FUNC()
{
    char add_info[MAX_LENGTH_STRING];
    sprintf(add_info, "%s/.neogit/.ADD_INFO", IS_INITED());
    DIR* ADD_INFO = opendir(add_info);
    if (ADD_INFO == NULL) {
        perror("failed to open this folder!");
        return;
    }
    

    struct dirent* entry;
    int num_name = 0;
    while((entry = readdir(ADD_INFO)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        num_name++;
    }
    closedir(ADD_INFO);


    char last_add[2 * MAX_LENGTH_STRING];
    sprintf(last_add, "%s/add%d.txt", add_info, num_name);

    FILE* LAST_ADD = fopen(last_add, "r");
    if(LAST_ADD == NULL) {perror("failed to open last add file!"); return;}
    char line[MAX_LENGTH_STRING];


    while(fgets(line, MAX_LENGTH_STRING, LAST_ADD) != NULL) {
        char input[MAX_LENGTH_STRING];
        sscanf(line, "%[^\n]", input);
        RESET_FUNC(input);
    }


    remove(last_add);
    printf("successfull undo!\n");

}




string attribute(string input)
{
    struct stat fileattrib;
    int fileMode;       
    string permissions = make_string(11);

    if (stat(input, &fileattrib) == 0) {
    fileMode = fileattrib.st_mode;

    permissions[0] = (S_ISDIR(fileMode) ? 'd' : '-');
    permissions[1] = (fileMode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (fileMode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (fileMode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (fileMode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (fileMode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (fileMode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (fileMode & S_IROTH) ? 'r' : '-';
    permissions[8] = (fileMode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (fileMode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';

    return permissions;
    }
}
void STATUS_FUNC_3(string staging, string unstage, string last_repo)
{
    DIR* REPO = opendir(last_repo);
    
    struct dirent* entry;

    while((entry = readdir(REPO)) != NULL) {
        if(strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) continue;

        if(entry->d_type == DT_DIR) {
            char new_stage[MAX_LENGTH_STRING];
            char new_repo[MAX_LENGTH_STRING];
            char new_unstage[MAX_LENGTH_STRING];

            sprintf(new_stage, "%s/%s", staging, entry->d_name);
            sprintf(new_repo, "%s/%s", last_repo, entry->d_name);
            sprintf(new_unstage, "%s/%s", unstage, entry->d_name);

            STATUS_FUNC_3(new_stage, new_unstage, new_repo);
        } else {
            if(!find_file(staging, entry->d_name) && !find_file(unstage, entry->d_name)) {
                printf(ANSI_RED"file name : %s --- status : -D"ANSI_RESET"\n", entry->d_name);
            }
        }
    }

    closedir(REPO);
}
void STATUS_FUNC_2(string unstage, string last_repo)
{
    DIR* UNSTAGE = opendir(unstage);

   struct dirent* entry;

   while((entry = readdir(UNSTAGE)) != NULL) {
    if(strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) continue;

    if(entry->d_type == DT_DIR) {
        char new_unstage[MAX_LENGTH_STRING];
        char new_repo[MAX_LENGTH_STRING];
        sprintf(new_unstage, "%s/%s", unstage, entry->d_name);
        sprintf(new_repo, "%s/%s", last_repo, entry->d_name);
        STATUS_FUNC_2(new_unstage, new_repo);
    } else {
        if(find_file(last_repo, entry->d_name)) {
            char file_in_unstage[MAX_LENGTH_STRING];
            char file_in_repo[MAX_LENGTH_STRING];
            sprintf(file_in_unstage, "%s/%s", unstage, entry->d_name);
            sprintf(file_in_repo, "%s/%s", last_repo, entry->d_name);

            if(strcmp(attribute(file_in_repo), attribute(file_in_unstage))) {
                printf(ANSI_BACK_BLUE"file name : %s --- status : -T"ANSI_RESET"\n", entry->d_name);
            }
            else if(compare_files(file_in_repo, file_in_unstage) == false) {
                printf(ANSI_YELLOW"file name : %s --- status : -M"ANSI_RESET"\n", entry->d_name);
            } 
        } else {
            printf(ANSI_GREEN"file name : %s --- status : -A"ANSI_RESET"\n", entry->d_name);
        }
    }
   }
   closedir(UNSTAGE);
}
void STATUS_FUNC_1(string staging, string last_repo)
{
   DIR* STAGING = opendir(staging);

   struct dirent* entry;

   while((entry = readdir(STAGING)) != NULL) {
    if(strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) continue;

    if(entry->d_type == DT_DIR) {
        char new_stage[MAX_LENGTH_STRING];
        char new_repo[MAX_LENGTH_STRING];
        sprintf(new_stage, "%s/%s", staging, entry->d_name);
        sprintf(new_repo, "%s/%s", last_repo, entry->d_name);
        STATUS_FUNC_1(new_stage, new_repo);
    } else {
        if(find_file(last_repo, entry->d_name)) {
            char file_in_stage[MAX_LENGTH_STRING];
            char file_in_repo[MAX_LENGTH_STRING];
            sprintf(file_in_stage, "%s/%s", staging, entry->d_name);
            sprintf(file_in_repo, "%s/%s", last_repo, entry->d_name);

            if(strcmp(attribute(file_in_repo), attribute(file_in_stage))) {
                printf(ANSI_BLUE"file name : %s --- status : +T"ANSI_RESET"\n", entry->d_name);
            }
            else if(compare_files(file_in_repo, file_in_stage) == false) {
                printf(ANSI_YELLOW"file name : %s --- status : +M"ANSI_RESET"\n", entry->d_name);
            } 
        } else {
            printf(ANSI_GREEN"file name : %s --- status : +A"ANSI_RESET"\n", entry->d_name);
        }
    }

   }
   closedir(STAGING);
}
void RUN_STATUS()
{
    char last_repo[MAX_LENGTH_STRING];
    char stage[MAX_LENGTH_STRING];
    char unstage[MAX_LENGTH_STRING];

    sprintf(last_repo, "%s/.neogit/.REPO_ON_COMMIT/%X", IS_INITED(), find_head());
    sprintf(stage, "%s/.neogit/.STAGING_AREA", IS_INITED());
    sprintf(unstage, "%s/.neogit/.UNSTAGED", IS_INITED());

    STATUS_FUNC_1(stage, last_repo);
    STATUS_FUNC_2(unstage, last_repo);
    STATUS_FUNC_3(stage, unstage, last_repo);

}



void PUT_COMMIT_INFORMATION(string Time, string Message, string AothurName, string AothurEmail, string branch, Uint commit_id, int num_commited)
{
    char commit_info[MAX_LENGTH_STRING];
    sprintf(commit_info, "%s/.neogit/.COMMIT_INFO", IS_INITED());
    DIR* COMMIT_INFO = opendir(commit_info);
    if (COMMIT_INFO == NULL) {
        perror("failed to open this folder!");
        return;
    }
    struct dirent* entry;
    int num_name = 0;
    while((entry = readdir(COMMIT_INFO)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        num_name++;
    }
    closedir(COMMIT_INFO);
    num_name++;
    char commit_txt[2 * MAX_LENGTH_STRING];
    sprintf(commit_txt, "%s/commit%d.txt", commit_info, num_name);
    FILE* COMMIT_TXT = fopen(commit_txt, "w");
    if(!COMMIT_TXT) {perror("fopen failed in showCommitInfo()"); return;}


    fprintf(COMMIT_TXT, "Date & Time : %s\n", Time);
    fprintf(COMMIT_TXT, "Commit Message : %s\n", Message);
    fprintf(COMMIT_TXT, "Author Name : %s\n", AothurName);
    fprintf(COMMIT_TXT, "Author Email : %s\n", AothurEmail);
    fprintf(COMMIT_TXT, "Branch : %s\n", branch);
    fprintf(COMMIT_TXT, "COMMIT ID(HASH) : %X\n", commit_id);
    fprintf(COMMIT_TXT, "Number of Files : %d\n", num_commited);

    fclose(COMMIT_TXT);
    
}
void make_repo_on_commit(Uint commit_id)
{
    char repo_path[MAX_LENGTH_STRING];
    char repo_on_commit_path[MAX_LENGTH_STRING];

    sprintf(repo_path, "%s/.neogit/.LOCAL_REPOSITORY", IS_INITED());
    sprintf(repo_on_commit_path, "%s/.neogit/.REPO_ON_COMMIT/%X", IS_INITED(),  commit_id);

    DIR* MAIN_REPO = opendir(repo_path);
    mkdir(repo_on_commit_path, 0755);

    struct dirent* entry;

    while((entry = readdir(MAIN_REPO)) != NULL) {
        if (strcmp(entry->d_name , ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        if(entry->d_type == DT_DIR) COPY_DIR(repo_path, repo_on_commit_path, entry->d_name);
        else COPY_FILE(repo_path, repo_on_commit_path, entry->d_name);
    } 
    closedir(MAIN_REPO);
}
void number_of_staging(string stage, int* last_num)
{
    DIR* STAGE = opendir(stage);

    struct dirent* entry;
    
    while((entry = readdir(STAGE)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        if(entry->d_type == DT_DIR) {
            char new_stage[MAX_LENGTH_STRING];
            sprintf(new_stage, "%s/%s", stage, entry->d_name);
            number_of_staging(new_stage, last_num);
        } else {
            (*last_num)++;
        }
    }
    closedir(STAGE);
}
void make_commit(Uint prev_commit, string branch, string commit_message, string aothurName, string aothurEmail)
{

    Uint commit_id = giveRandomNumber();
    time_t current_time;
    struct tm *time_info;
    char cur_time[80];
    current_time = time(NULL);
    time_info = localtime(&current_time);
    strftime(cur_time, sizeof(cur_time), "%Y-%m-%d %H:%M:%S", time_info);

    int num_stage = 0;
    char stage_address[MAX_LENGTH_STRING];
    sprintf(stage_address, "%s/.neogit/.STAGING_AREA", IS_INITED());
    number_of_staging(stage_address, &num_stage);

    PUT_COMMIT_INFORMATION(cur_time, commit_message, aothurName, aothurEmail, branch, commit_id, num_stage);
    
    char commit_tree[MAX_LENGTH_STRING];
    sprintf(commit_tree, "%s/.neogit/.TREE.txt", IS_INITED());
    FILE* TREE = fopen(commit_tree, "a");
    fprintf(TREE ,"prev_commit->%X commit_id->%X branch->%s\n", prev_commit, commit_id, branch);

    char new_commit[MAX_LENGTH_STRING];
    sprintf(new_commit, "%s/.neogit/.COMMITS/%X", IS_INITED(), commit_id);
    mkdir(new_commit, 0755);

    char repo_address[MAX_LENGTH_STRING];
    sprintf(repo_address, "%s/.neogit/.LOCAL_REPOSITORY", IS_INITED());
    DIR* STAGE = opendir(stage_address);
    struct dirent* stg;
    while((stg = readdir(STAGE)) != NULL) {
        if (strcmp(stg->d_name, ".") == 0 || strcmp(stg->d_name, "..") == 0) continue;
        char source[2 * MAX_LENGTH_STRING];
        char destination[2 * MAX_LENGTH_STRING];
        char repo_dest[2 * MAX_LENGTH_STRING];

        sprintf(source, "%s/%s", stage_address, stg->d_name);
        sprintf(destination, "%s/%s", new_commit, stg->d_name);
        sprintf(repo_dest, "%s/%s", repo_address, stg->d_name);
        if(stg->d_type == DT_DIR) {
            COPY_DIR(stage_address, repo_address, stg->d_name);
            char command[MAX_LENGTH_STRING];
            sprintf(command, "%s/%s", stage_address, stg->d_name);
            remove_directory(command);
        } else {
            COPY_FILE(stage_address, repo_address, stg->d_name);
            remove(source);
        }
        
   
    }
    char local_info[MAX_LENGTH_STRING];
    sprintf(local_info, "%s/LOCAL_INFO/%s.txt", CONFIG_ADDRESS(), name_project());
    FILE* INFO = fopen(local_info, "r+");
    
    char line[MAX_LENGTH_STRING];
    for(int i = 0;  i < 3; i++) {
        fgets(line, MAX_LENGTH_STRING, INFO);
    }
    char output[MAX_LENGTH_STRING];
    sprintf(output, "commit_id : %X", commit_id);
    fputs(output, INFO);
    fclose(INFO);
    fclose(TREE);
    char branch_commit[MAX_LENGTH_STRING];
    sprintf(branch_commit, "%s/.neogit/.BRANCHES/%s/COMMITS/%X.txt", IS_INITED(), branch, commit_id);
    FILE* COMMIT = fopen(branch_commit, "w");
    fprintf(COMMIT, "prev commit : %X\n", prev_commit);
    char head_commit[MAX_LENGTH_STRING];
    sprintf(head_commit, "%s/.neogit/.BRANCHES/%s/HEAD_COMMIT.txt", IS_INITED(), branch);

    FILE* HEAD = fopen(head_commit, "w");
    
    fprintf(HEAD, "HEAD COMMIT ID : %X\n", commit_id);
    fclose(COMMIT);
    fclose(HEAD);
    make_repo_on_commit(commit_id);
    printf("your commit was successfull!\nCommit Date & Time : %s\nCommit ID : %X\nCommit Message : %s\n", cur_time, commit_id, commit_message);    
}
void COMMIT_FUNC(string message)
{
    char is_okay[MAX_LENGTH_STRING];
    sprintf(is_okay, "%s/.neogit/IS_OKAY_COMMIT.txt", IS_INITED());
    FILE* IS_OKAY = fopen(is_okay, "r");
    char result[20];
    fscanf(IS_OKAY, "%s", result);
    if(strcmp(result, "NO") == 0) {
        printf("you can't commit know\nplease go back to head and then commit!\n");
        fclose(IS_OKAY);
        return;
    }
    fclose(IS_OKAY);


    char infoAddress[MAX_LENGTH_STRING];
    sprintf(infoAddress, "%s/LOCAL_INFO/%s.txt", CONFIG_ADDRESS(), name_project());
    FILE* info = fopen(infoAddress, "r");

    string line = make_string(MAX_LENGTH_STRING);
    char name[MAX_LENGTH_STRING];
    char email[MAX_LENGTH_STRING];
    char cur_branch[MAX_LENGTH_STRING];
    Uint prev_commit;

    fgets(line, MAX_LENGTH_STRING, info);
    sscanf(line, "user.name : %[^\n]", name);

    fgets(line, MAX_LENGTH_STRING, info);
    sscanf(line, "user.email : %[^\n]", email);

    fgets(line , MAX_LENGTH_STRING, info);
    sscanf (line , "branch : %[^\n]", cur_branch);

    fgets(line, MAX_LENGTH_STRING, info);
    sscanf(line, "commit_id : %X\n", &prev_commit);

    fclose(info);

    make_commit(prev_commit, cur_branch, message, name, email);

    char is_okay_checkout[MAX_LENGTH_STRING];
    sprintf(is_okay_checkout, "%s/.neogit/IS_OKAY_CHECKOUT.txt", IS_INITED());
    FILE* IS_OKAY_CHECKOUT = fopen(is_okay_checkout, "w");
    fprintf(IS_OKAY_CHECKOUT, "YES\n");
    fclose(IS_OKAY_CHECKOUT);
}



void SET_FUNC(string message, string shortcut) 
{
    char shrtct_path[MAX_LENGTH_STRING];
    sprintf(shrtct_path, "%s/.neogit/.COMMIT_SET/%s.txt", IS_INITED(), shortcut);
    FILE* SHORTCUT = fopen(shrtct_path, "w");
    fputs(message, SHORTCUT);
    fclose(SHORTCUT);
}
void REMOVE_SHORTCUT(string shorutcut)
{
    int ret;
    FILE *fp;
    char filename[MAX_LENGTH_STRING];
    sprintf(filename, "%s/.neogit/.COMMIT_SET/%s.txt", IS_INITED(), shorutcut);
   
    ret = remove(filename);

    if(ret == 0) {
        printf("Shortcut Message deleted successfully!\n");
    } else {
        printf("Error: unable to delete the Shortcut Message!\n");
    }
}
string find_message(string shortucut)
{
    char fileName[MAX_LENGTH_STRING];
    sprintf(fileName, "%s/.neogit/.COMMIT_SET/%s.txt", IS_INITED(), shortucut);
    FILE *filePointer=fopen(fileName,"r");
    string Message = make_string(MAX_LENGTH_STRING);
    fscanf(filePointer, "%[^\n]", Message);
    fclose(filePointer);
    return Message;
    
}



void PRITNT_BRANCHES()
{
    char branch[MAX_LENGTH_STRING];
    sprintf(branch, "%s/.neogit/.BRANCHES", IS_INITED());
    DIR* BRANCHES = opendir(branch);
    struct dirent *entry;
    while ((entry = readdir(BRANCHES)) != NULL){
        if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) continue;

        printf("branch name : "ANSI_BACK_MAGENTA"%s"ANSI_RESET"\n", entry->d_name);
    }
}
void COPY_COMMIT(string source , string destination, string name)
{
    if(find_file(destination, name) == 1) {
        COPY_FILE(source, destination, name);
        return;
    }
    else {
        DIR* DEST = opendir(destination);
        struct dirent* entry;

        while((entry = readdir(DEST)) != NULL) {
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".neogit") == 0) continue;

            if(entry->d_type == DT_DIR) {
                char new_destination[MAX_LENGTH_STRING];
                sprintf(new_destination, "%s/%s", destination, entry->d_name);
                COPY_COMMIT(source, new_destination, name);
            }
        }
    }
}
void change_folder(Uint commit_id)
{
    char source[MAX_LENGTH_STRING];
    sprintf(source, "%s/.neogit/.REPO_ON_COMMIT/%X", IS_INITED(), commit_id);
    DIR* COMMIT = opendir(source);
    if(COMMIT == NULL) {
        return;
    }
    struct dirent* entry;

    while((entry = readdir(COMMIT)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        if(entry->d_type == DT_DIR) {
            COPY_DIR(source, IS_INITED(), entry->d_name);
        } else {
            COPY_FILE(source, IS_INITED(), entry->d_name);
        }
    }

}
bool EVERYTHING_IS_COMMITED()
{
    char is_okay_checkout[MAX_LENGTH_STRING];
    sprintf(is_okay_checkout, "%s/.neogit/IS_OKAY_CHECKOUT.txt", IS_INITED());
    FILE* IS_OKAY = fopen(is_okay_checkout, "r");
    char condition[20];
    fscanf(IS_OKAY, "%s\n", condition);
    fclose(IS_OKAY);

    if (strcmp(condition, "YES") == 0) return true;
    else return false;
    
}
void BRANCH_CHECKOUT(string branch)
{
    char info[MAX_LENGTH_STRING];
    char temp_info[MAX_LENGTH_STRING];

    sprintf(info, "%s/LOCAL_INFO/%s.txt", CONFIG_ADDRESS(), name_project());
    sprintf(temp_info, "%s/LOCAL_INFO/temp.txt", CONFIG_ADDRESS());


    FILE* INFO = fopen(info, "r");
    FILE* TMP_INFO = fopen(temp_info, "w");
    if(INFO == NULL) {
        printf("%s\n", info);
        return;
    }
    for(int i = 0; i < 4; i++) {
        char temp[MAX_LENGTH_STRING];
        fgets(temp, MAX_LENGTH_STRING, INFO);
        if(i == 2) {
            sprintf(temp, "branch : %s\n", branch);
        }
        fputs(temp, TMP_INFO);
    }

    fclose(INFO);
    fclose(TMP_INFO);
    remove(info);
    rename(temp_info, info);


    char head[MAX_LENGTH_STRING];
    sprintf(head, "%s/.neogit/.BRANCHES/%s/HEAD_COMMIT.txt", IS_INITED(), branch);
    FILE* HEAD = fopen(head, "r");
  
    Uint head_id;
    fscanf(HEAD, "HEAD COMMIT ID : %X\n", &head_id);

    fclose(HEAD);
    change_folder(head_id);

}
void COMMIT_CHECKOUT(Uint id)
{   
    char is_okay[MAX_LENGTH_STRING];
    sprintf(is_okay, "%s/.neogit/IS_OKAY_COMMIT.txt", IS_INITED());
    FILE* IS_OKAY_COMMIT = fopen(is_okay, "w");
    fprintf(IS_OKAY_COMMIT, "NO");
    fclose(IS_OKAY_COMMIT);
    change_folder(id);
}
void CHECKOUT(string input)
{
    if(!EVERYTHING_IS_COMMITED()) {
        perror("please first commit then checkout!");
        exit(1);
    }
    char branch_dest[MAX_LENGTH_STRING];
    sprintf(branch_dest, "%s/.neogit/.BRANCHES", IS_INITED());

    char commit_dest[MAX_LENGTH_STRING];
    sprintf(commit_dest, "%s/.neogit/.REPO_ON_COMMIT", IS_INITED());
   
    if(find_file(branch_dest, input)) {
        BRANCH_CHECKOUT(input);
    }
    else if(find_file(commit_dest, input)) {
        Uint id;
        sscanf(input, "%X", &id);
        COMMIT_CHECKOUT(id);
    }
    else {
        perror("Invalid inputs!\n");
    }
}



void PRINT_LOG(int num_commit, string address)
{
    
    FILE* fp = fopen(address, "r");
    printf("-------start-------\n");
    printf("commit %d :\n", num_commit);
    char line[MAX_LENGTH_STRING];
    while(fgets(line, MAX_LENGTH_STRING, fp) != NULL) {
        printf("%s", line);
    }
    fclose(fp);
    printf("-------end-------\n\n");
}
void LOG_FUNC(int num)
{
    char commit_info[MAX_LENGTH_STRING];
    sprintf(commit_info, "%s/.neogit/.COMMIT_INFO", IS_INITED());
    DIR* COMMIT_INFO = opendir(commit_info);
    struct dirent* entry;
    int num_commit = 0;
    while((entry = readdir(COMMIT_INFO)) != NULL) {
        if(strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) continue;
        num_commit++;
    }
    closedir(COMMIT_INFO);
    while(num_commit > 0 && num > 0) {
        char address[2 * MAX_LENGTH_STRING];
        sprintf(address, "%s/commit%d.txt", commit_info, num_commit);
        PRINT_LOG(num_commit, address);
        num_commit--;
        num--;
    }
}
void LOG_BRANCH(string branch)
{
    char commit_info[MAX_LENGTH_STRING];
    sprintf(commit_info, "%s/.neogit/.COMMIT_INFO", IS_INITED());
    DIR* COMMIT_INFO = opendir(commit_info);
    struct dirent* entry;
    int num_commit = 0;
    while((entry = readdir(COMMIT_INFO)) != NULL) {
        if(strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) continue;
        num_commit++;
    }
    closedir(COMMIT_INFO);
    while(num_commit > 0) {
        char address[2 * MAX_LENGTH_STRING];
        sprintf(address, "%s/commit%d.txt", commit_info, num_commit);
        FILE* file = fopen(address, "r");
        char line[MAX_LENGTH_STRING];
        for(int i = 0; i < 5; i++) {
            fgets(line, MAX_LENGTH_STRING, file);
        }
        char read_branch[100];
        sscanf(line , "Branch : %[^\n]", read_branch);
        if(strcmp(read_branch , branch) == 0) {
            PRINT_LOG(num_commit, address);
        }
        fclose(file);
        num_commit--;
    }

}
void LOG_AUTHOR(string author)
{
    char commit_info[MAX_LENGTH_STRING];
    sprintf(commit_info, "%s/.neogit/.COMMIT_INFO", IS_INITED());
    DIR* COMMIT_INFO = opendir(commit_info);
    struct dirent* entry;
    int num_commit = 0;
    while((entry = readdir(COMMIT_INFO)) != NULL) {
        if(strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) continue;
        num_commit++;
    }
    closedir(COMMIT_INFO);
    while(num_commit > 0) {
        char address[2 * MAX_LENGTH_STRING];
        sprintf(address, "%s/commit%d.txt", commit_info, num_commit);
        FILE* file = fopen(address, "r");
        char line[MAX_LENGTH_STRING];
        for(int i = 0; i < 3; i++) {
            fgets(line, MAX_LENGTH_STRING, file);
        }
        char read_name[100];
        sscanf(line , "Author Name : %[^\n]", read_name);
        if(strcmp(read_name , author) == 0) {
            PRINT_LOG(num_commit, address);
        }
        fclose(file);
        num_commit--;        
    }

}
void LOG_SEARCH(string word)
{
    char commit_info[MAX_LENGTH_STRING];
    sprintf(commit_info, "%s/.neogit/.COMMIT_INFO", IS_INITED());
    DIR* COMMIT_INFO = opendir(commit_info);
    struct dirent* entry;
    int num_commit = 0;
    while((entry = readdir(COMMIT_INFO)) != NULL) {
        if(strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) continue;
        num_commit++;
    }
    closedir(COMMIT_INFO);
    while(num_commit > 0) {
        char address[2 * MAX_LENGTH_STRING];
        sprintf(address, "%s/commit%d.txt", commit_info, num_commit);
        FILE* file = fopen(address, "r");
        char line[MAX_LENGTH_STRING];
        for(int i = 0; i < 2; i++) {
            fgets(line, MAX_LENGTH_STRING, file);
        }
        char read_mesage[MAX_LENGTH_STRING];
        sscanf(line , "Commit Message : %[^\n]", read_mesage);
        if(strstr(read_mesage, word)) {
            PRINT_LOG(num_commit, address);
        }
        fclose(file);
        num_commit--;        
    }
}
int compare_date_time_strings(const char *date_time1, const char *date_time2) {
    struct tm tm1, tm2;
    time_t t1, t2;

    // Parse the date and time strings into struct tm format
    strptime(date_time1, "%Y-%m-%d %H:%M:%S", &tm1);
    strptime(date_time2, "%Y-%m-%d %H:%M:%S", &tm2);

    // Convert the struct tm format into time_t format
    t1 = mktime(&tm1);
    t2 = mktime(&tm2);
    // Compare the time_t values
    if (t1 < t2) {
        return -1;
    } else if (t1 > t2) {
        return 1;
    } else {
        return 0;
    }
}
void LOG_TIME(string inputTime, int valid)
{
    char commit_info[MAX_LENGTH_STRING];
    sprintf(commit_info, "%s/.neogit/.COMMIT_INFO", IS_INITED());
    DIR* COMMIT_INFO = opendir(commit_info);
    struct dirent* entry;
    int num_commit = 0;
    while((entry = readdir(COMMIT_INFO)) != NULL) {
        if(strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) continue;
        num_commit++;
    }
    closedir(COMMIT_INFO);  

    while(num_commit > 0) {
        char address[2 * MAX_LENGTH_STRING];
        sprintf(address, "%s/commit%d.txt", commit_info, num_commit);
        struct stat file_stat;
        if (stat(address, &file_stat) == 1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        // Convert the creation time to a readable string
        char time_buf[256];
        strftime(time_buf, sizeof(time_buf), "%F %T", localtime(&file_stat.st_ctime));
        if(compare_date_time_strings(inputTime, time_buf) == valid) {
            PRINT_LOG(num_commit, address);
        }

       
        num_commit--;        
    }  
}


int compare(const void *a, const void *b) {
  struct dirent **da = (struct dirent **)a;
  struct dirent **db = (struct dirent **)b;
  return strcmp((*da)->d_name, (*db)->d_name);
}
void TAG_FUNC(string tag_name, string message, Uint commit_id, bool overwrite)
{
    char tags[MAX_LENGTH_STRING];
    sprintf(tags, "%s/.neogit/.TAGS", IS_INITED());

    char commits[MAX_LENGTH_STRING];
    sprintf(commits, "%s/.neogit/.COMMITS", IS_INITED());
    char this_commit[MAX_LENGTH_STRING];
    sprintf(this_commit, "%X", commit_id);
    if(!find_file(commits, this_commit)) {
        perror("this commit does not exist!!");
        return;
    }


    char check[MAX_LENGTH_STRING];
    sprintf(check, "%s.txt", tag_name);
    if(find_file(tags,  check) && !overwrite) {
        printf("%s already exists.\n", tag_name);
        return;
    }

    char new_tag[2 * MAX_LENGTH_STRING];
    sprintf(new_tag, "%s/%s.txt", tags, tag_name);
    FILE* NEW_TAG = fopen(new_tag, "w");

    time_t current_time;
    struct tm *time_info;
    char cur_time[80];
    current_time = time(NULL);
    time_info = localtime(&current_time);
    strftime(cur_time, sizeof(cur_time), "%Y-%m-%d %H:%M:%S", time_info);

    fprintf(NEW_TAG, "commit id : %X\n", commit_id);
    fprintf(NEW_TAG, "Author : %s   <%s>\n", find_name(), find_email());
    fprintf(NEW_TAG, "Date : %s\n", cur_time);
    if(message == NULL) 
        fprintf(NEW_TAG, "This tag has no message!\n");
    else
    fprintf(NEW_TAG, "Message : %s\n", message);

    fclose(NEW_TAG);

}
void TAG_ANALYZE(char* input[], int arguments)
{
    if(arguments < 0) {
        perror("few arguments!");
        return;
    }
    else if(arguments == 0) {
        TAG_FUNC(input[0], NULL, find_head(), false);
    }
    else if(arguments == 1) {
        if(strcmp(input[1], "-f")) {
            perror("invalid inputs!");
            return;
        }
        TAG_FUNC(input[0], NULL, find_head(), true);
    }
    else if(arguments == 2) {
        if(strcmp(input[1], "-m") == 0) {
            TAG_FUNC(input[0], input[2], find_head(), false);
        }
        else if(strcmp(input[1], "-c") == 0) {
            Uint commit_id;
            sscanf(input[2], "%X", &commit_id);
            TAG_FUNC(input[0], NULL, commit_id, false);
        } else {
            perror("invalid inputs!");
            return;
        }
    }
    else if(arguments == 3) {
        if(strcmp(input[3], "-f")) {
            perror("invalid inputs!");
            return;
        }
        if(strcmp(input[1], "-m") == 0) {
            TAG_FUNC(input[0], input[2], find_head(), true);
        }
        else if(strcmp(input[1], "-c") == 0) {
            Uint commit_id;
            sscanf(input[2], "%X", &commit_id);
            TAG_FUNC(input[0], NULL, commit_id, true);
        } else {
            perror("invalid inputs!");
            return;
        }
    }
    else if(arguments == 4) {
        if(strcmp(input[1], "-m") || strcmp(input[3], "-c")) {
            perror("invalid inputs!");
            return;
        }
        Uint commit_id;
        sscanf(input[4], "%X", &commit_id);
        TAG_FUNC(input[0], input[2], commit_id, false);
    }
    else if(arguments == 5) {
        if(strcmp(input[1], "-m") || strcmp(input[3], "-c") || strcmp(input[5], "-f")) {
            perror("invalid inputs!");
            return;
        }
        Uint commit_id;
        sscanf(input[4], "%X", &commit_id);
        TAG_FUNC(input[0], input[2], commit_id, true);
    } else {
        perror("to many argumnets!");
        return;
    }
}
void show_all_tags()
{

    char tags[MAX_LENGTH_STRING];
    sprintf(tags, "%s/.neogit/.TAGS", IS_INITED());

    DIR *dir = opendir(tags);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent **namelist;
    int n = scandir(tags, &namelist, NULL, compare);
    if (n == -1) {
        perror("scandir");
        return;
    }

    for (int i = 0; i < n; ++i) {
        if(strcmp(namelist[i]->d_name, ".") == 0|| strcmp(namelist[i]->d_name, "..") == 0) continue;
        char temp[100];
        strcpy(temp,  namelist[i]->d_name);
        temp[strlen(namelist[i]->d_name) - 4] = '\0';
        printf("%s\n", temp);
        free(namelist[i]);
    }
    free(namelist);
    closedir(dir);
}
void show_tag_information(string tag_name)
{
    char tags[MAX_LENGTH_STRING];
    sprintf(tags, "%s/.neogit/.TAGS", IS_INITED());

    char tag_text[MAX_LENGTH_STRING];
    sprintf(tag_text, "%s.txt", tag_name);

    if(!find_file(tags,  tag_text)) {
        perror("there is no tag with this tag-name!");
        return;
    }
    
    char tag_address[2 * MAX_LENGTH_STRING];
    sprintf(tag_address, "%s/%s", tags, tag_text);
    FILE* TAG = fopen(tag_address, "r");
    if(TAG == NULL) {
        perror("failed to open tag file!");
        return;
    }

    printf("\ntag : %s\n", tag_name);
    for(int i = 0; i < 4; i++) {
        char line[MAX_LENGTH_STRING];
        fgets(line, MAX_LENGTH_STRING, TAG);
        printf("%s", line);
    }

    fclose(TAG);
    

}


int isWordBoundary(char ch) {
    return ch == ' '||  ch == '\t' || ch == '\n' || ch == '\0' || ispunct((unsigned char)ch);
}

// This helper function checks if the found word is standalone
int isStandaloneWord(const char *word, const char *found, const char *line) {
    // Check for word boundary at the start of the word
    if (found != line && !isWordBoundary(*(found - 1))) {
        return 0;
    }
    // Check for word boundary at the end of the word
    if (!isWordBoundary(*(found + strlen(word)))) {
        return 0;
    }
    return 1; // The word is standalone
}

// Function to find the word in a line and print the line with the word highlighted
void printHighlightedWordInLine(const char *word, const char *line, bool condition, int number) {
    const char *found = strstr(line, word);
    while (found) {
        // Check if the found word is a standalone word
        if (isStandaloneWord(word, found, line)) {
            if(condition) printf("line number : "ANSI_BOLD ANSI_CYAN"%d"ANSI_RESET" ---> ", number);
            // Start of the line before the word
            const char *start = line;
            // Print each segment of line with the word occurrences highlighted
            do {
                printf("%.*s"ANSI_BOLD ANSI_YELLOW"%s"ANSI_RESET"", (int)(found - start), start, word);
                // Update start position
                start = found + strlen(word);
                found = strstr(start, word); // Look for the next occurrence
            } while (found && isStandaloneWord(word, found, line));
            // Print the remainder of the line
            printf("%s\n", start);
            return; // Word found and line is printed, so return from function
        }
        // Move to the next character to keep looking for the word
        found = strstr(found + 1, word);
    }
    // If the word is not found, do not print anything.
}


void GREP(string file, string word, Uint commit_id, bool number)
{
    char commits[MAX_LENGTH_STRING];
    sprintf(commits, "%s/.neogit/.COMMITS", IS_INITED());
    char check[MAX_LENGTH_STRING];
    sprintf(check, "%X", commit_id);
    if(!find_file(commits, check)) {
        perror("\033[1;31mthis commit id does not exist!\033[0m");
        return;
    }
    char cwd[MAX_LENGTH_STRING];
    getcwd(cwd, MAX_LENGTH_STRING);
    char path_in_repo[MAX_LENGTH_STRING];
    sprintf(path_in_repo, "%s/.neogit/.REPO_ON_COMMIT/%X%s/%s", IS_INITED(), commit_id, find_file_path(cwd, IS_INITED()), file);
    
    FILE* FILE_ON_COMMIT = fopen(path_in_repo, "r");
    if(FILE_ON_COMMIT == NULL) {
        perror("file does not exist in the current repository!");
        return;
    }
    char line[MAX_LENGTH_STRING];
    for (int i = 1; fgets(line, MAX_LENGTH_STRING, FILE_ON_COMMIT); i++) {
        
            printHighlightedWordInLine(word, line, number, i);
    
    }
    fclose(FILE_ON_COMMIT);

} 
void GREP_ANALYZE(char* input[], int arguments)
{
    if(arguments < 4) {
        perror("few inputs\nplease enter at least one file and a word.");
        return;
    }
    if(strcmp(input[0], "-f") ||  strcmp(input[2], "-p")) {
        perror("invalid inputs!");
        return;
    }
    if(arguments == 4) {
        GREP(input[1], input[3], find_head(), false);
    }
    else if(arguments == 5) {
        if(strcmp(input[4], "-n")) {
            perror("invalid inputs!");
            return;
        }
        GREP(input[1], input[3], find_head(), true);
    }
    else if(arguments == 6) {
        if(strcmp(input[4], "-c")) {
            perror("invalid inputs!");
            return;
        }
        Uint commit_id;
        sscanf(input[5], "%X",  &commit_id);
        GREP(input[1], input[3], commit_id, false);
    }
    else if(arguments == 7) {
        if(strcmp(input[4], "-c") || strcmp(input[6], "-n")) {
            perror("invalid inputs!");
            return;
        }
        Uint commit_id;
        sscanf(input[5], "%X",  &commit_id);
        GREP(input[1], input[3], commit_id, true);
    } else {
        perror("too many inputs!");
        return;
    }
}


void REVERT(string message, Uint commit_id, bool commit)
{
    char commits[MAX_LENGTH_STRING];
    sprintf(commits, "%s/.neogit/.COMMITS", IS_INITED());

    char check[MAX_LENGTH_STRING];
    sprintf(check, "%X",  commit_id);
    if(!find_file(commits,  check)) {
        perror(ANSI_BACK_RED"Invalid inputs!"ANSI_RESET);
        return;
    }

    char is_okay_checkout[MAX_LENGTH_STRING];
    sprintf(is_okay_checkout, "%s/.neogit/IS_OKAY_CHECKOUT.txt", IS_INITED());
    FILE *file = fopen(is_okay_checkout, "w");
    fprintf(file, "YES");
    fclose(file);

    COMMIT_CHECKOUT(commit_id);

    char is_okay_commit[MAX_LENGTH_STRING];
    sprintf(is_okay_commit, "%s/.neogit/IS_OKAY_COMMIT.txt", IS_INITED());
    FILE *fp = fopen(is_okay_commit, "w");
    fprintf(fp, "YES");
    fclose(fp);

    char commit_message[MAX_LENGTH_STRING];
    if(message == NULL) {
        char commit_info[MAX_LENGTH_STRING];
        sprintf(commit_info, "%s/.neogit/.COMMIT_INFO", IS_INITED());
        DIR* INFO = opendir(commit_info);
        struct dirent* com;
        while((com = readdir(INFO)) != NULL) {
            if (strcmp(com->d_name, ".") == 0 || strcmp(com->d_name, "..") == 0) continue;
            char temp[2 * MAX_LENGTH_STRING];
            sprintf(temp, "%s/%s", commit_info,  com->d_name);
            FILE* TEMP = fopen(temp, "r");

            char line[MAX_LENGTH_STRING];
            char temp_message[MAX_LENGTH_STRING];
            Uint temp_id = 0;

            for(int i = 0; fgets(line,  MAX_LENGTH_STRING, TEMP) ;i++) {
                if(i == 1) sscanf(line, "Commit Message : %[^\n]", temp_message);
                if(i == 5) sscanf(line, "COMMIT ID(HASH) : %X", &temp_id);
            }
            fclose(TEMP);
            if(temp_id == commit_id) {
                strcpy(commit_message, temp_message);
                break;
            }
        }
    }
    if(commit) {
        char source[MAX_LENGTH_STRING];
        sprintf(source, "%s/.neogit/.REPO_ON_COMMIT/%X", IS_INITED(), commit_id);
        DIR* FOLDER = opendir(source);
        struct dirent* entry;
        
        char stage[MAX_LENGTH_STRING];
        sprintf(stage, "%s/.neogit/.STAGING_AREA", IS_INITED());
        char temp_stage[MAX_LENGTH_STRING];
        sprintf(temp_stage, "%s/.neogit/.temp_stage", IS_INITED());
        int a = rename(stage, temp_stage);
        if(a != 0) perror("ERROR: Failed to create temporary staging area");
        mkdir(stage, 0755);
        while((entry = readdir(FOLDER)) != NULL) {
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".neogit") == 0) 
                continue;
            if(entry->d_type == DT_DIR) {
                COPY_DIR(source, stage, entry->d_name);
            } else {
                COPY_FILE(source, stage, entry->d_name); 
            }
        }
        closedir(FOLDER);
        if(message == NULL) {
            DECISION_FOR_COMMIT(commit_message);
            
        } else {
            DECISION_FOR_COMMIT(message);
        }
        remove_directory(stage);
        rename(temp_stage, stage);
    }
}
void REVERT_ANALYZE(char* input[], int arguments) 
{
    if(arguments == 0) {
        REVERT(NULL, find_head(), true);
    }
    else if(arguments == 1) {
        if(strcmp(input[0], "-n") == 0) {
            REVERT(NULL, find_head(), false);
            
        }
        else if(strncmp(input[0], "HEAD-",  5) == 0) {
            int num = 0;
            sscanf(input[0],  "HEAD-%d", &num);
            REVERT(NULL, find_commit(num), true);
        }
        else {
            Uint commit_id;
            sscanf(input[0], "%X", &commit_id);
            REVERT(NULL, commit_id,  true);
        }
    }
    else if(arguments == 2) {
        if(strcmp(input[0], "-m") == 0) {
            REVERT(input[1],  find_head(), true);
        }
        else if(strcmp(input[0], "-n") == 0) {
            Uint commit_id;
            sscanf(input[1], "%X", &commit_id);
            REVERT(NULL, commit_id,  false);
        } else {
            perror(ANSI_BACK_RED"Invalid inputs!"ANSI_RESET);
            return;
        }
    }
    else if(arguments == 3) {
        if(strcmp(input[0], "-m")) {
            perror(ANSI_BACK_RED"Invalid inputs!"ANSI_RESET);
            return;
        }
        Uint commit_id;
        sscanf(input[2], "%X", &commit_id);
        REVERT(input[1], commit_id,  true);
    } else {
        perror(ANSI_BACK_RED"Too many argumetns!"ANSI_RESET);
            return;
    }
}

bool isWhiteSpace(char line[]) {
    for (int i = 0; i < strlen(line); i++) {
        if(isWordBoundary(line[i])) continue;

        return true;
    }
    return false;
}
bool num_diff(char line1[], char line2[],  int line_to_print)
{
    char line1_copy[MAX_LENGTH_STRING];
    char line2_copy[MAX_LENGTH_STRING];
    strcpy(line1_copy, line1);
    strcpy(line2_copy, line2);
    int differences = 0;
    char diff_in_1[100];
    char diff_in_2[100];
   
    char* words1[100]; 
    int wordCount1 = 0;

    char* token = strtok(line1_copy, " \n"); 

    while (token) {
        words1[wordCount1] = token;
        wordCount1++;
        token = strtok(NULL, " \n");
    }
    char* words2[100]; 
    int wordCount2 = 0;

    char* token2 = strtok(line2_copy, " \n"); 

    while (token2) {
        words2[wordCount2] = token2;
        wordCount2++;
        token2 = strtok(NULL, " \n");
    }

    if (wordCount1 != wordCount2) return false;
    for (int i = 0; i < wordCount1; i++) {
        if(strcmp(words1[i], words2[i])) {
            strcpy(diff_in_1, words1[i]);
            strcpy(diff_in_2, words2[i]);
            differences++;
        }
    }
    if(differences != 1) return false;
    if(line_to_print == 1) {
        printHighlightedWordInLine(diff_in_1, line1, false, 0);
        return true;
    }
    else if(line_to_print == 2) {
        printHighlightedWordInLine(diff_in_2, line2, false, 0);
        return true;
    }
    

}

void compare_and_print_lines(char line1[], char line2[], int num1, int num2, string file1, string file2)
{
    
    if(strcmp(line1, line2) == 0) return;
    
    printf(ANSI_BLUE"%s---%d"ANSI_RESET"\n", file1, num1);
    if(num_diff(line1, line2, 1) == false)
    printf("%s\n", line1);
    printf(ANSI_RED"%s---%d"ANSI_RESET"\n", file2, num2);
    if(num_diff(line1, line2, 2) == false)
    printf("%s\n", line2);


} 
void DIFF_FILE(string file1_address, string file2_address, int beginLine1, int endLine1, int beginLine2, int endLine2)
{
    FILE* first = fopen(file1_address, "r");
    FILE* second = fopen(file2_address, "r");
    char file1_name[50];
    char file2_name[50];
    for(int i = strlen(file1_address) - 1; i >= 0; i--) {
        if(i == 0) {
            strcpy(file1_name,  file1_address);
        }
        if(file1_address[i - 1] == '/') {
            strcpy(file1_name, &file1_address[i]);
            break;
        }
        
    }
    for(int i = strlen(file2_address) - 1; i >= 0; i--) {
        if(i == 0) {
            strcpy(file2_name,  file2_address);
        }
        if(file2_address[i - 1] == '/') {
            strcpy(file2_name, &file2_address[i]);
            break;
        }
    }
    char line1[MAX_LENGTH_STRING];
    char line2[MAX_LENGTH_STRING];
    for(int i = 0; i < beginLine1 - 1; i++) {
        fgets(line1, MAX_LENGTH_STRING, first);
    }
    for(int i = 0; i < beginLine2 - 1; i++) {
        fgets(line2, MAX_LENGTH_STRING, second);
    }
    int lineNum1 = beginLine1 - 1;
    int lineNum2 = beginLine2 - 1;
    while(true) {
        string check1;
        string check2;
        check1 = fgets(line1, MAX_LENGTH_STRING, first);
        lineNum1++;
        check2 = fgets(line2, MAX_LENGTH_STRING, second);
        lineNum2++;
        while(!isWhiteSpace(line1)) {
            lineNum1++;
           check1 = fgets(line1, MAX_LENGTH_STRING, first);
           if(check1 == NULL) break;
        }
        while(!isWhiteSpace(line2)) {
            lineNum2++;
           check2 = fgets(line2, MAX_LENGTH_STRING, second);
           if(check2 == NULL) break;
        }
        if(check1 == NULL || check2 == NULL) break;
        if(lineNum1 > endLine1 || lineNum2 > endLine2) break;
        compare_and_print_lines(line1, line2, lineNum1, lineNum2, file1_name, file2_name);

    }
    
}
void DIFF_FILE_ANALYZE(char* input[], int arguments)
{
    if(arguments < 2) {
        perror(ANSI_BACK_RED"few arguments please enter two file!"ANSI_RESET);
        return;
    }
    else if(arguments == 2) {
        DIFF_FILE(input[0], input[1], 0, (int)1e9, 0, (int)1e9);
    }
    else if(arguments == 4) {
        if(strcmp(input[2], "-line1") == 0) {
            int beginLine1 = 0;
            int endLine1 = 0;
            sscanf(input[3], "%d-%d", &beginLine1, &endLine1);
            DIFF_FILE(input[0], input[1], beginLine1, endLine1, 0, (int)1e9);
        }
        else if(strcmp(input[2], "-line2") == 0) {
            int beginLine2 = 0;
            int endLine2 = 0;
            sscanf(input[3], "%d-%d", &beginLine2, &endLine2);
            DIFF_FILE(input[0], input[1], 0, (int)1e9, beginLine2, endLine2);
        } else {
            perror(ANSI_RED"invalid inputs!"ANSI_RESET);
            return;
        }
    }
    else if(arguments == 6) {
        if(strcmp(input[2], "-line1") || strcmp(input[4], "-line2")) {
            perror(ANSI_BACK_RED"invalid inputs!"ANSI_RESET);
            return;   
        }
        int beginLine1 = 0;
        int beginLine2 = 0;
        int endLine1 = 0;
        int endLine2 = 0;
        sscanf(input[3], "%d-%d", &beginLine1, &endLine1);
        sscanf(input[5], "%d-%d", &beginLine2, &endLine2);
        DIFF_FILE(input[0], input[1], beginLine1, endLine1, beginLine2, endLine2);
    } else {
        perror(ANSI_BACK_RED"invalid inputs!"ANSI_RESET);
    }
}
void COMPARE_COMMITS(string repo1, string repo2, string id1, string id2)
{
    DIR* REPO_1 = opendir(repo1);
    struct dirent *entry; 
    while((entry = readdir(REPO_1)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        if(find_file(repo2, entry->d_name)) {
            char new_repo1[MAX_LENGTH_STRING];
            char new_repo2[MAX_LENGTH_STRING];
            sprintf(new_repo1, "%s/%s", repo1, entry->d_name);
            sprintf(new_repo2, "%s/%s", repo2, entry->d_name);
            if(entry->d_type == DT_DIR) {
                printf(ANSI_BACK_MAGENTA"parent : %s"ANSI_RESET"\n", entry->d_name);
                COMPARE_COMMITS(new_repo1, new_repo2, id1, id2);
            } else {
                if(is_text_file(new_repo1) && is_text_file(new_repo2))
                DIFF_FILE(new_repo1, new_repo2, 0, (int)1e9, 9, (int)1e9);
            }
        }
        else {
            if(entry->d_type == DT_DIR) {
                printf(ANSI_BLUE"%s"ANSI_RESET" directroy exist in commit "ANSI_GREEN"%s"ANSI_RESET" but does not exist in commit "ANSI_CYAN"%s"ANSI_RESET"\n", entry->d_name, id1, id2);
                continue;
            } else {
                printf(ANSI_BLUE"%s"ANSI_RESET" file exist in commit "ANSI_GREEN"%s"ANSI_RESET" but does not exist in commit "ANSI_CYAN"%s"ANSI_RESET"\n", entry->d_name, id1, id2);
            }
        }
        
    }
}
void DIFF_COMMIT(string first, string second)
{
    char commits[MAX_LENGTH_STRING];
    sprintf(commits, "%s/.neogit/.COMMITS", IS_INITED());
    if (!find_file(commits, first) || !find_file(commits, second)) {
        perror(ANSI_BACK_RED"this commit id's are not valid!"ANSI_RESET);
        return;
    }
    char repo_on_commit_1[MAX_LENGTH_STRING];
    char repo_on_commit_2[MAX_LENGTH_STRING];
    sprintf(repo_on_commit_1, "%s/.neogit/.REPO_ON_COMMIT/%s", IS_INITED(), first);
    sprintf(repo_on_commit_2, "%s/.neogit/.REPO_ON_COMMIT/%s", IS_INITED(),  second);

    COMPARE_COMMITS(repo_on_commit_1, repo_on_commit_2, first, second);
    COMPARE_COMMITS(repo_on_commit_2, repo_on_commit_1,  second, first);
    

}

void STASH_PUSH(string message)
{
    char stashs[MAX_LENGTH_STRING];
    sprintf(stashs, "%s/.neogit/.STASHS", IS_INITED());

    DIR* STASHS = opendir(stashs);
    struct dirent* entry;
    int numStashs = 0;
    while((entry = readdir(STASHS)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        numStashs++;
    }
    closedir(STASHS);
    for(int i = numStashs - 1; i >= 0; i--) {
        char old[MAX_LENGTH_STRING];
        char new[MAX_LENGTH_STRING];
        sprintf(old, "%s/.neogit/.STASHS/%d", IS_INITED(), i);
        sprintf(new, "%s/.neogit/.STASHS/%d",  IS_INITED(), i + 1);
        rename(old, new);
    }
    char head_stash[MAX_LENGTH_STRING];
    sprintf(head_stash, "%s/.neogit/.STASHS/0", IS_INITED());
    mkdir(head_stash, 0755);
    char folder_on_stash[MAX_LENGTH_STRING];
    sprintf(folder_on_stash, "%s/FOLDER", head_stash);
    mkdir(folder_on_stash, 0755);
    
    DIR* FOLDER =  opendir(IS_INITED());
    struct dirent*  file;
    while((file = readdir(FOLDER)) != NULL) {
        if (strcmp(file->d_name, "..") == 0 || strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, ".neogit") == 0) continue;

        if(file->d_type == DT_DIR) {
            COPY_DIR(IS_INITED(), folder_on_stash,  file->d_name);
        } else {
            COPY_FILE(IS_INITED(), folder_on_stash ,file->d_name);
        }
    }

    char stash_status[2 * MAX_LENGTH_STRING];
    sprintf(stash_status, "%s/text.txt", head_stash);
    FILE* TEXT = fopen(stash_status, "w");
    fprintf(TEXT, "stash was on branch : %s\n", find_branch());
    fprintf(TEXT, "stash was on commit : %X\n", find_head());
    if(message) fprintf(TEXT,  "Message: %s\n", message);
    else fprintf(TEXT, "there was no message for this stash!!\n");
    fclose(TEXT);


    COMMIT_CHECKOUT(find_head());
}
void STASH_LIST()
{
    char stashs[MAX_LENGTH_STRING];
    sprintf(stashs, "%s/.neogit/.STASHS", IS_INITED());
    DIR* STASHS = opendir(stashs);
    struct dirent* entry;
    int numStashs = 0;
    while((entry = readdir(STASHS)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        numStashs++;
    }
    closedir(STASHS);
    for(int i = 0; i < numStashs; i++) {
        printf(ANSI_BLUE"stash index : %d"ANSI_RESET"\n", i);
        char text[2 * MAX_LENGTH_STRING];
        sprintf(text, "%s/%d/text.txt", stashs, i);
        FILE* INFO = fopen(text, "r");
        char line[MAX_LENGTH_STRING];
        while(fgets(line, MAX_LENGTH_STRING, INFO)) {
            printf(ANSI_MAGENTA"%s"ANSI_RESET, line);
        }
        fclose(INFO);

    }
}
void compare_stash_and_commit(string repo1, string repo2, Uint commit_id, int stash_index, bool commit_base)
{
    DIR* REPO_1 = opendir(repo1);
    struct dirent *entry; 
    while((entry = readdir(REPO_1)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        if(find_file(repo2, entry->d_name)) {
            char new_repo1[MAX_LENGTH_STRING];
            char new_repo2[MAX_LENGTH_STRING];
            sprintf(new_repo1, "%s/%s", repo1, entry->d_name);
            sprintf(new_repo2, "%s/%s", repo2, entry->d_name);
            if(entry->d_type == DT_DIR) {
                printf(ANSI_BACK_MAGENTA"parent : %s"ANSI_RESET"\n", entry->d_name);
                if(commit_base)
                compare_stash_and_commit(new_repo1, new_repo2, commit_id, stash_index, true);
                else
                compare_stash_and_commit(new_repo1, new_repo2, commit_id, stash_index, false);
            } else {
                if(is_text_file(new_repo1) && is_text_file(new_repo2))
                DIFF_FILE(new_repo1, new_repo2, 0, (int)1e9, 9, (int)1e9);
            }
        }
        else {
            if(entry->d_type == DT_DIR) {
                if(commit_base)
                printf(ANSI_BLUE"%s"ANSI_RESET" directroy exist in commit "ANSI_GREEN"%X"ANSI_RESET" but does not exist in stash "ANSI_CYAN"%d"ANSI_RESET"\n", entry->d_name, commit_id, stash_index);
                else
                printf(ANSI_BLUE"%s"ANSI_RESET" directroy exist in stash "ANSI_GREEN"%d"ANSI_RESET" but does not exist in commit "ANSI_CYAN"%X"ANSI_RESET"\n", entry->d_name, stash_index, commit_id);
                continue;
            } else {
                if(commit_base)
                printf(ANSI_BLUE"%s"ANSI_RESET" file exist in commit "ANSI_GREEN"%X"ANSI_RESET" but does not exist in stash "ANSI_CYAN"%d"ANSI_RESET"\n", entry->d_name, commit_id, stash_index);
                else
                printf(ANSI_BLUE"%s"ANSI_RESET" file exist in stash "ANSI_GREEN"%d"ANSI_RESET" but does not exist in commit "ANSI_CYAN"%X"ANSI_RESET"\n", entry->d_name, stash_index, commit_id);
                continue;
            }
        }
        
    }
}
void STASH_SHOW(int index)
{
    char text[MAX_LENGTH_STRING];
    sprintf(text, "%s/.neogit/.STASHS/%d/text.txt", IS_INITED(), index);
    FILE* INFO = fopen(text, "r");
    char line[MAX_LENGTH_STRING];
    fgets(line, MAX_LENGTH_STRING, INFO);
    fgets(line, MAX_LENGTH_STRING, INFO);
    Uint commit_id = 0;
    sscanf(line, "stash was on commit : %X\n", &commit_id);
    fclose(INFO);
    char commitAddress[MAX_LENGTH_STRING];
    sprintf(commitAddress, "%s/.neogit/.REPO_ON_COMMIT/%X", IS_INITED(), commit_id);
    char stashAddress[MAX_LENGTH_STRING];
    sprintf(stashAddress, "%s/.neogit/.STASHS/%d/FOLDER", IS_INITED(), index);
   
    compare_stash_and_commit(commitAddress, stashAddress, commit_id, index, true);
    compare_stash_and_commit(stashAddress, commitAddress, commit_id, index, false);

}
void STASH_CHECK_FILE(char src[], char dest[], char name[])
{
    if(find_file(dest, name)) {
        char file1[MAX_LENGTH_STRING];
        sprintf(file1, "%s/%s", src, name);
        char file2[MAX_LENGTH_STRING];
        sprintf(file2, "%s/%s", dest, name);
        DIFF_FILE(file1, file2, 0, (int)1e9, 0, (int)1e9);
    }
    COPY_FILE(src, dest, name);
}
void STASH_COPY_DIR(string src, string dest, char name[]) 
{
    char source[MAX_LENGTH_STRING];
    sprintf(source, "%s/%s", src, name);

    char destination[MAX_LENGTH_STRING];
    sprintf(destination, "%s/%s", dest, name);

    DIR *directory = opendir(destination);
    if (directory == NULL) {
        mkdir(destination, 0755);
    }
    closedir(directory);

    DIR *dir = opendir(source);
    if (dir == NULL) {
        perror("failed to open");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".neogit") == 0) {
            continue;
        }

        if (entry->d_type == DT_DIR) {
            
            STASH_COPY_DIR(source, destination, entry->d_name);
        } else {
            
            STASH_CHECK_FILE(source, destination, entry->d_name);
        }
    }

    closedir(dir);

}
void STASH_POP(int index)
{
    char stashs[MAX_LENGTH_STRING];
    sprintf(stashs, "%s/.neogit/.STASHS", IS_INITED());

    DIR* STASHS = opendir(stashs);
    struct dirent* file;
    int numStashs = 0;
    while((file = readdir(STASHS)) != NULL) {
        if(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) continue;
        numStashs++;
    }
    closedir(STASHS);
    char cur_stash[MAX_LENGTH_STRING];
    sprintf(cur_stash, "%s/%d", stashs, index);
    char stash_folder[MAX_LENGTH_STRING];
    sprintf(stash_folder, "%s/FOLDER", cur_stash);
    DIR* FOLDER_ON_STASH = opendir(stash_folder);
    struct dirent* entry;

    while((entry = readdir(FOLDER_ON_STASH)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        if(entry->d_type == DT_DIR) {
            STASH_COPY_DIR(stash_folder, IS_INITED(), entry->d_name);
        }
        else {char file2[MAX_LENGTH_STRING];
            STASH_CHECK_FILE(stash_folder, IS_INITED(), entry->d_name);
        }
    }
    remove_directory(cur_stash);
    for(int i = index; i < numStashs - 1; i++) {
        char old_path[MAX_LENGTH_STRING];
        char new_path[MAX_LENGTH_STRING];
        sprintf(old_path, "%s/%d", stashs, i + 1);
        sprintf(new_path, "%s/%d",  stashs, i);
        rename(old_path, new_path);
    }
}
void STASH_BRANCH(string branchName, int index)
{
    char stash_info[MAX_LENGTH_STRING];
    sprintf(stash_info, "%s/.neogit/.STASHS/%d/text.txt", IS_INITED(),  index);
    
    FILE* INFO = fopen(stash_info, "r");

    char line[MAX_LENGTH_STRING];
    fgets(line, MAX_LENGTH_STRING, INFO);
    fgets(line, MAX_LENGTH_STRING, INFO);

    Uint commit_id = 0;
    sscanf(line, "stash was on commit : %X\n", &commit_id);
    make_branch(branchName, commit_id);
    BRANCH_CHECKOUT(branchName);
    STASH_POP(index);
}
void STASH_CLEAR()
{
    char stashs[MAX_LENGTH_STRING];
    sprintf(stashs, "%s/.neogit/.STASHS", IS_INITED());
    remove_directory(stashs);
    mkdir(stashs, 0755);
}
void STASH_DROP(int index)
{
    char stashs[MAX_LENGTH_STRING];
    sprintf(stashs, "%s/.neogit/.STASHS", IS_INITED());
    DIR* STASHS = opendir(stashs);
    struct dirent* entry;
    int numStashs = 0;
    while((entry = readdir(STASHS)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        numStashs++;
    }
    closedir(STASHS);
    char cur_stash[MAX_LENGTH_STRING];
    sprintf(cur_stash, "%s/%d", stashs, index);
    remove_directory(cur_stash);
    for(int i = index; i < numStashs - 1; i++) {
        char old_path[MAX_LENGTH_STRING];
        char new_path[MAX_LENGTH_STRING];
        sprintf(old_path, "%s/%d", stashs, i + 1);
        sprintf(new_path, "%s/%d",  stashs, i);
        rename(old_path, new_path);
    }
}
void STASH_ANALYZE(char* input[], int arguments)
{
    if(arguments == 0) {
        perror(ANSI_BACK_RED"few arguments!!"ANSI_RESET);
        return;
    }
    if(strcmp(input[0], "push") == 0) {
        if(arguments == 1) {
            STASH_PUSH(NULL);
        }
        else if(arguments == 3) {
            if(strcmp(input[1], "-m")) {
                perror(ANSI_BACK_RED"invalid inputs!"ANSI_RESET);
                return;
            }
            STASH_PUSH(input[2]);
        } else {
            perror(ANSI_BACK_RED"invalid inputs!"ANSI_RESET);
            return;
        }
    }
    else if(strcmp(input[0], "list") == 0) {
        if(arguments != 1) {
            perror(ANSI_BACK_RED"too many arguments!!"ANSI_RESET);
            return;
        }
        STASH_LIST();
    }
    else if(strcmp(input[0], "show") == 0) {
        if(arguments != 2) {
            perror(ANSI_BACK_RED"usage : neogit stash show <index>"ANSI_RESET);
            return;
        }
        STASH_SHOW(atoi(input[1]));
    }
    else if(strcmp(input[0], "pop") == 0) {
        if(arguments == 1) {
            STASH_POP(0);
        }
        else if(arguments == 2){
            int index = -1;
            sscanf(input[1], "%d", &index);
            if(index < 0) {
                perror(ANSI_BACK_RED"usage : neogit  stash drop [--index=<n>](optional)"ANSI_RESET);
                return;
            }
            STASH_DROP(index);
        } else {
            perror(ANSI_BACK_RED"usage : neogit  stash pop [--index=<n>](optional)"ANSI_RESET);
            return;
        }
    }
    else if(strcmp(input[0], "branch") == 0) {
        if(arguments == 2) {
            STASH_BRANCH(input[1], 0);
        }
        else if(arguments == 3) {
            int index = -1;
            sscanf(input[1], "%d", &index);
            if(index < 0) {
                perror(ANSI_BACK_RED"usage : neogit  stash drop [--index=<n>](optional)"ANSI_RESET);
                return;
            }
            STASH_BRANCH(input[1], index);
        } else {
            perror(ANSI_BACK_RED"usage : neogit stash branch  <branch-name>" ANSI_RESET);
            return;
        }
    }
    else if(strcmp(input[0], "clear") == 0) {
        if(arguments != 1) {
            perror(ANSI_BACK_RED"too many arguments!"ANSI_RESET);
            return;
        }
        STASH_CLEAR();
    }
    else if(strcmp(input[0], "drop") == 0) {
        if(arguments == 1) {
            STASH_DROP(0);
        }
        else if(arguments == 2){
            int index = -1;
            sscanf(input[1], "%d", &index);
            if(index < 0) {
                perror(ANSI_BACK_RED"usage : neogit  stash drop [--index=<n>](optional)"ANSI_RESET);
                return;
            }
            STASH_DROP(index);
        } else {
            perror(ANSI_BACK_RED"usage : neogit  stash drop [--index=<n>](optional)"ANSI_RESET);
            return;
        }
    } else {
        perror(ANSI_BACK_RED"invalid inputs!"ANSI_RESET);
        return;
    }
}

Uint head_branch(string branchName)
{
    char branchAddress[MAX_LENGTH_STRING];
    sprintf(branchAddress, "%s/.neogit/.BRANCHES/%s/HEAD_COMMIT.txt", IS_INITED(), branchName);
    FILE *file = fopen(branchAddress, "r");
    Uint id = 0;
    fscanf(file, "HEAD COMMIT ID : %X",  &id);
    return id;

}
void MERGE(string branch1, string branch2)
{
    char branches[MAX_LENGTH_STRING];
    sprintf(branches, "%s/.neogit/.BRANCHES", IS_INITED());
    if(find_file(branches, branch1) == 0) {
        printf(ANSI_BACK_RED"%s is not a branch!"ANSI_RESET, branch1);
        return;
    }
    if(find_file(branches, branch2) == 0) {
        printf(ANSI_BACK_RED"%s is not a branch!!"ANSI_RESET, branch2);
        return;
    }

    Uint branch1_head = head_branch(branch1);
    Uint branch2_head = head_branch(branch2);
    char commit_1[10];
    char commit_2[10];
    sprintf(commit_1, "%X", branch1_head);
    sprintf(commit_2, "%X", branch2_head);
            
}
void MERGE_CLEAN(string branch1, string branch2)
{
    char branches[MAX_LENGTH_STRING];
    sprintf(branches, "%s/.neogit/.BRANCHES", IS_INITED());
    if(find_file(branches, branch1) == 0) {
        printf(ANSI_BACK_RED"%s is not a branch!"ANSI_RESET, branch1);
        return;
    }
    if(find_file(branches, branch2) == 0) {
        printf(ANSI_BACK_RED"%s is not a branch!!"ANSI_RESET, branch2);
        return;
    }

    Uint branch1_head = head_branch(branch1);
    Uint branch2_head = head_branch(branch2);   
}

int todo_check(string file_path)
{
    char* file_extension = strrchr(file_path, '.');
    if(strcasecmp(file_extension, ".c") == 0 || strcasecmp(file_extension, ".cpp") == 0) {
        FILE *file = fopen(file_path, "r");
        if (!file) {
            perror("Error opening file");
            return -1;
        }

        char line[MAX_LINE_LENGTH];
        while (fgets(line, MAX_LINE_LENGTH, file)) {
            if (strstr(line, "//TODO")) {
                fclose(file);
                return -1;
            }
        }

        fclose(file);
        return 1;
    }
    else if(strcasecmp(file_extension, ".txt") == 0) {
        FILE *file = fopen(file_path, "r");
        if (!file) {
            perror("Error opening file");
            return -1;
        }

        char line[MAX_LINE_LENGTH];
        while (fgets(line, MAX_LINE_LENGTH, file)) {
            if (strstr(line, "TODO")) {
                fclose(file);
                return -1;
            }
        }

        fclose(file);
        return 1;
    }
    else return 0;
}
int eof_blank_space(string filePath)
{
    char* file_extension = strrchr(filePath, '.');
    if(strcasecmp(file_extension, ".c") == 0 || strcasecmp(file_extension, ".cpp") == 0 || strcasecmp(file_extension, ".txt") == 0) {
        FILE *file = fopen(filePath, "r");
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }

        char ch = 'a';
        int is_blank = 0;

        while (true) {
            
            if (ch == ' ') {
                is_blank = 1;
            }
            ch = fgetc(file);
            if(feof(file)) break; 
            if (ch != '\n') is_blank = 0;
        }

        fclose(file);
        if (is_blank == 1) return -1;
        else return 1;
    }
    else return 0;
}
int format_check(string file)
{
    char* file_extension = strrchr(file, '.');
    if(strcasecmp(file_extension, ".c") == 0) return 1;
    if(strcasecmp(file_extension, ".cpp") == 0) return 1;
    if(strcasecmp(file_extension, ".txt") == 0) return 1;
    if(strcasecmp(file_extension, ".mp3") == 0) return 1;
    if(strcasecmp(file_extension, ".mp4") == 0) return 1;
    if(strcasecmp(file_extension, ".wav") == 0) return 1;
    if(strcasecmp(file_extension, ".py") == 0) return 1;
    return -1;
}

void push(struct Stack *stack, char item) {
    if (stack->top == MAX - 1) {
        printf("Stack overflow.\n");
        exit(EXIT_FAILURE);
    }
    stack->items[++stack->top] = item;
}
char pop(struct Stack *stack) {
    if (stack->top == -1) {
        printf("Stack underflow.\n");
        exit(EXIT_FAILURE);
    }
    return stack->items[stack->top--];
}

bool isEmpty(struct Stack *stack) {
    return stack->top == -1;
}

bool checkBalanced(const char *filename) {
    struct Stack stack;
    stack.top = -1;

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int ch;
    bool balanced = true;

    while (balanced && (ch = fgetc(file)) != EOF) {
        if (ch == '(' || ch == '{' || ch == '[') {
            push(&stack, ch);
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (isEmpty(&stack)) {
                balanced = false;
            } else {
                char topItem = pop(&stack);
                balanced = (ch == ')' && topItem == '(') ||
                           (ch == '}' && topItem == '{') ||
                           (ch == ']' && topItem == '[');
            }
        }
    }

    fclose(file);

    return balanced;
}

int balance_braces(string file)
{
    char* file_extension = strrchr(file, '.');
    if(strcasecmp(file_extension, ".c") == 0 || strcasecmp(file_extension, ".cpp") == 0 || strcasecmp(file_extension, ".txt") == 0) {
        if(checkBalanced(file)) return 1;
        return -1;
    }
    else return 0;
}

int is_allman_style(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char line[1024];
    int prev_brace_column = -1;
    int line_number = 0;
    int allman_style = 1;

    while (fgets(line, sizeof(line), file)) {
        line_number++;

        // Skip comments and strings
        if (line[0] == '/' && (line[1] == '/' || line[1] == '*')) {
            continue;
        }
        if (line[0] == '"' || line[0] == '\'') {
            int i = 0;
            while (line[i] != '\0' && line[i] != line[0]) {
                i++;
            }
            if (line[i] == '\0') {
                allman_style = 0;
                break;
            }
            continue;
        }

        // Check for opening braces
        int brace_column = -1;
        int i = 0;
        while (line[i] != '\0' && line[i] != '{') {
            if (line[i] == ' ') {
                brace_column = i;
            }
            i++;
        }
        if (brace_column != -1) {
            prev_brace_column = brace_column;
        }

        // Check for closing braces
        int closing_brace_column = -1;
        i = brace_column;
        while (line[i] != '\0' && line[i] != '}') {
            i++;
        }
        if (i != brace_column) {
            closing_brace_column = i;
        }

        // Check for indentation
        if (brace_column != -1 && closing_brace_column != -1) {
            int indent = brace_column - prev_brace_column;
            if (closing_brace_column != brace_column + indent) {
                allman_style = 0;
                break;
            }
        }
    }

    fclose(file);

    return allman_style;
}


int check_kr_style(const char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return -1; // indicate error
    }

    int lineNumber = 1;
    char line[1000];
    int spaces = 0;
    int tabs = 0;

    while (fgets(line, sizeof(line), file)) {
        int i = 0;
        while (line[i] == ' ' || line[i] == '\t') {
            if (line[i] == ' ')
                spaces++;
            else if (line[i] == '\t')
                tabs++;
            i++;
        }
        if (tabs != 0) {
            return 0; // K & R style not followed
        }
        if (spaces % 4 != 0) {
            return 0; // K & R style not followed
        }
        lineNumber++;
    }
    fclose(file);
    return 1; // K & R style followed
}
int indentaion_check(string file)
{
    char* file_extension = strrchr(file, '.');
    if(strcasecmp(file_extension, ".c") == 0 || strcasecmp(file_extension, ".cpp") == 0) {
        if(check_kr_style(file) == 1 || is_allman_style(file) == 1) 
            return 1;
            return -1;
    }

    else return 0;
}
int static_error_check(string file)
{
    char* file_extension = strrchr(file, '.');
    if(strcasecmp(file_extension, ".c") == 0 || strcasecmp(file_extension, ".cpp") == 0) {

    }
    else return 0;
}
int file_size_check(string file)
{
    struct stat file_status;
    if(stat(file, &file_status) < 0) {
        return -1;
    }
    double size = (double)file_status.st_size / (1024 * 1024);
    if (size > 5) return -1;
    else return 1;
}
int character_limit(string filename)
{
    char* file_extension = strrchr(filename, '.');
    if(strcasecmp(file_extension, ".c") == 0 || strcasecmp(file_extension, ".cpp") == 0 || strcasecmp(file_extension, ".txt") == 0) {
        FILE *file;
        long count = 0;
        char ch;

        file = fopen(filename, "r");
        if (file == NULL) {
            perror("Error opening file");
            return -1;
        }

        while ((ch = fgetc(file)) != EOF) {
            if (count > 20000) return -1;
            if (ch != '\n') {
                count++;
            }
        }

        fclose(file);
        return 1;
    }
    else return 0;
}

int is_video_duration_less_than_10_minutes(const char *filename) {
    // AVFormatContext *format_context = NULL;
    // int ret;

    // if ((ret = avformat_open_input(&format_context, filename, NULL, NULL)) < 0) {
    //     return -1;
    // }

    // avformat_find_stream_info(format_context, NULL);

    // int64_t duration = format_context->duration;
    // avformat_close_input(&format_context);

    // if (duration == AV_NOPTS_VALUE) {
    //     fprintf(stderr, "Could not determine duration of input file '%s'\n", filename);
    //     return -1;
    // }

    // int64_t duration_seconds = duration / AV_TIME_BASE;
    // return duration_seconds < 600;
    return 1;
    
}
int time_limit(string file_path)
{
    char* file_extension = strrchr(file_path, '.');
    if(strcasecmp(file_extension, ".mp4") == 0 || strcasecmp(file_extension, ".wav") == 0 || strcasecmp(file_extension, ".mp3") == 0) {
        if(is_video_duration_less_than_10_minutes(file_path))
        return 1;
        else return -1;
    }
    return 0;
}
bool PRECOMMIT_TEST(string file, bool print)
{
    char filename[MAX_LENGTH_STRING];
    for(int i = strlen(file) - 1; i >= 0; i--) {
        if(file[i - 1] == '/') {
            sprintf(filename, "%s",  &file[i]);
            break;
        }
    }
    if(print) printf(ANSI_BACK_BLUE"%s"ANSI_RESET"\n", filename);

    bool result = true;
    char hooks[MAX_LENGTH_STRING];
    sprintf(hooks, "%s/.neogit/.HOOKS", IS_INITED());

    DIR* HOOKS = opendir(hooks);

    struct dirent* entry;

    while(entry = readdir(HOOKS)) {
        if(strcmp(entry->d_name, ".") == 0 || (strcmp(entry->d_name, "..")) == 0) continue;
            int status = -1;
        if(strcmp(entry->d_name, "todo-check") == 0) {
            status = todo_check(file);
        }
        else if(strcmp(entry->d_name, "eof-blank-space") == 0) {
            status = eof_blank_space(file);
        }
        else if(strcmp(entry->d_name, "format-check") == 0) {
            status = format_check(file);
        }
        else if(strcmp(entry->d_name, "balance-braces") == 0) {
            status = balance_braces(file);
        }
        else if(strcmp(entry->d_name, "indentation-check") == 0) {
            status = indentaion_check(file);
        }
        else if(strcmp(entry->d_name, "static-error-check") == 0) {
            status = static_error_check(file);
        }
        else if(strcmp(entry->d_name, "file-size-check") == 0) {
            status = file_size_check(file);
        }
        else if(strcmp(entry->d_name, "time-limit") == 0) {
            status = time_limit(file);
        }

        if(print) {
            if(status == -1) printf("\"%s\"----------------------------------"ANSI_RED"FAILED"ANSI_RESET"\n",  entry->d_name);
            else if(status == 1) printf("\"%s\"----------------------------------"ANSI_GREEN"PASSED"ANSI_RESET"\n",  entry->d_name);
            else if(status == 0) printf("\"%s\"----------------------------------"ANSI_YELLOW"SKIPPED"ANSI_RESET"\n",  entry->d_name);
        }
        
        if(status == -1) result = false;
     }
     return result;
}
void RUN_PRECOMMIT(char directory[])
{
    DIR* STAGE = opendir(directory);

    struct dirent* entry;
    while((entry = readdir(STAGE))) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        char input[2 * MAX_LENGTH_STRING];
        sprintf(input, "%s/%s",  directory, entry->d_name);
        if(entry->d_type == DT_DIR) {
            RUN_PRECOMMIT(input);
        }
        else {
            PRECOMMIT_TEST(input, true);
        }
    }
    closedir(STAGE);
}
void RUN_PRECOMMIT_F(char* files[], int number)
{
    char cwd[MAX_LENGTH_STRING];
    getcwd(cwd, MAX_LENGTH_STRING);
    for(int i = 0; i < number; i++) {
            char file_in_stage[MAX_LENGTH_STRING];
            sprintf(file_in_stage, "%s/.neogit/.STAGING_AREA%s/%s", IS_INITED(), find_file_path(cwd, IS_INITED()), files[i]);
            PRECOMMIT_TEST(file_in_stage, true);

        }
}
void HOOKS_LIST()
{
    printf(ANSI_CYAN"todo-check"ANSI_RESET"\n");
    printf(ANSI_CYAN"eof-blank-space"ANSI_RESET"\n");
    printf(ANSI_CYAN"format-check"ANSI_RESET"\n");
    printf(ANSI_CYAN"balance-braces"ANSI_RESET"\n");
    printf(ANSI_CYAN"indentaion-check"ANSI_RESET"\n");
    printf(ANSI_CYAN"static-error-check"ANSI_RESET"\n");
    printf(ANSI_CYAN"file-size-check"ANSI_RESET"\n");
    printf(ANSI_CYAN"character-limt"ANSI_RESET"\n");
    printf(ANSI_CYAN"time-limt"ANSI_RESET"\n");
}
void APPLIED_HOOKS()
{
    char hooks[MAX_LENGTH_STRING];
    sprintf(hooks, "%s/.neogit/.HOOKS", IS_INITED());
    DIR* HOOKS = opendir(hooks);
    struct dirent* entry;
    while((entry = readdir(HOOKS)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        printf(ANSI_MAGENTA"%s"ANSI_RESET"\n", entry->d_name);
    }
    closedir(HOOKS);
}
void PRECOMMIT_ADD_HOOK(string hook_id)
{
    char hook[MAX_LENGTH_STRING];
    sprintf(hook, "%s/.neogit/.HOOKS/%s", IS_INITED(), hook_id);

    FILE *fp = fopen(hook, "w");
    fclose(fp);
}
void PRECOMMIT_REMOVE_HOOK(char hook_id[])
{
    char hooks[MAX_LENGTH_STRING];
    sprintf(hooks, "%s/.neogit/.HOOKS", IS_INITED());
    if(find_file(hooks, hook_id)) {
        char hook[MAX_LENGTH_STRING];
        sprintf(hook, "%s/%s", hooks, hook_id);
        remove(hook);
    }
}
void PRECOMMIT_ANALYZE(char* input[], int arguments)
{
    if(arguments == 0) {
        char stage[MAX_LENGTH_STRING];
        sprintf(stage, "%s/.neogit/.STAGING_AREA", IS_INITED());
        RUN_PRECOMMIT(stage);
    }
    else if(strcmp(input[0], "-f") == 0) {
        RUN_PRECOMMIT_F(&input[1], arguments - 1);
    }
    else if(arguments == 2) {
        if(strcmp(input[0], "hooks") == 0 && strcmp(input[1], "list") == 0) {
            HOOKS_LIST();
        }
        else if(strcmp(input[0], "applied") == 0 && strcmp(input[1], "hooks") == 0) {
            APPLIED_HOOKS();
        } else {
            perror(ANSI_BACK_RED"invalid inpurs!"ANSI_RESET);
            return;
        }
    }
    else if(arguments == 3) {
        if(strcmp(input[0], "add") == 0 && strcmp(input[1], "hook") == 0) {
            PRECOMMIT_ADD_HOOK(input[2]);
        }
        else if(strcmp(input[0],  "remove") == 0 && strcmp(input[1], "hook") == 0) {
            PRECOMMIT_REMOVE_HOOK(input[2]);
        } else {
            perror(ANSI_BACK_RED"invalid inputs!"ANSI_RESET);
            return;
        }
    } else {
        perror(ANSI_BACK_RED"invalid inputs!"ANSI_RESET);
        return;
    }
}


void check_precommits(char stage[])
{
    DIR* dir = opendir(stage);
    struct dirent* entry;
    while(entry = readdir(dir)) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        char input[MAX_LENGTH_STRING];
        sprintf(input, "%s/%s", stage, entry->d_name);
        if(entry->d_type == DT_DIR) {
            check_precommits(input);
        } 
        else {
            if(PRECOMMIT_TEST(input, false) == false) {
                while(true) {
                    char decision;
                    printf(ANSI_YELLOW"%s failed pre-commit test\ndo you want to continue your commit or not ?[Y\\N]"ANSI_RESET"\n", entry->d_name);
                    scanf("%c", &decision);
                    getchar();
                    if(decision == 'Y' || decision == 'y') break;
                    else if(decision == 'N' || decision == 'n') {
                        printf(ANSI_BLINK ANSI_BLUE"not commited!!"ANSI_RESET"\n");
                        exit(1);
                    }
                    else printf(ANSI_BACK_RED"not valid!!"ANSI_RESET"\n");
                }
                
            }
        }
    }
}
void DECISION_FOR_COMMIT(string message)
{

    char stage[MAX_LENGTH_STRING];
    sprintf(stage, "%s/.neogit/.STAGING_AREA", IS_INITED());
    check_precommits(stage);

    COMMIT_FUNC(message);
}

Uint find_commit(int n) 
{
    if(n < 0) {
        perror("invalid inputs");
        exit(1);
    }


    Uint id = 0;
    char info[MAX_LENGTH_STRING];
    sprintf(info, "%s/LOCAL_INFO/%s.txt", CONFIG_ADDRESS(), name_project());
    FILE* INFO = fopen(info, "r");
    char line[MAX_LENGTH_STRING];
    for (int i = 0; i < 4; i++) fgets(line, MAX_LENGTH_STRING, INFO);
    sscanf(line , "commit_id : %X\n", &id);
    fclose(INFO);

    char tree[MAX_LENGTH_STRING];
    sprintf(tree, "%s/.neogit/.TREE.txt", IS_INITED());
    FILE* TREE = fopen(tree, "r");
    
    for (int i = 0; i <  n; i++) {
        rewind(TREE);
        while(true) {
            Uint temp = 0;
            Uint prev = 0;
            fgets(line,  MAX_LENGTH_STRING, TREE);
            sscanf(line, "prev_commit->%X commit_id->%X ", &prev, &temp);
            if(temp == id) {
                id = prev;
                break;
            }
            if (id == 0) break;
        }
    }
    return id;
}
Uint find_head()
{
    Uint head;
    char info[MAX_LENGTH_STRING];
    sprintf(info, "%s/LOCAL_INFO/%s.txt", CONFIG_ADDRESS(), name_project());
    FILE *INFO = fopen(info, "r");
    char line[MAX_LENGTH_STRING];
    for(int i = 0; i < 4; i++) {
        fgets(line, MAX_LENGTH_STRING, INFO);
    }
    sscanf(line, "commit_id : %X\n", &head);

    fclose(INFO);

    return head;

}
string find_name()
{
    string name = make_string(100);
    char info[MAX_LENGTH_STRING];
    sprintf(info, "%s/LOCAL_INFO/%s.txt", CONFIG_ADDRESS(), name_project());
    FILE *INFO = fopen(info, "r");
    
    char line[MAX_LENGTH_STRING];
    fgets(line, MAX_LENGTH_STRING, INFO);
    sscanf(line, "user.name : %[^\n]", name);

    fclose(INFO);

    return name;
}
string find_email()
{
    string email = make_string(100);
    char info[MAX_LENGTH_STRING];
    sprintf(info, "%s/LOCAL_INFO/%s.txt", CONFIG_ADDRESS(), name_project());
    FILE *INFO = fopen(info, "r");
    
    char line[MAX_LENGTH_STRING];
    for (int i = 0; i < 2; i++) {
        fgets(line, MAX_LENGTH_STRING, INFO);
    }
    sscanf(line, "user.email : %[^\n]", email);

    fclose(INFO);

    return email;
}
string find_branch()
{
    string branch = make_string(100);
    char info[MAX_LENGTH_STRING];
    sprintf(info, "%s/LOCAL_INFO/%s.txt", CONFIG_ADDRESS(), name_project());
    FILE *INFO = fopen(info, "r");
    
    char line[MAX_LENGTH_STRING];
    for (int i = 0; i < 3; i++) {
        fgets(line, MAX_LENGTH_STRING, INFO);
    }
    sscanf(line, "branch : %[^\n]", branch);

    fclose(INFO);

    return branch;
}


void make_empty_directories(string src, string dest, string name) 
{
    char source[MAX_LENGTH_STRING];
    sprintf(source, "%s/%s", src, name);

    char destination[MAX_LENGTH_STRING];
    sprintf(destination, "%s/%s", dest, name);

    DIR *directory = opendir(destination);
    if (directory == NULL) {
        mkdir(destination, 0755);
    }
    closedir(directory);

    DIR *dir = opendir(source);
    if (dir == NULL) {
        perror("failed to open");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".neogit") == 0) {
            continue;
        }

        if (entry->d_type == DT_DIR) {
            
            make_empty_directories(source, destination, entry->d_name);
        }   
    }

    closedir(dir);


}
void base_empty_directories(string source, string destination)
{
    DIR* FOLDER = opendir(source);
    if(FOLDER == NULL) {
        perror("it is an error from base part!");
        exit(1);
    }

    struct dirent* entry;

    while((entry = readdir(FOLDER)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".neogit") == 0) {
            continue;
        }

        if (entry->d_type == DT_DIR) {
            
            make_empty_directories(source, destination, entry->d_name);
        }   
    }
    closedir(FOLDER);
}





int main(int argc, char *argv[])
{  
    if(argc == 1) return 1;
    if(IS_INITED()) {
        printf(ANSI_BACK_MAGENTA "%s" ANSI_RESET "\n", find_branch());
        char repository[MAX_LENGTH_STRING];
        char stage[MAX_LENGTH_STRING];
        char unstage[MAX_LENGTH_STRING];

        sprintf(repository,"%s/.neogit/.LOCAL_REPOSITORY",IS_INITED());
        sprintf(stage,"%s/.neogit/.STAGING_AREA",IS_INITED());
        sprintf(unstage, "%s/.neogit/.UNSTAGED", IS_INITED());


        base_empty_directories(IS_INITED(), repository);
        base_empty_directories(IS_INITED(), stage);
        base_empty_directories(IS_INITED(),  unstage); 
    } 
    if (strcmp(argv[1], "init") == 0) {
        init();
        return 0;
    }
    else if (strcmp(argv[1], "config") == 0) {
        if(strcmp(argv[2], "-global") == 0) {
            if(strncmp(argv[3], "alias.", 6) == 0) {
                GLOBAL_ALIAS(argv[3], argv[4]); 
            }
            else global_information(argv[3], argv[4]);
        }
        else {
            if(strncmp(argv[2], "alias.", 6) == 0) {
                ALIAS(argv[2], argv[3], name_project());   
            }
            else {
                char project[MAX_LENGTH_STRING];
                sprintf(project, "%s.txt", name_project());
                information(argv[2], argv[3],  project);
            }
        }
        return 0;
    }
    if(IS_INITED() == NULL) {
        printf("please initilize neogit first!\n");
        return 0;
    }
    if(strcmp(argv[1], "add") == 0) {
        int num_added = 0;
        char added[20][MAX_LENGTH_STRING];
        if(strcmp(argv[2], "-f") == 0) {
            for (int i = 3; i < argc; i++) {
                ADD_FUNC(argv[i]);
                strcpy(added[num_added], argv[i]);
                num_added++;
            }
        }
        else if(strcmp(argv[2], "-n") == 0) {
            int depth;
            sscanf(argv[2], "%d", &depth);
            char cwd[500];
            getcwd(cwd, 500);
            char stage[500];
            sprintf(stage, "%s/.neogit/.STAGING_AREA", IS_INITED());
            show_in_add(depth, 0, cwd, stage);
        }
        else if(strcmp(argv[2], "-redo") == 0) {
            REDO_FUNC();
        }
        else {
            ADD_FUNC(argv[2]);
            strcpy(added[num_added], argv[2]);
            num_added++;
        }
        ADD_LIST(added, num_added);
    }
    else if (strcmp(argv[1], "reset") == 0) {
        if(strcmp(argv[2], "-undo") == 0) {
            UNDO_FUNC();
        }
        else if(strcmp(argv[2], "-f") == 0) {
            for (int i = 3; i < argc; i++) {
                RESET_FUNC(argv[i]);
            }
        } else {
            RESET_FUNC(argv[2]);
            char command[200];
            
        }
    }
    else if(strcmp(argv[1], "status") == 0) {
        RUN_STATUS();
    }
    else if(strcmp(argv[1], "commit") == 0) {
        if(strcmp(argv[2], "-m") == 0) {
            if(argc > 4) {
                printf("to many arguments!\n");
                return 1;
            }
            if (strlen(argv[3]) > 72) {
                perror("this message is too long please change it");
                return 1;
            }
            DECISION_FOR_COMMIT(argv[3]);
        }
        else if(strcmp(argv[2], "-s") == 0) {
            char shortcut[MAX_LENGTH_STRING];
            sprintf(shortcut, "%s.txt", argv[3]);

            char set_dir[MAX_LENGTH_STRING];
            sprintf(set_dir, "%s/.neogit/.COMMIT_SET", IS_INITED());

            if(find_file(set_dir, shortcut) == 0) {
                perror("this shortcut message does not exist!\n");
                return 1;
            }
            string message = find_message(argv[3]);
            DECISION_FOR_COMMIT(message);
        }
    }
    else if(strcmp(argv[1], "set") == 0) {
        if(argc != 6) {
            perror("not enough arguments!");
            return 1;
        }
        if(strcmp(argv[2], "-m") || strcmp(argv[4], "-s")) {
            perror("invalid input!");
            return 1;
        }
        SET_FUNC(argv[3], argv[5]);
    }
    else if(strcmp(argv[1], "replace") == 0) {
        if(argc != 6) {
            perror("not enough arguments!\n");
            return 1;
        }
        if(strcmp(argv[2], "-m") || strcmp(argv[4], "-s")) {
            perror("invalid input!");
            return 1;
        }
        char shortcut[MAX_LENGTH_STRING];
        sprintf(shortcut, "%s.txt", argv[5]);
        char set_dir[MAX_LENGTH_STRING];
        sprintf(set_dir, "%s/.neogit/.COMMIT_SET", IS_INITED());
        if(find_file(set_dir, shortcut) == 0) {
            perror("this shortcut message does not exist!\n");
            return 1;
        }
        SET_FUNC(argv[3], argv[5]);
    }
    else if(strcmp(argv[1], "remove") == 0) {
        if(argc != 4) {
            perror("not enough arguments!\n");
            return 1;
        }
        if(strcmp(argv[2], "-s")) {
            perror("invalid inputs!");
            return 1;
        }
        char shortcut[MAX_LENGTH_STRING];
        sprintf(shortcut, "%s.txt", argv[3]);

        char set_dir[MAX_LENGTH_STRING];
        sprintf(set_dir, "%s/.neogit/.COMMIT_SET", IS_INITED());

        if(find_file(set_dir, shortcut) == 0) {
            perror("this shortcut message does not exist!\n");
            return 1;
        }

        REMOVE_SHORTCUT(argv[3]);
    }
    else if(strcmp(argv[1], "log") == 0) {
        if(argc == 2) {
            LOG_FUNC((int)1e9);
        }
        else if(strcmp(argv[2], "-n") == 0) {
            if(argc != 4) {
                perror("invlalid inputs!");
                return 1;
            }
            int num = 0;
            sscanf(argv[3], "%d", &num);
            LOG_FUNC(num);

        }
        else if(strcmp(argv[2], "-branch") == 0) {
            if(argc != 4) {
                perror("invlalid inputs!");
                return 1;
            }
            LOG_BRANCH(argv[3]);
        }
        else if(strcmp(argv[2], "-author") == 0) {
            if(argc != 4) {
                perror("invlalid inputs!");
                return 1;
            }
            LOG_AUTHOR(argv[3]);
        }
        else if(strcmp(argv[2], "-since") == 0) {
            if(argc != 4) {
                perror("invalid inputs!");
                return 1;
            }
            LOG_TIME(argv[3], -1);
        }
        else if(strcmp(argv[2], "-before") == 0) {
            if(argc != 4) {
                perror("invalid inputs!");
                return 1;
            }
            LOG_TIME(argv[3], 1);
        }
        else if(strcmp(argv[2], "-search") == 0) {
            for(int i = 3; i < argc; i++) {
                LOG_SEARCH(argv[i]);
            }
        }
        else {perror("invalid inputs!"); return 1;}
    }
    else if(strcmp(argv[1], "branch") == 0) {
        if(argc > 3) {
            perror("not enough arguments!\n");
            return 1;
        }
        if (argc == 2) {
            PRITNT_BRANCHES();
        }
        else {
            make_branch(argv[2], find_head());
        }
    }
    else if(strcmp(argv[1], "checkout") == 0) {
        if (argc != 3) {
            perror("not enough arguments!\n");
            return 1;
        }
        if(strcmp(argv[2], "HEAD") == 0) {
            COMMIT_CHECKOUT(find_head());
            char is_okay_commit[MAX_LENGTH_STRING];
            sprintf(is_okay_commit, "%s/.neogit/IS_OKAY_COMMIT.txt", IS_INITED());
            FILE* IS_OKAY_COMMIT = fopen(is_okay_commit, "w");
            fprintf(IS_OKAY_COMMIT, "YES");
            fclose(IS_OKAY_COMMIT);
        }
        else if(strncmp(argv[2], "HEAD-", 5) == 0) {
            int n = 0;
            sscanf(argv[2], "HEAD-%d", &n);

            COMMIT_CHECKOUT(find_commit(n));

        }
        else {
            CHECKOUT(argv[2]);
        }
    }
    else if(strcmp(argv[1], "tag") == 0) {
        if(argc == 2) {
            show_all_tags();
        }
        else if(strcmp(argv[2], "show") == 0) {
            if(argc > 4) {
                perror("invalid inputs!");
                return 1;
            }
            show_tag_information(argv[3]);
        }
        else if(strcmp(argv[2], "-a") == 0) {
            TAG_ANALYZE(&argv[3], argc - 4);
        }
        else {
            perror("invalid inputs!");
            return 1;
        }
    }
    else if(strcmp(argv[1], "grep") == 0) {
        GREP_ANALYZE(&argv[2], argc - 2);

    }
    else if(strcmp(argv[1], "revert") == 0) {
        REVERT_ANALYZE(&argv[2], argc - 2);
    }
    else if(strcmp(argv[1], "diff") == 0) {
        if(strcmp(argv[2], "-f") == 0) {
            DIFF_FILE_ANALYZE(&argv[3], argc - 3);
        }
        else if(strcmp(argv[2], "-c") == 0) {
            if(argc != 5) {
                perror(ANSI_BACK_RED"Invalid Inputs!"ANSI_RESET);
                return 1;
            }
            DIFF_COMMIT(argv[3], argv[4]);
        } else {
            perror(ANSI_BACK_RED"Invalid Inputs!"ANSI_RESET);
            return 1;
        }
    }
    else if(strcmp(argv[1], "stash") == 0) {
        STASH_ANALYZE(&argv[2], argc - 2);
    }
    else if(strcmp(argv[1], "merge") == 0) {
        if(argc < 5 || argc > 6 || strcmp(argv[2], "-b")) {
            perror(ANSI_BACK_RED"Usage : neogit merge -b <branch1-name> <branch2-name> -clean(optional)"ANSI_RESET);
            return 1;
        }
        if(argc == 5) {
            MERGE(argv[3], argv[4]);
        }
        if(argc == 6) {
            if(strcmp(argv[5], "-clean")) {
                perror(ANSI_BACK_RED"Usage : neogit merge -b <branch1-name> <branch2-name> -clean(optional)"ANSI_RESET);
                return 1;
            }
            MERGE_CLEAN(argv[3], argv[4]);
        }
        
    }
    else if(strcmp(argv[1], "pre-commit") == 0) {
        PRECOMMIT_ANALYZE(&argv[2], argc - 2);
    }
    else {
        RUN_ALIAS(argv[1]);
    }

    return 0;
}