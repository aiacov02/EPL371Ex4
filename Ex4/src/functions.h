#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void tokenize(char ** contype ,char **path, char **RequestType, char *buf);
void execute(char * path,char * type,char * filetype,char** action,char ** body,int * havebody,unsigned int * filezize );
unsigned int countFileLength(FILE *fp);
void findMIME(char *filename, char **MIME);
void getSettings(int *port, char **ServerPath, int *numOfThreads);
void threadplay (int * x,int * newsock ,int * port,char ** serverpath , int * numofthreads);
