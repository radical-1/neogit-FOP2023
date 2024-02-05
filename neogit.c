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
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>
#include <wait.h>
#include <err.h>
#include <limits.h>
#include <libavformat/avformat.h>


#ifndef __USE_MISC
#define __USE_MISC /* for dirfd() */
#endif

#ifndef DT_DIR
#define DT_DIR 4 // Replace 4 with the appropriate value for your system
#endif

//define for colors 
#define ANSI_RESET   "\x1b[0m"
#define ANSI_BLACK   "\x1b[30m"
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
#define MAX_LINE_LENGTH 512
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
        project[strlen(project) - 4] = '\0';
        printf(ANSI_CYAN"name is updated for %s project successfully!"ANSI_RESET"\n", project);
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
        printf(ANSI_CYAN"email is updated for %s project successfully!"ANSI_RESET"\n", project);
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
    if(strncmp(command, "neogit", 6)) {
        perror(ANSI_BACK_RED"invalid command!!"ANSI_RESET);
        exit(1);
    }
    char alias[MAX_LENGTH_STRING];
    sscanf(input, "alias.%s", alias);

    char new_alias[2 * MAX_LENGTH_STRING];
    sprintf(new_alias, "%s/LOCAL_ALIAS/%s/%s.txt", CONFIG_ADDRESS(), project, alias);

    FILE *NEW_ALIAS = fopen(new_alias, "w");

    fputs(command, NEW_ALIAS);

    fclose(NEW_ALIAS);  

    printf("alias : "ANSI_BOLD ANSI_CYAN"%s"ANSI_RESET" --- command : "ANSI_BOLD ANSI_CYAN"%s"ANSI_RESET"\n", alias, command);
    printf("alias set successfully for "ANSI_BOLD ANSI_MAGENTA"%s"ANSI_RESET" project!\n", project);
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
    char branchs[MAX_LINE_LENGTH];
    sprintf(branchs, "%s/.neogit/.BRANCHES", IS_INITED());
    if(find_file(branchs, branchName)) {
        printf(ANSI_BACK_RED"This branch name already exists in active Branchs!"ANSI_RESET"\n");
        printf(ANSI_BACK_RED"Please choose another name for your branch!"ANSI_RESET"\n");
        return;
    }
    char merged[MAX_LINE_LENGTH];
    sprintf(merged, "%s/.neogit/.MERGED_BRANCHS", IS_INITED());
    if(find_file(merged, branchName)) {
        printf(ANSI_BACK_RED"This branch name was name of a merged branch!"ANSI_RESET"\n");
        printf(ANSI_BACK_RED"Please choose another name for your branch!"ANSI_RESET"\n");
        return;
    }
    char branchAddress[MAX_LENGTH_STRING];
    sprintf(branchAddress, "%s/%s", branchs, branchName);
    mkdir(branchAddress, 0755);
    
    char head_commit[MAX_LENGTH_STRING];
    char commits[MAX_LENGTH_STRING];
    sprintf(head_commit, "%s/HEAD_COMMIT.txt", branchAddress);
    sprintf(commits, "%s/COMMITS", branchAddress);
   
    char birth[MAX_LENGTH_STRING];
    sprintf(birth, "%s/BORN.txt",  branchAddress);
    FILE* BIRTH = fopen(birth, "w");
    fprintf(BIRTH, "BIRTH COMMIT ID : %X\n",  head);
    fclose(BIRTH);
   
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

    printf(ANSI_RED ANSI_BOLD"%s"ANSI_RESET" Branch created successfully!\n", branchName);
    printf("Birth Commit ID of "ANSI_RED ANSI_BOLD"%s"ANSI_RESET" is "ANSI_CYAN"%X"ANSI_RESET"\n", branchName, head);
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
    char merged_branchs[MAX_LENGTH_STRING];
    char merge_info[MAX_LENGTH_STRING];


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
    sprintf(merged_branchs, "%s/.MERGED_BRANCHS", neogitDir);
    sprintf(merge_info, "%s/.MERGE_INFO", neogitDir);


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
    mkdir(merged_branchs, 0755);
    mkdir(merge_info, 0755);

}
void init() {
    char cwd[200];
    getcwd(cwd, sizeof(cwd));
    if (IS_INITED()) {
        printf(ANSI_BACK_RED"noegit is already initialized in this folder!"ANSI_RESET"\n");
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
    char current_commmit[MAX_LENGTH_STRING];
    sprintf(current_commmit, "%s/CURRENT_COMMIT.txt", neoGitDir);
    FILE* CUR = fopen(current_commmit, "w");
    fprintf(CUR, "0");
    fclose(CUR);
    make_branch("master", 0);
    char base_0[MAX_LENGTH_STRING];
    sprintf(base_0, "%s/.neogit/.REPO_ON_COMMIT/0", IS_INITED());
    mkdir(base_0, 0755);
    base_empty_directories(IS_INITED(), base_0);


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


string read_info_from_commit(Uint commit_id, string what_you_want)
{
    char commits[MAX_LENGTH_STRING];
    sprintf(commits, "%s/.neogit/.COMMIT_INFO", IS_INITED());
    DIR* COMMITS = opendir(commits);
    struct dirent* entry;

    while(entry = readdir(COMMITS)) {
        string time = make_string(MAX_LENGTH_STRING);
        string message = make_string(MAX_LENGTH_STRING);
        string name = make_string(MAX_LENGTH_STRING);
        string email= make_string(MAX_LENGTH_STRING);
        string branch = make_string(50);
        Uint id = 0;

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        char file[MAX_LENGTH_STRING];
        sprintf(file, "%s/%s", commits, entry->d_name);
        FILE* fp = fopen(file, "r");
        char line[MAX_LENGTH_STRING];
        fgets(line, MAX_LENGTH_STRING,  fp);
        sscanf(line, "Date & Time : %[^\n]", time);

        fgets(line, MAX_LENGTH_STRING,  fp);
        sscanf(line, "Commit Message : %[^\n]", message);

        fgets(line, MAX_LENGTH_STRING,  fp);
        sscanf(line, "Author Name : %[^\n]", name);

        fgets(line, MAX_LENGTH_STRING,  fp);
        sscanf(line, "Author Email : %[^\n]", email);

        fgets(line, MAX_LENGTH_STRING,  fp);
        sscanf(line, "Branch : %[^\n]", branch);

        fgets(line, MAX_LENGTH_STRING,  fp);
        sscanf(line, "COMMIT ID(HASH) : %X", &id);

        fclose(fp);

        if(id == commit_id) {
            if(strcmp(what_you_want, "branch") == 0) {
                return branch;
            }
            else if(strcmp(what_you_want, "name") == 0) {
                return name;
            }
            else if(strcmp(what_you_want, "email") == 0) {
                return email;
            }
            else if(strcmp(what_you_want, "message") == 0) {
                return message;
            }
            else if(strcmp(what_you_want, "time") == 0) {
                return time;
            }
            else {
                printf("wrong input!!\n");
                return NULL;
            }
        }
        else {
            remove_string(message);
            remove_string(name);
            remove_string(email);
            remove_string(time);
            remove_string(branch);
        }

    }

}

void ADD_FUNC(string input)
{
    char cwd[MAX_LINE_LENGTH];
    getcwd(cwd, MAX_LINE_LENGTH);

    char unstage[MAX_LENGTH_STRING];
    sprintf(unstage, "%s/.neogit/.UNSTAGED", IS_INITED());

    if (find_file(cwd, input) == 0) {
        printf(ANSI_BACK_RED"No such a file or directory!"ANSI_RESET"\n");
        return;
    }
    char check[MAX_LENGTH_STRING];
    sprintf(check, "%s/%s", cwd, input);
    if (check_is_dir(check)) {
        char dest[MAX_LENGTH_STRING];
        sprintf(dest, "%s/.neogit/.STAGING_AREA%s", IS_INITED(), find_file_path(cwd, IS_INITED()));
        COPY_DIR(cwd, dest, input);
        if(find_file(unstage, input)) {
            char unstaged_directory[MAX_LENGTH_STRING];
            sprintf(unstaged_directory, "%s/%s", unstage, input);  
            remove_directory(unstaged_directory);
        }
    } 
    else {
        char destination[MAX_LENGTH_STRING];
        sprintf(destination, "%s/.neogit/.STAGING_AREA%s", IS_INITED(), find_file_path(cwd, IS_INITED()));

        COPY_FILE(cwd, destination, input);
        if(find_file(unstage,  input)) {
            char unstaged_file[MAX_LENGTH_STRING];
            sprintf(unstaged_file, "%s/%s", unstage, input);
            remove(unstaged_file);
        }
    }

    printf(ANSI_CYAN"%s added to stagging area successfully and it is ready to commit!"ANSI_RESET"\n", input);
}
void ADD_LIST(char list[][MAX_LENGTH_STRING], int number)
{
    char add_info[MAX_LENGTH_STRING];
    sprintf(add_info, "%s/.neogit/.ADD_INFO", IS_INITED());
    DIR* ADD_INFO = opendir(add_info);
    if (ADD_INFO == NULL) {
        perror(ANSI_BACK_RED"failed to open this folder!"ANSI_RESET);
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
    if(!ADD_TXT) {
        perror(ANSI_BACK_RED"fopen failed in showCommitInfo()"ANSI_RESET);
        return;
    }
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

    char source[MAX_LENGTH_STRING];
    char destination[MAX_LENGTH_STRING];
    sprintf(source , "%s/.neogit/.STAGING_AREA", IS_INITED());
    sprintf(destination, "%s/.neogit/.UNSTAGED", IS_INITED());

    char cwd[MAX_LENGTH_STRING];
    getcwd(cwd, MAX_LENGTH_STRING);
    char check[MAX_LENGTH_STRING];
    sprintf(check, "%s/%s", source, input);


    if (check_is_dir(check)) {
        COPY_DIR(source, destination, input);
        remove_directory(check);
    } 
    else {
        COPY_FILE(source, destination, input);
        remove(check);
    }

    printf(ANSI_CYAN"%s is unstage now!!"ANSI_RESET"\n", input);
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
            char command[MAX_LENGTH_STRING];
            sprintf(command, "%s/.neogit/.UNSTAGED/%s", IS_INITED(), entry->d_name);
            remove_directory(command);
        } 
        else {
            COPY_FILE(unstage, stage, entry->d_name);
            char command[MAX_LENGTH_STRING];
            sprintf(command, "%s/.neogit/.UNSTAGED/%s", IS_INITED(), entry->d_name);
            remove(command);
            printf(ANSI_CYAN"%s is now staged and ready to commit!"ANSI_RESET"\n", entry->d_name);
        }
    }

}

void UNDO_FUNC()
{
    char add_info[MAX_LENGTH_STRING];
    sprintf(add_info, "%s/.neogit/.ADD_INFO", IS_INITED());
    DIR* ADD_INFO = opendir(add_info);
    if (ADD_INFO == NULL) {
        perror(ANSI_BACK_RED"failed to open this folder!"ANSI_RESET);
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
    printf(ANSI_CYAN"successfull undo!\n"ANSI_RESET);

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
                printf(ANSI_FAINT"file name : %s --- status : -T"ANSI_RESET"\n", entry->d_name);
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
                printf(ANSI_FAINT"file name : %s --- status : +T"ANSI_RESET"\n", entry->d_name);
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
    printf(ANSI_BLINK ANSI_UNDERLINE"your commit was successfull!"ANSI_RESET"\n");
    printf(ANSI_GREEN"Commit Date & Time : %s\n", cur_time);
    printf("Commit ID : %X\n", commit_id);
    printf("Commit Message : %s"ANSI_RESET"\n", commit_message);  
}
void COMMIT_FUNC(string message)
{
    char current[MAX_LENGTH_STRING];
    sprintf(current, "%s/.neogit/CURRENT_COMMIT.txt", IS_INITED());
    FILE* CURRRENT = fopen(current, "r");
    Uint cur_commit = 0;
    fscanf(CURRRENT, "%X", &cur_commit);
    if(cur_commit != find_head()) {
        printf(ANSI_BACK_RED"you can't commit know ,please go back to head and then commit!"ANSI_RESET"\n");
        printf("%X -- %X\n", cur_commit, find_head());
        return;
        
    }
    fclose(CURRRENT);
    
    int nums = 0;
    char stage[MAX_LENGTH_STRING];
    sprintf(stage, "%s/.neogit/.STAGING_AREA", IS_INITED());
    number_of_staging(stage, &nums);
    if(nums == 0) {
        perror(ANSI_BACK_RED"stage is empty please first add something to stage then commit!!"ANSI_RESET);
        return;
    }


    char infoAddress[MAX_LENGTH_STRING];
    sprintf(infoAddress, "%s/LOCAL_INFO/%s.txt", CONFIG_ADDRESS(), name_project());
    FILE* info = fopen(infoAddress, "r");

    string line = make_string(MAX_LENGTH_STRING);
    char name[MAX_LENGTH_STRING];
    char email[MAX_LENGTH_STRING];   
    Uint commit_id = giveRandomNumber();
 
    char cur_branch[MAX_LENGTH_STRING];
    Uint prev_commit;

    fgets(line, MAX_LENGTH_STRING, info);
    sscanf(line, "user.name : %[^\n]", name);
    if(strlen(name) == 0) {
        perror(ANSI_BACK_RED"Author name is empty!!"ANSI_RESET);
        return;
    }
    fgets(line, MAX_LENGTH_STRING, info);
    sscanf(line, "user.email : %[^\n]", email);
    if(strlen(email) == 0) {
        perror(ANSI_BACK_RED"Author email is empty!!"ANSI_RESET);
        return;
    }
    fgets(line , MAX_LENGTH_STRING, info);
    sscanf (line , "branch : %[^\n]", cur_branch);

    fgets(line, MAX_LENGTH_STRING, info);
    sscanf(line, "commit_id : %X\n", &prev_commit);

    fclose(info);

    make_commit(prev_commit, cur_branch, message, name, email);

    char current_commmit[MAX_LENGTH_STRING];
    sprintf(current_commmit, "%s/.neogit/CURRENT_COMMIT.txt", IS_INITED());
    FILE* CUR = fopen(current_commmit, "w");
    fprintf(CUR, "%X", commit_id);
    fclose(CUR);

}



void SET_FUNC(string message, string shortcut) 
{
    char shrtct_path[MAX_LENGTH_STRING];
    sprintf(shrtct_path, "%s/.neogit/.COMMIT_SET/%s.txt", IS_INITED(), shortcut);
    FILE* SHORTCUT = fopen(shrtct_path, "w");
    fputs(message, SHORTCUT);
    fclose(SHORTCUT);
    printf(ANSI_BACK_BLUE"%s shortcut successfully is setted for <%s> message!"ANSI_RESET"\n", shortcut, message);
    printf(ANSI_BACK_BLUE"you can use this shortcut now with -s flag"ANSI_RESET"\n");
}
void REMOVE_SHORTCUT(string shorutcut)
{
    int ret;
    FILE *fp;
    char filename[MAX_LENGTH_STRING];
    sprintf(filename, "%s/.neogit/.COMMIT_SET/%s.txt", IS_INITED(), shorutcut);
   
    ret = remove(filename);

    if(ret == 0) {
        printf(ANSI_CYAN"Shortcut Message deleted successfully!"ANSI_RESET"\n");
    } else {
        printf(ANSI_BACK_RED"Error: unable to delete the Shortcut Message!"ANSI_RESET"\n");
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
    int nums = 0;
    char stage[MAX_LENGTH_STRING];
    sprintf(stage, "%s/.neogit/.STAGING_AREA", IS_INITED());
    number_of_staging(stage, &nums);
    if (nums == 0) return true;
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
        return;
    }

    char head[MAX_LENGTH_STRING];
    sprintf(head, "%s/.neogit/.BRANCHES/%s/HEAD_COMMIT.txt", IS_INITED(), branch);
    FILE* HEAD = fopen(head, "r");
  
    Uint head_id;
    fscanf(HEAD, "HEAD COMMIT ID : %X\n", &head_id);

    fclose(HEAD);

    for(int i = 0; i < 4; i++) {
        char temp[MAX_LENGTH_STRING];
        fgets(temp, MAX_LENGTH_STRING, INFO);
        if(i == 2) {
            sprintf(temp, "branch : %s\n", branch);
        }
        if(i == 3) {
            sprintf(temp, "commit_id : %x\n", head_id);
        }
        fputs(temp, TMP_INFO);
    }

    fclose(INFO);
    fclose(TMP_INFO);
    remove(info);
    rename(temp_info, info);


    
    change_folder(head_id);

    char current_commmit[MAX_LENGTH_STRING];
    sprintf(current_commmit, "%s/.neogit/CURRENT_COMMIT.txt", IS_INITED());
    FILE* CUR = fopen(current_commmit, "w");
    fprintf(CUR, "%X", head_id);
    fclose(CUR);

    printf(ANSI_BOLD ANSI_CYAN"You are now on Head of %s"ANSI_RESET"\n", branch);
}
void COMMIT_CHECKOUT(Uint id)
{   
    char current_commmit[MAX_LENGTH_STRING];
    sprintf(current_commmit, "%s/.neogit/CURRENT_COMMIT.txt", IS_INITED());
    FILE* CUR = fopen(current_commmit, "w");
    fprintf(CUR, "%X", id);
    fclose(CUR);

    change_folder(id);

    printf("You are now in Commit "ANSI_BOLD ANSI_CYAN"%X"ANSI_RESET" on Branch : "ANSI_BOLD ANSI_CYAN"%s"ANSI_RESET"!\n", id, read_info_from_commit(id, "branch"));
    printf(ANSI_BOLD ANSI_BOLD "Please notice that you "ANSI_UNDERLINE ANSI_FAINT ANSI_RED"can not Commit"ANSI_RESET ANSI_BOLD ANSI_BOLD" anything until you Checkout to Head!"ANSI_RESET"\n");
}
void CHECKOUT(string input)
{
    if(!EVERYTHING_IS_COMMITED()) {
        perror(ANSI_BACK_RED"please first commit then checkout!"ANSI_RESET);
        exit(1);
    }
    char branch_dest[MAX_LENGTH_STRING];
    sprintf(branch_dest, "%s/.neogit/.BRANCHES", IS_INITED());

    char commit_dest[MAX_LENGTH_STRING];
    sprintf(commit_dest, "%s/.neogit/.REPO_ON_COMMIT", IS_INITED());
   
    if(find_file(branch_dest, input)) {
        BRANCH_CHECKOUT(input);
        printf(ANSI_CYAN"Checkout was successfull\nyou are now on branch : %s"ANSI_RESET"\n", input);
    }
    else if(find_file(commit_dest, input)) {
        Uint id;
        sscanf(input, "%X", &id);
        COMMIT_CHECKOUT(id);
        printf(ANSI_CYAN"Checkout was successfull\nyou are now on commit : %X"ANSI_RESET"\n", id);
    }
    else {
        printf(ANSI_BACK_RED"%s is not a commit id or branch name!!"ANSI_RESET"\n", input);
        return;
    }
}



void PRINT_LOG(int num_commit, string address)
{
    
    FILE* fp = fopen(address, "r");
    printf(ANSI_YELLOW"--------------start--------------"ANSI_RESET"\n");
    printf(ANSI_BLUE"commit %d :"ANSI_RESET"\n", num_commit);
    char line[MAX_LENGTH_STRING];
    while(fgets(line, MAX_LENGTH_STRING, fp)) {
        printf(ANSI_FAINT"%s"ANSI_RESET, line);
    }
    fclose(fp);
    printf(ANSI_YELLOW"---------------end---------------"ANSI_RESET"\n\n");
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
        printf(ANSI_CYAN ANSI_BOLD"%s"ANSI_RESET"\n", temp);
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
        perror(ANSI_BACK_RED"there is no tag with this tag-name!"ANSI_RESET);
        return;
    }
    
    char tag_address[2 * MAX_LENGTH_STRING];
    sprintf(tag_address, "%s/%s", tags, tag_text);
    FILE* TAG = fopen(tag_address, "r");
    if(TAG == NULL) {
        perror(ANSI_BACK_RED"failed to open tag file!"ANSI_RESET);
        return;
    }

    printf("\n"ANSI_CYAN ANSI_BOLD"tag : %s"ANSI_RESET"\n", tag_name);
    for(int i = 0; i < 4; i++) {
        char line[MAX_LENGTH_STRING];
        fgets(line, MAX_LENGTH_STRING, TAG);
        printf(ANSI_FAINT"%s"ANSI_RESET, line);
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
        perror(ANSI_BACK_RED"this commit id does not exist!"ANSI_RESET);
        return;
    }
    char cwd[MAX_LENGTH_STRING];
    getcwd(cwd, MAX_LENGTH_STRING);
    char path_in_repo[MAX_LENGTH_STRING];
    sprintf(path_in_repo, "%s/.neogit/.REPO_ON_COMMIT/%X%s/%s", IS_INITED(), commit_id, find_file_path(cwd, IS_INITED()), file);
    
    FILE* FILE_ON_COMMIT = fopen(path_in_repo, "r");
    if(FILE_ON_COMMIT == NULL) {
        perror(ANSI_BACK_RED"file does not exist in the current repository!"ANSI_RESET);
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
        perror(ANSI_BACK_RED"few inputs"ANSI_RESET"\n"ANSI_BACK_RED"please enter at least one file and a word."ANSI_RESET);
        return;
    }
    if(strcmp(input[0], "-f") ||  strcmp(input[2], "-p")) {
        perror(ANSI_BACK_RED"invalid inputs!"ANSI_RESET);
        return;
    }
    if(arguments == 4) {
        GREP(input[1], input[3], find_head(), false);
    }
    else if(arguments == 5) {
        if(strcmp(input[4], "-n")) {
            perror(ANSI_BACK_RED"invalid inputs!"ANSI_RESET);
            return;
        }
        GREP(input[1], input[3], find_head(), true);
    }
    else if(arguments == 6) {
        if(strcmp(input[4], "-c")) {
            perror(ANSI_BACK_RED"invalid inputs!"ANSI_RESET);
            return;
        }
        Uint commit_id;
        sscanf(input[5], "%X",  &commit_id);
        GREP(input[1], input[3], commit_id, false);
    }
    else if(arguments == 7) {
        if(strcmp(input[4], "-c") || strcmp(input[6], "-n")) {
            perror(ANSI_BACK_RED"invalid inputs!"ANSI_RESET);
            return;
        }
        Uint commit_id;
        sscanf(input[5], "%X",  &commit_id);
        GREP(input[1], input[3], commit_id, true);
    } else {
        perror(ANSI_BACK_RED"too many inputs!"ANSI_RESET);
        return;
    }
}


void REVERT(string message, Uint commit_id, bool commit)
{
    COMMIT_CHECKOUT(commit_id);

    char curr[MAX_LENGTH_STRING];
    sprintf(curr, "%s/.neogit/CURRENT_COMMIT.txt", IS_INITED());
    FILE* CURR = fopen(curr, "w");
    fprintf(CURR, "%X", find_head());
    fclose(CURR);
    

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
        if(a != 0) perror(ANSI_BACK_RED"ERROR: Failed to create temporary staging area"ANSI_RESET);
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
        
        printf(ANSI_BLUE"Revert was successfull!"ANSI_RESET"\n");

    }

}
void CHECK_MERGE(string message, Uint commit_id, bool commit)
{
    char commits[MAX_LENGTH_STRING];
    sprintf(commits, "%s/.neogit/.COMMITS", IS_INITED());

    char check[MAX_LENGTH_STRING];
    sprintf(check, "%X",  commit_id);
    if(!find_file(commits,  check)) {
        fprintf(stderr, ANSI_BACK_RED"There is no commit with this commit id!!"ANSI_RESET"\n");
        return;
    }

    string branch = read_info_from_commit(commit_id, "branch");
    printf("%s\n", branch);
    
    char merge[MAX_LENGTH_STRING];
    sprintf(merge, "%s/.neogit/.MERGE_INFO", IS_INITED());
    DIR* MERGE = opendir(merge);
    struct dirent* entry;
    
    while(entry = readdir(MERGE)) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        char info_file[MAX_LENGTH_STRING];
        sprintf(info_file, "%s/%s", merge, entry->d_name);
        FILE* fp = fopen(info_file, "r");
        char line[MAX_LINE_LENGTH];

        Uint base_commit;
        char base[100];
        fgets(line, MAX_LINE_LENGTH, fp);
        sscanf(line, "%X -> %[^\n]",  &base_commit, base);

        Uint merge_commit;
        char merged[100];
        fgets(line, MAX_LINE_LENGTH, fp);
        sscanf(line,"%X -> %s",&merge_commit, merged);

        fclose(fp);
        if(strcmp(base,  branch) == 0) {
            string time_new_commit = read_info_from_commit(commit_id, "time");
            string time_merged_commit = read_info_from_commit(base_commit, "time");
            if(compare_date_time_strings(time_new_commit, time_merged_commit) == -1) {
                printf(ANSI_BACK_RED"there is a merge in this branch after this commit, so you can not revert to this commit!"ANSI_RESET"\n");
                return;
            }
            
        }
        else if(strcmp(merged, branch) == 0) {
            printf(ANSI_BACK_RED"this commit is for a merged branch and you can not revert to it!"ANSI_RESET"\n");
            return;
        }

    }
    closedir(MERGE);

    REVERT(message, commit_id, commit);
}
void REVERT_ANALYZE(char* input[], int arguments) 
{
    if(arguments == 0) {
        CHECK_MERGE(NULL, find_head(), true);
    }
    else if(arguments == 1) {
        if(strcmp(input[0], "-n") == 0) {
            CHECK_MERGE(NULL, find_head(), false);
            
        }
        else if(strncmp(input[0], "HEAD-",  5) == 0) {
            int num = 0;
            sscanf(input[0],  "HEAD-%d", &num);
            CHECK_MERGE(NULL, find_commit(num), true);
        }
        else {
            Uint commit_id;
            sscanf(input[0], "%X", &commit_id);
            CHECK_MERGE(NULL, commit_id,  true);
        }
    }
    else if(arguments == 2) {
        if(strcmp(input[0], "-m") == 0) {
            CHECK_MERGE(input[1],  find_head(), true);
        }
        else if(strcmp(input[0], "-n") == 0) {
            Uint commit_id;
            sscanf(input[1], "%X", &commit_id);
            CHECK_MERGE(NULL, commit_id,  false);
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
void COMPARE_COMMITS(string repo1, string repo2, string id1, string id2, bool compare_files)
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
                COMPARE_COMMITS(new_repo1, new_repo2, id1, id2, compare_files);
            } else {
                if(is_text_file(new_repo1) && is_text_file(new_repo2) && compare_files)
                DIFF_FILE(new_repo1, new_repo2, 0, (int)1e9, 0, (int)1e9);
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
 
    COMPARE_COMMITS(repo_on_commit_1, repo_on_commit_2, first, second, true);
    COMPARE_COMMITS(repo_on_commit_2, repo_on_commit_1,  second, first, false);
    

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


    BRANCH_CHECKOUT(find_branch());
    printf(ANSI_BOLD ANSI_BOLD ANSI_BLUE"new stash is created!"ANSI_RESET"\n");
    printf("This stash is created on "ANSI_YELLOW"%s"ANSI_RESET" Branch and "ANSI_YELLOW"%X"ANSI_RESET" Commit\n", find_branch(), find_head());
    if(message) printf("Message : <"ANSI_YELLOW"%s"ANSI_RESET">\n", message);
    else printf("No message provided for stash!\n");
   
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
        printf("stash index : "ANSI_MAGENTA"%d"ANSI_RESET"\n", i);
        char text[2 * MAX_LENGTH_STRING];
        sprintf(text, "%s/%d/text.txt", stashs, i);
        FILE* INFO = fopen(text, "r");
        char line[MAX_LENGTH_STRING];
        while(fgets(line, MAX_LENGTH_STRING, INFO)) {
            printf(ANSI_YELLOW"%s"ANSI_RESET, line);
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
                DIFF_FILE(new_repo1, new_repo2, 0, (int)1e9, 0, (int)1e9);
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

    printf("stash with "ANSI_YELLOW"%d"ANSI_RESET" index have been applied on your folder successfully and it is removed from stashs stack!\n");    
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
    
    printf(ANSI_ITALIC ANSI_MAGENTA"All of stashs are removed from stash stack successfully!"ANSI_RESET"\n");
}
void STASH_DROP(int index)
{
    char stashs[MAX_LENGTH_STRING];
    sprintf(stashs, "%s/.neogit/.STASHS", IS_INITED());
    DIR* STASHS = opendir(stashs);
    struct dirent* entry;
   
   
    char check[5];
    sprintf(check, "%d", index); 
    if(find_file(stashs, check) == 0) {
        printf(ANSI_BACK_RED"There is no stash with this index!"ANSI_RESET"\n");
        return;
    }
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
    printf("Stash with "ANSI_BOLD ANSI_BOLD ANSI_YELLOW"< %d >"ANSI_RESET" is dropped from the stash stack.\n", index);
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
bool print_conflict_lines(char line1[], char line2[], int num1, int num2, string branch1, string branch2, string file_address, string mergedFile)
{
    FILE* MERGED = fopen(mergedFile, "a");
       
    if(strcmp(line1, line2) == 0) {
        fputs(line1, MERGED);
        fclose(MERGED);
        return true;
    } 
    printf(ANSI_BLUE"<%s>"ANSI_RESET"\n", file_address);
    printf(ANSI_BACK_CYAN"<%s>-<%d>"ANSI_RESET"\n", branch1, num1 + 1);
    if(num_diff(line1, line2, 1) == false)
    
    printf("%s\n", line1);
    printf(ANSI_BACK_CYAN"<%s>-<%d>"ANSI_RESET"\n", branch2, num2 + 1);
    if(num_diff(line1, line2, 2) == false)
    printf("%s\n", line2);
    
    printf("there is a conflict happend here!\n");
    printf("which line do you want in your final file?");
    printf("you have this options :\n");
    printf("line that is in branch : %s (enter 1)\n", branch1);
    printf("line that is in branch : %s (enter 2)\n", branch2);
    printf("you can enter another line : (enter n)\n");
    printf("you can stop merging! (enter q)\n");
    
    char choice;
    scanf(" %c", &choice);
    getchar();
    
    if(choice == '1') {
        fputs(line1, MERGED);
        fclose(MERGED);
    }
    else if(choice == '2') {
        fputs(line2, MERGED);
        fclose(MERGED);
    }
    else if(choice == 'n') {
        printf("you decided to enter new line, please enter your line :\n");
        char input_line[MAX_LINE_LENGTH];
        fgets(input_line, MAX_LINE_LENGTH, stdin);
        fputs(input_line,  MERGED);
        fclose(MERGED);
    }
    else if(choice  == 'q') {
        printf(ANSI_BLINK ANSI_BLUE"merge is canceled!!"ANSI_RESET);
        fclose(MERGED);
        return false;
    }
    else {
        printf(ANSI_RED "wrong option!!\ni decide first line for merge"ANSI_RESET);
        fputs(line1, MERGED);
        fclose(MERGED);
    }
    

    return true;
} 
bool CONFLICT_DIFF(string file1_address, string file2_address, string branch1, string branch2, string file_address, string finalFile)
{
    FILE* first = fopen(file1_address, "r");
    FILE* second = fopen(file2_address, "r");

    char line1[MAX_LENGTH_STRING];
    char line2[MAX_LENGTH_STRING];

    int lineNum1 = -1;
    int lineNum2 = -1;
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
        
        bool result = print_conflict_lines(line1, line2,  lineNum1, lineNum2, branch1, branch2, file_address, finalFile);
        if(result == false) return false;

    }

    FILE* merged = fopen(finalFile, "a");
    
    while(fgets(line1,  MAX_LENGTH_STRING, first)){
        fputs(line1,  merged);
    }
    while(fgets(line2,  MAX_LENGTH_STRING, second)) {
        fputs(line2,merged);
    }
    
    fclose(merged);
    return true;    
}
bool applying_second_commit_on_marge(string repo, string finalRepo, string branch1, string branch2, string repo_address)
{
    DIR* dp = opendir(repo);
    struct dirent* entry;

    while(entry = readdir(dp)) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        char new_repo[MAX_LENGTH_STRING];
        char new_finalRepo[MAX_LENGTH_STRING];
        sprintf(new_repo, "%s/%s", repo, entry->d_name);
        sprintf(new_finalRepo, "%s/%s", finalRepo, entry->d_name);

        if(entry->d_type == DT_DIR) {

            applying_second_commit_on_marge(new_repo, new_finalRepo, branch1, branch2, repo_address);
        }
        else {
            if(find_file(finalRepo, entry->d_name) && is_text_file(new_repo)) {
                char merged[MAX_LENGTH_STRING];
                sprintf(merged, "%s/temp.txt", finalRepo);
                FILE* temp = fopen(merged, "w");
                fclose(temp);
                
                bool okay_conflict =  CONFLICT_DIFF(new_repo, new_finalRepo, branch1, branch2, find_file_path(new_finalRepo, repo_address), merged);
                if(okay_conflict == false) return false;
                
                remove(new_finalRepo);
                rename(merged, new_finalRepo);
            }
            else COPY_FILE(repo, finalRepo, entry->d_name);
        }
    }
    return true;
}
bool making_merged_commit(Uint base, Uint merging, string branch1, string branch2) 
{
    char temp_repo[MAX_LENGTH_STRING];
    sprintf(temp_repo, "%s/.neogit/.REPO_ON_COMMIT/temp_repo", IS_INITED());
    mkdir(temp_repo, 0755);
    
    char base_repo[MAX_LENGTH_STRING];
    sprintf(base_repo, "%s/.neogit/.REPO_ON_COMMIT/%X", IS_INITED(), base);
    
    DIR*  dp = opendir(base_repo);
   
    struct dirent* entry; 
    
    while(entry = readdir(dp)) {
        if(strcmp(entry->d_name , ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        if(entry->d_type == DT_DIR) COPY_DIR(base_repo, temp_repo, entry->d_name);
        else COPY_FILE(base_repo, temp_repo, entry->d_name);
    }
    closedir(dp);

    
    char merging_repo[MAX_LENGTH_STRING];
    sprintf(merging_repo, "%s/.neogit/.REPO_ON_COMMIT/%X", IS_INITED(), merging);
    bool result = applying_second_commit_on_marge(merging_repo, temp_repo, branch1, branch2, temp_repo);
    if(result == false) {
        remove_directory(temp_repo);
        return false;
    }
    remove_directory(base_repo);
    rename(temp_repo, base_repo);

    return true;

}
void remove_stashs_in_merge(string target_branch)
{
    char stashs[MAX_LENGTH_STRING];
    sprintf(stashs, "%s/.neogit/.STASHS", IS_INITED());
    
    DIR *dir = opendir(stashs);
    struct dirent* entry;
    
    while(entry = readdir(dir)) {
        if(strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) continue;

        char text[MAX_LENGTH_STRING];
        sprintf("%s/%s/text.txt", stashs, entry->d_name);
        FILE* info = fopen(text, "r");
        
        char line[MAX_LINE_LENGTH];
        fgets(line , MAX_LINE_LENGTH, info);
        fclose(info);

        char branch[50];
        sscanf(line, "stash was on branch : %[^\n]", branch);
        if(strcmp(target_branch, branch) == 0) {
            STASH_DROP(atoi(entry->d_name));
        }

    }
}
void put_merge_information(string base_branch, Uint base_commit, string merge_branch, Uint merge_commit)
{
    char new_merge[MAX_LENGTH_STRING];
    sprintf(new_merge, "%s/.neogit/.MERGE_INFO/%s.txt",  IS_INITED(), merge_branch);
    FILE* info = fopen(new_merge, "w");

    fprintf(info, "%X -> %s\n", base_commit, base_branch);
    fprintf(info, "%X -> %s\n", merge_commit, merge_branch);

    fclose(info);

}
void MERGE(char base_branch[], char merging_branch[])
{
    //check for valid branches
    char branchs[MAX_LENGTH_STRING];
    sprintf(branchs, "%s/.neogit/.BRANCHES", IS_INITED());
    if (find_file(branchs,  base_branch) == 0 || find_file(branchs, merging_branch) == 0) {
        perror(ANSI_BACK_RED"invalid branch!!"ANSI_RESET);
        return;
    }
    // base branch for merge is always first one but if second one is master we shourld chanhge them 
    if(strcmp(merging_branch, "master") == 0) {
        printf(ANSI_BACK_RED"you can not merge master to another branch!"ANSI_RESET"\n");
        return;
    }
    // get the HEAD of each branch
    Uint head_of_base = head_branch(base_branch);
    Uint head_of_merging = head_branch(merging_branch);
    bool result = making_merged_commit(head_of_base, head_of_merging, base_branch, merging_branch);
    if(result == false) {
        printf("Merge failed\n");
        return;
    }
    char merged_branch[MAX_LENGTH_STRING];
    sprintf(merged_branch, "%s/%s", branchs, merging_branch);

    char is_merged[MAX_LENGTH_STRING];
    sprintf(is_merged, "%s/.neogit/.MERGED_BRANCHS", IS_INITED());
    COPY_DIR(branchs, is_merged, merging_branch);
    remove_directory(merged_branch);
    remove_stashs_in_merge(merging_branch);
    put_merge_information(base_branch, head_of_base, merging_branch, head_of_merging);

    printf(ANSI_BOLD ANSI_CYAN"%s branch merged to %s branch succesfully!"ANSI_RESET"\n",merging_branch, base_branch);
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
        char command[100];
        sprintf(command, "cppcheck --enable=all %s 2>&1", file);

        FILE *pipe = popen(command, "r");
        if (!pipe) {
            fprintf(stderr, "Error: popen() failed");
            return 1;
        }

        char buffer[128];
        int errorCount = 0;

        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            if (strstr(buffer, "error") != NULL) {
                errorCount++;
            }
        }

        pclose(pipe);

        if (errorCount > 0) {
            return -1;
        } else {
            return 1;
        }
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
            if (ch != '\n') {
                count++;
            }
        }

        fclose(file);
        if (count > 20000) return -1;
        return 1;
    }
    else return 0;
}
int is_video_duration_less_than_10_minutes(const char *filename) {
    char command[256];
    snprintf(command, sizeof(command), "ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 %s", filename);

    FILE *fp = popen(command, "r");
    if (!fp) {
        printf("Error: Failed to execute ffprobe command\n");
        return 1;
    }

    char duration_str[16];
    if (fgets(duration_str, sizeof(duration_str), fp) == NULL) {
        printf("Error: Failed to read output from ffprobe command\n");
        pclose(fp);
        return 1;
    }

    pclose(fp);

    double duration = atof(duration_str);
    if (duration > 10 * 60) {
        return 1;
    } else {
        return 0;
    }

    return 1;
    
}
int time_limit(string file_path)
{
    char* file_extension = strrchr(file_path, '.');
    if(strcasecmp(file_extension, ".mp4") == 0 || strcasecmp(file_extension, ".wav") == 0 || strcasecmp(file_extension, ".mp3") == 0) {
        if(is_video_duration_less_than_10_minutes(file_path)) return -1;
        else return 1;
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
    if(print) printf(ANSI_BOLD ANSI_YELLOW"%s"ANSI_RESET"\n", filename);

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
        else if(strcmp(entry->d_name, "character-limit") == 0) {
            status = character_limit(file);
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
    printf(ANSI_CYAN"indentation-check"ANSI_RESET"\n");
    printf(ANSI_CYAN"static-error-check"ANSI_RESET"\n");
    printf(ANSI_CYAN"file-size-check"ANSI_RESET"\n");
    printf(ANSI_CYAN"character-limit"ANSI_RESET"\n");
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
bool check_is_valid_hook(string hook_id)
{
    if(strcmp(hook_id, "todo-check") == 0) return true;
    if(strcmp(hook_id, "eof-blank-space") == 0) return true;
    if(strcmp(hook_id, "format-check") == 0) return true;
    if(strcmp(hook_id, "balance-braces") == 0) return true;
    if(strcmp(hook_id, "indentation-check") == 0) return true;
    if(strcmp(hook_id, "static-error-check") == 0) return true;
    if(strcmp(hook_id, "file-size-check") == 0) return true;
    if(strcmp(hook_id, "character-limit") == 0) return true;
    if(strcmp(hook_id, "time-limit") == 0) return true;

    return false;
    
}
void PRECOMMIT_ADD_HOOK(string hook_id)
{
    char hooks[MAX_LENGTH_STRING];
    sprintf(hooks, "%s/.neogit/.HOOKS", IS_INITED());
    char hook[MAX_LENGTH_STRING];
    sprintf(hook, "%s/%s", hooks, hook_id);

    if(find_file(hooks, hook_id)) {
        printf(ANSI_BACK_RED"< %s > is already in list of applied hooks!"ANSI_RESET"\n", hook_id);
        return;
    }

    if(check_is_valid_hook(hook_id) == false) {
        printf(ANSI_BACK_RED"This is not a valid hook id!"ANSI_RESET"\n");
        return;
    }
    FILE *fp = fopen(hook, "w");
    fclose(fp);

    printf(ANSI_BOLD ANSI_YELLOW"< %s >"ANSI_RESET" added to list of applied hooks successfully!\n", hook_id);
}
void PRECOMMIT_REMOVE_HOOK(char hook_id[])
{
    char hooks[MAX_LENGTH_STRING];
    sprintf(hooks, "%s/.neogit/.HOOKS", IS_INITED());
    if(find_file(hooks, hook_id)) {
        char hook[MAX_LENGTH_STRING];
        sprintf(hook, "%s/%s", hooks, hook_id);
        remove(hook);
        printf(ANSI_BOLD ANSI_YELLOW"< %s >"ANSI_RESET" is successfully removed from list of applied hooks!\n", hook_id);
        return;
    }
    printf(ANSI_BACK_RED"< %s > is not a valid hook id!"ANSI_RESET"\n", hook_id);
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
                    printf(ANSI_YELLOW"%s failed pre-commits test\ndo you want to continue your commit or not ?[Y\\N]"ANSI_RESET"\n", entry->d_name);
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
            char cwd[MAX_LINE_LENGTH];
            getcwd(cwd, MAX_LINE_LENGTH);
            char stage[MAX_LENGTH_STRING];
            sprintf(stage, "%s/.neogit/.STAGING_AREA", IS_INITED());
            show_in_add(depth, 0, cwd, stage);
        }
        else if(strcmp(argv[2], "-redo") == 0) {
            REDO_FUNC();
        }
        else {
            for(int i = 2; i < argc; i++) {
                ADD_FUNC(argv[i]);
                strcpy(added[num_added], argv[i]);
                num_added++;
            }
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
                perror(ANSI_BACK_RED"this shortcut message does not exist!"ANSI_RESET);
                return 1;
            }
            string message = find_message(argv[3]);
            DECISION_FOR_COMMIT(message);
        }
    }
    else if(strcmp(argv[1], "set") == 0) {
        if(argc != 6) {
            perror(ANSI_BACK_RED"not enough arguments!"ANSI_RESET);
            return 1;
        }
        if(strcmp(argv[2], "-m") || strcmp(argv[4], "-s")) {
            perror(ANSI_BACK_RED"invalid input!"ANSI_RESET);
            return 1;
        }
        if(strlen(argv[3]) > 72) {
            printf(ANSI_BACK_RED"Too long message!"ANSI_RESET"\n");
            return 1;
        }
        SET_FUNC(argv[3], argv[5]);
    }
    else if(strcmp(argv[1], "replace") == 0) {
        if(argc != 6) {
            perror(ANSI_BACK_RED"not enough arguments!"ANSI_RESET);
            return 1;
        }
        if(strcmp(argv[2], "-m") || strcmp(argv[4], "-s")) {
            perror(ANSI_BACK_RED"invalid input!"ANSI_RESET);
            return 1;
        }
        char shortcut[MAX_LENGTH_STRING];
        sprintf(shortcut, "%s.txt", argv[5]);
        char set_dir[MAX_LENGTH_STRING];
        sprintf(set_dir, "%s/.neogit/.COMMIT_SET", IS_INITED());
        if(find_file(set_dir, shortcut) == 0) {
            perror(ANSI_BACK_RED"This shortcut message does not exist!"ANSI_RESET);
            return 1;
        }
        SET_FUNC(argv[3], argv[5]);
    }
    else if(strcmp(argv[1], "remove") == 0) {
        if(argc != 4) {
            perror(ANSI_BACK_RED"Not enough arguments!"ANSI_RESET);
            return 1;
        }
        if(strcmp(argv[2], "-s")) {
            perror(ANSI_BACK_RED"Invalid inputs!"ANSI_RESET);
            return 1;
        }
        char shortcut[MAX_LENGTH_STRING];
        sprintf(shortcut, "%s.txt", argv[3]);

        char set_dir[MAX_LENGTH_STRING];
        sprintf(set_dir, "%s/.neogit/.COMMIT_SET", IS_INITED());

        if(find_file(set_dir, shortcut) == 0) {
            perror(ANSI_BACK_RED"This shortcut message does not exist!"ANSI_RESET);
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
                perror(ANSI_BACK_RED"Invlalid inputs!"ANSI_RESET);
                return 1;
            }
            int num = 0;
            sscanf(argv[3], "%d", &num);
            LOG_FUNC(num);

        }
        else if(strcmp(argv[2], "-branch") == 0) {
            if(argc != 4) {
                perror(ANSI_BACK_RED"Invlalid inputs!"ANSI_RESET);
                return 1;
            }
            LOG_BRANCH(argv[3]);
        }
        else if(strcmp(argv[2], "-author") == 0) {
            if(argc != 4) {
                perror(ANSI_BACK_RED"Invlalid inputs!"ANSI_RESET);
                return 1;
            }
            LOG_AUTHOR(argv[3]);
        }
        else if(strcmp(argv[2], "-since") == 0) {
            if(argc != 4) {
                perror(ANSI_BACK_RED"Invalid inputs!"ANSI_RESET);
                return 1;
            }
            LOG_TIME(argv[3], -1);
        }
        else if(strcmp(argv[2], "-before") == 0) {
            if(argc != 4) {
                perror(ANSI_BACK_RED"Invalid inputs!"ANSI_RESET);
                return 1;
            }
            LOG_TIME(argv[3], 1);
        }
        else if(strcmp(argv[2], "-search") == 0) {
            for(int i = 3; i < argc; i++) {
                LOG_SEARCH(argv[i]);
            }
        }
        else {
            perror(ANSI_BACK_RED"Invalid inputs!"ANSI_RESET); 
            return 1;
        }
    }
    else if(strcmp(argv[1], "branch") == 0) {
        if(argc > 3) {
            perror(ANSI_BACK_RED"Not enough arguments!"ANSI_RESET);
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
            perror(ANSI_BACK_RED"Not enough arguments!"ANSI_RESET);
            return 1;
        }
        if(strcmp(argv[2], "HEAD") == 0) {
            COMMIT_CHECKOUT(find_head());
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
                perror(ANSI_BACK_RED"Invalid inputs!"ANSI_RESET);
                return 1;
            }
            show_tag_information(argv[3]);
        }
        else if(strcmp(argv[2], "-a") == 0) {
            TAG_ANALYZE(&argv[3], argc - 4);
        }
        else {
            perror(ANSI_BACK_RED"Invalid inputs!"ANSI_RESET);
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
        if(argc != 5 || strcmp(argv[2], "-b")) {
            perror(ANSI_BACK_RED"Usage : neogit merge -b <branch1-name> <branch2-name>"ANSI_RESET);
            return 1;
        }
        if(argc == 5) {
            MERGE(argv[3], argv[4]);
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

