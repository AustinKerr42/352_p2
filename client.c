#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAXLINE 80 /* The maximum length command */
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"

#define PARSE_ACCESS		16	/* Access scheme, e.g. "HTTP" */
#define PARSE_HOST		 8	/* Host name, e.g. "www.w3.org" */
#define PARSE_PATH		 4	/* URL Path, e.g. "pub/WWW/TheProject.html" */

#define PARSE_VIEW               2      /* Fragment identifier, e.g. "news" */
#define PARSE_FRAGMENT           PARSE_VIEW
#define PARSE_ANCHOR		 PARSE_VIEW

#define PARSE_PUNCTUATION	 1	/* Include delimiters, e.g, "/" and ":" */
#define PARSE_ALL		31

int main(int argc, char *argv[]){
    // char *args[MAXLINE/2 + 1]; /* command line arguments */
    // int numCommands = 0;
    //
    // //getting user input
    // char input[MAXLINE];
    // char *token;
    // int argNum, j, status;
    // char *helper;
    // int fd[2];
    // bool pipeIt = false;
    //
    // //history functionality
    // FILE *historyLog;
    // char history[10][MAXLINE];
    // int h_length = 0;
    // // char home[30];
    // // strncpy(home, "/home/austin/Documents/352/", 30);
    // char lastDir[50];

    bool headerOnly = false, validURL = false;
    char *timeInterval, *url, protocol[10], portString[6];
    int port, portPlace;

    int i, j, k;
    for(i=1; i < argc; i++){
      if(strcmp(argv[i], "-h") == 0){
        printf("Header flag.\n");
        headerOnly = true;
      }else if(strcmp(argv[i], "-d") == 0){
        printf("Date flag.\n");
        if(i+2 < argc){
          timeInterval = argv[i+1];
        }else{
          printf("Incorrect paramaters.\n");
        }
      }else{
        url = argv[i];
        if(strlen(url) < 11){
          printf("Invalid URL.\n");
          return 0;
        }
        printf("Starting url: %s\n", url);

        //make sure url is using http:// protocol
        memcpy(protocol, &url[0], 7);
        protocol[7] = '\0';
        strcpy(url, &url[7]);

        //get the port from url
        for(j=0; j < strlen(url); j++){
          //standard port used
          if(strcmp(url[j], "/")){
            port = 80;
            break;
          }else if(strcmp(url[j], ":")){
            //port is specified
            for(k=j; k < strlen(url); k++){
              if(strcmp(url[k], "/")){
                //end of port number
                break;
              }else if(portPlace < 6){
                //make sure it is a number and add to port
                if(isdigit(url[k])){
                  portString += url[k];
                  portPlace++;
                }
              }else{
                printf("Specified port is invalid.\n");
                return 0;
              }
            }
            //convert portString to int
            port = (int) portString;
          }
        }
        printf("Port: %d\n", port);

        //get the hostname

        if(strcmp(protocol, "http://") == 0){
          validURL = true;
        }


        printf("protocol: %s\n", protocol);
      }
    }

//
//     //get input flags
//     fgets(input, MAXLINE, stdin);
//     if (strlen(input) > 0){
//       if (input[strlen (input) - 1] == '\n'){
//         input[strlen (input) - 1] = '\0';
//       }
//     }
//
// //for history commands
//     if(numCommands > 0){
//       helper = strdup(args[0]);
//     }
// //user pushed 'enter' w/out any text
//     if(strcmp(input, "") == 0){
//       continue;
//     }else if(strcmp(input, "exit") == 0){
//       break;
//     }
// //history command !!
//     else if(strcmp(args[0], "!!") == 0){
//       strcpy(input, history[h_length-1]);
//     }
// //history command !h_num
//     else if(numCommands > 0 && helper[0] == '!'){
//       //get int for history command
//       for(int h=0; h < strlen(helper)-1; h++){
//         helper[h] = helper[h+1];
//       }
//       helper[strlen(helper)-1] = '\0';
//       int h_num = atoi(helper);
//       if(h_num > 0 && h_num <= h_length){
//         strcpy(input, history[h_length-h_num]);
//       }else{
//         printf("Command not in the valid range\n");
//         continue;
//       }
//
//     }
// //pipe
//     //TODO not functional
//     // else if(argNum > 0){
//     //   if(strcmp(args[1], "|more") == 0){
//     //     pipeIt = true;
//     //     pipe(fd);
//     //   }
//     // }
// //help command
//     else if(strcmp(args[0], "help") == 0){
//       printf(KRED "\nHelp menu\n" KNRM);
//       printf("List of commands:\n");
//       printf("\
// cd: change directories\n\
// pwd: print working directory\n\
// ls: list files\n\
// dir: list files\n\
// cat ..: display the text of a file\n\
// cal: show calendar\n\
// whoami: print current user\n\
// .. | more: pipe command output to the pager\n\
// exit: stop program\n");
//       continue;
//     }
// //if histroy command was used, deallocate space
//     if(numCommands > 0){
//       free(helper);
//     }
//
// //add to history
//     if(h_length == 10){
//       for(int z=0; z < h_length; z++){
//         strcpy(history[z], history[z+1]);
//       }
//       strcpy(history[h_length-1], input);
//     }else{
//       strcpy(history[h_length], input);
//       h_length++;
//     }
//
// //put history in log file
//     historyLog = fopen("/home/austin/Documents/352/352_p1/history.txt", "a");
//     for(j=0; j < h_length; j++){
//         fprintf(historyLog, "h[%d]: %s ", j, history[j]);
//     }
//     fprintf(historyLog, "\n");
//     fclose(historyLog);
//
// //check for multiple commands
//     if(numMultiple < 1){
//       memset(delimited, 0, 10);
//       numMultiple = 0;
//       tokenD = strtok(input, ";");
//
//       //first command is put in input, rest stored in delimited[]
//       strcpy(input, tokenD);
//       tokenD = strtok(NULL, ";");
//       while (tokenD != NULL)
//       {
//           delimited[numMultiple++] = tokenD;
//           tokenD = strtok(NULL, ";");
//       }
//     }
//
//     //parse input & clear args
//     memset(args, 0, MAXLINE/2 + 1);
//     argNum=0; status = 0;
//
//     /* Establish string and get the first token: */
//     token = strtok(input, " ");
//     while(token != NULL){
//       args[argNum] = token;
//       argNum++;
//       /* Get next token: */
//       token = strtok(NULL, " ");
//     }
    return 0;
}
