
#include "functions.h"

    void threadplay (int * x,int * newsock ,int * port,char ** serverpath , int * numofthreads){

        char buf[2560];
        char * newpath;
        char * path;
        char * type =NULL;
        char * filetype =NULL;
        char * message=NULL;
        char * body=NULL;
        char * contype;
        int  havebody=0;
        unsigned int filesize;




        bzero(buf, sizeof(buf)); /* Initialize buffer */
				if (read(*newsock, buf, sizeof(buf)) < 0) { /* Get message */
					perror("read");
					exit(1);
				}
                if(strlen(buf)==0){
//                    if(path!=NULL) {
//                        free(path);
//                    }
                    *x=1;
                    return;
                }

                if(*serverpath == NULL){

                    free(*serverpath);
                }

                getSettings(port,serverpath,numofthreads);
                printf("\nThe server path inside FUNCTION is : %s \n",*serverpath);
                printf("\nThe PORT inside FUNCTION is : %d \n",*port);
                printf("\nThe NUMBER OF THREADS inside FUNCTION is : %d \n\n",*numofthreads);
				printf("\nRead string: ****%s***\n", buf);
                tokenize(&contype,&path,&type,buf);
                printf("\nThe PATH OUTSIDE TOKENIZE IS : *****%s*****\n",path);
                printf("\nThe CONNECTION TYPE OUTSIDE TOKENIZE IS : *****%s*****\n",contype);
                printf("\nThe REQUEST TYPE OUTSIDE TOKENIZE IS : *****%s*****\n",type);

                if(strcmp(contype,"close")==0){
                    *x=1;

                }

                 if(strcmp(contype,"keep-alive")==0){
                    *x=0;
                 }
                path++;
                newpath = malloc(356);
                strcpy(newpath,  *serverpath);
                strcat(newpath,"/");
                strcat(newpath,path);

                printf("\nTHE FULL PATH IS : ******%s*****\n",newpath);
                //printf("***********\n%s\n",path);
                printf("\nTHE TYPE OF REQUEST IS : %s\n",type);
                findMIME(path,&filetype);
                //findMIME(path,NULL);
                printf("\nTHE FILE TYPE OF MIME IS: ****%s****\n",filetype);
                execute(newpath,type,filetype,&message,&body,&havebody,&filesize);
                printf("\n***********\n%s\n",message);
				printf("\nTHE BODY OUTSIDE EXECUTE IS:\n%s\n*****\n",body);

                printf("\nTHE HAVEBODY VALUE before write is : %d \n",havebody);

				if (write(*newsock, message,strlen(message)) < 0) {/* Send message */
					perror("write");
					exit(1);
				}
                if(havebody==1 && strcmp(type,"HEAD") != 0) {
                    if (write(*newsock, body, filesize + 1) < 0) {/* Send message */
                        perror("write");
                        exit(1);
                    }
                }
                if(newpath!=NULL) {
                    free(newpath);
                }

                if(message!=NULL) {
                     free(message);
                }
                if (havebody==1) {
                    free(body);
                }
                 if (filetype!=NULL) {
                    free(filetype);
                 }


        printf("\nTEST\n");

}


void getSettings(int *port, char **ServerPath, int *numOfThreads){
    FILE *fp;
    fp = fopen("conf.txt","r");
    if(!fp){
        perror("Error");
        exit(EXIT_FAILURE);
    }

    char line1[256];
    char line2[256];
    char line3[256];

    (fgets(line1, sizeof(line1), fp));
    *port = atoi(line1);
    if(*port<=0){
        fprintf(stderr, "Number of ports must be positive number");
        exit(EXIT_FAILURE);
    }

    (fgets(line2, sizeof(line2), fp));
    *ServerPath= malloc(256);
    strcpy(*ServerPath,  line2);

    char *pos;

    if ((pos=strchr(*ServerPath, '\n')) != NULL)
        *pos = '\0';

    (fgets(line3, sizeof(line3), fp));
    *numOfThreads = atoi(line3);
    if(*numOfThreads<=0){
        fprintf(stderr, "Number of threads must be bigger than 1");
        exit(EXIT_FAILURE);
    }


}


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
    char textCSS[9] = "text/css";

    if(!strcmp(temp,"txt") || !strcmp(temp,"sed") || !strcmp(temp,"awk") || !strcmp(temp,"c") || !strcmp(temp,"h")){
        *MIME = malloc(11);
        strcpy(*MIME,  textPlain);
    }
    else if(!strcmp(temp,"html") || !strcmp(temp,"htm")){
        *MIME = malloc(10);
        strcpy(*MIME,  textHtml);
    }
    else if(!strcmp(temp,"css")){
        *MIME = malloc(9);
        strcpy(*MIME,  textCSS);
    }
    else if(!strcmp(temp,"jpeg") || !strcmp(temp,"jpg")){
        *MIME = malloc(11);
        strcpy(*MIME,  imageJpeg);
    }
    else if(!strcmp(temp,"gif")){
        *MIME = malloc(10);
        strcpy(*MIME,  imageGif);
    }
    else if(!strcmp(temp,"pdf")){
        *MIME = malloc(16);
        strcpy(*MIME,  applicationPDF);
    }
    else{
        *MIME = malloc(25);
        strcpy(*MIME,  applicationOctetStream);
    }

    //printf("eXtension: %s", temp);
}





