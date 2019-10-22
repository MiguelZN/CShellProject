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
#define PREFIX_LEN 10
#define COMMAND_LEN 50
#define LONG_BUFFER 100

int sh( int argc, char **argv, char **envp ){
    
    /* Put PATH into a linked list */
    
//    printf("HELLO");
//    char*arg[4];
//    arg[0] ="/bin/ls";
//    arg[1] ="-l";
//    arg[2] =NULL;
//    execve(arg[0], arg, envp);
    int go = 1;
    char* prefix = malloc(sizeof(char)*PREFIX_LEN); //What gets displayed before the prompt_text
    
  while (go){
      printf("-----------------------------\n");
      struct pathelement *pathlist = get_path();
//      int i = 0;
//      int status;
      int forkid = 1;
      
      char* prompt_text = pwd();
      printf("%s [%s]>>",prefix,prompt_text);
      
      char* user_input = getInput();
      char **arguments = getArguments2(user_input, " ");
      
      free(prefix);
      free(prompt_text);
      free(arguments);
      
      break;
      
      int num_args = strToint(arguments[0]);//number of arguments
      //printf("NUM ARGS)
      char* command = arguments[1]; //the command to be executed EX: 'ls'
      //printf("SIZEOFARRAY:%d\n",num_args);
      //printf("ARGS0:%s\n",arguments[0]);
      //printf("INPUTTED COMMAND:%s\n",command);2
      char* command_path = which(command, pathlist);

      
      
    /* check for each built in command and implement */
      //printf("COMMANDPATH:%s\n",command_path);
      
      /*Custom Commands*/
      //Exits the shell
      if(strcmp(command, "exit")==0){
          printBlock("Executing exit");
          
          free(prefix);
          printf("Exiting..\n");
          go = 0;
      }
      else if(strcmp(command, "which")==0){
          printBlock("Executing which");
          //printf("ENTERED WHICH------------------\n");
          if(which(command,pathlist)!=NULL){
              char* checkCommand = arguments[2]; //Checks where the second argument is located
              //EX: which pwd returns where pwd is located
              char* found_path = which(checkCommand,pathlist);
              
              printf("PATH:=%s\n",found_path);
              free(found_path);
          }
      }
      else if(strcmp(command, "where")==0){
          printBlock("Executing where");
          char* checkCommand = arguments[2]; //Checks where the second argument is located
          //EX: which pwd returns where pwd is located
          
          char* allInstances = where(checkCommand, pathlist);
          printf("PATHS:=%s\n",allInstances);
          free(allInstances);
      }
      else if(strcmp(command, "cd")==0){
          printBlock("Executing cd");
          char* desired_directory = arguments[2]; //EX: cd .. thus arguments[2] == ".."
          cd(desired_directory);
      }
      else if(strcmp(command, "pwd")==0){
          printBlock("Executing pwd");
          char* current_dir = pwd();
          printf("%s\n",current_dir);
          free(current_dir);
      }
      else if(strcmp(command, "list")==0){
          printBlock("Executing list");
          char* current_dir = pwd();
          if(arguments[2]!=NULL){
              list(arguments[2]);
          }
          else{
              list(current_dir);
          }
          free(current_dir);
      }
      else if(strcmp(command, "pid")==0){
          printBlock("Executing pid");
          int pid = getpid();
          printf("%d\n",pid);
      }
      else if(strcmp(command, "kill")==0){
          printBlock("Executing kill");
          //EX: kill pid, kill -9 pid
          //Arguments EX: 4 kill pid# NULL  or Arguments: 5 kill signal# pid# NULL
          int pid_num, signal_num;
          char* pid_str = arguments[2];
          char* signal_str = arguments[3];
          //printf("ENTERED KILL, NUM ARGS:%d\n",num_args);
          
          if(num_args>=5){ //atleast 5 arguments
              //Converts pid_str to integer
              pid_num = strToint(pid_str);
              signal_num =strToint(signal_str);
              //printf("5 ARGS, PID:%d, SIG:%d\n",pid_num,signal_num);
              
              kill(pid_num,signal_num);
          }
          else if(num_args>=4){//atleast 4 arguments
              pid_num = strToint(pid_str);
              //printf("4 ARGS:%d\n",pid_num);
              kill(pid_num,SIGTERM);
          }
      }
      else if(strcmp(command, "prompt")==0){
          printBlock("Executing prompt");
          char* new_prefix = arguments[2];
          //printf("NEW PREFIX:%s\n",new_prefix);
          //printf("CURRENT PREFIX:%s\n",prefix);
          free(prefix);
          prefix = malloc(sizeof(char)*PREFIX_LEN);
          prefix = "";
          prefix = concat(prefix, new_prefix);
      }
      else if(strcmp(command, "printenv")==0){
          printBlock("Executing printenv");
          int num_args =strToint(arguments[0]);
          if(num_args>=4){ //EX: arguments: 4 printenv HOME NULL
              char* arg = arguments[2];
              printenv(arg);
          }
          else{
              printenv(NULL);
          }
      }
      else if(strcmp(command, "setenv")==0){
          printBlock("Executing setenv");
          int num_args = strToint(arguments[0]);
          char* name;
          char* value;
          
          
          
          if(num_args==5){//EX: arguments: 5 setenv NAME VALUE NULL
              name = arguments[2];
              value = arguments[3];
              
              if(strcmp(name, "PATH")==0){
                  setenv(name, value,1);
                  //Should update linked list for path directories (free old one)
                  free(pathlist);
                  pathlist = get_path();
              }
              else if(strcmp(name, "HOME")==0){
                  DIR *directory = opendir(value);
                  if(directory){
                      setenv(name, value, 1);
                      printf("Succesful!\n");
                      closedir(directory);
                  }
                  else{
                      printf("Directory does not exist!\n");
                  }
              }
              else{
                  setenv(name, value, 1);
              }
          }
          else if(num_args==4){ //EX: arguments: 4 setenv NAME NULL
              name = arguments[2];
              //printf("NAME:%s\n",name);
              setenv(name, "", 1);
          }
          else if(num_args==3){//EX: arguments: 3 setenv NULL
              //printf("ENTERED PRINTENV\n");
              printenv(NULL);
          }
      }
      else if(access(command_path, F_OK) == 0 ||arguments[1][0]=='.' ||arguments[1][0]=='/'){
          //printf("ENTERED FORK\n");
          forkid = fork();
      }
      

      //printf("FREED MEMORY\n");
      
      
      //Checks for built-in commands and executes them
      if(forkid<0){
          ;
      }
      //Child
      else if(forkid==0){
          //printf("FOUND COMMAND\n");
          int k=0;
          while(arguments[k]!=NULL){
              printf("FOUND:%s\n",arguments[k]);
              k+=1;
          }
          
          printf("COMMAND:%s\n",arguments[1]);
          //    ../  Go up to parent directory and run
          if(arguments[1][0]=='.' && arguments[1][1]=='.' && arguments[1][2]=='/'){
              printf("ENTERED ../\n");
              char* curr_directory = pwd();
              int index = getOccurrence(curr_directory, '/', "last");
              char* parent_directory = getSubstring(curr_directory, 0, index);
              
              //Gets file name
              printf("SUBSTRING:%s\n", &arguments[1][3]);

              parent_directory = concat(parent_directory, &arguments[1][3]);
              printf("Parent Directory:%s\n",parent_directory);
              if(execvp(parent_directory,&arguments[1])==-1){
                  kill(getpid(),SIGTERM);
                  printf("Not a valid exec\n");
              }
              else{
                  execvp(parent_directory,&arguments[1]);
                  printf("Successful exec\n");
              }
          }
          //   ./  run within current directory
          else if(arguments[1][0]=='.' && arguments[1][1]=='/' ){
              
          }else{
            //NOTE: execve deallocates automatically child process memory
              char* new_str = concat("Executing Built-in", arguments[1]);
              printf("NEW STRING:%s\n",new_str);
              printBlock(new_str);
              execve(command_path,&arguments[1],NULL);
          }

              //printf("AFTER EXECUTING\n");
      }
      //Parent
      else if(forkid>0){
          waitpid(forkid, NULL,0);
          //printf("PARENT:\n");
          
      }
      
      //Freeing memory
      freePath(pathlist);
      free(user_input);
      freeArguments(arguments);
      free(command_path);
      
      //printf("REACHED END OF WHILE LOOP\n");
      //printf("-----------------------------\n\n");
  }
  return 0;
} /* sh() */

