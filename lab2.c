#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define CHILDREN 15

int main(void){
    //print parent process ID
    printf("Parent: PID = %d\n", getpid());
    pid_t children[CHILDREN];

    for (int i = 0; i < 15; i++){
        pid_t pid = fork();

        char *argv[6] = {0};
        const char *cmd_label = NULL;

        if(pid<0){
            perror("fork failed");
            return 1;
        }

        if (pid==0){
            if (i == 0){
                //set first child to print name
                cmd_label = "echo \"Hello Esther Law\"";
                argv[0] = "echo";
                argv[1] = "Hello Esther Law";
                argv[2] = NULL;

                //execute command
                printf("Child[%d] PID = %d command = %s\n", i, getpid(), cmd_label);
                execvp(argv[0], argv);

                perror("execvp failed");
                exit(127);
                
            }
            //abort two of the commands
            if (i == 11 || i == 12) {
                printf("Child[%d] PID = %d command = abort()\n", i, getpid());
                abort();
            }

            //fail two child commands
            if(i == 13 || i == 14){
                cmd_label = "command failed";
                argv[0] = "invalid_command";
                argv[1] = NULL;

                //execute command
                printf("Child[%d] PID = %d command = %s\n", i, getpid(), cmd_label);
                execvp(argv[0], argv);

                //display that command failed
                printf("Execution failed");
                exit(127);
            }
            
            //use switch case for unique commands
            switch(i) {
                case 1:
                    cmd_label = "history";
                    argv[0] = "history";
                    argv[1] = NULL;
                    break;
                case 2:
                    cmd_label = "ls -l";
                    argv[0] = "ls";
                    argv[1] = "-l";
                    argv[2] = NULL;
                    break;
                case 3:
                    cmd_label = "pwd";
                    argv[0] = "pwd";
                    argv[1] = NULL;
                    break;
                case 4:
                    cmd_label = "whoami";
                    argv[0] = "whoami";
                    argv[1] = NULL;
                    break;
                case 5:
                    cmd_label = "free";
                    argv[0] = "free";
                    argv[1] = NULL;
                    break;
                case 6:
                    cmd_label = "id";
                    argv[0] = "id";
                    argv[1] = NULL;
                    break;
                case 7:
                    cmd_label = "hostname";
                    argv[0] = "hostname";
                    argv[1] = NULL;
                    break;
                case 8:
                    cmd_label = "uptime";
                    argv[0] = "uptime";
                    argv[1] = NULL;
                    break;
                case 9:
                    cmd_label = "df -h";
                    argv[0] = "df";
                    argv[1] = "-h";
                    argv[2] = NULL;
                    break;
                case 10:
                    cmd_label = "ps";
                    argv[0] = "ps";
                    argv[1] = NULL;
                    break;
                default:
                    cmd_label = "echo Default case";
                    argv[0] = "echo";
                    argv[1] = "Default case";
                    argv[2] = NULL;
                    break;
            }
            //execute command
            printf("Child[%d] PID = %d command= %s\n", i, getpid(), cmd_label);
            execvp(argv[0], argv);
            //error handling
            perror("Execution failed");
            exit(1);
        }else{
            children[i] = pid;
        }
}
    int normal_exit_0 = 0;
    int normal_exit_nonzero = 0;
    int terminated_by_signal = 0;

    //wait for child process
    for (int i = 0; i < CHILDREN; i++) {
        int status = 0;

        pid_t w = waitpid(children[i], &status, 0);
        if (w < 0) {
            perror("waitpid failed");
            continue;
        }
        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            printf("Parent: child[%d] PID=%d exited normally, code=%d\n",
                i, w, code);
            
            //increment counts
            if (code == 0) normal_exit_0++;
            else normal_exit_nonzero++;
        }
        //check if signal terminated
        else if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);

            printf("Parent: child[%d] PID=%d terminated by signal=%d\n",
                i, w, sig);
            // increment count
            terminated_by_signal++;
        }
        else {
            printf("Parent: child[%d] PID=%d ended differently\n", i, w);
        }
    }

    //display summary
    printf("\nSummary (Parent PID=%d)\n", getpid());
    printf("Exited normally with exit code 0: %d\n", normal_exit_0);
    printf("Exited normally with non-zero exit code: %d\n", normal_exit_nonzero);
    printf("Terminated by a signal: %d\n", terminated_by_signal);
    
    return 0;
}