unsigned int countFileLength(FILE *fp){
    fseek(fp, 0, SEEK_END);
    unsigned int length = ftell(fp);
    return length;
}


void tokenize(char ** contype ,char **path, char **RequestType, char *buf) {
    char *pch;
    int i =0;
    *contype =  NULL;

    while ( i != strlen(buf) ){
        if (buf[i]=='\n'){
            buf[i]=' ';
        }
        if (buf[i]=='\r'){
            buf[i]=' ';
        }
        i++;
    }

    pch = strtok(buf, " ");
    * RequestType= pch;
    pch = strtok (NULL, " ");
    * path = pch;

    while (pch != NULL) {
        if(strcmp(pch,"Connection:")==0 ){
            pch = strtok (NULL, " ");
            *contype = pch;

        }
        pch = strtok (NULL, " ");
    }

    if(*contype == NULL){
        *contype = "close";
    }

//    int myflag = 0;


//*path = "nothing";
//    * RequestType= pch;
//
////    while (pch != NULL) {
//
//        if(myflag){
//            *path = pch;
//            myflag=0;
//        }
//        if(strcmp(pch,"GET")==0 || strcmp(pch,"HEAD")==0 || strcmp(pch,"DELETE")==0){
//            myflag=1;
//        }
//        pch = strtok (NULL, " ");
//
//    }
}


