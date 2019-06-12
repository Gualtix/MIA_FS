#ifndef ANALIZER_H
#define ANALIZER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "Fw/DoublyGenericList.h"
#include "Fw/StringHandler.h"
#include "Container.h"
#include "F1_do.h"
#include "Rep/F1_Rep.h"

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ T O O L S
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

InfoCatcher* fillInfoCatcher(DoublyGenericList* CommandList,InfoCatcher** nwInf){

    char* Prm_Izq = NULL;
    char* Prm_Der = NULL;

    while(CommandList->Length > 0){

        Prm_Izq = (char*)DeQueue(CommandList);
        Prm_Der = (char*)DeQueue(CommandList);

        if(strcmp(Prm_Izq,"-path") != 0){
            if(Prm_Der != NULL){
                String_ByRef_toLower(&Prm_Der);
            }
        }

        //(^< ............ ............ ............   _size
        if(strcmp(Prm_Izq,"-size") == 0){
            int Nm = atoi(Prm_Der);
            (*nwInf)->_size =  atoi(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _fit
        if(strcmp(Prm_Izq,"-fit") == 0){
            (*nwInf)->_fit = newString(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _unit
        if(strcmp(Prm_Izq,"-unit") == 0){
            (*nwInf)->_unit = newString(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _path
        if(strcasecmp(Prm_Izq,"-path") == 0){
            (*nwInf)->_path = newString(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _type
        if(strcmp(Prm_Izq,"-type") == 0){
            (*nwInf)->_type = newString(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _delete
        if(strcmp(Prm_Izq,"-delete") == 0){
            (*nwInf)->_delete = newString(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _add
        if(strcmp(Prm_Izq,"-add") == 0){
            int Nm = atoi(Prm_Der);
            (*nwInf)->_add =  atoi(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _name
        if(strcmp(Prm_Izq,"-name") == 0){
            (*nwInf)->_name = newString(Prm_Der);
            continue;
        }

        //(^< ............ ............ ............   _id
        if(strcmp(Prm_Izq,"-id") == 0){
            (*nwInf)->_id = newString(Prm_Der);
            continue;
        }

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

    while(1){

        Bf = strtok(NULL," ~:~");
        if(Bf == NULL){
            break;
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
    //Check

    mkdisk_do(nwInf);
}

void rm_disk_cmd(InfoCatcher* nwInf){
    //Check
    rmdisk_do(nwInf);
}

void f_disk_cmd(InfoCatcher* nwInf){

    //(^< ............ ............ ............ ............ ............ C H E C K

    //(^< ............ ............ ............ Obligatory
    if(nwInf->_size == NULL && nwInf->_add == -1){
        printf("\n");
        printf("FDISK ERROR: Parametro Obligatorio -size Faltante\n");
        return;
    }

    if(nwInf->_path == NULL){
        printf("\n");
        printf("FDISK ERROR: Parametro Obligatorio -path Faltante\n");
        return;
    }

    if(nwInf->_name == NULL){
        printf("\n");
        printf("FDISK ERROR: Parametro Obligatorio -name Faltante\n");
        return;
    }
    //(^< ............ ............ ............

    //(^< ............ ............ ............ Default
    if(nwInf->_unit == NULL){
        nwInf->_unit = newString("k");
    }

    if(nwInf->_type == NULL){
        nwInf->_type = newString("p");
    }

    if(nwInf->_fit == NULL){
        nwInf->_fit = newString("wf");
    }

    if(nwInf->_unit[0] == 'k'){
        nwInf->_size = nwInf->_size * 1024;
    }
    else{
        nwInf->_size = nwInf->_size * 1024 * 1024;
    }

    //(^< ............ ............ ............

    //(^< ............ ............ ............ BusinessRules
    MBR* Disk = LoadMBR(nwInf->_path);

    if(Disk == NULL){
        printf("\n");
        printf("FDISK ERROR: Disco No Encontrado\n");
        return;
    }

    //(^< ............ ............ ............ Delete
    if(nwInf->_delete != NULL){

        Mounted_Part* mP = getPartMounted_By_Name(nwInf->_name);
    
        if(mP != NULL){
            printf("\n");
            printf("FDISK ERROR: La Particion   -> %s <-   Esta Montada, No puede Eliminarse\n",nwInf->_name);
            return;
        }

        Delete_Part(nwInf,Disk);
        return;
    }
    //(^< ............ ............ ............
    if(nwInf->_add > -1){
        printf("\n");
        printf("FDISK ERROR: Esta Version No Soporta el Comando -add\n");
        return;
    }

    //(^< ............ ............ ............ ADD


    //(^< ............ ............ ............

    int nPrimary  = MBRPartArray_PrimaryCounter(Disk);
    int nExtended = MBRPartArray_ExtendedCounter(Disk);

    int Sum = nPrimary + nExtended;

    if(nExtended > 0 && nwInf->_type[0] == 'e'){
        printf("\n");
        printf("FDISK ERROR: Ya Existe una Particion Extendida\n");
        return;
    }

    if(nExtended == 0 && nwInf->_type[0] == 'l'){
        printf("\n");
        printf("FDISK ERROR: No Existe Particion Extendida para Crear Particion Logica\n");
        return;
    }

    if(Sum == 4 && nwInf->_type[0] != 'l'){
        printf("\n");
        printf("FDISK ERROR: Ya Existen 4 Particiones Titulares\n");
        return;
    }

    //int NameExists = MBRPartArray_PartNameExists(Disk,nwInf->_name);
    Batch* Nm = getBatch_By_PartName(nwInf->_path,Disk,nwInf->_name);
            
    if(Nm != NULL){
        printf("\n");
        printf("FDISK ERROR: El Nombre de la Particion   -> %s <-   ya Existe\n",nwInf->_name);
        return;
    }

    //(^< ............ ............ ............

    
    //(^< ............ ............ ............ ............ ............ D O
    
    fdisk_do(nwInf,Disk);

}

void rep_cmd(InfoCatcher* nwInf){
    //rep –id~:~vda1 -Path~:~/home/user/reports/reporte1.jpg -name~:~mbr
    Mounted_Part* mP = getPartMounted_By_vID(nwInf->_id);
    
    if(mP == NULL){
        printf("\n");
        printf("REP ERROR: El ID de Montaje   -> %s <-   No Existe\n",nwInf->_id);
        return;
    } 

    char* tmp = nwInf->_path;
    int ln = strlen(tmp);
    tmp[ln - 1] = 't';
    tmp[ln - 2] = 'o';
    tmp[ln - 3] = 'd';

    char* RepName = Path_Get_FileName(newString(nwInf->_path));
    char* RepPath = Path_Get_Isolated(newString(nwInf->_path));

    Locat* lcat = vdTransform(nwInf->_id);
    char*  Disk_Dir = UsingDisk_List[lcat->Letter].CompletePathDir;

    if(strcasecmp(nwInf->_name,"mbr") == 0){
        Generate_MBR_Report(Disk_Dir,RepPath,RepName);
        printf("\n");
        printf("REP SUCCESS: Reporte DISK   -> %s <-   Generado con Exito\n",RepName);
    }
    else if(strcasecmp(nwInf->_name,"disk") == 0){
        GenerateDiskRender(Disk_Dir,RepPath,RepName);
        printf("\n");
        printf("REP SUCCESS: Reporte DISK   -> %s <-   Generado con Exito\n",RepName);
    }
    else{
        printf("\n");
        printf("REP ERROR: Parametro -name   -> %s <-   No Valido\n",nwInf->_name);
        return;
    }
}

void mount_cmd(InfoCatcher* nwInf){

    
    MBR* Disk = LoadMBR(nwInf->_path);

    if(Disk == NULL){
        printf("\n");
        printf("MOUNT ERROR: Disco   -> %s <-   , NO Encontrado\n",nwInf->_name);
        return;
    }

    Batch* Part = getBatch_By_PartName(nwInf->_path,Disk,nwInf->_name);

    if(Part == NULL){
        printf("\n");
        printf("MOUNT ERROR: La Particion   -> %s <-   No Existe\n",nwInf->_name);
        return;
    }

    Mounted_Part* mP = getPartMounted_By_Name(nwInf->_name);
    
    if(mP != NULL){
        printf("\n");
        printf("MOUNT ERROR: La Particion   -> %s <-   Ya esta Montada\n",nwInf->_name);
        return;
    }
    

    char* mID = get_MountedPart_String_ID(nwInf->_path,nwInf->_name);
    Locat* lcat = vdTransform(mID);

    Mounted_Part* nw_mP = newMounted_Part();
    nw_mP->status = 1;
    nw_mP->ParName = newString(nwInf->_name);
    nw_mP->index = lcat->Num - 1;

    Disk_in_Use* dI = get_Disk_in_Use_By_DiskName(nwInf->_path);

    if(dI == NULL){
        dI = newDisk_in_Use();
        dI->status = 1;
        dI->index = lcat->Letter;
        dI->CompletePathDir = newString(nwInf->_path);
        dI->mntList[lcat->Num - 1] = *nw_mP;
        UsingDisk_List[lcat->Letter] = *dI;
    }
    else{
        dI->mntList[lcat->Num - 1] = *nw_mP;
    }

    printf("\n");
    printf("MOUNT SUCCESS: Particion   -> %s <-   Montada con Exito: ID = %s\n",nwInf->_name,mID);    

}

void unmount_cmd(InfoCatcher* nwInf){
    Mounted_Part* mP = getPartMounted_By_vID(nwInf->_id);
    
    if(mP == NULL){
        printf("\n");
        printf("MOUNT ERROR: El ID de Montaje   -> %s <-   No Existe\n",nwInf->_id);
        return;
    }   

    Locat* lcat = vdTransform(nwInf->_id);

    UsingDisk_List[lcat->Letter].mntList[lcat->Num - 1] = *newMounted_Part();

    printf("\n");
    printf("MOUNT SUCCESS: Particion   -> %s <-   Desmontada con Exito\n",nwInf->_id);
    return;
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
        //GenerateDiskRender("/home/archivos/fase1/Disco1.disk","/home/wrm/Desktop/","Ds.dot");
        return 0;
    }
    else if(strcasecmp(Bf, "rmdisk") == 0){
        rm_disk_cmd(nwInf);
        return 0;
    }
    else if(strcasecmp(Bf, "fdisk") == 0){
        f_disk_cmd(nwInf);
        //GenerateDiskRender("/home/archivos/fase1/Disco1.disk","/home/wrm/Desktop/","Ds.dot");
        //getchar();
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
    else if(strcasecmp(Bf, "pause") == 0){
        getchar();
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

    InputString = StringCloneWithOut(InputString,'\t');
    InputString = StringCloneWithOut(InputString,'\r');
    InputString = StringCloneWithOut(InputString,'\"');
    InputString = StringCloneWithOut(InputString,'\'');
    InputString = StringCloneWithOut(InputString,'\n');

    int rs = String_Has_this_Char(InputString,'\\');

    if(newLine_Flag == 1){
        
        InputString  = Concat_Izq_with_Der(cmdString,InputString,'s','s');
        newLine_Flag = 0;
        cmdString    = NULL;
        printf("CommandLine->   %s\n",InputString);
    }

    if(rs > -1){
        InputString  = StringCloneWithOut(InputString,'\\');
        newLine_Flag = 1;
        cmdString    = newString(InputString);
        return;
    }

    if(strcasecmp(InputString, "Exit\n") == 0){
        printf("\n");
        printf("Saliendo de la APP...\n");
        return;
    }

    DoublyGenericList* CommandList = new_DoublyGenericList();

    char* Main_CMD = strtok(InputString, " ");
    int unknownCMD = 1;
    FillCommandList(Main_CMD,CommandList);

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

    unknownCMD = ScanF1(Main_CMD,nwInf);

    if(unknownCMD != 0){
        unknownCMD = ScanF2(Main_CMD,nwInf);
    }
    
    if(unknownCMD != 0){
        if (strcasecmp(Main_CMD,"Exec") == 0){        
            Exec_CMD(CommandList);
            while (CommandList->Length > 0)
            {
                char* NewCommand = (char*)DeQueue(CommandList);
                ExecuteComand(NewCommand);
            }   
        }
        else{
            printf("\n");
            printf("Analizer ERROR: Comando: %s No Reconocido\n", Main_CMD);
        }
    }
    
    getchar();
}


#endif // ANALIZER_H