char *which(char *command, struct pathelement *pathlist ){
    struct pathelement *p = pathlist;
    char *located_path = malloc(sizeof(char)*COMMAND_LEN);
    int found = 0;
    
    while (p) {         // WHERE
        //printf("PATHELEMENT:%s\n",p->element);
        sprintf(located_path, "%s/%s", p->element,command);
        if (access(located_path, F_OK) == 0){
            //printf("FOUND");
            //printf("[%s]\n", located_path);
            found = 1;
            break;
        }
        p = p->next;
    }
    
    return located_path;
}

char* where(char *command, struct pathelement *pathlist){
    struct pathelement *p = pathlist;
    char *allInstances = malloc(sizeof(char)*COMMAND_LEN);
    char *curr_str = malloc(sizeof(char)*COMMAND_LEN);
    //char curr_str[50];
    //printf("ENTERED WHERE\n");
    allInstances = ""; //Initializes the string to empty string
    
    while (p) {// WHERE
        sprintf(curr_str, "%s/%s", p->element,command);
        //printf("WHERE:%s\n",curr_str);
        if (access(curr_str, F_OK) == 0){
            //printf("CURRENT ALL INSTANCES:%s\n",allInstances);
            allInstances = concat(allInstances, curr_str);
            //printf("FOUND\n");
        }
        p = p->next;
    }
    
    return allInstances;
}

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
    //printf("FOUND %d WS\n",num_whitespaces);
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
    
    
    //Inserts the size of array into the 0th index
    //printf("%d",sizeofarray);
    char* num_malloc = malloc(sizeof(char)*sizeof(int));
    sprintf(num_malloc, "%d",sizeofarray);
    //snprintf(stringarr[0],strlen(str),"%d",sizeofarray);
    stringarr[0] = num_malloc;
    //printf("SIZE ARGS0:%s\n",stringarr[0]);
    
    //Starts at 1st index and inserts the different spliced strings
    int i = 1;
    char *rest2 = NULL;
    for (token = strtok_r(str_copy2, specifer, &rest2);token != NULL;
        token = strtok_r(NULL, specifer, &rest2)) {
        //printf("token:%s\n", token);
        stringarr[i] = strdup(token);
        //printf("INDEX%d\n",i);
        //printf("INDEX%d:%s\n",stringarr[i]);
        i+=1;
    }
    
    //Sets last to NULL
    stringarr[sizeofarray-1] = NULL;

    free(str_copy);
    free(str_copy2);
    return stringarr;

}

