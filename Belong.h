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

MBR* Ascending_MBRPartArray_BubbleSort(MBR* Bf){
    
    int f = 0;
    while(f < 4){
        int Rs = (Bf->mbr_partition[f].part_start == (-1));
        if(Rs == 1){
            Bf->mbr_partition[f].part_start = Bf->mbr_tamano; 
        }
        f++;
    }

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

    f = 0;
    while(f < 4){
        int Rs = (Bf->mbr_partition[f].part_start == (Bf->mbr_tamano));
        if(Rs == 1){
            Bf->mbr_partition[f].part_start = (-1); 
        }
        f++;
    }

    int asd = 0;
    int asde = 0;
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
                if(CurrentEBR->part_next > -1){
                    eB         = Set_Batch(_startByte,_size,_endByte,'s',"FreeSpace",CurrentEBR->part_start,_next);
                }
                else{
                    eB         = Set_Batch(_startByte,_size,_endByte,'s',"FreeSpace",_prev,_next);
                }
                
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

Batch* getBatch_By_PartName(char* CompletePahtDir,MBR* Disk,char* PartName){
    
    int eek = 7;
    DoublyGenericList* btList = getBatchList_FromDisk(CompletePahtDir,Disk);

    while (btList->Length > 0){

        Batch* tmp = (Batch*)(DeQueue(btList));

        if(tmp->Type == 'e'){
            while(tmp->LgParts->Length > 0){
                Batch* inn = (Batch*)(DeQueue(tmp->LgParts));
                if(strcasecmp(PartName,inn->PartName) == 0){
                    return inn;
                }
            }    
        }

        if(strcasecmp(PartName,tmp->PartName) == 0){
            return tmp;
        }
    }
    
    return NULL;
}

void Part_1024_Erase(char* CompletePathDir,int StartByte,int PartSize){
    

    char* Bf = newString(1024);
    int Times = PartSize / 1024;
    
    FILE* Fl = fopen(CompletePathDir,"r+");
    if(Fl){
        
        int cnt = 0;
        while(cnt < Times){
            fseek(Fl,StartByte + (1024 * cnt),SEEK_SET);
            fwrite(Bf,1024,1,Fl);
            cnt++;
        }
        fclose(Fl);
    }
}

void Isolate_SpaceBatch(DoublyGenericList* btList){
    int i = 0;
    while(i < btList->Length){
        Batch* tmp = (Batch*)(getNodebyIndex(btList,i)->Dt);

        if(tmp->Type == 'e'){

            int j = 0;
            while(j < tmp->LgParts->Length){
                Batch* xp = (Batch*)(getNodebyIndex(tmp->LgParts,j)->Dt);
                if(xp->Type != 's'){
                    SpecificRemove(tmp->LgParts,j);
                    continue;
                }
                else{
                    j++;
                }
            }
        }

        if(tmp->Type != 's' && tmp->Type != 'e'){
            SpecificRemove(btList,i);
            continue;
        }
        else{
            i++;
        }
    }

}

void Ascending_Bubble_to_SpaceList(DoublyGenericList* SpaceList){

    int lSize = SpaceList->Length;
    int i = 0;
    while (i < lSize) {
        int j = 0;
        while (j < (lSize - (i + 1))) {
            Batch* tmp_iz = (Batch*)(getNodebyIndex(SpaceList,j)->Dt);
            Batch* tmp_der = (Batch*)(getNodebyIndex(SpaceList,j + 1)->Dt);

            int izval = (tmp_iz->Size)/1024;
            int derval = (tmp_der->Size)/1024;
            if(tmp_iz->Size > tmp_der->Size){
                Batch* swap = newBatch();
                *swap    = *tmp_iz;
                *tmp_iz  = *tmp_der;      
                *tmp_der = *swap;

                int sval = (swap->Size/1024);
                izval  = (tmp_iz->Size)/1024;
                derval = (tmp_der->Size)/1024;
            }
            j++;
        }
        i++;
    }
}

void Descending_Bubble_to_SpaceList(DoublyGenericList* SpaceList){
    int lSize = SpaceList->Length;
    int i = 0;
    while (i < lSize) {
        int j = 0;
        while (j < (lSize - (i + 1))) {
            Batch* tmp_iz = (Batch*)(getNodebyIndex(SpaceList,j)->Dt);
            Batch* tmp_der = (Batch*)(getNodebyIndex(SpaceList,j + 1)->Dt);

            int izval = (tmp_iz->Size)/1024;
            int derval = (tmp_der->Size)/1024;
            if(tmp_iz->Size < tmp_der->Size){
                Batch* swap = newBatch();
                *swap    = *tmp_iz;
                *tmp_iz  = *tmp_der;      
                *tmp_der = *swap;

                int sval = (swap->Size/1024);
                izval  = (tmp_iz->Size)/1024;
                derval = (tmp_der->Size)/1024;
            }
            j++;
        }
        i++;
    }
}

void get_Ascending_BatchSpace_List(DoublyGenericList* btList){

    Isolate_SpaceBatch(btList);

    DoublyGenericList* SpaceList     = btList;
    DoublyGenericList* ext_SpaceList = NULL;

    int i = 0;
    while(i < SpaceList->Length){
        Batch* tmp = (Batch*)(getNodebyIndex(btList,i)->Dt);
        if(tmp->Type == 'e'){
            ext_SpaceList = tmp->LgParts;
        }
        i++;
    }

    if(SpaceList != NULL){
        Ascending_Bubble_to_SpaceList(SpaceList);
    }

    if(ext_SpaceList != NULL){
        Ascending_Bubble_to_SpaceList(ext_SpaceList);
    }  
}

void get_Descending_BatchSpace_List(DoublyGenericList* btList){
    Isolate_SpaceBatch(btList);

    DoublyGenericList* SpaceList     = btList;
    DoublyGenericList* ext_SpaceList = NULL;

    int i = 0;
    while(i < SpaceList->Length){
        Batch* tmp = (Batch*)(getNodebyIndex(btList,i)->Dt);
        if(tmp->Type == 'e'){
            ext_SpaceList = tmp->LgParts;
        }
        i++;
    }

    if(SpaceList != NULL){
        Descending_Bubble_to_SpaceList(SpaceList);
    }

    if(ext_SpaceList != NULL){
        Descending_Bubble_to_SpaceList(ext_SpaceList);
    } 
}

Batch* get_First_SpaceBatch_That_Fits(DoublyGenericList* btList,int RqSpace){
    int cnt = 0;
    while (cnt < btList->Length){
        Batch* tmp = (Batch*)(getNodebyIndex(btList,cnt)->Dt);
        if(tmp->Size >= RqSpace && tmp->Type == 's'){
            return tmp;
        }
        cnt++;
    }
    return NULL;
}

int DeleteAsk(char* Nm){
    printf("\n");
    printf("FDISK WARNING: Esta seguro de Eliminar la Particion   -> %s <-   ?\n",Nm);
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
        return 0;
    }

    return 1;
}





