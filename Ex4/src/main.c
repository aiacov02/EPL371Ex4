/* File:
 server.c
 */
#include <sys/types.h>
/* For sockets */
#include <sys/socket.h>
/* For sockets */
#include <netinet/in.h>
/* For Internet sockets */
#include <netdb.h>
/* For gethostbyaddr() */
#include <stdio.h>
/* For I/O */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sudo_plugin.h>
#include "functions.h"

void reverse(char *);



//char newpath[1000];
//char message[1024];
char * newpath;
char * serverpath=NULL;
// function prototype for reversing func.
/* Server with Internet stream sockets */
main(int argc, char *argv[]) {
	int port, sock, newsock, serverlen, clientlen;
	//char buf[2560];
	struct sockaddr_in server, client;
	struct sockaddr *serverptr, *clientptr;
	struct hostent *rem;
//    char * path =malloc(787);
//    char * type =NULL;
//    char * filetype =NULL;
//    char * message=NULL;
//    char * body=NULL;



    int numofthreads;
//    int  havebody;
//    unsigned int filesize;


//    FILE * file;
//    file = fopen("villa.txt", "w");
//    if (file){
//        printf("exist");
//        fclose(file);
//    }else{
//        printf("notexist");
//        //file doesn't exists or cannot be opened (es. you don't have access permission )
//    }


//	sprintf(test, "HTTP/1.0 200 OK\r\n");    //line:netp:servestatic:beginserve
//   	sprintf(test, "%sServer: Sysstatd Web Server\r\n", test);
//   	sprintf(test, "%sContent-length: 211\r\n", test);
//	sprintf(test, "%sConnection: keep-alive\r\n", test);
//	sprintf(test, "%sContent-type: text/html\r\n\r\n", test);
//	sprintf(test, "%s<html><head><title>It worked!!!</title></head><body><h1>Yes, It worked!!!</h1></body></html>",test);



//	if (argc < 2) { /* Check if server's port number is given */
//		printf("Please give the port number \n");
//		exit(1);
//	}

	/* Create socket */
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	/* Convert port number to integer */
	getSettings(&port,&serverpath,&numofthreads);

    printf("\n*********************************************");
    printf("\nCHECK GET SETTINGS FUNCTION");
    printf("\nThe port is : %d",port);
    printf("\nThe server path is : %s",serverpath);
    printf("\nThe number of threds is : %d",numofthreads);
    printf("\n*********************************************\n\n");


    //port = atoi(argv[1]);
    //strcpy(newpath,  serverpath);
    //strcat(newpath,"/");
    //strcat(newpath,path);
    //printf("\n********************THE FULL PATH IS : %s*************************",newpath);

    server.sin_family = PF_INET; /* Internet domain */
	server.sin_addr.s_addr = htonl(INADDR_ANY); /* My Internet address */
	server.sin_port = htons(port); /* The given port */
	serverptr = (struct sockaddr *) &server;
	serverlen = sizeof(server);

	/* Bind socket to address */
	if (bind(sock, serverptr, serverlen) < 0) {
		perror("bind");
		exit(1);
	}
	/* Listen for connections */
	if (listen(sock, 5) < 0) {
		/* 5
		 max. requests in queue */
		perror("listen");
		exit(1);
	}
	printf("Listening for connections to port %d\n", port);

	while (1) {


        //havebody = 0;
		clientptr = (struct sockaddr *) &client;
		clientlen = sizeof(client);
		/* Accept connection */
		if ((newsock = accept(sock, clientptr, &clientlen)) < 0) {
			perror("accept");
			exit(1);
		}
		/* Using IP address find DNS name (i.e., reverse DNS)*/
		if ((rem = gethostbyaddr((char *) &client.sin_addr.s_addr,
				sizeof(client.sin_addr.s_addr), client.sin_family)) == NULL) {
			herror("gethostbyaddr");

			exit(1);
		}
		printf("\nAccepted connection from %s\n", rem->h_name);

		/* Create child for serving the client */
		switch (fork()) {
		case -1:
			perror("fork");
			exit(1);
		case 0: /* Child process */

			do {
                threadplay (&newsock ,& port,&serverpath , &numofthreads);
                //threadplay (&newsock ,&port,&serverpath ,&numofthreads,&path,&type,&newpath,&filetype, &message, &body,&havebody,&filesize);
//				bzero(buf, sizeof(buf)); /* Initialize buffer */
//				if (read(newsock, buf, sizeof(buf)) < 0) { /* Get message */
//					perror("read");
//					exit(1);
//				}
//                if(strlen(buf)==0){
//                    exit(EXIT_FAILURE);
//                }
//                getSettings(&port,&serverpath,&numofthreads);
//                printf("\nThe server path inside FORK is : %s \n\n",serverpath);
//				printf("\nRead string: ****%s***\n", buf);
//                tokenize(&path,&type,buf);
//                printf("\nThe FULL PATH OUTSIDE TOKENIZE IS : ***** %s *****\n",path);
//                path++;
//                newpath = malloc(356);
//                strcpy(newpath,  serverpath);
//                strcat(newpath,"/");
//                strcat(newpath,path);
//
//                printf("\n********************THE FULL PATH IS : %s *************************",newpath);
//                printf("***********\n%s\n",path);
//                printf("***********\n%s\n",type);
//                findMIME(path,&filetype);
//                //findMIME(path,NULL);
//                printf("\ncccccccccc %s ccccccccccc\n",filetype);
//                execute(newpath,type,filetype,&message,&body,&havebody,&filesize);
//                printf("\n***********\n%s\n",message);
//				printf("\n******\n%s\n*****\n",body);
//
//                printf("\nTHE HAVEBODY VALUE before write is : %d \n",havebody);
//
//				if (write(newsock, message,strlen(message)) < 0) {/* Send message */
//					perror("write");
//					exit(1);
//				}
//                if(havebody==1 && strcmp(type,"HEAD") != 0) {
//                    if (write(newsock, body, filesize + 1) < 0) {/* Send message */
//                        perror("write");
//                        exit(1);
//                    }
//                }
//                if(newpath!=NULL) {
//                    free(newpath);
//                }
//
//                if(message!=NULL) {
//                    free(message);
//                }
//                if (havebody==1) {
//                    free(body);
//                }
			} while (1); /*Finish on "end" message*/
			close(newsock); /* Close socket */
			exit(0);
		} /* end of switch */
	} /* end of while(1) */
} /* end of main() */

/* Function for reversing a string */
//void reverse(char *s) {
//	int c, i, j;
//	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
//		c = s[i];
//		s[i] = s[j];
//		s[j] = c;
//	}
//}