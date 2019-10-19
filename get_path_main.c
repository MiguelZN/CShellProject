//#include "get_path.h"
//
//int main()
//{
//  char cmd[64];
//  struct pathelement *p;
//
//  p = get_path();
//  while (p) {         // WHERE
//    sprintf(cmd, "%s/gcc", p->element);
//    if (access(cmd, F_OK) == 0)
//      printf("[%s]\n", cmd);
//    p = p->next;
//  }
//
//  printf("----------\n");
//
//  p = get_path();
//  while (p) {         // WHICH
//    sprintf(cmd, "%s/gcc", p->element);
//    if (access(cmd, X_OK) == 0) {
//      printf("[%s]\n", cmd);
//      break;
//    }
//    p = p->next;
//  }

//char*[] getArgs(char* str);

//  char *prompt = calloc(PROMPTMAX, sizeof(char));
//  char *commandline = calloc(MAX_CANON, sizeof(char));
//  char *command, *arg, *commandpath, *p, *pwd, *owd;
//  char **args = calloc(MAXARGS, sizeof(char*));
//  int uid, i, status, argsct, go = 1;
//  struct passwd *password_entry;
//  char *homedir;

//  uid = getuid();
//  password_entry = getpwuid(uid);               /* get passwd info */
//  homedir = password_entry->pw_dir;        /* Home directory to start
//                          out with*/
//
//  if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
//  {
//    perror("getcwd");
//    exit(2);
//  }
//  owd = calloc(strlen(pwd) + 1, sizeof(char));
//  memcpy(owd, pwd, strlen(pwd));
//  prompt[0] = ' '; prompt[1] = '\0';


/*Should return a string array:
 0th index: Command
 1th index: File name
 2th+index: flags
 */
//char** getArgs(char* str){
//    int length = strlen(str);
//    int arr_size=1;
//
//
//    int found_letter = 0; //false
//
//    //Checking for white-spaces to distinguish arguments
//    //EX: "ls -r"
//    for(int i=0;i<length;i++){
//        if(str[i]==' ' && found_letter==0){
//            arr_size+=1;
//        }
//        else if(str[i]!=' '){
//            found_letter = 1;
//        }
//    }
//
//    //2d string array
//    char** args = malloc(sizeof(char)*arr_size); //allocates string indexes
//    for(int i=0;i<arr_size;i++){
//        args[i] = malloc(sizeof(char)*length); //allocates size of string for each index
//    }
//
//
//    //fills 2d string array
//    for(int i=0;i<arr_size;i++){
//        for(int k=0;k<length;k++){
//            if(str[k]!=' '){
//                args[i][k] = str[k];
//            }
//            else if(str[k]==' '){
//                break;
//            }
//        }
//    }
//
//    return args;
//}

//}

//char** getArguments(char* str, char* specifer){
//    char *rest = NULL;
//    char *token;
//
//    int i =0;
//    for (token = strtok_r(str, specifer, &rest);token != NULL;
//         token = strtok_r(NULL, specifer, &rest)) {
//        printf("token:%s\n", token);
//        i+=1;
//    }
//
//    char** stringarr = malloc(sizeof(char)*i);
//    for(int k=0;k<i;k++){
//        stringarr[k] = malloc(sizeof(char)*(strlen(str)/i));
//    }
//
//    i = 0;
//    for (token = strtok_r(str, specifer, &rest);token != NULL;
//        token = strtok_r(NULL, specifer, &rest)) {
//        printf("token:%s\n", token);
//        stringarr[i] = strdup(token);
//        i+=1;
//    }
//
//    return stringarr;
//
//}



