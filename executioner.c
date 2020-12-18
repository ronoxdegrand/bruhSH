#include "headers.h"
#include "builtin.h"
#include "ls.h"
#include "fgarg.h"
#include "pinfo.h"
#include "sbeve.h"
#include <ctype.h>

void prompt(char username[], char hostname[], char printdir[])
{
    printf("<");
    green();
    printf("%s@%s", username, hostname);
    reset();
    printf(":");
    blue();
    printf("%s", printdir);
    reset();
    printf("> ");
}

void execution(char *input, char initialdir[], char currdir[], char printdir[], int piping)
{
    //redirection starts here
    dup2(0, 100);
    dup2(1, 101);

    int fdin = 0;
    int fdout = 1;
    int ogredirspot = 0;
    for (int i = 0; i < strlen(input); i++)
        if (input[i] == '<')
        {
            if (ogredirspot == 0)
                ogredirspot = i - 1;
            i++;
            while (isspace(input[i]))
                i++;

            char infile[500];

            int j;
            for (j = 0; input[i] != '\0'; j++)
            {
                if (((input[i + 1] == '>') && (input[i] == ' ')) || (input[i + 1] == '>'))
                    break;
                infile[j] = input[i];
                i++;
            }
            infile[j++] = '\0';
            fdin = open(infile, O_RDONLY);
            if (fdin == -1)
            {
                red();
                printf("error: '%s' does not exist or have permissions\n", infile);
                reset();
                fdin = 0;
            }
            dup2(fdin, 0);
        }
        else if (input[i] == '>')
        {
            if (ogredirspot == 0)
                ogredirspot = i - 1;
            int app = 0;
            if (input[i + 1] == '>')
            {
                i++;
                app = 1;
            }
            i++;
            while (isspace(input[i]))
                i++;

            char outfile[500];

            int j;
            for (j = 0; input[i] != '\0'; j++)
            {
                if (((input[i + 1] == '<') && (input[i] == ' ')) || (input[i + 1] == '<'))
                    break;
                outfile[j] = input[i];
                i++;
            }
            outfile[j++] = '\0';
            if (app == 0)
                fdout = open(outfile, O_WRONLY | O_CREAT, 0644);
            else
                fdout = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fdout == -1)
            {
                red();
                printf("error: '%s' could not be created or may not have permissions\n", outfile);
                reset();
                fdout = 1;
            }
            dup2(fdout, 1);
        }
    if (ogredirspot != 0)
        input[ogredirspot] = '\0';
    //redirection ends here

    char inputcopy[500];
    strcpy(inputcopy, input);

    char *arg = strtok(input, " ");
    if (strcmp(arg, "quit") == 0) //type "quit" to quit
        exit(0);
    else if (strcmp(arg, "bg") == 0)
    {
        arg = strtok(NULL, " ");
        if (arg == NULL)
        {
            red();
            printf("Invalid number of arguements for command 'bg'\n");
            reset();
        }
        else
            bgnotarg(arg);

        dup2(100, 0);
        dup2(101, 1);
        if (piping == 1)
            exit(0);
    }
    else if (strcmp(arg, "fg") == 0)
    {
        arg = strtok(NULL, " ");
        if (arg == NULL)
        {
            red();
            printf("Invalid number of arguements for command 'fg'\n");
            reset();
        }
        else
            fgnotarg(arg);

        dup2(100, 0);
        dup2(101, 1);
        if (piping == 1)
            exit(0);
    }
    else if (strcmp(arg, "kjob") == 0)
    {
        arg = strtok(NULL, " ");
        if (arg == NULL)
        {
            red();
            printf("Invalid number of arguements for command 'kjob'\n");
            reset();
        }
        else
            kjob(arg);

        dup2(100, 0);
        dup2(101, 1);
        if (piping == 1)
            exit(0);
    }
    else if (strcmp(arg, "overkill") == 0)
    {
        arg = strtok(NULL, " ");
        if (arg != NULL)
        {
            red();
            printf("Too many arguements for command\n");
            reset();
        }
        else
            overkill();

        dup2(100, 0);
        dup2(101, 1);
        if (piping == 1)
            exit(0);
    }
    else if (strcmp(arg, "jobs") == 0)
    {
        arg = strtok(NULL, " ");
        if (arg != NULL)
        {
            red();
            printf("Too many arguements for command\n");
            reset();
        }
        else
            jobs();

        dup2(100, 0);
        dup2(101, 1);
        if (piping == 1)
            exit(0);
    }
    else if (strcmp(arg, "setenv") == 0)
    {
        arg = strtok(NULL, " ");
        if (arg == NULL)
        {
            red();
            printf("Invalid number of arguements for setenv\n");
            reset();
        }
        else
            setenvcustom(arg);

        dup2(100, 0);
        dup2(101, 1);
        if (piping == 1)
            exit(0);
    }
    else if (strcmp(arg, "unsetenv") == 0)
    {
        arg = strtok(NULL, " ");
        if (arg == NULL)
        {
            red();
            printf("Invalid number of arguements for setenv\n");
            reset();
        }
        else
            unsetenvcustom(arg);

        dup2(100, 0);
        dup2(101, 1);
        if (piping == 1)
            exit(0);
    }
    else if (strcmp(arg, "echo") == 0)
    {
        arg = strtok(NULL, " ");
        echo(arg);

        dup2(100, 0);
        dup2(101, 1);
        if (piping == 1)
            exit(0);
    }
    else if (strcmp(arg, "pwd") == 0)
    {
        printf("%s\n", currdir);

        dup2(100, 0);
        dup2(101, 1);
        if (piping == 1)
            exit(0);
    }
    else if (strcmp(arg, "cd") == 0)
    {
        arg = strtok(NULL, " ");
        if (arg == NULL)
        {
            red();
            printf("Arguement required for cd\n");
            reset();
        }
        else
            cd(arg, initialdir, printdir, currdir);

        dup2(100, 0);
        dup2(101, 1);
        if (piping == 1)
            exit(0);
    }
    else if (strcmp(arg, "ls") == 0)
    {
        arg = strtok(NULL, " ");
        ls(arg, initialdir);

        dup2(100, 0);
        dup2(101, 1);
        if (piping == 1)
            exit(0);
    }
    else if (strcmp(arg, "pinfo") == 0)
    {
        arg = strtok(NULL, " ");
        pinfo(arg);

        dup2(100, 0);
        dup2(101, 1);
        if (piping == 1)
            exit(0);
    }
    else //foreground background
    {
        if (inputcopy[strlen(inputcopy) - 1] != '&') //foreground
            fgproc(inputcopy);
        else //background, idk
            bgproc(inputcopy);

        dup2(100, 0);
        dup2(101, 1);
        if (piping == 1)
            exit(0);
    }
}