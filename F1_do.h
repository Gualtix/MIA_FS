#ifndef F1_DO_H
#define F1_DO_H



#include "Fw/Helper.h"

#include "Container.h"
#include "Belong.h"
#include "Rep/F1_Rep.h"
#include "Rep/F2_Rep.h"


//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ M K D I S K
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void SetDiskSize(char* CompletePathDir,int DiskSize,char Unit){

    FILE* Fl = fopen(CompletePathDir,"wb+");
    char Bf[1024];

    if(Fl){

        memset(Bf,'\0',1024);

        int cnt = DiskSize;

        if('k' == Unit){
            while (cnt > 0) {
                fwrite(Bf,1024,1,Fl);
                cnt--;
            }
        }
        else{
            cnt = DiskSize * 1024;
            while (cnt > 0) {
                fwrite(Bf,1024,1,Fl);
                cnt--;
            }
        }
        fclose(Fl);
    }
}

void SetMBR_to_Disk(char* CompletePathDir,int DiskSize,char Unit,char Fit){

    FILE* Fl = fopen(CompletePathDir,"r+");

    if(Fl){
        int RandomID = rand()%(100);
        MBR* MBR_DEF = newMBR();

        MBR_DEF->mbr_tamano = CalcSize_in_Bytes(DiskSize,Unit);
        MBR_DEF->mbr_disk_signature = RandomID;
        MBR_DEF->disk_fit = Fit;

        fseek(Fl,0,SEEK_SET);
        fwrite(MBR_DEF,sizeof(MBR),1,Fl);
        fclose(Fl);
    }
}

