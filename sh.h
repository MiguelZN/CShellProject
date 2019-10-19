#include "get_path.h"

#ifndef SH_H
#define SH_H

int pid;
int sh( int argc, char **argv, char **envp);
char *which(char *command, struct pathelement *pathlist);
char *where(char *command, struct pathelement *pathlist);
void list ( char *dir );
void printenv(char **envp);


/*Helper Functions*/
char* getInput();
char** getArguments(char* str, char* specifer);
void freeArguments(char** arguments);
char* concat(const char *s1, const char *s2);




#define PROMPTMAX 32
#define MAXARGS 10

#endif /*SH_H*/
