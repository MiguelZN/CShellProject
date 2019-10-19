#include "get_path.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"
#define COMMAND_LEN 50

int sh( int argc, char **argv, char **envp ){
    char cmd[COMMAND_LEN];
    /* Put PATH into a linked list */
    struct pathelement *pathlist = get_path();
    char* command_path;
    char *arg[] = {NULL,"*.c",0};
    char **arguments;
    int go = 1;
    int i = 0;
    int status;
    int forkid;

  while (go){
      printf(">>");
      char* user_input = getInput();
      arguments = getArguments(user_input, " ");
      
      
      int num_args = (int)strtol(arguments[0], (char **)NULL, 10);//number of arguments
      char* command = arguments[1]; //the command to be executed EX: 'ls'
      printf("SIZEOFARRAY:%d\n",num_args);
      printf("INPUTTED COMMAND:%s\n",command);

      
      
    /* check for each built in command and implement */
      command_path = which(command, pathlist);
      printf("COMMANDPATH:%s\n",command_path);
    
     /*  else  program to exec */
    
      
      //Exits the shell
      if(strcmp(command, "exit")==0){
          printf("Exiting..\n");
          freePath(pathlist);
          free(user_input);
          
          go = 0;
      }
      if(forkid<0){
          ;
      }
      //Checks for built-in commands and executes them
      else if(access(command_path, F_OK) == 0){
          printf("FOUND COMMAND\n");
          int k=0;
          while(arguments[k]!=NULL){
              printf("FOUND:%s\n",arguments[k]);
              k+=1;
          }
          
          execve(arguments[1],&arguments[1],NULL);
          //execve(arguments[1],NULL,NULL);

          freeArguments(arguments);
      }
      //Child
      else if(forkid==0){
          /*Custom commands go in here*/
          
      }
      //Parent
      else{
          forkid = fork();
          int waitid = waitpid(forkid, &status,0);
          printf("PARENT:\n");
          free(command_path);
          
      }
       /* find it */
       /* do fork(), execve() and waitpid() */

      /* else */
        /* fprintf(stderr, "%s: Command not found.\n", args[0]); */
  }
  return 0;
} /* sh() */

char *which(char *command, struct pathelement *pathlist )
{
    struct pathelement *p = pathlist;
    char *located_path = malloc(sizeof(char)*COMMAND_LEN);
    int found = 0;
    
    while (p) {         // WHERE
        printf("PATHELEMENT:%s\n",p->element);
        sprintf(located_path, "%s/%s", p->element,command);
        if (access(located_path, F_OK) == 0){
            printf("FOUND");
            printf("[%s]\n", located_path);
            found = 1;
            break;
        }
        p = p->next;
    }
    
    if(found){
        return located_path;
    }
    else{
        return NULL;
    }
} /* which() */

char* where(char *command, struct pathelement *pathlist){
    struct pathelement *p = pathlist;
    char *allInstances = malloc(sizeof(char)*COMMAND_LEN);
    
    while (p) {         // WHERE
        printf("PATHELEMENT:%s\n",p->element);
        sprintf(allInstances, "%s/%s", p->element,command);
        if (access(allInstances, F_OK) == 0){
            printf("FOUND");
            printf("[%s]\n", allInstances);
        }
        p = p->next;
    }
    
    return allInstances;
}

void list ( char *dir )
{
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
} /* list() */



char* getInput(){
    int max_string_length = 30;
    char *input = malloc(sizeof(char)*max_string_length);
    fgets(input, max_string_length,stdin);
    input[strlen(input)-1] = '\0'; //NULL
    return input;
}

char** getArguments(char* str, char* specifer){
    //Creates two copies of str (since strok_r modifies them)
    char* str_copy = strdup(str);
    char* str_copy2 = strdup(str);
    char *rest = NULL;
    char *token;

    //Checks for how many whitespaces there are
    int num_whitespaces =0;
    for (token = strtok_r(str_copy, specifer, &rest);token != NULL;
         token = strtok_r(NULL, specifer, &rest)) {
        num_whitespaces+=1;
    }
    printf("FOUND %d WS\n",num_whitespaces);
    //EX: if found 2 whitespaces then EX command looks like: >>cd(ONEWS)ls(SECONDWS)main.c
    //String array should look like {SIZE,"cd","ls","main.c",NULL}
    //where SIZE is the size of the array
    //and the last element is NULL for arguments to be executed

    //Allocates array where:
    /*0th index: size of array
     1th index: command
     2th index: file EX: main.c
     3th+ index: flags
     */
    int sizeofarray = num_whitespaces+2; //1 extra for first element, 2 for SIZE and NULL
    char** stringarr = malloc(sizeof(char)*sizeofarray);
    for(int k=0;k<sizeofarray;k++){
        stringarr[k] = malloc(sizeof(char)*(strlen(str)));
    }
    
    //Sets last to NULL
    stringarr[sizeofarray-1] = NULL;
    
    //Inserts the size of array into the 0th index
    printf("%d",sizeofarray);
    snprintf(stringarr[0],strlen(str),"%d",sizeofarray);
    
    
    //Starts at 1st index and inserts the different spliced strings
    int i = 1;
    char *rest2 = NULL;
    for (token = strtok_r(str_copy2, specifer, &rest2);token != NULL;
        token = strtok_r(NULL, specifer, &rest2)) {
        //printf("token:%s\n", token);
        stringarr[i] = strdup(token);
        printf("INDEX%d\n",i);
        //printf("INDEX%d:%s\n",stringarr[i]);
        i+=1;
    }

    free(str_copy);
    free(str_copy2);
    return stringarr;

}

void freeArguments(char** arguments){
    int sizeofarr = (int)strtol(arguments[0], (char **)NULL, 10);
    for(int i=sizeofarr-1;i>=0;i--){
        printf("ARGUMENTS%s\n",arguments[i]);
        printf("FREED ARG:%s\n", arguments[i]);
        free(arguments[i]);
    }
}
