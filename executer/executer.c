/*
    © Vazquez Gutierrez Marcelo (Canti). All rights reserved.

    The following programm works as an executer. That is, this program can
    summon other programs, with the plus of showing debug data such as PID
    and PPID, and if the program concluded successfully or not.
    If not, the programm allows to show not only the exit code, but also a
    message of what that code means.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    char input[200];
    char *args[12];
    int status;
    pid_t pid;

    while(1)
    {
        // Prints 'Executer$' in bold and blue.
        printf("\033[1;34mExecuter$\033[0m ");
        fgets(input, 200, stdin);
        input[strlen(input)-1] = '\0';

        if(strcmp(input,"exit") == 0)
            break;

        /*
            Split user input into 'words' usign blank space
            as spliting token
        */
        int i = 0;
        args[i] = strtok(input, " ");
        while(args[i]!=NULL)
        {
            i++;
            args[i] = strtok(NULL, " ");
        }

        pid = fork();
        if (pid == 0)
        {
            // Child Process
            execv(args[0], args);
            exit(255);
        }
        else
        {
            // Parent Process
            int w = wait(&status);

            if (WIFEXITED(status) && !WEXITSTATUS(status))
            {
                printf("\nProcess \033[1m%d\033[0m with parent \033[1m%d\033[0m has concluded successfully.\n", w, getpid());
            }
            else
            {
                printf("\nProcess \033[1m%d\033[0m exited with error code \033[1m%d\033[0m.\n", w, WEXITSTATUS(status));
                switch (WEXITSTATUS(status))
                {
                    case 2:
                        printf("Missing arguments.\n");
                        break;
                    
                    case 3:
                        printf("Too many arguments.\n");
                        break;

                    case 4:
                        printf("Number must be equal or greater than zero.\n");
                        break;
                        
                    case 5:
                        printf("Number too big.\n");
                        break;

                    case 6:
                        printf("Not a number.\n");
                        break;

                    case 255:
                        printf("Unknown command.\n");
                        break;
                    
                    default:
                        printf("Unknown error.\n");
                        break;
                }
            }
        }
    }

    return 0;
}
