#ifndef CONTAINER_H
#define CONTAINER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Byte 1
#define Kilo 1024
#define Mega (Kilo * 1024)


typedef struct EBR{
    char  part_status;
    char  part_fit;
    int   part_start;
    int   part_size;
    int   part_next;
    char  part_name[16];

}EBR;

EBR* newEBR(){
    EBR* nEBR = (EBR*)malloc(sizeof(EBR));

    nEBR->part_status = '^';
    nEBR->part_fit    = '^';
    nEBR->part_start  = -1;
    nEBR->part_size   = -1;
    nEBR->part_next   = -1;
    memset(nEBR->part_name,'\0',16);
    strcpy(nEBR->part_name,"UNDEFINED");

    return nEBR;
}

typedef struct Partition
{
    char  part_status;
    char  part_type;
    char  part_fit[2];
    int   part_start;
    int   part_size;
    char  part_name[16];
}Partition;

Partition* newPartition(){
    Partition* nPart = (Partition*)malloc(sizeof(Partition));

    nPart->part_status   = '^';
    nPart->part_type     = '^';
    nPart->part_fit[0]   = '^';
    nPart->part_fit[1]   = '^';
    nPart->part_start    = -1;
    nPart->part_size     = -1;
    nPart->part_name[16] = '\0';
    memset(nPart->part_name,'\0',16);
    strcpy(nPart->part_name,"UNDEFINED_");
    
    return nPart;
}

typedef struct MBR
{
    int       mbr_tamano;
    char      mbr_fecha_creacion [17];
    int       mbr_disk_signature;
    char      disk_fit;
    Partition mbr_partition [4];
}MBR;

MBR* newMBR(){

    MBR* nMBR = (MBR*)malloc(sizeof(MBR));

    time_t Tmp = time(0);
    char ShapeDate[20];
    strftime(ShapeDate,128,"%d/%m/%y %H:%M:%S",localtime(&Tmp));

    nMBR->mbr_tamano = -1;
    memset(nMBR->mbr_fecha_creacion,'^',17);
    strcpy(nMBR->mbr_fecha_creacion,ShapeDate);
    nMBR->mbr_disk_signature = -1;
    nMBR->disk_fit = '^';
    nMBR->mbr_partition[0] = *newPartition();
    nMBR->mbr_partition[1] = *newPartition();
    nMBR->mbr_partition[2] = *newPartition();
    nMBR->mbr_partition[3] = *newPartition();

    nMBR->mbr_partition[0].part_name[10] = '1';
    nMBR->mbr_partition[1].part_name[10] = '2';
    nMBR->mbr_partition[2].part_name[10] = '3';
    nMBR->mbr_partition[3].part_name[10] = '4';

    return nMBR;

}


typedef struct InfoCatcher
{
    int   _size;
    char* _fit;
    char* _unit;
    char* _path;
    char* _type;
    char* _delete;
    char* _name;
    int   _add;
    char* _id;
}InfoCatcher;

InfoCatcher* newInfoCatcher(){
    InfoCatcher* nwInfo = (InfoCatcher*)malloc(sizeof(InfoCatcher));
    nwInfo->_size   = -1;
    nwInfo->_fit    = NULL;
    nwInfo->_unit   = NULL;
    nwInfo->_path   = NULL;
    nwInfo->_type   = NULL;
    nwInfo->_delete = NULL;
    nwInfo->_name   = NULL;
    nwInfo->_add    = -1;
    nwInfo->_id     = NULL;
    return nwInfo;
}






#endif // CONTAINER_H
