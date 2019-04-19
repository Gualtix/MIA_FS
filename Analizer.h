#ifndef ANALIZER_H
#define ANALIZER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "Fw/DoublyGenericList.h"
#include "Fw/StringHandler.h"
#include "Container.h"

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ T O O L S
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

InfoCatcher* fillInfoCatcher(DoublyGenericList* CommandList,InfoCatcher** nwInf){

    char* Prm_Izq = NULL;
    char* Prm_Der = NULL;

    while(CommandList->Length > 0){

        Prm_Izq = (char*)DeQueue(CommandList);
        Prm_Der = (char*)DeQueue(CommandList);

        /*

        if(strcasecmp(Prm_Izq,"-path") && strcasecmp(Prm_Izq,"-cont")){
            if(Prm_Der != NULL){
                String_To_Lower(&Prm_Der);
            }
        }

        //(^< ............ ............ ............   _filen
        //if(!strcasecmp(Prm_Izq,"-filen")){
        //    if(Prm_Der != NULL){
        //        (*nwInf)->_filen = New_Char_Init_Loaded(Prm_Der);
        //    }
        //    continue;
        //}

        //(^< ............ ............ ............   _size
        if(!strcasecmp(Prm_Izq,"-size")){
            if(IsString_a_Number(Prm_Der) == 1){
                (*nwInf)->_size =  atoi(Prm_Der);
            }
            continue;
        }

        //(^< ............ ............ ............   _P
        if(!strcasecmp(Prm_Izq,"-P")){
            (*nwInf)->_P = 'p';
            if(Prm_Der != NULL){
                FrontInsert(CommandList,Prm_Der);
            }
            
            continue;
        }

        //(^< ............ ............ ............   _R
        if(!strcasecmp(Prm_Izq,"-R")){
            (*nwInf)->_R = 'r';
            FrontInsert(CommandList,Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _filen
        //if(!strcasecmp(Prm_Izq,"-file")){
        //    (*nwInf)->_R = 'r';
        //    FrontInsert(CommandList,Prm_Der);
        //}

        //(^< ............ ............ ............   _id
        if(!strcasecmp(Prm_Izq,"-id")){
            (*nwInf)->_id = New_Char_Init_Loaded(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _grp
        if(!strcasecmp(Prm_Izq,"-grp")){
            (*nwInf)->_grp = New_Char_Init_Loaded(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _pws
        if(!strcasecmp(Prm_Izq,"-pwd")){
            (*nwInf)->_pwd = New_Char_Init_Loaded(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _path
        if(!strcasecmp(Prm_Izq,"-path")){
            (*nwInf)->_path = New_Char_Init_Loaded(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _dest
        if(!strcasecmp(Prm_Izq,"-dest")){
            (*nwInf)->_dest = New_Char_Init_Loaded(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _ugo
        if(!strcasecmp(Prm_Izq,"-ugo")){
            (*nwInf)->_ugo = New_Char_Init_Loaded(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _usr
        if(!strcasecmp(Prm_Izq,"-usr")){
            (*nwInf)->_usr = New_Char_Init_Loaded(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _name
        if(!strcasecmp(Prm_Izq,"-name")){
            (*nwInf)->_name = New_Char_Init_Loaded(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _ruta
        if(!strcasecmp(Prm_Izq,"-ruta")){
            (*nwInf)->_ruta = New_Char_Init_Loaded(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _cont
        if(!strcasecmp(Prm_Izq,"-cont")){
            (*nwInf)->_cont = New_Char_Init_Loaded(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _type
        if(!strcasecmp(Prm_Izq,"-type")){
            (*nwInf)->_type = New_Char_Init_Loaded(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _fs
        if(!strcasecmp(Prm_Izq,"-fs")){
            (*nwInf)->_fs = New_Char_Init_Loaded(Prm_Der);
            continue;
        }
        */
    }
    
}

