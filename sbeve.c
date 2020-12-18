#include "headers.h"

void jobs()
{
    int tempjobcounter = 0;
    for (int i = 0; i < jobcount; i++)
        if (currjobs[i].isrunning)
        {
            tempjobcounter++;

            char pinfoish[500];
            sprintf(pinfoish, "/proc/%d/stat", currjobs[i].pid);
            int statfd = open(pinfoish, O_RDONLY);
            if (statfd < 0)
            {
                currjobs[i].isrunning = 0;
                continue;
            }

            char statbuff[10000];
            read(statfd, statbuff, 10000);

            char *pinfoarg = strtok(statbuff, " ");
            pinfoarg = strtok(statbuff, " ");
            pinfoarg = strtok(statbuff, " ");

            char runstate[100] = "Running";
            if (pinfoarg == "T" || pinfoarg == "t")
                strcpy(runstate, "Stopped");

            printf("[%d]\t%s\t%s\t[%d]\n", tempjobcounter, runstate, currjobs[i].name, currjobs[i].pid);

            close(statfd);
        }
}

void kjob(char arg[])
{
    int jobno = atoi(arg);
    arg = strtok(NULL, " ");
    if (arg == NULL)
    {
        red();
        printf("Invalid number of arguements for command 'kjob'\n");
        reset();
    }
    else
    {
        int signo = atoi(arg);
        arg = strtok(NULL, " ");
        if (arg != NULL)
        {
            red();
            printf("Invalid number of arguements for setenv\n");
            reset();
        }
        else
        {
            int tempjobcounter = 0;
            int procflag = 0;
            for (int i = 0; i < jobcount; i++)
                if (currjobs[i].isrunning == 1)
                {
                    tempjobcounter++;
                    if (tempjobcounter == jobno)
                    {
                        if (kill(currjobs[i].pid, signo) < 0)
                        {
                            red();
                            printf("Error: process not killed\n");
                            reset();
                        }
                        else
                            currjobs[i].isrunning = 0;
                        procflag++;
                        break;
                    }
                }
            if (procflag == 0)
            {
                red();
                printf("Error: process not found\n");
                reset();
            }
        }
    }
}

void overkill()
{
    for (int i = 0; i < jobcount; i++)
        if (currjobs[i].isrunning == 1)
            if (kill(currjobs[i].pid, SIGKILL) < 0)
                perror("Error in kill");
            else
                currjobs[i].isrunning = 0;
}