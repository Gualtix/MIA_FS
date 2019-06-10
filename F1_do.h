#ifndef F1_DO_H
#define F1_DO_H

#include "Container.h"
#include "Fw/Helper.h"
#include "Belong.h"

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


void BestFit(DoublyGenericList* batchList,InfoCatcher* nwInf,MBR* Disk){

}

void BestFit_Logic(){

}

void WorstFit(DoublyGenericList* batchList,InfoCatcher* nwInf,MBR* Disk){

}

void WorstFit_Logic(){
    
}


void FirstFit(DoublyGenericList* batchList,InfoCatcher* nwInf,MBR* Disk){
    int SpNeeded = nwInf->_size;
    int cnt = 0;
    Batch* batiSpot = NULL;

    while(cnt < batchList->Length){
        batiSpot = (Batch*)(getNodebyIndex(batchList,cnt)->Dt);
        if(SpNeeded <= batiSpot->Size && batiSpot->Type == 's'){
            Partition* Part = newPartition();

            Part->part_fit[0] = nwInf->_fit[0];
            Part->part_fit[1] = nwInf->_fit[1];

            strcpy(Part->part_name,newString(nwInf->_name));
            Part->part_size   = nwInf->_size;
            Part->part_start  = batiSpot->StartByte;
            Part->part_status = '0';
            Part->part_type   = nwInf->_type[0];

            if(Part->part_type == 'e'){

                int index = MBRPartArray_GetAvailableIndex(Disk);
                Disk->mbr_partition[index] =  *Part;

                Ascending_MBRPartArray_BubbleSort(Disk);
                UpdateMBR(nwInf->_path,Disk);

                EBR* eB = newEBR();
                eB->part_start = Part->part_start;
                UpdateEBR(eB,nwInf->_path);

            }
            if(Part->part_type == 'p'){
                int index = MBRPartArray_GetAvailableIndex(Disk);
                Disk->mbr_partition[index] =  *Part;
                Ascending_MBRPartArray_BubbleSort(Disk);
                UpdateMBR(nwInf->_path,Disk);
            }
            
            printf("\n");
            printf("FDISK SUCESS: Particion   -> %s <-   Creada Exitosamente por Primer Ajuste\n",nwInf->_name);
            return;
        }
        cnt++;
    }

    printf("\n");
    printf("FDISK ERROR: No Existe Espacio para Crear La Particion\n");
}

void FirstFit_Logic(DoublyGenericList* batchList,InfoCatcher* nwInf,MBR* Disk){
    Batch* ExtBatch = getExtended_Batch(batchList);

    EBR* DefaultEBR = LoadEBR(nwInf->_path,ExtBatch->StartByte);
    //ExtBatch->LgParts = getBatchList_FromExtended(DefaultEBR,nwInf->_path,ExtBatch->StartByte,ExtBatch->EndByte);

    int SpNeeded = nwInf->_size;
    int cnt = 0;
    Batch* batiSpot = NULL;

    while(cnt < ExtBatch->LgParts->Length){
        batiSpot = (Batch*)(getNodebyIndex(ExtBatch->LgParts,cnt)->Dt);
        if(SpNeeded <= batiSpot->Size && batiSpot->Type == 's'){

            EBR* new_lgPart = newEBR();

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

                EBR* prev_lgPart = LoadEBR(nwInf->_path,batiSpot->Prev);

                new_lgPart->part_start = batiSpot->StartByte;
                new_lgPart->part_size  = nwInf->_size;
                new_lgPart->part_next  = batiSpot->Next;
                new_lgPart->part_status = '0';
                strcpy(new_lgPart->part_name,nwInf->_name);
                new_lgPart->part_fit = nwInf->_fit[0];

                prev_lgPart->part_next = new_lgPart->part_start;
                new_lgPart->part_next  = batiSpot->Next;

                UpdateEBR(prev_lgPart,nwInf->_path);
                UpdateEBR(new_lgPart,nwInf->_path);
                
            }
    
            printf("\n");
            printf("FDISK SUCESS: Particion Logica   -> %s <-   Creada Exitosamente por Primer Ajuste\n",nwInf->_name);
            return;
        }
        cnt++;
    }

    printf("\n");
    printf("FDISK ERROR: No Existe Espacio para Crear La Particion\n");

}

