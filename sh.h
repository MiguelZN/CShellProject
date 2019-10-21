#include "get_path.h"

#ifndef SH_H
#define SH_H

int pid;
int sh( int argc, char **argv, char **envp);
char *which(char *command, struct pathelement *pathlist);
char *where(char *command, struct pathelement *pathlist);
char* pwd();
void list(char* directory_path);
void printenv(char **envp);
//void prompt(char* current_prompt, char* prefix);


/*Helper Functions*/
char* getInput();
char** getArguments(char* str, char* specifer);
void freeArguments(char** arguments);
char* concat(const char *s1, const char *s2);
char* getSubstring(char* str,int start,int end);
int getOccurrence(char* str, char c, char* firstorlast);



#define PROMPTMAX 32
#define MAXARGS 10

#endif /*SH_H*/
