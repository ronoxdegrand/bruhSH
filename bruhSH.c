#include "headers.h"
#include "executioner.h"
#include "fgarg.h"
#include <signal.h>
#include <ctype.h>

int size = 500;

int main()
{
    jobcount = 0;
    theonefgproc.isrunning = 0;

    char hostname[500];
    char username[500];
    char currdir[size];
    char initialdir[size];
    char input[size];

    gethostname(hostname, size);
    getlogin_r(username, size);
    getcwd(initialdir, size);

    strcpy(prevdir, initialdir);

    int homelen = strlen(initialdir);

    while (1)
    {
        signal(SIGCHLD, &childhandler);
        signal(SIGINT, &ctrlc);
        signal(SIGTSTP, &ctrlz);

        getcwd(currdir, size);

        char printdir[500];

        int currdirlen = strlen(currdir);

        if ((strncmp(initialdir, currdir, homelen) == 0) && ((currdir[homelen] == '\0') || (currdir[homelen] == ' ') || (currdir[homelen] == '\n') || (currdir[homelen] == '/'))) //display dir correctness
        {
            strcpy(printdir, "~");
            int loopvar = 1;
            for (int i = homelen; i < currdirlen; i++)
            {
                printdir[loopvar] = currdir[i];
                loopvar++;
            }
            printdir[loopvar] = '\0';
        }
        else
            strcpy(printdir, currdir);

        prompt(username, hostname, printdir);

        char *ctrld = fgets(input, size, stdin);
        if (ctrld == NULL)
        {
            printf("\n");
            return 0;
        }

        if (strcmp(input, "\n") == 0) //for no input
            continue;

        input[strcspn(input, "\n")] = 0; //removes trailing newline

        char *colontok;
        char *colontok_r = input;
        while (colontok = strtok_r(colontok_r, ";", &colontok_r))
        {
            int p = 0; //p is number of pipes
            int colonlen = strlen(colontok);
            for (int i = 0; i < colonlen; i++)
                if (colontok[i] == '|')
                    p++;
            //do error handling for pipes without arguements

            if (p == 0)
                execution(colontok, initialdir, currdir, printdir, 0);
            else
            {
                int status;
                int pipefds[2 * p];

                for (int i = 0; i < p; i++)
                    if (pipe(pipefds + i * 2) < 0)
                    {
                        red();
                        printf("Error: Couldn't pipe\n");
                        reset();
                    }

                int j = 0;
                int i = 0;
                char *pipetok;
                char *pipetok_r = colontok;
                while (pipetok = strtok_r(pipetok_r, "|", &pipetok_r))
                {
                    while (isspace((unsigned char)*pipetok)) //trims leading white space
                        pipetok++;                           //trims leading white space

                    int pid = fork();
                    if (pid < 0)
                    {
                        red();
                        printf("Error: error in forking while piping\n");
                        reset();
                    }
                    else if (pid == 0)
                    {
                        if (i < p)
                            dup2(pipefds[j + 1], 1);
                        if (j != 0)
                            dup2(pipefds[j - 2], 0);

                        int q;
                        for (q = 0; q < 2 * p; q++)
                            close(pipefds[q]);

                        execution(pipetok, initialdir, currdir, printdir, 1);
                    }
                    j += 2;
                    i++;
                }

                for (i = 0; i < 2 * p; i++)
                    close(pipefds[i]);

                for (i = 0; i < p + 1; i++)
                    wait(&status);
            }
        }
    }
    return 0;
}