/*
* CSCI3150 Assignment 1 - Writing a Simple Shell
* Feel free to modify the given code.
* Press Ctrl-D to end the program
*
*  You may put down your reference here:
*
*/
////////////////////////////////////////////////

/* Header Declaration */
#include <stdio.h>
#include <string.h>
#include <unistd.h> //
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <limits.h>
#include <signal.h>

/* Function Declaration */
int getUserInput(char* input);
void tokenizeInput(char* input);
char *getcwd(char *buf, size_t size);
int chdir(const char *path);
int pcount = 0;
int j;
char* stack[255];
int checkcommand(char** input, int counter);

/* Functions */
int main(int argc, char* argv[])
{
  int isExit = 0;
  do {
    char rawInput[255];
    isExit = getUserInput(rawInput);
    if(isExit) break;
    tokenizeInput(rawInput);

  } 
  while(isExit = 1);
  return 0;
}
 

int getUserInput(char* input)
{
  char buf[255];
  char *s = buf;
  char *pathstr;

  char cwd[255];
  getcwd(cwd, sizeof(cwd));

  printf("[3150 Shell:%s]=> ", cwd);

  if(fgets(buf,255,stdin) == NULL)
  { //if buf is empty
    putchar('\n');
    return 1;
  }
  for(;*s != '\n';s++){/* No statement */};
  *s = '\0';
  strcpy(input,buf);
  return 0;
}

/*
  tokenizeInput()
  - Tokenize the string stored in parameter, delimiter is space
  - In given code, it only prints out the token.
  Please implement the remaining part.
*/

void tokenizeInput(char* input)
{
  char buf[255];
  strcpy(buf, input); 
  int tempcount = 1;
  int i = 0;
  int cutcount = 1;
  int cutcountlast = 1;
  int cutcountmid = 1;
  int help = 0;

  char **record = (char**) malloc(sizeof(char*) * 255); 
  *record = (char*)malloc(sizeof(char) * 10);

  char **recordlast = (char**) malloc(sizeof(char*) * 255); 
  *recordlast = (char*)malloc(sizeof(char) * 10);

  char **recordmid = (char**) malloc(sizeof(char*) * 255); 
  *recordmid = (char*)malloc(sizeof(char) * 10);

  // 1.) && ----- if ok run next, else return
  if(strstr(buf, "&&") != NULL){
    char* token = strtok(buf, "&&");

    char** tempbuf = (char**) malloc(sizeof(char*) * 255); 
    *tempbuf = (char*)malloc(sizeof(char) * 10);
    strcpy(tempbuf[0], token);

    while(token != NULL){
      token = strtok(NULL, "&&");
      tempbuf[tempcount] = token;
      tempcount++;
    }
    tempbuf[tempcount] = NULL;

    for (i = 1; i < tempcount; i++){
      if(i == 1){ // 1st record
        strncpy(*record, tempbuf[i - 1], strlen(tempbuf[i - 1]) - 1);
        char* cut = strtok(record[0], " ");
        while(cut != NULL){
          cut = strtok(NULL, " ");
          record[cutcount] = cut;
          cutcount++;
        }
        tempbuf[cutcount] = NULL;
        if (checkcommand(record, cutcount + 1) == 0){
          break;
        }
      }
      else if(i == tempcount - 1){ 
        char **tmpp = (char**) malloc(sizeof(char*) * 255); 
        *tmpp = (char*)malloc(sizeof(char) * 10);
        strcpy(*tmpp, tempbuf[i - 1]);
        char* cut2 = strtok(*tmpp, " ");
        strcat(*recordlast, cut2);
        while(cut2 != NULL){ 
          cut2 = strtok(NULL, " ");
          recordlast[cutcountlast] = cut2; 
          cutcountlast++;
        }
        recordlast[cutcountlast] = NULL;
        if (checkcommand(recordlast, cutcountlast + 1) == 0){
          break;
        }
      }
      else{
        char **tmppmiddle = (char**) malloc(sizeof(char*) * 255); 
        *tmppmiddle = (char*)malloc(sizeof(char) * 10);
        strncpy(*tmppmiddle, tempbuf[i - 1], strlen(tempbuf[i - 1]) - 1);
        char* cut3 = strtok(*tmppmiddle, " ");
        strcat(*recordmid, cut3);
        while(cut3 != NULL){
          cut3 = strtok(NULL, " ");
          recordmid[cutcountmid] = cut3;
          cutcountmid++;
        }
        recordmid[cutcountmid] = NULL;
        if (checkcommand(recordmid, cutcountmid + 1) == 0){
          break;
        }
      }
    }
    
  }

  // 2.) || ----- if ok return, else run next
  else if (strstr(buf, "||") != NULL){
    char* token = strtok(buf, "||");

    char** tempbuf = (char**) malloc(sizeof(char*) * 255); 
    *tempbuf = (char*)malloc(sizeof(char) * 10);
    strcpy(tempbuf[0], token);

    while(token != NULL){
      token = strtok(NULL, "||");
      tempbuf[tempcount] = token;
      tempcount++;
    }
    tempbuf[tempcount] = NULL;

    for (i = 1; i < tempcount; i++){
      if(i == 1){
        strncpy(*record, tempbuf[i - 1], strlen(tempbuf[i - 1]) - 1);
        char* cut = strtok(record[0], " ");
        while(cut != NULL){
          cut = strtok(NULL, " ");
          record[cutcount] = cut;
          cutcount++;
        }
        tempbuf[cutcount] = NULL;
        if (checkcommand(record, cutcount + 1) != 0){
          break;
        }
      }
      else if (i == tempcount - 1){
        char **tmpp = (char**) malloc(sizeof(char*) * 255); 
        *tmpp = (char*)malloc(sizeof(char) * 10);
        strcpy(*tmpp, tempbuf[i - 1]);
        char* cut2 = strtok(*tmpp, " ");
        strcat(*recordlast, cut2);
        while(cut2 != NULL){ 
          cut2 = strtok(NULL, " ");
          recordlast[cutcountlast] = cut2; 
          cutcountlast++;
        }
        recordlast[cutcountlast] = NULL;
        if (checkcommand(recordlast, cutcountlast + 1) != 0){
          break;
        }
      }
      else{

      }
    }
  }
  
  // 3.) Run directly
  else{
    char* token = strtok(buf, " ");
    int counter = 1;

    strcpy(record[0], token); 

    while(token != NULL) 
    { 
      token = strtok(NULL, " "); 
      record[counter] = token; 
      counter++;
    }
    record[counter] = NULL;
    checkcommand(record, counter);  
  }

}