void mkdisk_do(InfoCatcher* nwInf){

    char* PathOnly = Path_Get_Isolated(nwInf->_path);
    CreatePathDir(PathOnly);
    SetDiskSize(nwInf->_path,nwInf->_size,nwInf->_unit[0]);
    SetMBR_to_Disk(nwInf->_path,nwInf->_size,nwInf->_unit[0],nwInf->_fit[0]);

    char* DiskName = Path_Get_FileName(nwInf->_path);

    printf("\n");
    printf("MKDISK SUCCESS:   -> %s   <-   Creado Correctamente\n",DiskName);
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ R M D I S K
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void rmdisk_do(InfoCatcher* nwInf){
    char* DiskName = Path_Get_FileName(nwInf->_path);
    FILE* Fl = fopen(nwInf->_path,"r");
    if(Fl){
        fclose(Fl);
        printf("\n");
        printf("RMDISK WARNING: Esta Seguro de Eliminar   -> %s <-   ?\n",DiskName);
        printf("Y = Yes , Any Other Key = No \n");

        char Op;
        Op = getchar();
        if(Op == '\n' || Op == '\r'){
            Op = getchar();
        }

        if(putchar(tolower(Op)) == 'y' ){
            remove(nwInf->_path);
            printf("\n");
            printf("RMDISK SUCCESS:   -> %s <-   Eliminado con Exito\n",DiskName);
            getchar();
        }
        else{
            printf("\n");
            printf("RMDISK WARNING: Operacion Eliminar Disco Cancelada\n");
            getchar();
        }
    }
    else{
        printf("\n");
        printf("RMDISK ERROR: Disco   -> %s <-   , NO Encontrado\n",DiskName);
    }
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ F D I S K
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void Delete_Part(InfoCatcher* nwInf, MBR* Disk){

    
    Batch* Prt = getBatch_By_PartName(nwInf->_path,Disk,nwInf->_name);
    if(Prt == NULL){
        printf("\n");
        printf("FDISK ERROR: La Particion   -> %s <-   No Existe\n",nwInf->_name);
        getchar();
        return;
    }

    int Op = DeleteAsk(nwInf->_name);
    if(Op == 0){
        return;
    }

    if(strcasecmp(nwInf->_delete,"fast") == 0 || strcasecmp(nwInf->_delete,"full") == 0){

        if(Prt->Type == 'p' || Prt->Type == 'e'){
            int index = MBRPartArray_GetIndex_By_PartName(Disk,Prt->PartName);
            Disk->mbr_partition[index] = *(newPartition());

            Ascending_MBRPartArray_BubbleSort(Disk);
            UpdateMBR(nwInf->_path,Disk);
        }

        if(Prt->Type == 'q'){

            if(Prt->Prev > -1){
                EBR* Prev = LoadEBR(nwInf->_path,Prt->Prev);
                Prev->part_next = Prt->Next;
                UpdateEBR(Prev,nwInf->_path);
            }
            else{
                EBR* DefaultEBR = LoadEBR(nwInf->_path,Prt->StartByte);
                DefaultEBR->part_fit = '^';
                DefaultEBR->part_size = sizeof(EBR);
                DefaultEBR->part_status = '^';
                memset(DefaultEBR->part_name,'\0',16);
                strcpy(DefaultEBR->part_name,"UNDEFINED");
                UpdateEBR(DefaultEBR,nwInf->_path);
            }
        }

        if(strcasecmp(nwInf->_delete,"fast") == 0){
            printf("\n");
            printf("FDISK SUCESS: Particion   -> %s <-   Eliminada Exitosamente por Fast Delete\n",nwInf->_name);
            getchar();
            return;
        }

    }
    if(strcasecmp(nwInf->_delete,"full") == 0){
        printf("\n");
        printf("FDISK SUCESS: Particion   -> %s <-   Eliminada Exitosamente por Full Delete\n",nwInf->_name);
        getchar();
        return;
    }

    printf("\n");
    printf("FDISK ERROR: Parametro -delete:   -> %s <-   No Valido\n",nwInf->_name);
    printf("FDISK WARNING: Operacion Eliminar Particion Cancelada\n" );
    getchar();
    return;
}

void fdisk_do(InfoCatcher* nwInf, MBR* Disk){

    //(^< ............ ............ ............ Space Validation
    DoublyGenericList* batchList = getBatchList_FromDisk(nwInf->_path,Disk);

    char* FType =  newString("Primer Ajuste");

    if(Disk->disk_fit == 'f'){
        Isolate_SpaceBatch(batchList);
    }

    if(Disk->disk_fit == 'b'){
        get_Ascending_BatchSpace_List(batchList);
        FType = newString("Mejor Ajuste");
    }

    if(Disk->disk_fit == 'w'){
        get_Descending_BatchSpace_List(batchList);
        FType = newString("Peor Ajuste");
    }

    //(^< ............ ............ ............ Regular
    if(nwInf->_type[0] != 'l'){

        Batch* tmp = get_First_SpaceBatch_That_Fits(batchList,nwInf->_size);
        if(tmp == NULL){
            printf("\n");
            printf("FDISK ERROR: No Existe Espacio para Crear La Particion\n");
            return;
        }

        int index = MBRPartArray_GetAvailableIndex(Disk);

        Partition* Part = newPartition();

        strcpy(Part->part_name,nwInf->_name);
        strcpy(Part->part_fit,nwInf->_fit);
        Part->part_type = nwInf->_type[0];
        Part->part_size = nwInf->_size;
        Part->part_start = tmp->StartByte;
        Part->part_status = '0';

        Disk->mbr_partition[index] = *Part;

        if(Part->part_type == 'e'){
            EBR* eB = newEBR();
            eB->part_start = Part->part_start;
            UpdateEBR(eB,nwInf->_path);
        }

        Ascending_MBRPartArray_BubbleSort(Disk);
        UpdateMBR(nwInf->_path,Disk);

        printf("\n");
        printf("FDISK SUCESS: Particion   -> %s <-   Creada Exitosamente por %s\n",nwInf->_name,FType);
        return;
        
    }
    //(^< ............ ............ ............ Logic
    else{

        Batch* ext = getExtended_Batch(batchList);
        Batch* tmp = get_First_SpaceBatch_That_Fits(ext->LgParts,nwInf->_size);

        //(^< ............ ............ ............ ............ ............
        batchList = getBatchList_FromDisk(nwInf->_path,Disk);

        int ext_index = MBRPartArray_GetIndex_By_PartName(Disk,ext->PartName);
        Partition extPart = Disk->mbr_partition[ext_index];

        FType =  newString("Primer Ajuste");

        if(extPart.part_fit[0] == 'f'){
            Isolate_SpaceBatch(batchList);
        }

        if(extPart.part_fit[0] == 'b'){
            get_Ascending_BatchSpace_List(batchList);
            FType = newString("Mejor Ajuste");
        }

        if(extPart.part_fit[0] == 'w'){
            get_Descending_BatchSpace_List(batchList);
            FType = newString("Peor Ajuste");
        }
        //(^< ............ ............ ............ ............ ............

        if(tmp == NULL){
            printf("\n");
            printf("FDISK ERROR: No Existe Espacio para Crear La Particion\n");
            return;
        }

        EBR* DefaultEBR = LoadEBR(nwInf->_path,ext->StartByte);
        
        EBR* new_lgPart = newEBR();
        
        //if(DefaultEBR->part_start == tmp->Prev && strcasecmp(DefaultEBR->part_name,"UNDEFINED") == 0){
        if(strcasecmp(DefaultEBR->part_name,"UNDEFINED") == 0 && DefaultEBR->part_next > -1){

            new_lgPart->part_start = ext->StartByte;
            new_lgPart->part_size  = nwInf->_size;
            new_lgPart->part_next  = tmp->Next;
            new_lgPart->part_status = '0';
            strcpy(new_lgPart->part_name,nwInf->_name);
            new_lgPart->part_fit = nwInf->_fit[0];
            UpdateEBR(new_lgPart,nwInf->_path);

            GenerateDiskRender("/home/archivos/fase1/Disco1.disk","/home/wrm/Desktop/","Ds.dot");

            printf("\n");
            printf("FDISK SUCESS: Particion Logica   -> %s <-   Creada Exitosamente por %s\n",nwInf->_name,FType);
            return;
        }
    

        if(DefaultEBR->part_size == -1){

                new_lgPart->part_start = DefaultEBR->part_start;
                new_lgPart->part_size = nwInf->_size;
                new_lgPart->part_next = -1;
                new_lgPart->part_status = '0';
                strcpy(new_lgPart->part_name,nwInf->_name);
                new_lgPart->part_fit = nwInf->_fit[0];
                UpdateEBR(new_lgPart,nwInf->_path);

            }
            else{

                EBR* prev_lgPart = LoadEBR(nwInf->_path,tmp->Prev);

                new_lgPart->part_start = tmp->StartByte;
                new_lgPart->part_size  = nwInf->_size;
                new_lgPart->part_next  = tmp->Next;
                new_lgPart->part_status = '0';
                strcpy(new_lgPart->part_name,nwInf->_name);
                new_lgPart->part_fit = nwInf->_fit[0];

                prev_lgPart->part_next = new_lgPart->part_start;
                new_lgPart->part_next  = tmp->Next;

                UpdateEBR(prev_lgPart,nwInf->_path);
                UpdateEBR(new_lgPart,nwInf->_path);
                
            }
    
            printf("\n");
            printf("FDISK SUCESS: Particion Logica   -> %s <-   Creada Exitosamente por %s\n",nwInf->_name,FType);
            return;
    }
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ F 2
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............


void mkfs_do(InfoCatcher* nwInf){

    setOmni(nwInf->_id);

    if(strcasecmp(nwInf->_type,"fast") == 0){
        Fast_PartFormat();
    }

    if(strcasecmp(nwInf->_type,"full") == 0){
        Full_PartFormat();
    }

    if(strcasecmp(nwInf->_fs,"2fs") == 0){
        Format_to_EXT2();
    }

    if(strcasecmp(nwInf->_fs,"3fs") == 0){
        Format_to_EXT3();
        Load_Defaut_txt(nwInf);
        //FullViewRender("/home/wrm/Desktop/Todito.dot","tree");   
    }

    Omni = newGLS();
}


void login_do(InfoCatcher* nwInf){
    setOmni(nwInf->_id);
}

void mkgrp_do(InfoCatcher* nwInf){

    DoublyGenericList*  grpList = getGroupsList();
    DoublyGenericList*  usrList = getUsersList();
    GroupUserInfo* gu = newGrus();

    if(grpList != NULL){
        gu->ID = grpList->Length + 1;
        gu->Type = 'g';
        gu->GrpName = nwInf->_name;
    }
    EnQueue(grpList,gu);
    txtUsers_Update(grpList,usrList);
}

void rmgrp_do(InfoCatcher* nwInf){

    DoublyGenericList*  grpList = getGroupsList();
    DoublyGenericList*  usrList = getUsersList();

    int Lm = grpList->Length;
    int cnt = 0;
    int found = 0;
    while(cnt < Lm){
        GroupUserInfo* tmp = (GroupUserInfo*)getNodebyIndex(grpList,cnt)->Dt;
        if(strcasecmp(nwInf->_name,tmp->GrpName) == 0){
            tmp->ID = 0;
            found++;
            break;
        }
        cnt++;
    }

    txtUsers_Update(grpList,usrList);

    grpList = getGroupsList();
    usrList = getUsersList();

    int ask = 554;
}

void mkusr_do(InfoCatcher* nwInf){

    DoublyGenericList*  grpList = getGroupsList();
    DoublyGenericList*  usrList = getUsersList();
    GroupUserInfo* gu = newGrus();

    int grpEx = grpExists(nwInf->_grp,grpList);
    int usrEx = usrExists(nwInf->_usr,usrList);

    if(usrList != NULL){
        gu->ID = usrList->Length + 1;
        gu->Type = 'u';
        gu->GrpName = nwInf->_grp;
        gu->UsrName = nwInf->_usr;
        gu->Password = nwInf->_pwd;
    }

    EnQueue(usrList,gu);
    txtUsers_Update(grpList,usrList);
}

void rmusr_do(InfoCatcher* nwInf){

    DoublyGenericList*  grpList = getGroupsList();
    DoublyGenericList*  usrList = getUsersList();

    int Lm = usrList->Length;
    int cnt = 0;
    int found = 0;
    while(cnt < Lm){
        GroupUserInfo* tmp = (GroupUserInfo*)getNodebyIndex(usrList,cnt)->Dt;
        if(strcasecmp(nwInf->_usr,tmp->UsrName) == 0){
            tmp->ID = 0;
            found++;
            break;
        }
        cnt++;
    }
    txtUsers_Update(grpList,usrList);
}

void mkdir_do(InfoCatcher* nwInf){
    int Deepest_Bit_ID = make_newFolder(nwInf);
}

int mkfile_do(InfoCatcher* nwInf){
    FileFolderInfo* ffInf = get_FFInfo(nwInf);
    int Deepest_Bit_ID = make_newFolder(nwInf);
    if(Deepest_Bit_ID > -1){
        GroupUserInfo* ggs = getGRP_by_Name(Omni->LoggedUser->GrpName,getGroupsList());
        int nwFL_Bit_ID = allocate_newFile(Deepest_Bit_ID,ffInf->FileName,ffInf->txtData,664,Omni->LoggedUser->ID,ggs->ID);
        return nwFL_Bit_ID;
    }
    else{
        return -1;
    }

}

void rem_do(char* tmp,int istxt){
    if(istxt == 1){
        EraseFile(tmp);
    }
    else{
        EraseFolder(tmp);
    }
}

void mv_do(InfoCatcher* nwInf){
    DoublyGenericList* Ph = PathSeparate(nwInf->_path);
    Pop(Ph);
    char* OrgName = (char*)Pop(Ph);
    int  istxt    = Check_If_Is_txtFile(OrgName);

    Ph = PathSeparate(nwInf->_dest);
    Pop(Ph);
    char* DestName = (char*)Pop(Ph);


    //-------------------------------------->Fuente

    SeekInfo* Origin = CompleteSeeker(0,OrgName);
    SeekInfo* Dest   = CompleteSeeker(0,DestName);

    FolderBlock* FolderB = (FolderBlock*)BinLoad_Str(Origin->FB_Bit_ID,"FolderBlock");
    memset(FolderB->b_content[Origin->FB_Index].b_name,'\0',12);
    FolderB->b_content[Origin->FB_Index].b_inodo = -1;

    BinWrite_Struct(FolderB,Origin->FB_Bit_ID,"FolderBlock");

    //-------------------------------------->Destino
    pushMoved(Dest->iNode_Bit_ID,Origin->iNode_Bit_ID,OrgName);

}

void rep_F1_do(InfoCatcher* nwInf){

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
        printf("REP SUCCESS: Reporte MBR   -> %s <-   Generado con Exito\n",RepName);
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



void rep_F2_do(InfoCatcher* nwInf){
    setOmni(nwInf->_id);
    if(strcasecmp(nwInf->_name,"file") == 0){
        Generate_File_Rep(nwInf->_path,nwInf->_ruta);
        Omni = newGLS();
        return;
    }
    if(strcasecmp(nwInf->_name,"ls") == 0){
        Generate_Ls_Rep(nwInf->_path,nwInf->_ruta);
        Omni = newGLS();
        return;
    }
    FullViewRender(newString(nwInf->_path),nwInf->_name);
    Omni = newGLS();

}



#endif // F1_DO_H