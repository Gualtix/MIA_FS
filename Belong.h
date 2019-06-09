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

EBR* LoadEBR(char* CompletePathDir,int StartByte){

    FILE* Fl = fopen(CompletePathDir,"rb+");

    EBR* Bf = newEBR();

    if(Fl){
        fseek(Fl,StartByte,SEEK_SET);
        fread(Bf,sizeof(EBR),1, Fl);
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

void UpdateEBR(EBR* eB,char* CompletePathDir){
    FILE* Fl = fopen(CompletePathDir,"r+");
    if(Fl){
        fseek(Fl,eB->part_start,SEEK_SET);
        fwrite(eB,sizeof(EBR),1,Fl);
        fclose(Fl);
    }
}

Batch* Set_Batch(int StartByte,int _Size,int EndByte,char Type,char* PartName,int Prev,int Next){
    Batch* eB = newBatch();

    eB->StartByte = StartByte;
    eB->Size      = _Size;
    eB->EndByte   = EndByte;
    eB->Type      = Type;
    eB->PartName  = PartName;
    eB->Prev      = Prev;
    eB->Next      = Next;

    return eB;
}

DoublyGenericList* getBatchList_FromExtended(EBR* DefaultEBR,char* CompletePathDir,int Ext_StartByte,int Ext_EndByte){

    DoublyGenericList* EBR_List = new_DoublyGenericList();

    int _startByte = -1;
    int _size      = -1;
    int _endByte   = -1;
    int _prev      = -1;
    int _next      = -1;

    Batch* eB = newBatch();

    if(DefaultEBR->part_size == -1){
        //(^< ............ ............ ............ Default EBR
        _startByte = Ext_StartByte;
        _size      = sizeof(EBR);
        _endByte   = _startByte + _size - 1;
        _prev      = -1;
        _next      = -1;
        eB         = Set_Batch(_startByte,_size,_endByte,'q',DefaultEBR->part_name,_prev,_next);
        EnQueue(EBR_List,eB);

        
        //(^< ............ ............ ............ Space Check
        _prev      = _startByte;
        _next      = -1;
        _startByte = _endByte + 1;
        _size      = Ext_EndByte - (_endByte + 1) + 1;
        _endByte   = Ext_EndByte;
        eB         = Set_Batch(_startByte,_size,_endByte,'s',"FreeSpace",_prev,_next);
        EnQueue(EBR_List,eB);
        
        return EBR_List;

    }
    else{
        EBR* CurrentEBR = DefaultEBR;
        _prev = -1;
        _next = CurrentEBR->part_next;
        while(1){

            //(^< ............ ............ ............ Current EBR
            _startByte = CurrentEBR->part_start;
            _size      = CurrentEBR->part_size;
            _endByte   = _startByte + _size - 1;

            eB         = Set_Batch(_startByte,_size,_endByte,'q',CurrentEBR->part_name,_prev,_next);
            EnQueue(EBR_List,eB);

            //(^< ............ ............ ............ Space Check
            int space = (_next) - (_endByte);
            if(space > 1){
                _startByte = _endByte + 1;
                _size      = space;
                _endByte   = (_next - 1);
                eB         = Set_Batch(_startByte,_size,_endByte,'s',"FreeSpace",_prev,_next);
                EnQueue(EBR_List,eB);
            }

            if(CurrentEBR->part_next > -1){
                CurrentEBR = LoadEBR(CompletePathDir,CurrentEBR->part_next);
                _prev = _startByte;
                _next = CurrentEBR->part_next;
            }
            else{
                break;
            }
        }
    }

    //(^< ............ ............ ............ Space Check
    int space = Ext_EndByte - _endByte;
    if(space > 1){
        _prev      = _startByte;
        _next      = -1;
        _startByte = _endByte + 1;
        _size      = space;
        _endByte   = Ext_EndByte;
        eB         = Set_Batch(_startByte,_size,_endByte,'s',"FreeSpace",_prev,_next);
        EnQueue(EBR_List,eB);
    }
   return EBR_List;
}


DoublyGenericList* getBatchList_FromDisk(char* CompletePathDir,MBR* Disk){

    
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
        

        if(Part.part_type == 'e'){
            EBR* DefaultEBR = LoadEBR(CompletePathDir,Part.part_start);
            Sp->LgParts = getBatchList_FromExtended(DefaultEBR,CompletePathDir,Sp->StartByte,Sp->EndByte);
        }

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



Batch* getExtended_Batch(DoublyGenericList* BtList){
    int cnt = 0;
    while (cnt < BtList->Length){
        Batch* tB = (Batch*)(getNodebyIndex(BtList,cnt)->Dt);
        if(tB->Type == 'e'){
            return tB;
        }
        cnt++;
    }
    return NULL;
}

void Ascend_BubbleSort_BatchList(DoublyGenericList* btList){

}

void Desend_BubbleSort_BatchList(DoublyGenericList* btList){
    
}




#endif // BELONG_H