void Exec_CMD(DoublyGenericList* CommandList){

    char* Prm_Izq = (char*)DeQueue(CommandList);
    char* Prm_Der = (char*)DeQueue(CommandList);

    char* CompletePathDir = Prm_Der;

    FILE * Fl;
    Fl = fopen(CompletePathDir, "r");

    if(Fl != NULL){

        while(feof(Fl) == 0)
        {
            char* CMDLine = newString(200);
            fgets(CMDLine,200, Fl);

            int isEmpty = String_IsEmpty(CMDLine);

            // Si hay un Comentario lo ignora
            if (CMDLine[0] == '#' || CMDLine[0] == '\r' || CMDLine[0] == '\n' || isEmpty){
                fflush(Fl);
                continue;
            }
            //Eliminando Comentarios a Fin de Linea
            else if(isEmpty == 0){
                int i = strlen(CMDLine) - 1;
                while(i > 0 ){
                    if(CMDLine[i] == '#'){
                        CMDLine[i] = '\0';
                        CMDLine = newString(CMDLine);
                    }
                    i--;
                }
            }

            EnQueue(CommandList,CMDLine);
            fflush(Fl);
        }
        fclose(Fl);
    }

    else {
        printf("EXEC ERROR: Archivo .sh No Encontrado\n");
    }
}


void FillCommandList(char* Bf,DoublyGenericList* CommandList){

    Bf = StringCloneWithOut(Bf,'\t');
    Bf = StringCloneWithOut(Bf,'\r');
    Bf = StringCloneWithOut(Bf,'\"');
    Bf = StringCloneWithOut(Bf,'\'');

    while(1){
        Bf = strtok(NULL," ~:~");
        if(Bf == NULL){
            break;
        }

        Bf = StringCloneWithOut(Bf,' ');

        int Lst = strlen(Bf) - 1;

        if(Bf[Lst] == '\n'){
            Bf[Lst] = '\0';
        }

        if(Bf[Lst] == ' '){
            Bf[Lst] = '\0';
        }

        if(Bf[Lst] == '\r'){
            Bf[Lst] = '\0';
        }

        EnQueue(CommandList,Bf);
    }    
}

