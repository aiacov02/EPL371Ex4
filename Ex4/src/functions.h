#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <fnmatch.h>
#include "functions.h"


void tokenize(char **path, char *buf){
    char * pch;

    pch = strtok (buf," ");
    int myflag=0;
    while (pch != NULL){

        if(myflag){
            *path = pch;
            myflag=0;
        }
        if(strcmp(pch,"GET")==0){
            myflag=1;
        }
        pch = strtok (NULL, " ,.-");
    }

}



