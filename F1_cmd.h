#ifndef F1_CMD_H
#define F1_CMD_H

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


void fdisk_do(InfoCatcher* nwInf){

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

#endif // F1_CMD_H