void execute(char * path,char * type,char * filetype, char** action,char ** body,int * havebody, unsigned int * filesize ){


printf("\n\nFILE TYPE: %s\n\n",filetype);

    if (strcmp(type,"GET") == 0 || strcmp(type,"HEAD") == 0) {


        if (strcmp(filetype,"text/html") == 0){


            unsigned int length;
            FILE * file;
            file = fopen(path, "rb");

            if (file){
                * havebody=1;
                length = countFileLength(file);
                *filesize=length;
                printf("\n\nTHE LENGTH INSIDE EXECUTE IS :^^^^^^^^^%d^^^^^^^^^^\n\n",length);
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
                printf("\n\nTHE BODY INSIDE EXECUTE IS :\n%s\n\n",*body);

                return;
            }


            else{
                * action = malloc(656);
                sprintf(*action, "HTTP/1.1 404 Not Found\r\n");    //line:netp:servestatic:beginserve
                sprintf(*action, "%sServer: Sysstatd Web Server\r\n", *action);
                sprintf(*action, "%sContent-length: 20\r\n", *action);
                sprintf(*action, "%sConnection: keep-alive\r\n", *action);
                sprintf(*action, "%sContent-type: text/plain\r\n\r\n", *action);
                sprintf(*action, "%sDocument not found!\r\n\r\n", *action);
                *havebody = 0;
                return;
            }




        }

        if (strcmp(filetype,"text/css") == 0){

            //printf("\n\nwwwwwwwwwwwoooooooorrrrrrrrrrrrkkkkk\n\n");
            unsigned int length;
            FILE * file;
            file = fopen(path, "rb");

            if (file){
                * havebody=1;
                length = countFileLength(file);
                *filesize=length;
                printf("\n\nTHE LENGTH INSIDE EXECUTE IS :^^^^^^^^^%d^^^^^^^^^^\n\n",length);
                * action = malloc(length + 356);
                * body = malloc(length+87);

                sprintf(*action, "HTTP/1.1 200 OK\r\n");    //line:netp:servestatic:beginserve
                sprintf(*action, "%sServer: MY SERVER AD\r\n", *action);
                sprintf(*action, "%sContent-length: %d\r\n", *action,length);
                sprintf(*action, "%sConnection: keep-alive\r\n", *action);
                sprintf(*action, "%sContent-type: text/css\r\n\r\n", *action);
                //sprintf(*action, "%s<html><head><title>It worked!!!</title></head><body><h1>Yes, It worked!!!</h1></body></html>",*action);
                fclose(file);


                file = fopen(path, "rb");
                fread(*body,length, 1, file);
                printf("\n\nTHE BODY INSIDE EXECUTE IS :\n%s\n\n",*body);

                return;
            }


            else{
                * action = malloc(656);
                sprintf(*action, "HTTP/1.1 404 Not Found\r\n");    //line:netp:servestatic:beginserve
                sprintf(*action, "%sServer: Sysstatd Web Server\r\n", *action);
                sprintf(*action, "%sContent-length: 20\r\n", *action);
                sprintf(*action, "%sConnection: keep-alive\r\n", *action);
                sprintf(*action, "%sContent-type: text/plain\r\n\r\n", *action);
                sprintf(*action, "%sDocument not found!\r\n\r\n", *action);
                *havebody = 0;
                return;
            }




        }


        if (strcmp(filetype,"text/plain") == 0){
            unsigned int length;
            FILE * file;
            file = fopen(path, "rb");


            if (file){
                  printf("\niparxei\n");
                * havebody=1;
                length = countFileLength(file);
                *filesize=length;
                printf("\n\n^^^^^^^^^%d^^^^^^^^^^\n\n",length);
                * action = malloc(length + 356);
                * body = malloc(length+87);

                sprintf(*action, "HTTP/1.1 200 OK\r\n");    //line:netp:servestatic:beginserve
                sprintf(*action, "%sServer: MY SERVER AD\r\n", *action);
                sprintf(*action, "%sContent-length: %d\r\n", *action,length);
                sprintf(*action, "%sConnection: keep-alive\r\n", *action);
                sprintf(*action, "%sContent-type: text/plain\r\n\r\n", *action);

                fclose(file);


                file = fopen(path, "rb");
                fread(*body,length,1,file);
                printf("\n\nxxxxxxxxxxxxxx\n%s\nxxxxxxxxxxxxxxxxxx\n\n",*body);

                return;
            }


            else{
                printf("\nDEN iparxei\n");
                * action = malloc(656);
                sprintf(*action, "HTTP/1.1 404 Not Found\r\n");    //line:netp:servestatic:beginserve
                sprintf(*action, "%sServer: Sysstatd Web Server\r\n", *action);
                sprintf(*action, "%sContent-length: 20\r\n", *action);
                sprintf(*action, "%sConnection: keep-alive\r\n", *action);
                sprintf(*action, "%sContent-type: text/plain\r\n\r\n", *action);
                sprintf(*action, "%sDocument not found!\r\n\r\n", *action);
                *havebody = 0;
                return;
            }


        }

        if (strcmp(filetype,"image/jpeg") == 0){
            unsigned int length;
            FILE * file;
            file = fopen(path, "rb");


            if (file){
                printf("\niparxei\n");
                * havebody=1;
                length = countFileLength(file);
                *filesize=length;
                printf("\n\n^^^^^^^^^%d^^^^^^^^^^\n\n",length);
                * action = malloc(length + 356);
                * body = malloc(length+87);

                sprintf(*action, "HTTP/1.1 200 OK\r\n");    //line:netp:servestatic:beginserve
                sprintf(*action, "%sServer: MY SERVER AD\r\n", *action);
                sprintf(*action, "%sContent-length: %d\r\n", *action,length);
                sprintf(*action, "%sConnection: keep-alive\r\n", *action);
                sprintf(*action, "%sContent-type: image/jpeg\r\n\r\n", *action);

                fclose(file);


                file = fopen(path, "rb");
                fread(*body,length,1,file);
                printf("\n\nxxxxxxxxxxxxxx\n%s\nxxxxxxxxxxxxxxxxxx\n\n",*body);

                return;
            }


            else{
                * action = malloc(656);
                sprintf(*action, "HTTP/1.1 404 Not Found\r\n");    //line:netp:servestatic:beginserve
                sprintf(*action, "%sServer: Sysstatd Web Server\r\n", *action);
                sprintf(*action, "%sContent-length: 20\r\n", *action);
                sprintf(*action, "%sConnection: keep-alive\r\n", *action);
                sprintf(*action, "%sContent-type: text/plain\r\n\r\n", *action);
                sprintf(*action, "%sDocument not found!\r\n\r\n", *action);
                *havebody = 0;
                return;
            }


        }

        if (strcmp(filetype,"image/gif") == 0){
            unsigned int length;
            FILE * file;
            file = fopen(path, "rb");


            if (file){
                printf("\niparxei\n");
                * havebody=1;
                length = countFileLength(file);
                *filesize=length;
                printf("\n\n^^^^^^^^^%d^^^^^^^^^^\n\n",length);
                * action = malloc(length + 356);
                * body = malloc(length+87);

                sprintf(*action, "HTTP/1.1 200 OK\r\n");    //line:netp:servestatic:beginserve
                sprintf(*action, "%sServer: MY SERVER AD\r\n", *action);
                sprintf(*action, "%sContent-length: %d\r\n", *action,length);
                sprintf(*action, "%sConnection: keep-alive\r\n", *action);
                sprintf(*action, "%sContent-type: image/gif\r\n\r\n", *action);


                fclose(file);


                file = fopen(path, "rb");
                fread(*body,length,1,file);
                printf("\n\nxxxxxxxxxxxxxx\n%s\nxxxxxxxxxxxxxxxxxx\n\n",*body);

                return;
            }


            else{
                * action = malloc(656);
                sprintf(*action, "HTTP/1.1 404 Not Found\r\n");    //line:netp:servestatic:beginserve
                sprintf(*action, "%sServer: Sysstatd Web Server\r\n", *action);
                sprintf(*action, "%sContent-length: 20\r\n", *action);
                sprintf(*action, "%sConnection: keep-alive\r\n", *action);
                sprintf(*action, "%sContent-type: text/plain\r\n\r\n", *action);
                sprintf(*action, "%sDocument not found!\r\n\r\n", *action);
                *havebody = 0;
                return;
            }


        }

        if (strcmp(filetype,"application/pdf") == 0){
            unsigned int length;
            FILE * file;
            file = fopen(path, "rb");


            if (file){
                printf("\niparxei\n");
                * havebody=1;
                length = countFileLength(file);
                *filesize=length;
                printf("\n\n^^^^^^^^^%d^^^^^^^^^^\n\n",length);
                * action = malloc(length + 356);
                * body = malloc(length+87);

                sprintf(*action, "HTTP/1.1 200 OK\r\n");    //line:netp:servestatic:beginserve
                sprintf(*action, "%sServer: MY SERVER AD\r\n", *action);
                sprintf(*action, "%sContent-length: %d\r\n", *action,length);
                sprintf(*action, "%sConnection: keep-alive\r\n", *action);
                sprintf(*action, "%sContent-type: application/pdf\r\n\r\n", *action);

                fclose(file);


                file = fopen(path, "rb");
                fread(*body,length,1,file);
                printf("\n\nxxxxxxxxxxxxxx\n%s\nxxxxxxxxxxxxxxxxxx\n\n",*body);

                return;
            }


            else{
                * action = malloc(656);
                sprintf(*action, "HTTP/1.1 404 Not Found\r\n");    //line:netp:servestatic:beginserve
                sprintf(*action, "%sServer: Sysstatd Web Server\r\n", *action);
                sprintf(*action, "%sContent-length: 20\r\n", *action);
                sprintf(*action, "%sConnection: keep-alive\r\n", *action);
                sprintf(*action, "%sContent-type: text/plain\r\n\r\n", *action);
                sprintf(*action, "%sDocument not found!\r\n\r\n", *action);
                *havebody = 0;
                return;
            }


        }

        unsigned int length;
        FILE * file;
        file = fopen(path, "rb");


        if (file){
            * havebody=1;
            length = countFileLength(file);
            *filesize=length;
            printf("\n\n^^^^^^^^^%d^^^^^^^^^^\n\n",length);
            * action = malloc(length + 356);
            * body = malloc(length+87);

            sprintf(*action, "HTTP/1.1 200 OK\r\n");    //line:netp:servestatic:beginserve
            sprintf(*action, "%sServer: MY SERVER AD\r\n", *action);
            sprintf(*action, "%sContent-length: %d\r\n", *action,length);
            sprintf(*action, "%sConnection: keep-alive\r\n", *action);
            sprintf(*action, "%sContent-type: application/octet-stream\r\n\r\n", *action);

            fclose(file);


            file = fopen(path, "rb");
            fread(*body,length,1,file);
            printf("\n\nxxxxxxxxxxxxxx\n%s\nxxxxxxxxxxxxxxxxxx\n\n",*body);

            return;
        }


        else{
            * action = malloc(656);
            sprintf(*action, "HTTP/1.1 404 Not Found\r\n");    //line:netp:servestatic:beginserve
            sprintf(*action, "%sServer: Sysstatd Web Server\r\n", *action);
            sprintf(*action, "%sContent-length: 20\r\n", *action);
            sprintf(*action, "%sConnection: keep-alive\r\n", *action);
            sprintf(*action, "%sContent-type: text/plain\r\n\r\n", *action);
            sprintf(*action, "%sDocument not found!\r\n\r\n", *action);
            *havebody = 0;
            return;
        }

    }


    if (strcmp(type,"DELETE") == 0) {
        printf("\nThe file for delete is: %s\n",path);

        int status = remove(path);

        if( status == 0 ) {
            printf("\n %s file deleted successfully.\n", path);
            * action = malloc(356);
            sprintf(*action, "HTTP/1.1 200 OK\r\n");
            return;
        }
        else
        {
            * action = malloc(656);
            sprintf(*action, "HTTP/1.1 404 Not Found\r\n");    //line:netp:servestatic:beginserve
            sprintf(*action, "%sServer: Sysstatd Web Server\r\n", *action);
            sprintf(*action, "%sContent-length: 20\r\n", *action);
            sprintf(*action, "%sConnection: keep-alive\r\n", *action);
            sprintf(*action, "%sContent-type: text/plain\r\n\r\n", *action);
            sprintf(*action, "%sDocument not found!\r\n\r\n", *action);
            *havebody = 0;
            return;
        }




    }
    * action = malloc(656);
    sprintf(*action, "HTTP/1.1 501 Not Implemented\r\n");    //line:netp:servestatic:beginserve
    sprintf(*action, "%sServer: Sysstatd Web Server\r\n", *action);
    sprintf(*action, "%sConnection: close\r\n", *action);
    sprintf(*action, "%sContent-type: text/plain\r\n\r\n", *action);
    sprintf(*action, "%sMethod not implemented!\r\n\r\n", *action);
    *havebody = 0;
    return;




}

