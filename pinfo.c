#include "headers.h"

void pinfo(char arg[])
{
    int id;
    if (arg != NULL)
        sscanf(arg, "%d", &id);
    else
        id = getpid();
    char pidstat[500];
    strcpy(pidstat, "/proc/");
    char idchar[500];
    sprintf(idchar, "%d", id);
    strcat(pidstat, idchar);

    char execstat[500];
    strcpy(execstat, pidstat);

    strcat(pidstat, "/stat");
    strcat(execstat, "/exe");

    int printerr = 0;

    int fd = open(pidstat, O_RDONLY);
    if (fd < 0)
    {
        red();
        printf("Process doesnt exist\n");
        reset();
        printerr = 1;
    }
    else
    {
        char statbuff[10000];
        read(fd, statbuff, 10000);
        char *pinfoarg = strtok(statbuff, " ");

        printf("pid -- %s\n", pinfoarg);

        pinfoarg = strtok(NULL, " ");
        pinfoarg = strtok(NULL, " ");

        printf("Process Status -- %s\n", pinfoarg);

        for (int i = 0; i < 20; i++)
            pinfoarg = strtok(NULL, " ");

        printf("Memory -- %s\n", pinfoarg);
    }

    char execbuff[10000];
    int t = readlink(execstat, execbuff, 10000);
    if (t < 0)
    {
        if (printerr == 0)
        {
            red();
            printf("Readlink error\n");
            reset();
        }
    }
    else
        printf("Executable path -- %s\n", execbuff);

    close(fd);
}