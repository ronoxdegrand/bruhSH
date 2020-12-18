#include "headers.h"
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

struct stat stats;

void ls(char arg[], char initialdir[])
{
    int lflag = 0;
    int aflag = 0;
    char specarr[10][500];
    int speccounter = 0;
    while (arg != NULL)
    {
        if ((strcmp(arg, "-la") == 0) || (strcmp(arg, "-al") == 0))
        {
            lflag = 1;
            aflag = 1;
        }
        else if (strcmp(arg, "-l") == 0)
            lflag = 1;
        else if (strcmp(arg, "-a") == 0)
            aflag = 1;
        else if (strcmp(arg, "~") == 0)
        {
            strcpy(specarr[speccounter], initialdir);
            speccounter++;
        }
        else
        {
            strcpy(specarr[speccounter], arg);
            speccounter++;
        }

        arg = strtok(NULL, " ");
    }

    if (speccounter == 0)
    {
        strcpy(specarr[speccounter], ".");
        speccounter++;
    }

    for (int i = 0; i < speccounter; i++)
    {
        DIR *lsdir;
        struct dirent *sd;
        if (speccounter > 1)
        {
            blue();
            printf("%s:\n", specarr[i]);
            reset();
        }
        lsdir = opendir(specarr[i]);
        if (lsdir == NULL)
        {
            red();
            printf("Invalid arguement for ls command\n");
            reset();
        }
        else if (aflag == 1)
            while ((sd = readdir(lsdir)) != NULL)
            {
                if (lflag == 1)
                {
                    if (stat(sd->d_name, &stats) == 0)
                    {
                        printf((S_ISDIR(stats.st_mode)) ? "d" : "-");
                        printf((stats.st_mode & S_IRUSR) ? "r" : "-");
                        printf((stats.st_mode & S_IWUSR) ? "w" : "-");
                        printf((stats.st_mode & S_IXUSR) ? "x" : "-");
                        printf((stats.st_mode & S_IRGRP) ? "r" : "-");
                        printf((stats.st_mode & S_IWGRP) ? "w" : "-");
                        printf((stats.st_mode & S_IXGRP) ? "x" : "-");
                        printf((stats.st_mode & S_IROTH) ? "r" : "-");
                        printf((stats.st_mode & S_IWOTH) ? "w" : "-");
                        printf((stats.st_mode & S_IXOTH) ? "x " : "- ");
                        printf("%ld ", stats.st_nlink);
                        struct passwd *pass = getpwuid(stats.st_uid);
                        printf("%s ", pass->pw_name);
                        struct group *grp = getgrgid(stats.st_gid);
                        printf("%s ", grp->gr_name);
                        printf("%ld ", stats.st_size);
                        char gettime[500];
                        strcpy(gettime, ctime(&stats.st_mtime));
                        gettime[strcspn(gettime, "\n")] = 0;
                        printf("%s ", gettime);
                    }
                    else
                    {
                        red();
                        printf("Unable to get properties for\n");
                        reset();
                    }
                }
                printf("%s\n", sd->d_name);
            }
        else
            while ((sd = readdir(lsdir)) != NULL)
            {
                if ((sd->d_name[0] != '.') && (lflag == 1))
                {
                    if (stat(sd->d_name, &stats) == 0)
                    {
                        printf((S_ISDIR(stats.st_mode)) ? "d" : "-");
                        printf((stats.st_mode & S_IRUSR) ? "r" : "-");
                        printf((stats.st_mode & S_IWUSR) ? "w" : "-");
                        printf((stats.st_mode & S_IXUSR) ? "x" : "-");
                        printf((stats.st_mode & S_IRGRP) ? "r" : "-");
                        printf((stats.st_mode & S_IWGRP) ? "w" : "-");
                        printf((stats.st_mode & S_IXGRP) ? "x" : "-");
                        printf((stats.st_mode & S_IROTH) ? "r" : "-");
                        printf((stats.st_mode & S_IWOTH) ? "w" : "-");
                        printf((stats.st_mode & S_IXOTH) ? "x " : "- ");
                        printf("%ld ", stats.st_nlink);
                        struct passwd *pass = getpwuid(stats.st_uid);
                        printf("%s ", pass->pw_name);
                        struct group *grp = getgrgid(stats.st_gid);
                        printf("%s ", grp->gr_name);
                        printf("%ld ", stats.st_size);
                        char gettime[500];
                        strcpy(gettime, ctime(&stats.st_mtime));
                        gettime[strcspn(gettime, "\n")] = 0;
                        printf("%s ", gettime);
                    }
                    else
                    {
                        red();
                        printf("Unable to get properties for\n");
                        reset();
                    }
                }
                if (sd->d_name[0] != '.')
                    printf("%s\n", sd->d_name);
            }
        closedir(lsdir);
        if (speccounter > 1)
            printf("\n");
    }
}