char *CatchCommandLine(){

    char *line = (char *)malloc(100), *linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if (line == NULL)
        return NULL;

    for (;;)
    {
        c = fgetc(stdin);
        if (c == EOF)
            break;

        if (--len == 0)
        {
            len = lenmax;
            char *linen = (char *)realloc(linep, lenmax *= 2);

            if (linen == NULL)
            {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if ((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ F 1
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void mk_disk_cmd(InfoCatcher* nwInf){

}

void rm_disk_cmd(InfoCatcher* nwInf){

}

void f_disk_cmd(InfoCatcher* nwInf){

}

void rep_cmd(InfoCatcher* nwInf){

}

void mount_cmd(InfoCatcher* nwInf){

}

void unmount_cmd(InfoCatcher* nwInf){

}


//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ F 2
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............


//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ P H A S E S
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

int ScanF1(char* Bf,InfoCatcher* nwInf){

    if(strcasecmp(Bf, "mkdisk") == 0){
        mk_disk_cmd(nwInf);
        return 0;
    }
    else if(strcasecmp(Bf, "rmdisk") == 0){
        rm_disk_cmd(nwInf);
        return 0;
    }
    else if(strcasecmp(Bf, "fdisk") == 0){
        f_disk_cmd(nwInf);
        return 0;
    }
    else if(strcasecmp(Bf, "rep") == 0){
        rep_cmd(nwInf);
        return 0;
    }
    else if(strcasecmp(Bf, "mount") == 0){
        mount_cmd(nwInf);
        return 0;
    }
    else if(strcasecmp(Bf, "unmount") == 0){
        unmount_cmd(nwInf);
        return 0;
    }
    return 1;
}

int ScanF2(char* Bf,InfoCatcher* nwInf){

    /*
    if (!strcasecmp(Bf, "mkfs")){
        mkfs_cmd(CommandList);
        return 0;
    }
    else if (!strcasecmp(Bf, "login")){
        if(Usr_inUse != NULL){
            printf("\n");
            printf("LOGIN ERROR: Ya existe una Sesion Iniciada...\n");
            return 0;
        }
        login_cmd(CommandList);
        return 0;
    }
    else if (!strcasecmp(Bf, "logout")){
        int lg = is_it_Logged();
        if(lg == 0){
            printf("\n");
            printf("LOGOUT ERROR: No Hay Ninguna Sesion Iniciada...\n");
            return 0;
        }
        logout_cmd();
        return 0;
    }
    else if (!strcasecmp(Bf, "mkgrp")){
        int lg = is_it_Logged();
        if(lg == 0){
            return 0;
        }
        mkgrp_cmd(CommandList);
        return 0;
    }
    else if (!strcasecmp(Bf, "rmgrp")){
        int lg = is_it_Logged();
        if(lg == 0){
            return 0;
        }
        rmgrp_cmd(CommandList);
        return 0;
    }
    else if (!strcasecmp(Bf, "mkusr")){
        int lg = is_it_Logged();
        if(lg == 0){
            return 0;
        }
        mkusr_cmd(CommandList);
        return 0;
    }
    else if (!strcasecmp(Bf, "rmusr")){
        int lg = is_it_Logged();
        if(lg == 0){
            return 0;
        }
        rmusr_cmd(CommandList);
        return 0;
    }
    else if (!strcasecmp(Bf, "pause")){
        getchar();
        return 0;
    }
    else if (!strcasecmp(Bf, "mkdir")){
        int lg = is_it_Logged();
        if(lg == 0){
            return 0;
        }
        mkdir_cmd(CommandList);
        return 0;
    }
    else if (!strcasecmp(Bf, "mkfile")){
        int lg = is_it_Logged();
        if(lg == 0){
            return 0;
        }
        mkfile_cmd(CommandList);
        return 0;
    }
    else if (!strcasecmp(Bf, "rep")){
        rep_cmd(CommandList);
        return 0;
    }
    else if (!strcasecmp(Bf, "rem")){
        rem_cmd(CommandList);
        return 0;
    }
    
    return 1;
    */



   return 1;
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ A N A L I T I C S
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
void ExecuteComand(char *InputString){

    system("clear");
    printf("\n");
    printf("CommandLine->   %s\n",InputString);

    if (*(InputString) == '\n'){
        return;
    }

    if(strcasecmp(InputString, "Exit\n") == 0){
        printf("\n");
        printf("Saliendo de la APP...\n");
        return;
    }

    DoublyGenericList* CommandList = new_DoublyGenericList();
    
    char* Bf = strtok(InputString, "# \n");

    int unknownCMD = 1;
    FillCommandList(Bf,CommandList);

    // T E M P ----
    DoublyGenericList* tmpCL = new_DoublyGenericList();
    int i = 0;
    while(i < CommandList->Length){
        char* tmp = (char*)getNodebyIndex(CommandList,i)->Dt;
        EnQueue(tmpCL,tmp);
        i++;
    }
    // ------------

    InfoCatcher* nwInf = newInfoCatcher();
    fillInfoCatcher(tmpCL,&nwInf);

    unknownCMD = ScanF1(Bf,nwInf);

    if(unknownCMD != 0){
        unknownCMD = ScanF2(Bf,nwInf);
    }

    if(unknownCMD != 0){
        if (strcasecmp(Bf,"Exec") == 0){        
            Exec_CMD(CommandList);
            while (CommandList->Length > 0)
            {
                char* NewCommand = (char*)DeQueue(CommandList);
                int Ln = strlen(NewCommand);
                if (NewCommand[Ln - 1] == '\n'){
                    NewCommand[Ln - 1] = '\0';
                }
                ExecuteComand(NewCommand);
            }   
        }
        else{
            printf("\n");
            printf("Analizer ERROR: Comando: %s No Reconocido\n", Bf);
        }
    }
    
    getchar();
}


#endif // ANALIZER_H
