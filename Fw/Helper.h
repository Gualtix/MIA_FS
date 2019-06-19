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


void AppDiv(){
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

void Div2(){
    printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
}

char* getDateTime(){
    time_t Tmp = time(0);
    char ShapeDate[20];
    strftime(ShapeDate,128,"%d/%m/%y %H:%M:%S",localtime(&Tmp));
    char* Dt = newString(&ShapeDate[0]);
    return Dt;
}


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

char* Path_get_Last_FolderName(char* CompletePathDir){
    DoublyGenericList* PathList = PathSeparate(CompletePathDir);
    
    char* Op = (char*)Pop(PathList);
    char* Path;
    char* FileName;

    if(strcmp(Op,"true") == 0){
        FileName = (char*)Pop(PathList);
        return FileName;
    }

    if(strcmp(Op,"false") == 0){
        Op = NULL;
        if(PathList->Length > 0){
            Op = (char*)Pop(PathList);
        }
        return Op;
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

char* Write_txtFile(char* Path, char* Content){
    FILE* Fl = fopen(Path,"w");
    if(Fl){
        int results = fputs(Content, Fl);
        fclose(Fl);
    }
    
}

char* getString_from_File(char* Path){

    FILE* fp = fopen (Path,"rb");
    long lSize;
    char* buffer;

    if(fp){
        
        //fp = fopen (Path, "rb" );
        //if( !fp ) perror(Path),exit(1);

        fseek(fp,0L,SEEK_END);
        lSize = ftell(fp);
        rewind(fp);

        /* allocate memory for entire content */
        buffer = (char*)calloc( 1, lSize+1 );
        if(!buffer)fclose(fp),fputs("memory alloc fails",stderr),exit(1);

        /* copy the file into the buffer */
        if(1!=fread(buffer,lSize,1,fp))
        fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

        /* do your work here, buffer is a string contains the whole text */
        fclose(fp);
        //free(buffer);

        return newString(buffer);
    }
    else{
        return NULL;
    }
    
}

DoublyGenericList* getSubstring_64CharList(char* Content){

    DoublyGenericList* Ls = new_DoublyGenericList();

    // ******* File Content Sharing *******
    int contLen = strlen(Content);
    int Base = 63;
    int nB = (contLen / Base);
    int nR = (contLen % Base);
    
    int ptr = 0;
    
    int cnt = 0;
    while(cnt < nB){
        char* str = newString(Base);
    
        //memset(str,'\0',sizeof(char) * Base);
        int i = 0;
        while(i < Base){
            str[i] = Content[ptr];
            i++;
            ptr++;
        }

        //str[63] = '\0';
        EnQueue(Ls,str);
        cnt++;
    }


    if(nR > 0){

        int Lm = nR;
        int i = 0;

        char* str = newString(Base);
        memset(str,'\0',sizeof(char) * Base);

        while(i < Lm){
            str[i] = Content[ptr];
            i++;
            ptr++;
        }

        EnQueue(Ls,str);
    }


    if(Ls->Length > 0){
        return Ls;
    }

    return NULL;
}



#endif // HELPER_H