//int main(){
//    char * reqtype;
//    char * path;
//    char * contype;
//    char *MIME;
//    char buf [] = "GET /index.html Connection: keep-alive";
//    char buf2 [] = "HEAD /index.html Connection: close";
//
//    printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
//    printf("   **********TEST FOR FUNCTION TOKENIZE**********");
//
//    printf("\n\nThe request message 1 is : %s",buf);
//    tokenize(&contype,&path,&reqtype,buf);
//
//    printf("\nThe request type is : %s",reqtype);
//    printf("\nThe path is: *****%s*****",path);
//    printf("\nThe Connection Type is: ****%s****\n",contype);
//    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
//
//    printf("\n\nThe request message 2 is : %s",buf2);
//    tokenize(&contype,&path,&reqtype,buf2);
//
//    printf("\nThe request type is : %s",reqtype);
//    printf("\nThe path is: *****%s*****",path);
//    printf("\nThe Connection Type is: ****%s****\n",contype);
//    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
//
//
//    printf("\n   **********TEST FOR FUNCTION FINDMIME**********");
//
//    printf("\n\nThe file 1 is: test.html\n");
//    findMIME("test.html",&MIME);
//    printf("The MIME is : %s\n",MIME);
//
//    printf("\nThe file 1 is: test.css\n");
//    findMIME("test.css",&MIME);
//    printf("The MIME is : %s\n",MIME);
//
//    printf("\nThe file 1 is: test.gif\n");
//    findMIME("test.gif",&MIME);
//    printf("The MIME is : %s\n",MIME);
//
//    printf("\nThe file 1 is: test.jpg\n");
//    findMIME("test.jpg",&MIME);
//    printf("The MIME is : %s\n",MIME);
//
//    printf("\nThe file 1 is: test.txt\n");
//    findMIME("test.txt",&MIME);
//    printf("The MIME is : %s\n",MIME);
//
//    printf("\nThe file 1 is: test.pdf\n");
//    findMIME("test.pdf",&MIME);
//    printf("The MIME is : %s\n",MIME);
//
//    printf("\nThe file 1 is: test.no\n");
//    findMIME("test.no",&MIME);
//    printf("The MIME is : %s\n",MIME);
//
//    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
//
//    printf("\n   **********TEST FOR FUNCTION GET SETTINGS**********");
//    int port;
//    char *serverPath;
//    int NumOfThreads;
//
//
//    getSettings(&port, &serverPath, &NumOfThreads);
//    //printf("%d , %s, %d", port, serverPath, NumOfThreads);
//    printf("\n THE NUMBER OF PORT FOR cond.txt IS: %d",port);
//    printf("\n THE SERVER PATH FOR cond.txt IS: %s",serverPath);
//    printf("\n THE NUMBER OF PORT FOR cond.txt IS: %d\n",NumOfThreads);
//    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
//
//}

