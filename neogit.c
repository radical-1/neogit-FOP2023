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

typedef char* string;
#define make_string(n) (string)calloc(n, sizeof(char))
#define remove_string(str) free(str)
typedef unsigned int Uint;
#define MAX_LENGTH_STRING 500
#define GLOBAL true
#define LOCAL false


void make_empty_directories(string  src, string dest, string name); 
void base_empty_directories(string source, string destination);
Uint find_head();

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



void make_branch(char branchName[])
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
    FILE* INFO = fopen(local_info, "r");
    if(INFO == NULL) {
        perror("unable to open file\n");
        exit(1);
    }
    char temp[MAX_LENGTH_STRING];
    for (int i = 0; i < 4; i++) {
        fgets(temp, MAX_LENGTH_STRING, INFO);
    }
    Uint head;
    Uint prev = 0;
    sscanf(temp, " commit_id : %X\n", &head);
    fprintf(HEAD_COMMIT, "HEAD COMMIT ID : %X", head);
    fclose(HEAD_COMMIT);
    fclose(INFO);
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
    printf("khare besaz khob\n");
    fclose(GLOBAL_INFO);
    fclose(LOCAL_INFO);
    fclose(IS_OKAY_COMMIT);
    fclose(IS_OKAY_CHECKOUT);


    make_branch("master");


    printf("Initialized empty Git repository in %s\n", neoGitDir);
}



void COPY_FILE(char src[], char dest[], char name[])
{
    char destinationAddress[MAX_LENGTH_STRING];
    sprintf(destinationAddress, "%s/%s", dest, name);

    FILE *source, *destination;


    char buffer[1024];
    size_t bytes_read;


    char sourceAddress[MAX_LENGTH_STRING];
    sprintf(sourceAddress, "%s/%s", src, name);


    source = fopen(sourceAddress, "rb");
    if (source == NULL) {
        printf("Error: Could not open source file\n");
        exit(1);
    }

    destination = fopen(destinationAddress, "wb");
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
    if (cur == 0) printf("-----------------------\n");
    if(depth == cur) {
        printf("-----------------------\n");
        return;
    }
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
             if(find_file(destination, entry->d_name) == 1)
                printf("file : %s -- status : STAGED\n", entry->d_name);
            else
                printf("file : %s -- status : UNSTAGED\n", entry->d_name);

            show_in_add(depth , cur + 1, source_path, destination_path);
            
        } else {
            if(find_file(destination, entry->d_name) == 1)
                printf("file : %s -- status : STAGED\n", entry->d_name);
            else
                printf("file : %s -- status : UNSTAGED\n", entry->d_name);
            
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


    char last_add[MAX_LENGTH_STRING];
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
                printf("file name : %s --- status : -D\n", entry->d_name);
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
                printf("file name : %s --- status : -T\n", entry->d_name);
            }
            else if(compare_files(file_in_repo, file_in_unstage) == false) {
                printf("file name : %s --- status : -M\n", entry->d_name);
            } 
        } else {
            printf("file name : %s --- status : -A\n", entry->d_name);
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
                printf("file name : %s --- status : +T\n", entry->d_name);
            }
            else if(compare_files(file_in_repo, file_in_stage) == false) {
                printf("file name : %s --- status : +M\n", entry->d_name);
            } 
        } else {
            printf("file name : %s --- status : +A\n", entry->d_name);
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
    //printf("%s\n%s\n%s\n", last_repo, stage, unstage);
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
            //fill this part!!
        } else {
            COPY_FILE(stage_address, repo_address, stg->d_name);
        }
        rename(source , destination);
   
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

        printf("branch name : %s\n", entry->d_name);
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
            printf("%X & %X --> %X\n", prev, temp, id);
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
    if(IS_INITED()) {
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
    int crt_arg = 1; //current argument
    if (strcmp(argv[crt_arg], "init") == 0) {
        init();
        return 0;
    }
    else if (strcmp(argv[crt_arg], "config") == 0) {
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
    if(strcmp(argv[crt_arg], "add") == 0) {
        crt_arg++;
        int num_added = 0;
        char added[20][MAX_LENGTH_STRING];
        if(strcmp(argv[crt_arg], "-f") == 0) {
            for (int i = crt_arg + 1; i < argc; i++) {
                ADD_FUNC(argv[i]);
                strcpy(added[num_added], argv[i]);
                num_added++;
            }
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
            strcpy(added[num_added], argv[crt_arg]);
            num_added++;
        }
        ADD_LIST(added, num_added);
    }
    else if (strcmp(argv[crt_arg], "reset") == 0) {
        crt_arg++;
        if(strcmp(argv[crt_arg], "-undo") == 0) {
            UNDO_FUNC();
        }
        else if(strcmp(argv[crt_arg], "-f") == 0) {
            for (int i = crt_arg + 1; i < argc; i++) {
                RESET_FUNC(argv[i]);
            }
        } else {
            RESET_FUNC(argv[crt_arg]);
            char command[200];
            
        }
    }
    else if(strcmp(argv[crt_arg], "status") == 0) {
        RUN_STATUS();
    }
    else if(strcmp(argv[crt_arg], "commit") == 0) {
        if(strcmp(argv[2], "-m") == 0) {
            if(argc > 4) {
                printf("to many arguments!\n");
                return 1;
            }
            COMMIT_FUNC(argv[3]);
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
            COMMIT_FUNC(message);
        }
    }
    else if(strcmp(argv[crt_arg], "set") == 0) {
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
    else if(strcmp(argv[crt_arg], "replace") == 0) {
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
    else if(strcmp(argv[crt_arg], "remove") == 0) {
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
    else if(strcmp(argv[crt_arg], "log") == 0) {
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
    else if(strcmp(argv[crt_arg], "branch") == 0) {
        if(argc > 3) {
            perror("not enough arguments!\n");
            return 1;
        }
        if (argc == 2) {
            PRITNT_BRANCHES();
        }
        else {
            make_branch(argv[2]);
        }
    }
    else if(strcmp(argv[crt_arg], "checkout") == 0) {
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
    else {
        RUN_ALIAS(argv[1]);
    }

    return 0;
}
