#ifndef __BUILTIN_H
#define __BUILTIN_H

void echo(char arg[]);
void cd(char arg[], char initialdir[], char printdir[], char currdir[]);
void setenvcustom(char arg[]);
void unsetenvcustom(char arg[]);

#endif