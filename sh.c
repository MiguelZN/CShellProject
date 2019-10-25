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
    int go = 1;

    char prefix[COMMAND_LEN];
    strcpy(prefix,"");
    
    
  while (go){
      printf("-----------------------------\n");
      struct pathelement *pathlist = get_path();
      int forkid = 1;
      
      char* prompt_text = pwd();
      printf("%s [%s]>>",prefix,prompt_text);
      
      char* user_input = getInput();
      
      int len_userinput = strlen(user_input);
      char cpy_userinput[len_userinput];
      char cpy2_userinput[len_userinput];
      strcpy(cpy_userinput, user_input);
      strcpy(cpy2_userinput,user_input);

      //Arguments:
      //Gets the number of arguments
      char* token = strtok(cpy_userinput, " ");
      int num_args = 0;
      while(token!=NULL){
          printf("%s\n",token);
          token = strtok(NULL, " ");
          num_args+=1;
      }
      printf("NUM ARGS:%d\n",num_args);
      num_args+=1; //To include NULL at the end
      printf("AFTER NUM ARGS:%d\n",num_args);
      
      //Initializes 2d string array
      char *arguments[num_args];
      
      //Inputs the tokens into arguments 2d string array
      int index = 0;
      token = strtok(cpy2_userinput, " ");
      while(token!=NULL){
          printf("Inputting args:%s\n",token);
          arguments[index] = "";
          arguments[index] = token;
          //strcpy(arguments[index],token);
          token = strtok(NULL, " ");
          
          //printf("ARG%d, %s\n",index,arguments[index]);
          index+=1;
      }
      
      //sprintf(command, "%d", num_args);
      arguments[num_args-1] = NULL;
      
      for(int i=0;i<num_args;i++){
          printf("ARG%d:%s\n",i,arguments[i]);
      }
      
      //printf("NUM ARGS)
      char* command = arguments[0]; //the command to be executed EX: 'ls'
      char* second_arg = NULL;
      char* third_arg = NULL;
      
      if(num_args==3){
          second_arg = arguments[1];
      }
      else if(num_args>=4){
          second_arg = arguments[1];
          third_arg = arguments[2];
      }
      //printf("SIZEOFARRAY:%d\n",num_args);
      //printf("ARGS0:%s\n",command);
      //printf("INPUTTED COMMAND:%s\n",command);2
      char* command_path = which(command, pathlist);
      
      
      
    /* check for each built in command and implement */
      //printf("COMMANDPATH:%s\n",command_path);
      
      /*Custom Commands*/
      if(strcmp(command, "which")==0){
          printBlock("Executing which");
          //printf("ENTERED WHICH------------------\n");
          if(num_args>=3){
              struct pathelement *temppathlist = get_path();
              char* checkCommandExists = which(second_arg,pathlist);
              printf("SECOND ARG:%s\n",second_arg);
              printf("checkCommandExists%s\n",checkCommandExists);
              
              if(access(checkCommandExists, F_OK) == 0){
                  printf("PATH:=%s\n",checkCommandExists);
              }
              else{
                  printf("Could not find command path!\n");
              }
              
              free(checkCommandExists);
              freePath(temppathlist);
          }
      }
      else if(strcmp(command, "where")==0){
          printBlock("Executing where");
          
          if(num_args>=3){
              struct pathelement *temppathlist = get_path();
              where(second_arg,temppathlist);
              freePath(temppathlist);
          }
      }
      else if(strcmp(command, "cd")==0){
          printBlock("Executing cd");
          char* desired_directory = second_arg; //EX: cd .. thus arguments[2] == ".."
          if(second_arg==NULL){
              printf("SECOND ARG IS NULL\n");
          }
          else{
              printf("second arg IS NOT NULL\n");
          }
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

          //If there are atleast 3 arguments which signify a directory was given as the second argument
          //EX: list /Users
          if(num_args>=3){
              printf("ENTERED 2nd ARGUMENT NOT NULL\n");
              list(second_arg);
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
          //Arguments EX:kill pid# NULL  or Arguments: kill signal# pid# NULL
          //printf("ENTERED KILL, NUM ARGS:%d\n",num_args);
          
          if(num_args>=4){ //atleast 5 arguments
              //Converts pid_str to integer
              int pid_num = strToint(third_arg);
              int signal_num =strToint(second_arg);
              //printf("5 ARGS, PID:%d, SIG:%d\n",pid_num,signal_num);
              
              kill(pid_num,signal_num);
          }
          else if(num_args>=3){//atleast 4 arguments
              int pid_num = strToint(second_arg);
              //printf("4 ARGS:%d\n",pid_num);
              kill(pid_num,SIGTERM);
          }
      }
      else if(strcmp(command, "prompt")==0){
          printBlock("Executing prompt");
          //printf("NEW PREFIX:%s\n",new_prefix);
          //printf("CURRENT PREFIX:%s\n",prefix);
          memset(prefix, 0, sizeof(prefix));
          strcpy(prefix, "");
          
          strcat(prefix, second_arg);
          //sprintf(prefix,"%s%s",prefix,new_prefix);
          //prefix = concat(prefix, new_prefix);
      }
      else if(strcmp(command, "printenv")==0){
          printBlock("Executing printenv");
          if(num_args>=3){ //EX: arguments: printenv HOME NULL
              char* env_var = second_arg;
              printenv(env_var);
          }
          else{
              printenv(NULL);
          }
      }
      else if(strcmp(command, "setenv")==0){
          printBlock("Executing setenv");
          char* name;
          char* value;
          
          
          
          if(num_args==4){//EX: arguments: setenv NAME VALUE NULL
              printf("ENTERED SETENV NAME VALUE NULL\n");
              name = second_arg;
              value = third_arg;
              
              printf("NAME:%s, VALUE:%s\n",name,value);
              
//              if(strcmp(name, "PATH")==0){
//                  setenv(name, value,1);
//                  //Should update linked list for path directories (free old one)
//                  freePath(pathlist);
//                  pathlist = get_path();
//              }
              if(strcmp(name, "HOME")==0){
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
          else if(num_args==3){ //EX: arguments: setenv NAME NULL
              name = second_arg;
              //printf("NAME:%s\n",name);
              setenv(name, "", 1);
          }
          else if(num_args==2){//EX: arguments: setenv NULL
              //printf("ENTERED PRINTENV\n");
              printenv(NULL);
          }
      }
      else if(access(command_path, F_OK) == 0 ||command[0]=='.' ||command[0]=='/'){
          //printf("ENTERED FORK\n");
          forkid = fork();
      }
      
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
          
          printf("COMMAND:%s\n",command);
          //    ../  Go up to parent directory and run
          if(command[0]=='.' && command[1]=='.' && command[2]=='/'){
              printf("ENTERED ../\n");
              char* curr_directory = pwd();
              int index = getOccurrence(curr_directory, '/', "last");
              char* parent_directory = getSubstring(curr_directory, 0, index);
              
              //Gets file name
              printf("SUBSTRING:%s\n", &command[3]);

              parent_directory = concat(parent_directory, &command[3]);
              printf("Parent Directory:%s\n",parent_directory);
              if(execvp(parent_directory,arguments)==-1){
                  kill(getpid(),SIGTERM);
                  printf("Not a valid exec\n");
              }
              else{
                  execvp(parent_directory,arguments);
                  printf("Successful exec\n");
              }
              
              free(parent_directory);
              free(curr_directory);
          }
          //   ./  run within current directory
          else if(command[0]=='.' && command[1]=='/' ){
              printf("Executing ./ command:%s\n",command);
              printf("VALUE%d",execvp(command,arguments));
          }else{
            //NOTE: execve deallocates automatically child process memory
              char* new_str = concat("Executing Built-in", command);
              printf("NEW STRING:%s\n",new_str);
              printBlock(new_str);
              free(new_str);
              execve(command_path,arguments,NULL);
          }

              //printf("AFTER EXECUTING\n");
      }
      //Parent
      else if(forkid>0){
          waitpid(forkid, NULL,0);
          //printf("PARENT:\n");
          
      }
      
      //Exits the shell
      if(strcmp(command, "exit")==0){
          printBlock("Executing exit");
          printf("Exiting..\n");
          //Freeing memory paths
          freePath(pathlist);
          free(command_path);
          free(user_input);
          free(prompt_text);
          go = 0;
      }
      else{
          //Freeing memory paths
          freePath(pathlist);
          free(command_path);
          free(user_input);
          free(prompt_text);
      }
      
      
      printf("REACHED END OF WHILE LOOP\n");
      //printf("-----------------------------\n\n");
  }
  return 0;
} /* sh() */

char *which(char *command, struct pathelement *pathlist ){
    struct pathelement *p = pathlist;
    char *located_path = malloc(sizeof(char)*COMMAND_LEN);
    int found = 0;
    
    while (p) {         // WHERE
        printf("PATHLIST:%s\n",p->element);
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

void where(char *command, struct pathelement *pathlist){
    struct pathelement *p = pathlist;
    //printf("ENTERED WHERE\n");
    char curr_str[100];
    strcpy(curr_str, "");
    
    char allInstances[250];
    strcpy(curr_str, "");
    
    while (p) {// WHERE
        sprintf(curr_str, "%s/%s", p->element,command);
        //printf("WHERE:%s\n",curr_str);
        if (access(curr_str, F_OK) == 0){
            //printf("CURRENT ALL INSTANCES:%s\n",allInstances);
            sprintf(allInstances,"%s%s\n",allInstances,curr_str);
            //printf("FOUND\n");
        }
        p = p->next;
    }
    
    printf("PATHS:=%s\n",allInstances);
}

char* getInput(){
    int max_string_length = 30;
    char *input = malloc(sizeof(char)*max_string_length);
    fgets(input, max_string_length,stdin);
    input[strlen(input)-1] = '\0'; //NULL
    return input;
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
            free(new_dir);
            free(current_dir);
        }
        else{
            free(new_dir);
            free(current_dir);
            chdir(directory);
        }
    }
}

void printBlock(char* str){
    printf("%s:\n",str);
}
