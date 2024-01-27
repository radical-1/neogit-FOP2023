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
#define GLOBAL 1
#define LOCAL 0

int IS_GLOBAL = LOCAL;


bool check_is_global(char*, int*);
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

int compare_files(string fileName1, string fileName2) 
{
    /*debug this part*/

    FILE *file1 = fopen(fileName1, "rb");
    FILE *file2 = fopen(fileName2, "rb");

    if (file1 == NULL || file2 == NULL) {
        printf("Error: Unable to open files.\n");
        return 1;
    }

    char buffer1[1024];
    char buffer2[1024];

    while (!feof(file1) && !feof(file2)) {
        size_t bytes_read1 = fread(buffer1, 1, sizeof(buffer1), file1);
        size_t bytes_read2 = fread(buffer2, 1, sizeof(buffer2), file2);

        if (memcmp(buffer1, buffer2, bytes_read1 < bytes_read2 ? bytes_read1 : bytes_read2) != 0) {
            printf("Error: Files are not identical.\n");
            fclose(file1);
            fclose(file2);
            return 1;
        }
    }

    if (!feof(file1) || !feof(file2)) {
        printf("Error: Files have different sizes.\n");
        fclose(file1);
        fclose(file2);
        return 1;
    }

    printf("Success: Files are identical.\n");
    fclose(file1);
    fclose(file2);
    return 0;    
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
    /*need to debug*/
    printf("debug this part first!!!!\n");
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
        FILE* GLOB = fopen("/GLOBAL_info.txt", "r+");
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
            fprintf(LOC, "user.name = %s\n", input[1]);
        }
        else if (MOD == EMAIL) {
            char ch[200];
            fgets(ch, 200, LOC);
            fprintf(LOC, "user.email = %s\n", input[1]);
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
        FILE* GLOB = fopen("/GLOBAL_alias.txt", "a");
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
    sprintf(local_info, "%s/.neogit/LOCAL_info.txt", IS_INITED());
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
    fprintf(HEAD_COMMIT, " HEAD COMMIT ID : %X", head);
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
// think about what is needed to add in .neogit directory when you are making it
void Open_dirctories_for_init(char neogitDir[]) 
{
    char repository[MAX_LENGTH_STRING];
    char stage[MAX_LENGTH_STRING];
    char unstage[MAX_LENGTH_STRING];
    char branches[MAX_LENGTH_STRING];
    char commit[MAX_LENGTH_STRING];
    char commit_info[MAX_LENGTH_STRING];
    char commit_set[MAX_LENGTH_STRING];

    sprintf(repository,"%s/.LOCAL_REPOSITORY",neogitDir);
    sprintf(stage,"%s/.STAGING_AREA",neogitDir);
    sprintf(unstage, "%s/.UNSTAGED", neogitDir);
    sprintf(branches, "%s/.BRANCHES", neogitDir);
    sprintf(commit,"%s/.COMMITS",neogitDir);
    sprintf(commit_info, "%s/.COMMIT_INFO", neogitDir);
    sprintf(commit_set, "%s/.COMMIT_SET", neogitDir);

    mkdir(repository, 0755);
    mkdir(stage, 0755);
    mkdir(unstage, 0755);
    mkdir(branches, 0755);
    mkdir(commit, 0755);
    mkdir(commit_info, 0755);
    mkdir(commit_set, 0755);
    
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
    mkdir(neoGitDir, 0755);
    Open_dirctories_for_init(neoGitDir);
    char local_info[MAX_LENGTH_STRING];
    sprintf(local_info, "%s/LOCAL_info.txt", neoGitDir);
    FILE* LOCAL_INFO = fopen(local_info, "w");
    FILE* GLOBAL_INFO = fopen("/GLOBAL_info.txt", "r");
    if (GLOBAL_INFO == NULL){
        printf("error opening");
        exit(1);
    }
    for(int i = 0; i < 4; i++) {
        char line[MAX_LENGTH_STRING];
        fgets(line, MAX_LENGTH_STRING, GLOBAL_INFO);
        fputs(line, LOCAL_INFO);
    }
    char add_info[500];
    sprintf(add_info, "%s/.ADD_INFO", neoGitDir);
    FILE* ADD_INFO = fopen(add_info, "w");
    fclose(GLOBAL_INFO);
    fclose(LOCAL_INFO);
    fclose(ADD_INFO);
    make_branch("master");
    printf("Initialized empty Git repository in %s\n", neoGitDir);
}

void ADD_TXT(char sourceAddress[], char destinationAddress[])
{
    printf("%s\n", destinationAddress);
    FILE *source, *destination;
    char buffer[1024];
    size_t bytes_read;

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
void ADD_PLUS(const char* source, const char* destination) 
{
    DIR *dir = opendir(source);
    if (dir == NULL) {
        perror("Unable to open the directory");
        return;
    }
    /*this is wrong fix it later!!!!!*/
    mkdir(destination, 0755);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char source_path[1024];
        char destination_path[1024];
        char stage_path[1024];
        sprintf(stage_path, "%s/.neogit/.STAGING_AREA", IS_INITED());
        snprintf(source_path, sizeof(source_path), "%s/%s", source, entry->d_name);
        snprintf(destination_path, sizeof(destination_path), "%s/%s", stage_path, entry->d_name);

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

        snprintf(source_path, sizeof(source_path), "%s/%s", source, entry->d_name);

        if (entry->d_type == DT_DIR) {
            if(find_file(destination, entry->d_name) == 1) {
                printf("directory : %s -- status : STAGED\n", entry->d_name);
                show_in_add(depth , cur + 1, source_path, destination);
            } else {
                printf("directory : %s -- status : UNSTAGED\n", entry->d_name);
                show_in_add(depth, cur + 1, source_path, destination);
            }
        } else {
            if(find_file(destination, entry->d_name) == 1)
                printf("file : %s -- status : STAGED\n", entry->d_name);
            else
                printf("file : %s -- status : UNSTAGED\n", entry->d_name);
            
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
    char source[300];
    char destination[300];
    sprintf(source , "%s/.neogit/.STAGING_AREA/%s", IS_INITED(), input);
    sprintf(destination, "%s/.neogit/.UNSTAGED/%s", IS_INITED(), input);
    if (check_is_dir(input)) {
        ADD_PLUS(source, destination);
    } 
    else {
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
    char input[MAX_LENGTH_STRING];
    char total[2 * MAX_LENGTH_STRING];
    int index = 0;
    while(fgets(input, MAX_LENGTH_STRING, file) != NULL) {
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

void STATUS_FUNC(string directory)
{
    /*need debug*/
    char stage[MAX_LENGTH_STRING];
    sprintf(stage, "%s/.neogit/.STAGING_AREA", IS_INITED());
    DIR* CUR_DIR = opendir(directory);
    struct dirent *current;
    while((current = readdir(CUR_DIR)) != NULL) {
        if (strcpy(current->d_name, ".") == 0 || strcpy(current->d_name, "..") == 0 || strcpy(current->d_name, ".neogit") == 0) 
            continue;
        if(find_file(stage, current->d_name)) 
            printf("%s +", current->d_name);
        
        else printf("%s -", current->d_name);

        if (current->d_type == DT_DIR) {
            string new_dir = make_string(MAX_LENGTH_STRING);
            sprintf(new_dir, "%s/%s", directory, current->d_name);
            STATUS_FUNC(new_dir);
            remove_string(new_dir);
        }
    }
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
    struct dirent* stage;
    DIR* STAGING_AREA = opendir(stage_address);
    while((stage = readdir(STAGING_AREA)) != NULL) {
        if (strcmp(stage->d_name, ".") == 0 || strcmp(stage->d_name ,"..") == 0)
            continue;
        num_stage++;
    }
    closedir(STAGING_AREA);

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
            ADD_TXT(source, repo_dest);
        }
        int result = rename(source , destination);
        if(result == 0) {
            printf("File moved successfully.\n");
        } else {
            printf("Unable to move the file.\n");
        }
    }
    char local_info[MAX_LENGTH_STRING];
    sprintf(local_info, "%s/.neogit/LOCAL_info.txt", IS_INITED());
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
    printf("%s\n", head_commit);
    FILE* HEAD = fopen(head_commit, "w");
    
    fprintf(HEAD, "HEAD COMMIT ID : %X\n", commit_id);
    fclose(COMMIT);
    fclose(HEAD);

    printf("your commit was successfull!\nCommit Date & Time : %s\nCommit ID : %X\nCommit Message : %s\n", cur_time, commit_id, commit_message);    
}
void COMMIT_FUNC(string message)
{
    char infoAddress[MAX_LENGTH_STRING];
    sprintf(infoAddress, "%s/.neogit/LOCAL_info.txt", IS_INITED());
    FILE* info = fopen(infoAddress, "r");\

    string line = make_string(MAX_LENGTH_STRING);
    char name[MAX_LENGTH_STRING];
    char email[MAX_LENGTH_STRING];
    char cur_branch[MAX_LENGTH_STRING];
    Uint prev_commit;

    fgets(line, MAX_LENGTH_STRING, info);
    sscanf(line, "user.name = %[^\n]", name);

    fgets(line, MAX_LENGTH_STRING, info);
    sscanf(line, "user.email = %[^\n]", email);

    fgets(line , MAX_LENGTH_STRING, info);
    sscanf (line , "branch : %[^\n]", cur_branch);

    fgets(line, MAX_LENGTH_STRING, info);
    sscanf(line, "commit_id : %X\n", &prev_commit);

    fclose(info);

    make_commit(prev_commit, cur_branch, message, name, email);
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


void change_folder(Uint commit_id)
{

}

bool EVERYTHING_IS_COMMITED()
{
    char stage[MAX_LENGTH_STRING];
    sprintf(stage, "%s/.neogit/.STAGING_AREA", IS_INITED());

    DIR* STAGE = opendir(stage);

    struct dirent* entry;
    int staged = 0;
    
    while((entry = readdir(STAGE)) != NULL) {
        if(strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) continue;
        
        staged++;
    }
    if(staged) return false;

    return true;
}
void BRANCH_CHECKOUT(string branch)
{
    char info[MAX_LENGTH_STRING];
    sprintf(info, "%s/.neogit/LOCAL_info.txt", IS_INITED());
    char temp_info[MAX_LENGTH_STRING];
    sprintf(temp_info, "%s/.neogit/info.txt", IS_INITED());

    FILE* INFO = fopen(info, "r");
    FILE* TMP_INFO = fopen(temp_info, "w");
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
void COMMIT_CHECKOUT(string COMMIT_ID)
{
    Uint id;
    sscanf(COMMIT_ID, "%X", &id);
    
    change_folder(id);
}
void CHECKOUT(string input)
{
    bool is_okay = EVERYTHING_IS_COMMITED();
    if(!is_okay) {
        perror("please first commit then checkout!");
        exit(1);
    }
    char branch_dest[MAX_LENGTH_STRING];
    sprintf(branch_dest, "%s/.neogit/.BRANCHES", IS_INITED());

    char commit_dest[MAX_LENGTH_STRING];
    sprintf(commit_dest, "%s/.neogit/.COMMITS", IS_INITED());

    if(find_file(branch_dest, input)) {
       BRANCH_CHECKOUT(input);
    }
    else if(find_file(commit_dest, input)) {
        COMMIT_CHECKOUT(input);
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
        sscanf(line , "Branch : %s\n", read_branch);
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
    else if(strcmp(argv[crt_arg], "status") == 0) {
        STATUS_FUNC(IS_INITED());
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

        }
        if(strcmp(argv[2], "HEAD-n") == 0) {

        }
        else {
            CHECKOUT(argv[2]);
        }
    }
    else {
        string input = make_string(300);
    }
    printf("end of our project!\n");
    return 0;
}
bool check_is_global(char argv[], int* crt_arg)
{
    if(strcmp(argv, "-global") == 0) {
        IS_GLOBAL = GLOBAL;
        (*crt_arg)++;
        return true;
    }
    return false;
}