char **getArguments2(char*str, char* specifier){
//Creates two copies of str (since strok_r modifies them)
char str_copy[strlen(str)];
strcpy(str_copy, str);

char str_copy2[strlen(str)];
strcpy(str_copy2, str);

char *rest = NULL;
char *token;

//Checks for how many whitespaces there are
int num_whitespaces =0;
for (token = strtok_r(str_copy, specifier, &rest);token != NULL;
     token = strtok_r(NULL, specifier, &rest)) {
    num_whitespaces+=1;
}
num_whitespaces-=1;
//printf("FOUND %d WS\n",num_whitespaces);
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
int sizeofarray = num_whitespaces+3; //1 extra for first element, 2 for SIZE and NULL
char** stringarr = malloc(sizeof(char)*sizeofarray);
for(int k=0;k<sizeofarray;k++){
    stringarr[k] = malloc(sizeof(char)*(strlen(str)));
    stringarr[k] = "";
}


//Inserts the size of array into the 0th index
//printf("%d",sizeofarray);
char* num_malloc = malloc(sizeof(char)*sizeof(int));
sprintf(num_malloc, "%d",sizeofarray);
stringarr[0] = num_malloc;
//printf("SIZE ARGS0:%s\n",stringarr[0]);

//Starts at 1st index and inserts the different spliced strings
int i = 1;
char *rest2 = NULL;
for (token = strtok_r(str_copy2, specifier, &rest2);token != NULL;
     token = strtok_r(NULL, specifier, &rest2)) {
    //printf("token:%s\n", token);
    stringarr[i] = strdup(token);
    //printf("INDEX%d\n",i);
    //printf("INDEX%d:%s\n",stringarr[i]);
    i+=1;
}

//Sets last to NULL
stringarr[sizeofarray-1] = NULL;
return stringarr;

}

