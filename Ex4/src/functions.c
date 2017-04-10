#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <fnmatch.h>
#include "functions.h"


void findMIME(char *filename, char **MIME){
    char *temp;
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename){
        temp = "";
        return;
    }
    temp = dot + 1;

    char textPlain[11] = "text/plain";
    char textHtml[10] = "text/html";
    char imageJpeg[11] = "image/jpeg";
    char imageGif[10] = "image/gif";
    char applicationPDF[16] = "application/pdf";
    char applicationOctetStream[25] = "application/octet-stream";

    if(!strcmp(temp,"txt") || !strcmp(temp,"sed") || !strcmp(temp,"awk") || !strcmp(temp,"c") || !strcmp(temp,"h")){
        *MIME = textPlain;
    }
    else if(!strcmp(temp,"html") || !strcmp(temp,"htm")){
        *MIME = textHtml;
    }
    else if(!strcmp(temp,"jpeg") || !strcmp(temp,"jpg")){
        *MIME = imageJpeg;
    }
    else if(!strcmp(temp,"gif")){
        *MIME = imageGif;
    }
    else if(!strcmp(temp,"pdf")){
        *MIME = applicationPDF;
    }
    else{
        *MIME = applicationOctetStream;
    }

    //printf("eXtension: %s", temp);
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
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


void execute(char * path,char * type,char * filetype, char** action,char ** body,int * havebody ){


    if (strcmp(type,"GET") == 0) {
        //printf("\n\ninside GET $$$$$$$$$$$$$$$$$$$$\n\n");

        if (strcmp(filetype,"text/html") == 0){
            * havebody=1;
            //printf("\n\nwwwwwwwwwwwoooooooorrrrrrrrrrrrkkkkk\n\n");
            unsigned int length;
            FILE * file;
            file = fopen(path, "rb");

            if (file){
                length = countFileLength(file);
                printf("\n\n^^^^^^^^^%d^^^^^^^^^^\n\n",length);
                * action = malloc(length + 356);
                * body = malloc(length+87);

                sprintf(*action, "HTTP/1.1 200 OK\r\n");    //line:netp:servestatic:beginserve
                sprintf(*action, "%sServer: MY SERVER AD\r\n", *action);
                sprintf(*action, "%sContent-length: %d\r\n", *action,length);
                sprintf(*action, "%sConnection: keep-alive\r\n", *action);
                sprintf(*action, "%sContent-type: text/html\r\n\r\n", *action);
                //sprintf(*action, "%s<html><head><title>It worked!!!</title></head><body><h1>Yes, It worked!!!</h1></body></html>",*action);
                fclose(file);


                file = fopen(path, "rb");
                fread(*body,length, 1, file);
                printf("\n\nxxxxxxxxxxxxxx %s xxxxxxxxxxxxxxxxxx\n\n",*body);

                return;
            }
            else{
//                sprintf(action, "HTTP/1.1 404 Not Found\r\n");    //line:netp:servestatic:beginserve
//                sprintf(action, "%sServer: Sysstatd Web Server\r\n", action);
//                sprintf(action, "%sContent-length: 20\r\n", action);
//                sprintf(action, "%sConnection: keep-alive\r\n", action);
//                sprintf(action, "%sContent-type: text/plain\r\n\r\n", action);
//                sprintf(action, "%sDocument not found!\r\n\r\n", action);
            }




        }




    }

//    printf("\n\nwwwwwwwwwwwoooooooorrrrrrrrrrrrkkkkk\n\n");
//    * action = malloc(4355);
//
//        sprintf(*action, "HTTP/1.1 200 OK\r\n");    //line:netp:servestatic:beginserve
//        sprintf(*action, "%sServer: Sysstatd Web Server\r\n", *action);
//        sprintf(*action, "%sContent-length: 211\r\n", *action);
//        sprintf(*action, "%sConnection: keep-alive\r\n", *action);
//        sprintf(*action, "%sContent-type: text/html\r\n\r\n", *action);
//        //sprintf(*action, "%s<html><head><title>It worked!!!</title></head><body><h1>Yes, It worked!!!</h1></body></html>",*action);
//
//    * body = malloc(4355);
//
//




}


