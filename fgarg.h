#ifndef __FGARG_H
#define __FGARG_H

void ctrlc(int sig);
void ctrlz(int sig);
void childhandler(int sig);
void fgproc(char input[]);
void bgproc(char input[]);
void fgnotarg(char arg[]);
void bgnotarg(char arg[]);

#endif