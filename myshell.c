#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_LINE 1024  

//global variables
int exit_code = 0;
char *home_path;


//method to take care of built in commands
void exec_buitin(int argc, char *argv[]){
    if (!strcmp(argv[0], "cd")){
        if (argc == 1){
            chdir(home_path);
        }else if(argc == 2){
            //check for error
            if(chdir(argv[1]) != 0){
                fprintf(stderr, "cd: %s: no such file or directory\n", argv[1]);
                exit_code = 1;
            }
        }else {
            fprintf(stderr, "cd: expects at most one argument\n");
            exit_code = 2;
        }
    }
    //exit
    else {
        if (argc == 1){
            exit(exit_code);
        }else if(argc == 2){
            char *end;
            long code = strtol(argv[1], &end, 10);
            if (end == argv[1]){
                fprintf(stderr, "exit: %s: numeric argument required\n", argv[1]);
                exit_code = 1;
            }else if(code > INT_MAX || code < INT_MIN){
                fprintf(stderr, "exit: %s: value must be between %d and %d\n", argv[1], INT_MIN, INT_MAX);
                exit_code = 2;
            }else{
                exit((int)code);
            }
        }else {
            fprintf(stderr, "exit: expects at most one argument\n");
            exit_code = 3;
        }
    }
}

void exec_command(int argc, char *argv[]) {
    pid_t pid = fork();

    //child process
    if (pid == 0){
        //check for input redirection
        char *clean_argv[argc];
        int arg = 0;
        for (int i = 0; argv[i] != 0; ++i){
            if (!strcmp(argv[i], "<")){
                //check that redirection isnt orphaned
                if(i + 1 < argc){
                    //0644 sets permissions on who can use file
                    int fd = open(argv[++i], O_RDONLY, 0644);
                    if (fd < 0) {
                        //print error message
                        fprintf(stderr, "%s: cannot open file %s \n", argv[0], argv[i]);
                        exit(1);
                    }
                    //change stdin to fd
                    dup2(fd, STDIN_FILENO);
                    //close file
                    close(fd);
                    continue;
                }
            }else if (!strcmp(argv[i], ">")){
                if(i + 1 < argc){
                    int fd = open(argv[++i], O_WRONLY|O_CREAT, 0644);
                    if (fd < 0) {
                        fprintf(stderr, "%s: cannot open file %s \n", argv[0], argv[i]);
                        exit(1);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                    continue;
                }
            }else if (!strcmp(argv[i], ">>")){
                if(i + 1 < argc){
                    int fd = open(argv[++i], O_WRONLY|O_CREAT|O_APPEND, 0644);
                    if (fd < 0) {
                        fprintf(stderr, "%s: cannot open file %s \n", argv[0], argv[i]);
                        exit(1);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                    continue;
                }
            }else {
                clean_argv[arg++] = argv[i];
                continue;
            }
            fprintf(stderr, "%s: orphaned I/O redirect %s\n", argv[0], argv[i]);
            exit(1);

        }
        clean_argv[arg] = 0;
        execvp(clean_argv[0], clean_argv);
        fprintf(stderr, "%s: cannot find executable\n", clean_argv[0]);
        exit(1);
    }
    //parent
    int status;
    waitpid(pid, &status, 0);
    if(WIFEXITED(status)){
        exit_code = WEXITSTATUS(status);
    }else{
        exit_code = 0;
    }
}
 
int main() {
    //get home path
    home_path = getenv("HOME");
    chdir(home_path);

    //create regex pattern
    const char *regexp = "[^[:space:]\" ]+|\"([^\"]*)\"";
    regex_t pattern;
    regcomp(&pattern, regexp, REG_EXTENDED);

    char line[MAX_LINE];
    while (1) { 
        printf("myshell> ");
        fflush(stdout);      
        if (!fgets(line, sizeof(line), stdin))
            break;   
 
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) continue;

        //make array to hold tokenized input
        char *args[MAX_LINE/2];

        //tokenize input
        int arg = 0;
        regmatch_t matches[2];

        char *line_ptr = line;
        //loop over input
        while (regexec(&pattern, line_ptr, 2, matches, 0) != REG_NOMATCH){
            regoff_t start, end;

            if (matches[1].rm_so != -1) {
                start = matches[1].rm_so;
                end   = matches[1].rm_eo;
            } else {
                start = matches[0].rm_so;
                end   = matches[0].rm_eo;
            }
            
            args[arg++] = line_ptr + start;
            line_ptr[end] = 0;
            line_ptr += end+1;
        }

        if(arg == 0) continue;

        args[arg] = 0;

        //check if built in command
        if (!strcmp(args[0], "cd") || !strcmp(args[0], "exit")){
            exec_buitin(arg, args);
        } else {
            exec_command(arg, args);
        }

        // For now, just echo back what the user typed
        //printf("[DEBUG] You entered: %s\n", line);
        //printf("[DEBUG] Command not executed (parsing not implemented)\n");
    }

    //printf("\nGoodbye!\n");
    return 0;
}