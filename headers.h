#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include "colors.h"

int jobcount;
struct joblist
{
    int isrunning;
    char name[50];
    pid_t pid;
};
struct joblist currjobs[200];
struct joblist theonefgproc;
char prevdir[500];