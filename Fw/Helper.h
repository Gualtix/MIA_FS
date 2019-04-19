#ifndef HELPER_H
#define HELPER_H

#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

#include "StringHandler.h"
#include "DoublyGenericList.h"


DoublyGenericList* PathSeparate(char* CompletePathDir){

    DoublyGenericList* PathList = new_DoublyGenericList();

    char* token;
    char* FileName;

    char* tmp = newString(CompletePathDir);
    token = strtok(tmp,"/");
   
    while(token != NULL ) {        
        EnQueue(PathList,newString(token));
        FileName = newString(token);
        token = strtok(NULL, "/");
    }

    char* pch;
    pch = strchr(FileName,'.');

    if(pch == NULL){
        EnQueue(PathList,newString("false"));
    }
    else{
        EnQueue(PathList,newString("true"));
    }

    return PathList;
}

char* Path_Get_Isolated(char* CompletePathDir){

    DoublyGenericList* PathList = PathSeparate(CompletePathDir);
    
    char* Op = (char*)Pop(PathList);
    char* Path;
    char* FileName;

    if(strcmp(Op,"true") == 0){

        FileName = (char*)Pop(PathList);

        int nwLn = strlen(CompletePathDir) - strlen(FileName);
        char* tmp = newString(CompletePathDir);
        tmp[nwLn] = '\0';
        tmp = newString(tmp);
        return tmp;
    }

    if(strcmp(Op,"false") == 0){
        return CompletePathDir;
    }

}

char* Path_Get_FileName(char* CompletePathDir){
    DoublyGenericList* PathList = PathSeparate(CompletePathDir);
    
    char* Op = (char*)Pop(PathList);
    char* Path;
    char* FileName;

    if(strcmp(Op,"true") == 0){
        FileName = (char*)Pop(PathList);
        return FileName;
    }

    if(strcmp(Op,"false") == 0){
        return NULL;
    }

}


char* Path_Get_LastDirName(char* CompletePathDir){
    DoublyGenericList* PathList = PathSeparate(CompletePathDir);
    
    char* Op = (char*)Pop(PathList);
    char* Path;
    char* FileName;
    char* LastDir;

    if(strcmp(Op,"true") == 0){

        FileName = (char*)Pop(PathList);
        LastDir  = (char*)Pop(PathList);
        return LastDir;
    }

    if(strcmp(Op,"false") == 0){
        LastDir  = (char*)Pop(PathList);
        return LastDir;
    }
}


#endif // HELPER_H