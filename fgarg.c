#include "headers.h"
#include <stdlib.h>
#include <signal.h>

void ctrlc(int sig)
{
    if (theonefgproc.isrunning == 1)
    {
        if (kill(theonefgproc.pid, SIGINT) < 0)
        {
            red();
            printf("Error: couldnt kill\n");
            reset();
        }
        theonefgproc.isrunning = 0;
    }

    blue();
    printf("\nEnter your next command below\n");
    reset();
    fflush(stdout);
    return;
}

void ctrlz(int sig)
{
    if (theonefgproc.isrunning == 1)
    {
        if (kill(theonefgproc.pid, SIGTSTP) < 0)
        {
            red();
            printf("Error in kill\n");
            reset();
        }

        printf("Process %d stopped\n", theonefgproc.pid);
        currjobs[jobcount].pid = theonefgproc.pid;
        currjobs[jobcount].isrunning = 1;
        strcpy(currjobs[jobcount].name, theonefgproc.name);
        jobcount++;
        theonefgproc.isrunning = 0;

        blue();
        printf("\nEnter your next command below\n");
        reset();
        fflush(stdout);
        return;
    }
}

void childhandler(int sig)
{
    int stat;
    pid_t pid = waitpid(-1, &stat, WNOHANG);

    while (pid > 0)
    {
        if (WIFEXITED(stat) == 0)
        {
            red();
            printf("process %d terminated abnormally\n", pid);
            reset();
        }
        else
            printf("process %d terminated\n", pid);

        for (int i = 0; i < jobcount; i++)
            if (currjobs[i].pid == pid)
                currjobs[i].isrunning = 0;

        blue();
        printf("Enter your next command below\n");
        reset();
        fflush(stdout);

        pid = waitpid(-1, &stat, WNOHANG);
    }
    return;
}

void fgproc(char input[])
{
    char *arg[20];
    int i = 0;
    char *tok = strtok(input, " ");

    while (tok != NULL)
    {
        arg[i] = tok;
        tok = strtok(NULL, " ");
        i++;
    }
    arg[i] = NULL;

    pid_t pid = fork();

    if (pid < 0) //error
    {
        red();
        printf("Forking error\n");
        reset();
    }
    if (pid == 0) //child
    {
        if (execvp(arg[0], arg) < 0)
        {
            red();
            printf("Command '%s' not found\n", arg[0]);
            reset();
        }
        exit(0);
    }
    if (pid > 0) //parent
    {
        theonefgproc.pid = pid;
        theonefgproc.isrunning = 1;
        strcpy(theonefgproc.name, arg[0]);
        wait(0);
    }
}

void bgproc(char input[])
{
    char *arg[20];
    int i = 0;
    char *tok = strtok(input, " ");

    while (tok != NULL)
    {
        arg[i] = tok;
        tok = strtok(NULL, " ");
        i++;
    }
    arg[i - 1] = NULL;

    pid_t pid = fork();

    if (pid < 0) //error
    {
        red();
        printf("Forking error\n");
        reset();
    }
    if (pid == 0) //child
    {
        setpgid(0, 0);
        if (execvp(arg[0], arg) < 0)
        {
            red();
            printf("Execution error after correct forking\n");
            reset();
        }
        exit(0);
    }
    if (pid > 0) //parent
    {
        printf("Process is currently running\n");

        currjobs[jobcount].pid = pid;
        currjobs[jobcount].isrunning = 1;
        strcpy(currjobs[jobcount].name, arg[0]);
        jobcount++;
    }
}

void fgnotarg(char arg[])
{
    int jobno = atoi(arg);
    arg = strtok(NULL, " ");
    if (arg != NULL)
    {
        red();
        printf("Invalid number of arguements for fg\n");
        reset();
    }
    else
    {
        int tempjobno = 0;
        int flag = 0;
        for (int i = 0; i < jobcount; i++)
            if (currjobs[i].isrunning == 1)
            {
                tempjobno++;
                if (tempjobno == jobno)
                {
                    if (kill(currjobs[i].pid, SIGCONT) < 0)
                    {
                        red();
                        printf("Kill error smh\n");
                        reset();
                    }

                    currjobs[i].isrunning = 0;
                    theonefgproc.pid = currjobs[i].pid;
                    strcpy(theonefgproc.name, currjobs[i].name);
                    theonefgproc.isrunning = 1;
                    flag = 1;
                    break;
                }
            }
        if (flag == 0)
        {
            red();
            printf("No such bg process\n");
            reset();
        }
    }
}

void bgnotarg(char arg[])
{
}