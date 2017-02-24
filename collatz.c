#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


void compute_collatz(int input, int count)
{
    int im;

    if(input != 1)
    {
        if(input % 2 == 0 )
        {
            im = input / 2;
            printf("--%d\n", im);
            compute_collatz(im);
        }
        else
        {
            im = (3 * input) + 1;
            printf("--%d\n", im);
            compute_collatz(im);
        }
    }
}

int main(int argc, char *argv[])
{
    pid_t w,cpid;
    int status;

    if ( argc != 2 ) /* argc should be 2 for correct execution */
    {
        printf( "usage: %s [number >= 1]", argv[0] );
        return 1;
    }

    char *ptr1;
    int inputvalue;
    inputvalue = strtol(argv[1], &ptr1, 10);

    if(inputvalue < 1)
    {
        printf( "usage: %s [number >= 1]", argv[0] );
        return 1;
    }

    cpid = fork();

    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {            /* Code executed by child */
        printf("Child PID is %ld\n", (long) getpid());
        if (argc == 1)
            pause();                    /* Wait for signals */

        compute_collatz(inputvalue);


        _exit(atoi(argv[1]));

   }
   else {                    /* Code executed by parent */
        do {
            w = waitpid(cpid, &status, WUNTRACED | WCONTINUED);
            if (w == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }

           if (WIFEXITED(status)) {
                printf("exited, status=%d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("killed by signal %d\n", WTERMSIG(status));
            } else if (WIFSTOPPED(status)) {
                printf("stopped by signal %d\n", WSTOPSIG(status));
            } else if (WIFCONTINUED(status)) {
                printf("continued\n");
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));




        exit(EXIT_SUCCESS);
    }

    return 0;
}









