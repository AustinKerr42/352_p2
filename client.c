#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <ctype.h>

#define MAXLINE 80 /* The maximum length command */
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"

int main(int argc, char *argv[]){

    bool headerOnly = false, validURL = false, validPort = true;
    char *timeInterval, *url, protocol[10], portString[5];
    int port=80, portPlace=0, fileStart=0;

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
          if(url[j] == '/'){
	    fileStart = j+1;
            break;
          }else if(url[j] == ':'){
            //port is specified
            for(k=j+1; k < strlen(url); k++){
              if(url[k] == '/'){
                //end of port number
                fileStart = k+1;
                break;
              }else if(portPlace < 4){
                //make sure it is a number and add to port
                if(isdigit(url[k])){
                  portString[portPlace] = url[k];
                  portPlace++;
                }else{
		  validPort = false;
		}
              }else{
                validPort = false;
              }
            }
          }
        }

	if(validPort == false){
	  printf("Port is invalid\n");
	  return 0;
	}else if(portString[0] != '\0'){
	  port = atoi(portString);
	}

        printf("Port: %d\n", port);

        //get the hostname

        if(strcmp(protocol, "http://") == 0){
          validURL = true;
        }


        printf("Protocol: %s\n", protocol);
      }
    }

    return 0;
}
