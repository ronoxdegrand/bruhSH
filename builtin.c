#include "headers.h"

void echo(char arg[])
{
    while (arg != NULL)
    {
        printf("%s ", arg);
        arg = strtok(NULL, " ");
    }
    printf("\n");
}

void cd(char arg[], char initialdir[], char printdir[], char currdir[])
{
    if (strcmp(arg, "~") == 0)
    {
        if (chdir(initialdir) != 0)
        {
            red();
            printf("Fatal error\n");
            reset();
        }
    }

    else if (strcmp(arg, "-") == 0)
    {
        if (chdir(prevdir) != 0)
        {
            red();
            printf("Fatal error\n");
            reset();
        }
    }

    else if (chdir(arg) != 0)
    {
        red();
        printf("Invalid directory\n");
        reset();
    }
    strcpy(prevdir, currdir);
}

void setenvcustom(char arg[])
{
    char *var = arg;
    arg = strtok(NULL, " ");
    if (arg == NULL)
    {
        if (setenv(var, "\0", 1) < 0)
        {
            red();
            printf("Error: setenv did not work\n");
            reset();
        }
    }
    else
    {
        char *value = arg;
        arg = strtok(NULL, " ");
        if (arg != NULL)
        {
            red();
            printf("Invalid number of arguements for setenv\n");
            reset();
        }
        else if (setenv(var, value, 1) < 0)
        {
            red();
            printf("Error: setenv did not work\n");
            reset();
        }
    }
}

void unsetenvcustom(char arg[])
{
    char *var = arg;
    arg = strtok(NULL, " ");
    if (arg != NULL)
    {
        red();
        printf("Invalid number of arguements for setenv\n");
        reset();
    }
    else if (unsetenv(var) < 0)
    {
        red();
        printf("Error: unsetenv did not work\n");
        reset();
    }
}