void freeArguments(char** arguments){
    int sizeofarr = strToint(arguments[0]);
    //printf("ENTERED FREE ARGUMENTS\n");
    //printf("FA, SIZEOFARRAY:%d\n",sizeofarr);
    for(int i=sizeofarr-1;i>=0;i--){
        //printf("ARGUMENTS%s\n",arguments[i]);
        //printf("FREED ARG:%s\n", arguments[i]);
        free(arguments[i]);
        
    }
}

char* concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/*start:0th index
 end: length
 -it will copy a string from [start,end)
 */
char* getSubstring(char* str,int start,int end){
    int size_substr = end-start+2;//+1 for '\0' character
    char* new_str = malloc(size_substr);
    
    for(int i=start;i<=end+1;i++){
        new_str[i] = str[i];
    }
    new_str[size_substr-1] = '\0';
    
    return new_str;
}

int getOccurrence(char* str, char c, char* firstorlast){
    int index = -1;
    
    //Checks for last, first occurrence
    if(strcmp(firstorlast, "last")){
        for(int i=strlen(str)-1;i>=0;i--){
            if(str[i]==c){
                //printf("CHAR:%c, INDEX:%d\n",str[i],i);
                index = i;
                break;
            }
        }
    }
    //Checks for first occurrence
    else{
        for(int i=0;i<strlen(str);i++){
            if(str[i]==c){
                index = i;
            }
        }
    }
    
    return index;
}

char* pwd(){
    char* current_dir = getcwd(NULL,LONG_BUFFER);
    return current_dir;
}

void list(char* directory_path){
    DIR *curr_dir;
    struct dirent *curr_dirent;
    
    curr_dir = opendir(directory_path);
    while((curr_dirent = readdir(curr_dir))!=NULL){
        char* file_name= curr_dirent->d_name;
        
        if(!strcmp(file_name, ".") || !strcmp(file_name, "..")){ //removes "." and ".." files

        }
        else{
            printf("%s\n",file_name); //only prints readable files
        }
    }
    closedir(curr_dir);
}

//void prompt(char* current_prompt, char* prefix){
//    //char* new_prefix = arguments[2];
//    //printf("NEW PREFIX:%s\n",new_prefix);
//    //printf("CURRENT PREFIX:%s\n",prefix);
//    char *new_prefix = prefix;
//    free(prefix);
//    prefix = malloc(sizeof(char)*PREFIX_LEN);
//    prefix = "";
//    prefix = concat(prefix, new_prefix);
//}

void printenv(char *name){
    extern char **environ;
    
    if(name==NULL){
        int i=0;
        while(environ[i]){
            printf("ENVIRONMENT:%s\n",environ[i++]);
        }
    }
    else{
        printf("ENVIRONMENT:%s",getenv(name));
    }
}

//void setenv(char *name, char *value){
//    if(strcmp(value, "")==0){
//        setenv(name,"");
//    }
//    else{
//        setenv(name, value);
//    }
//}

int strToint(char* int_str){
    int num = (int)strtol(int_str, (char **)NULL, 10);
    return num;
}

void cd(char* directory){
    if(directory==NULL){
        chdir(getenv("HOME"));
    }
    else if(strcmp(directory, "-")==0){
        //printf("ENTERED CD - \n");
        char* current_dir = getcwd(NULL,LONG_BUFFER);
        int index = getOccurrence(current_dir, '/', "first");
        char* prev_dir = getSubstring(current_dir, 0, index-1);
        //printf("INDEXF:%d\n",index);
        //printf("ACTUAL:%c\n",current_dir[57]);
        
        //printf("PREVDIR,%s\n",prev_dir);
        int status = chdir(prev_dir);
        //printf("STATUS:%d\n",status);
        free(prev_dir);
        free(current_dir);
        
    }
    else{
        //printf("ENTERING CHANGE DIR\n");
        char* current_dir = pwd();
        
        //Cd from current directory
        char* new_dir = malloc(strlen(current_dir)+strlen(directory)+1);
        new_dir = concat(current_dir, "/");
        new_dir = concat(new_dir,directory);
        //printf("NEW DIR:%s\n",new_dir);
        
        //Cd to any directory
        if(chdir(new_dir)==0){
            
        }
        else{
            chdir(directory);
        }
        free(new_dir);
        free(current_dir);
    }
}

void printBlock(char* str){
    printf("%s:\n",str);
}
