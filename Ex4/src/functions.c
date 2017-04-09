#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <fnmatch.h>
#include "functions.h"




const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

void findMIME(char *filename, char **MIME){
    char *temp;
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) temp = "";
    else temp = dot + 1;

    if(temp=="")
        printf("aktinidio");
    else
    printf("eXtension: %s", temp);
}

unsigned int countFileLength(FILE *fp){
    fseek(fp, 0, SEEK_END);
    unsigned int length = ftell(fp);
    return length;
}


void tokenize(char **path, char **RequestType, char *buf) {
    char *pch;

    pch = strtok(buf, " ");
    int myflag = 0;



    * RequestType= pch;

    while (pch != NULL) {

        if(myflag){
            *path = pch;
            myflag=0;
        }
        if(strcmp(pch,"GET")==0 || strcmp(pch,"HEAD")==0 || strcmp(pch,"DELETE")==0){
            myflag=1;
        }
        pch = strtok (NULL, " ");

    }
}


void execute(char * path,char * type, char* action ){




    FILE * file;
    file = fopen(path, "r");

    if (file){
	sprintf(action, "HTTP/1.1 200 OK\r\n");    //line:netp:servestatic:beginserve
   	sprintf(action, "%sServer: Sysstatd Web Server\r\n", action);
   	sprintf(action, "%sContent-length: 211\r\n", action);
	sprintf(action, "%sConnection: keep-alive\r\n", action);
	sprintf(action, "%sContent-type: text/html\r\n\r\n", action);
	sprintf(action, "%s<html><head><title>It worked!!!</title></head><body><h1>Yes, It worked!!!</h1></body></html>",action);
        fclose(file);
    }
    else{
        sprintf(action, "HTTP/1.1 404 Not Found\r\n");    //line:netp:servestatic:beginserve
        sprintf(action, "%sServer: Sysstatd Web Server\r\n", action);
        sprintf(action, "%sContent-length: 20\r\n", action);
        sprintf(action, "%sConnection: keep-alive\r\n", action);
        sprintf(action, "%sContent-type: text/plain\r\n\r\n", action);
        sprintf(action, "%sDocument not found!\r\n\r\n", action);
    }


}