int checkcommand(char** input, int counter){
  // checkcommand -> goldfolder
  if(strcmp(input[0], "gofolder") == 0){ 
    if(counter != 3){ printf("gofolder: wrong number of arguments\n");}
    else{
      if (chdir(input[1]) != 0){ 
        printf("%d\n", chdir(input[1]));
        printf("{%s}: cannot change directory\n", input[1]);
      }
    }return 0;}
  // checkcommand -> push
  else if (strcmp(input[0], "push") == 0){ 
    if(counter != 3){ printf("push: wrong number of arguments\n");}
      else{
        if(opendir(input[1])){
          char* eriklo = malloc(sizeof(char) *255);
          char currentcwd[255];
          getcwd(currentcwd, sizeof(currentcwd));
          strcpy(eriklo, currentcwd);
          //pcount++;
          stack[pcount] = eriklo;
          pcount++;
          chdir(input[1]);
          for(j = pcount; j > 0; j--){
            printf("%d %s\n", pcount - j, stack[j - 1]);
          }
        }
        else{ printf("{%s}: cannot change directory\n", input[1]);}
      }return 0;}
  // checkcommand pop
  else if (strcmp(input[0], "pop") == 0){ 
    if(counter != 2){printf("pop: wrong number of arguments\n");}
      else{
        pcount--;
        if (pcount == -1){ printf("pop: directory stack empty\n");}
        chdir(stack[pcount]);
        for(j = 0; j < pcount; j++){
          printf("%d %s\n", j, stack[j]);
        }
      }
      return 0;
    }
  // checkcommand dirs
  else if (strcmp(input[0], "dirs") == 0){ 
    if(counter != 2){ printf("dirs: wrong number of arguments\n");}
      else{
        for(j = pcount; j > 0; j--){
          printf("%d %s\n", pcount - j, stack[j - 1]);
        }
      }
      return 0;
    }
  // checkcommand -> bye
  else if (strcmp(input[0], "bye") == 0){ 
    if(counter != 2){ printf("bye: wrong number of arguments\n");}
      else{ kill(getpid(),SIGSEGV);}
        return 0;
    }
  else{}

  //fork
  int pid = fork();
  if (pid == 0){
    if(strcmp(input[0], "ifconfig") == 0){
      printf("{%s}: command not found\n", input[0]);
      exit(1);
    }
    else{
      execvp(input[0], input);
    }
    if(errno != 0){
      char* addstr = malloc(sizeof(char) * 255); 

      strcpy(addstr, "/bin/");
      strcat(addstr, *input);
      execvp(addstr, &addstr);

      strcpy(addstr, "/usr/bin/");
      strcat(addstr, *input);
      execvp(addstr, &addstr);

      strcpy(addstr, "./");
      strcat(addstr, *input);
      execvp(addstr, &addstr);

      printf("{%s}: command not found\n", *input);
      exit(1);
    }
  }
  else{
    wait(&pid);
    if(WEXITSTATUS(pid) == 0){
      return 1;
    }
    else{
      return 0;
    }
  }
  return 0;
}

