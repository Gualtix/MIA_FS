#ifndef BELONG_H
#define BELONG_H

#include "Container.h"
#include "Fw/Helper.h"




int MBRPartArray_ExtendedCounter(MBR* Bf){
    int cnt = 0;
    int i  = 0;
    while(cnt < 4){
        if(Bf->mbr_partition[cnt].part_type == 'e'){
            i++;
        }
        cnt++;
    }
    return i;
}

int MBRPartArray_PrimaryCounter(MBR* Bf){
    int cnt = 0;
    int i  = 0;
    while(cnt < 4){
        if(Bf->mbr_partition[cnt].part_type == 'p'){
            i++;
        }
        cnt++;
    }
    return i;
}


MBR* MBRPartArray_BubbleSort(MBR* Bf){
    int i = 0;
    while (i < 4) {
        int j = 0;
        while (j < (4 - (i + 1))) {
            if(Bf->mbr_partition[j].part_start > Bf->mbr_partition[j + 1].part_start){
                Partition Tmp = Bf->mbr_partition[j];
                Bf->mbr_partition[j] = Bf->mbr_partition[j + 1];
                Bf->mbr_partition[j + 1] = Tmp;
            }
            j++;
        }
        i++;
    }
    return Bf;
}

int MBRPartArray_GetAvailableIndex(MBR* Bf){
    int cnt = 0;
    while(cnt < 4){
        if(Bf->mbr_partition[cnt].part_start == -1){
            return cnt;
        }
        cnt++;
    }
    return -1;
}

int MBRPartArray_GetIndex_By_PartName(MBR* Bf,char* PartName){
    int cnt = 0;
    while (cnt < 4) {
        Partition* Tmp = &Bf->mbr_partition[cnt];
        if(Tmp->part_size > 0){
            if(strcasecmp(Bf->mbr_partition[cnt].part_name,PartName) == 0){
                return cnt;
            }
        }
        cnt++;
    }
    return -1;
}

int MBRPartArray_PartNameExists(MBR* Bf,char* PartName){
    int cnt = 0;
    while (cnt < 4) {
        Partition* Tmp = &Bf->mbr_partition[cnt];
        if(Tmp->part_size > 0){
            if(strcasecmp(Bf->mbr_partition[cnt].part_name,PartName) == 0){
                return 1;
            }
        }
        cnt++;
    }
    return 0;
}

MBR* LoadMBR(char* CompletePathDir){

    FILE* Fl = fopen(CompletePathDir,"rb+");

    MBR* Bf = (MBR*)malloc(sizeof(MBR));

    if(Fl){
        fseek(Fl,0,SEEK_SET);
        fread(Bf,sizeof(MBR),1, Fl);
        fclose(Fl);
        return Bf;
    }
    else{
        return NULL;
    }
}

void UpdateMBR(char* CompltePathDir,MBR* Disk){

    FILE* Fl = fopen(CompltePathDir,"r+");

    if(Fl){
        fseek(Fl,0,SEEK_SET);
        fwrite(Disk,sizeof(MBR),1,Fl);
        fclose(Fl);
        return;
    }
}

DoublyGenericList* getBatchList_FromDisk(MBR* Disk){

    DoublyGenericList* batchList = new_DoublyGenericList();

    Batch* Mb     = newBatch();
    Mb->StartByte = 0;
    Mb->Size      = sizeof(MBR);
    Mb->EndByte   = Mb->Size - 1; 
    Mb->Type      = 'm';
    Mb->PartName  = newString("MBR");
    EnQueue(batchList,Mb);
    
    int tmpEndByte = sizeof(MBR) - 1;

    if(Disk->mbr_partition[0].part_start == -1){
        Batch* Sp     = newBatch();
        Sp->StartByte = tmpEndByte + 1;
        Sp->Size      = Disk->mbr_tamano -  sizeof(MBR);
        Sp->EndByte   = Disk->mbr_tamano - 1; 
        Sp->Type      = 's';
        Sp->PartName  = newString("FreeSpace");
        EnQueue(batchList,Sp);
        return batchList;
    }

    int i = 0;
    while(i < 4 ){

        Partition Part = Disk->mbr_partition[i];
        if(Part.part_start == -1){
            break;
        }

        int Space = (Part.part_start - tmpEndByte - 1);
        if(Space > 0){
            Batch* Sp     = newBatch();
            Sp->StartByte = tmpEndByte + 1;
            Sp->Size      = Space;
            Sp->EndByte   = Sp->StartByte + Sp->Size - 1; 
            Sp->Type      = 's';
            Sp->PartName  = newString("FreeSpace");
            tmpEndByte    = Part.part_start - 1;
            EnQueue(batchList,Sp);
        }

        Batch* Sp     = newBatch();
        Sp->StartByte = Part.part_start;
        Sp->Size      = Part.part_size;
        Sp->EndByte   = Sp->StartByte + Sp->Size - 1; 
        Sp->Type      = Part.part_type;
        Sp->PartName  = newString(Part.part_name);
        tmpEndByte    = Sp->EndByte;
        EnQueue(batchList,Sp);
        i++;
        
    }

    int Space = Disk->mbr_tamano - tmpEndByte - 1;
    if(Space > 0){
        Batch* Sp     = newBatch();
        Sp->StartByte = tmpEndByte + 1;
        Sp->Size      = Space;
        Sp->EndByte   = Disk->mbr_tamano - 1; 
        Sp->Type      = 's';
        Sp->PartName  = newString("FreeSpace");
        EnQueue(batchList,Sp);
    }

    return batchList; 
}

void BinWrite_Partition(Partition* Part,char* CompletePathDir,MBR* Disk){
    int index = MBRPartArray_GetAvailableIndex(Disk);
    Disk->mbr_partition[index] =  *Part;
    UpdateMBR(CompletePathDir,Disk);
}




#endif // BELONG_H