void Delete_Part(InfoCatcher* nwInf, MBR* Disk){

    printf("\n");
    printf("FDISK WARNING: Esta seguro de Eliminar la Particion   -> %s <-   ?\n",nwInf->_name);
    printf("Y = Yes , Any Other Key = No \n");

    char Op;
    Op = getchar();
    if(Op =='\n' || Op =='\r'){
        Op = getchar();
    }

    if(putchar(tolower(Op)) != 'y' ){
        printf("\n");
        printf("FDISK WARNING: Operacion Eliminar Particion Cancelada\n" );
        Op = getchar();
        return;
    }

    int index = MBRPartArray_GetIndex_By_PartName(Disk,nwInf->_name);

    if(index == -1){

        Batch* Prt = getBatch_By_PartName(nwInf->_path,Disk,nwInf->_name);
        if(Prt == NULL){
            printf("\n");
            printf("FDISK ERROR: La Particion   -> %s <-   No Existe\n",nwInf->_name);
            getchar();
            return;
        }
        else{
            //(^< ............ ............ ............ Delete Logic
            if(Prt->Prev > 0){
                EBR* Prev = LoadEBR(nwInf->_path,Prt->Prev);    
                Prev->part_next = Prt->Next;
                UpdateEBR(Prev,nwInf->_path);
                if(strcasecmp(nwInf->_delete,"full") == 0){
                    Part_1024_Erase(nwInf->_path,Prt->StartByte,Prt->Size);
                    printf("\n");
                    printf("FDISK SUCESS: Particion   -> %s <-   Eliminada Exitosamente por Full Delete\n",nwInf->_name);
                    getchar();
                    return;
                }
            }
        }
    }
    else{
        int tmpStartByte = Disk->mbr_partition[index].part_start;
        int tmpPartSize  = Disk->mbr_partition[index].part_size;

        Disk->mbr_partition[index] = *newPartition();
        Ascending_MBRPartArray_BubbleSort(Disk);
        UpdateMBR(nwInf->_path,Disk);
        if(strcasecmp(nwInf->_delete,"full") == 0){
            Part_1024_Erase(nwInf->_path,tmpStartByte,tmpPartSize);
            printf("\n");
            printf("FDISK SUCESS: Particion   -> %s <-   Eliminada Exitosamente por Full Delete\n",nwInf->_name);
            Op = getchar();
            return;
        }
    }

    printf("\n");
    printf("FDISK SUCESS: Particion   -> %s <-   Eliminada Exitosamente por Fast Delete\n",nwInf->_name); 
    Op = getchar();   
}

void fdisk_do(InfoCatcher* nwInf, MBR* Disk){

    //(^< ............ ............ ............ Space Validation
    DoublyGenericList* batchList = getBatchList_FromDisk(nwInf->_path,Disk);

    if(Disk->disk_fit == 'f'){

        if(nwInf->_type[0] == 'l'){
            FirstFit_Logic(batchList,nwInf,Disk);
            return;
        }
        FirstFit(batchList,nwInf,Disk);
        return;
    }

    if(Disk->disk_fit == 'w'){
        if(nwInf->_type[0] == 'l'){
            WorstFit_Logic();
            return;
        }
        WorstFit(batchList,nwInf,Disk);
        return;
    }

    if(Disk->disk_fit == 'b'){
        if(nwInf->_type[0] == 'l'){
            BestFit_Logic();
            return;
        }
        BestFit(batchList,nwInf,Disk);
        return;
    }
}


//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ M O U N T
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void mount_do(InfoCatcher* nwInf){

}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ U N M O U N T
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............


void unmount_do(InfoCatcher* nwInf){

}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ R E P
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

void rep_do(InfoCatcher* nwInf){

}

#endif // F1_DO_H