Disk_in_Use* get_Disk_in_Use_By_DiskName(char* CompletePathDir){
    
    Mounted_Part* mP;
    Disk_in_Use*  dI;

    int i = 0;
    while (i < 25){
        dI = &UsingDisk_List[i];
        if(dI->status == 1){
            if(strcasecmp(dI->CompletePathDir,CompletePathDir) == 0){
                //dI = dI + i; 
                return dI;
            }
        }
        i++;
    }

    return NULL;
}

int get_First_EmptyIndex_of_UsingDisk_List(){
    int i = 0;
    while (i < 25){
        if(UsingDisk_List[i].status == 0){
            return i;
        }
        i++;
    }
    return -1;
}

int get_First_EmptyIndex_of_mntList(Disk_in_Use* dI){
    
    int i = 0;
    while (i < 25){
        Mounted_Part* mP = &dI->mntList[i];
        if(mP->status == 0){
            return i;
        }
        i++;
    }
    return -1;
}




Mounted_Part* getPartMounted_By_Name(char* PartName){

    Mounted_Part* mP;
    Disk_in_Use*  dI;

    int i = 0;
    while (i < 25){
        dI = &UsingDisk_List[i];
        if(dI->status == 1){
            int j = 0;
            while (j < 25){
                mP = &dI->mntList[j];
                if(mP->status == 1){
                    
                    if(strcasecmp(mP->ParName,PartName) == 0){
                        return mP;
                    }

                }
                j++;
            }
        }
        i++;
    }
    return NULL;
}

char* get_MountedPart_String_ID(char* CompletePathDir,char* PartName){

    //------------------------------
    char lt;
    int  nm;
    char* mID = newString("vd");
    //------------------------------

    Disk_in_Use* dI = get_Disk_in_Use_By_DiskName(CompletePathDir);

    if(dI == NULL){
        lt = get_First_EmptyIndex_of_UsingDisk_List() + 97;
        nm = 1;
    }
    else{
        lt = dI->index + 97;
        nm = get_First_EmptyIndex_of_mntList(dI) + 1;
    }

    //Concat Letter & Number ------------------
    mID = Concat_Izq_with_Der(mID,&lt,'s','c');
    mID = Concat_Izq_with_Der(mID,&nm,'s','i');
    //-----------------------------------------

    return mID;

}


Locat* vdTransform(char* mID){
    Locat* lcat = newLocat();

    lcat->Letter = mID[2] - 97;

    if(strlen(mID) == 5){

        int a = lcat->Num = mID[3] - 48;
        int b = lcat->Num = mID[4] - 48;
        lcat->Num = (a * 10) + b;
    }
    else{
        lcat->Num = mID[3] - 48;
    }

    return lcat;
}

Mounted_Part* getPartMounted_By_vID(char* mID){

    Locat* lcat = vdTransform(mID);

    Mounted_Part* Rs = &(UsingDisk_List[lcat->Letter].mntList[lcat->Num - 1]);

    if(Rs->status == 0){
        return NULL;
    }
    return Rs;
}



#endif // BELONG_H

/*
Mounted_Part* mP;
Disk_in_Use*  dI;

int i = 0;
while (i < 25){
    dI = &UsingDisk_List[i];
    if(dI->status == 1){
        
        
        int j = 0;
        while (j < 25){
            mP = &dI->mntList[j];
            if(mP->status == 1){
                
            }
            j++;
        }

    }
    i++;

}
*/