#ifndef __EXECUTIONER_H
#define __EXECUTIONER_H

void prompt(char username[], char hostname[], char printdir[]);
void execution(char *input, char initialdir[], char currdir[], char printdir[], int piping);

#endif