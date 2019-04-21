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

#define Byte 1
#define Kilo 1024
#define Mega (Kilo * 1024)


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

int CalcSize_in_Bytes(int PD_Size,char Unit){

    if(Unit =='b'){
        return PD_Size;
    }
    if(Unit == 'k'){
        return PD_Size * Kilo;
    }
    else if(Unit == 'm'){
        return PD_Size * Mega;
    }
    else{
        return -1;
    }
}

int Validate_If_Path_Exists(char* Path){
    DIR* Test = NULL;
    Test = opendir(Path);
    return Test? 1 : 0;
}

int CreatePathDir(char* Path){

    if(Validate_If_Path_Exists(Path)){
        return 1;
    }

    char NewPath[200];

    // To get Permission on /home
    // sudo chown -R $USER:$USER /home
    snprintf(NewPath,sizeof(NewPath),"mkdir -p -v -m777 \"%s\"",Path);
    system(NewPath);

    return Validate_If_Path_Exists(Path)? 1:0;
}


#endif // HELPER_H