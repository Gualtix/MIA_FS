#ifndef BELONG_H
#define BELONG_H

#include "Fw/Helper.h"
#include "Glovs.h"

#pragma GCC diagnostic ignored "-Wwrite-strings"

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

void Print_Mounted_Parts(){
    Mounted_Part* mP;
    Disk_in_Use*  dI;

    int i = 0;
    while (i < 25){
        dI = &UsingDisk_List[i];
        
        if(dI->status == 1){
            char* DiskName = Path_Get_FileName(dI->CompletePathDir);
            printf("\n");
            printf("Disco   -> %s <-\n",DiskName);
            int j = 0;
            while (j < 25){
                mP = &dI->mntList[j];
                if(mP->status == 1){
                    char* IDE = newString("vd");
                    char lt = (i + 97);
                    int  Nm = (j + 1);
                    IDE = Concat_Izq_with_Der(IDE,&lt,'s','c');
                    IDE = Concat_Izq_with_Der(IDE,&Nm,'s','i');
                    printf("\tParticion   -> %s <-       ID   -> %s <-\n",mP->ParName,IDE);
                }
                j++;
            }
        }
        i++;
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

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ T O O L S
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

FileFolderInfo* get_FFInfo(InfoCatcher* nwInf){
    
    FileFolderInfo* ffInf = newFileFolderInfo();

    char* Name = Path_Get_FileName(newString(nwInf->_path));
    ffInf->PathPlacesList = PathSeparate(newString(nwInf->_path));
    ffInf->FileName = Name;
    
    if(nwInf->_R == 1 || nwInf->_P == 1){
        ffInf->isRecursive = 1;
    }
    else{
        ffInf->isRecursive = 0;
    }

    ffInf->txtData = nwInf->txtData;

    return ffInf;
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ B I N
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

SuperBlock* LoadSuperBlock(int StartByte){
    FILE* Fl = fopen(Omni->CompletePathDir_of_Disk_inUse,"rb+");
    SuperBlock* SB = newSuperBlock();
    if(Fl){
        fseek(Fl,StartByte,SEEK_SET);
        fread(SB,sizeof(SuperBlock),1, Fl);
        fclose(Fl);
        return SB;
    }
    else{
        return NULL;
    }
}

int UpdateSuperBlock(){
    int StartByte = Omni->PartBatch_inUse->StartByte;
    FILE* Fl = fopen(Omni->CompletePathDir_of_Disk_inUse,"rb+");
    if(Fl){
        fseek(Fl,StartByte,SEEK_SET);
        fwrite(Omni->SBinuse,sizeof(SuperBlock),1,Fl);
        fclose(Fl);
        return 1;
    }
    return 0;
}

int Bit_to_StartByte(int Bit,char* BiType){
    if(strcasecmp(BiType,"Inode") == 0){
        return (Omni->SBinuse->s_inode_start + (Bit * sizeof(Inode)));
    }
    if(strcasecmp(BiType,"Block") == 0){
        return (Omni->SBinuse->s_block_start + (Bit * sizeof(FileBlock)));
    }
    return -1;
}

int StartByte_to_InodeBit(int StartByte){
    return((StartByte - Omni->SBinuse->s_inode_start)/(sizeof(Inode)));
}

int StartByte_to_BlockBit(int StartByte){
    return((StartByte - Omni->SBinuse->s_block_start)/(sizeof(FileBlock)));
}

void BinWriteUpdate_BlockBit(char Op,int Bit_ID){

    FILE* Fl = fopen(Omni->CompletePathDir_of_Disk_inUse,"rb+");
    if(Fl){
        // < < < B L O C K   B I T M A P   U P D A T E > > >
        //char ch = '1';
        int StartByte = Omni->SBinuse->s_bm_block_start + Bit_ID;
        fseek(Fl,StartByte,SEEK_SET);
        fwrite(&Op,sizeof(char),1,Fl);
    }

    fclose(Fl);
}

void BinWriteUpdate_InodeBit(char Op,int Bit_ID){
    FILE* Fl = fopen(Omni->CompletePathDir_of_Disk_inUse,"rb+");
    if(Fl){
        // < < < I N O D E   B I T M A P   U P D A T E > > >
        //char ch = '1';
        int StartByte = Omni->SBinuse->s_bm_inode_start + Bit_ID;
        fseek(Fl,StartByte,SEEK_SET);
        fwrite(&Op,sizeof(char),1,Fl);
    }
    
    fclose(Fl);
}
void* BinLoad_Str(int Bit_ID,char* StrType){

    int StartByte = -1;
    
    if(strcasecmp(StrType,"Inode") == 0){
        StartByte = Bit_to_StartByte(Bit_ID,"Inode");
    }
    else{
        StartByte = Bit_to_StartByte(Bit_ID,"Block");
    }

    FILE* Fl = fopen(Omni->CompletePathDir_of_Disk_inUse,"rb+");
    if(Fl){
        fseek(Fl,StartByte,SEEK_SET);
        if(strcasecmp(StrType,"Inode") == 0){
            Inode* In = newInode();
            fread(In,sizeof(Inode),1, Fl);
            fclose(Fl);
            return In;
        }
        else if(strcasecmp(StrType,"FolderBlock") == 0){
            FolderBlock* Fold_B = newFolderBlock();
            fread(Fold_B,sizeof(FolderBlock),1, Fl);
            fclose(Fl);
            return Fold_B;
        }
        else if(strcasecmp(StrType,"FileBlock") == 0){
            FileBlock* File_B = newFileBlock();
            fread(File_B,sizeof(FileBlock),1, Fl);
            fclose(Fl);
            return File_B;
        }
        else if(strcasecmp(StrType,"PointerBlock") == 0){
            PointerBlock* Point_B = newPointerBlock();
            fread(Point_B,sizeof(PointerBlock),1, Fl);
            fclose(Fl);
            return Point_B;
        }
        else{
            fclose(Fl);
            return NULL;
        }
    }
    return NULL;
}

int BinWrite_Struct(void* Str,int Bit_ID,char* StrType){

    int StartByte = -1;
    
    if(strcasecmp(StrType,"Inode") == 0){
        StartByte = Bit_to_StartByte(Bit_ID,"Inode");
    }
    else{
        StartByte = Bit_to_StartByte(Bit_ID,"Block");
    }
    
    FILE* Fl = fopen(Omni->CompletePathDir_of_Disk_inUse,"rb+");
    if(Fl){
        fseek(Fl,StartByte,SEEK_SET);
        if(strcasecmp(StrType,"Inode") == 0){
            Inode* In = (Inode*)Str;
            fwrite(In,sizeof(Inode),1,Fl);

            // < < < I N O D E   B I T M A P   U P D A T E > > >
            char ch = '1';
            int StartByte = Omni->SBinuse->s_bm_inode_start + Bit_ID;
            fseek(Fl,StartByte,SEEK_SET);
            fwrite(&ch,sizeof(char),1,Fl);

        }
        else if(strcasecmp(StrType,"FolderBlock") == 0){
            FolderBlock* Fold_B = (FolderBlock*)Str;
            fwrite(Fold_B,sizeof(FolderBlock),1,Fl);

            // < < < B L O C K   B I T M A P   U P D A T E > > >
            char ch = '1';
            int StartByte = Omni->SBinuse->s_bm_block_start + Bit_ID;
            fseek(Fl,StartByte,SEEK_SET);
            fwrite(&ch,sizeof(char),1,Fl);
            
        }
        else if(strcasecmp(StrType,"FileBlock") == 0){
            FileBlock* File_B = (FileBlock*)Str;
            fwrite(File_B,sizeof(FileBlock),1,Fl);

            // < < < B L O C K   B I T M A P   U P D A T E > > >
            char ch = '1';
            int StartByte = Omni->SBinuse->s_bm_block_start + Bit_ID;
            fseek(Fl,StartByte,SEEK_SET);
            fwrite(&ch,sizeof(char),1,Fl);
        }
        else if(strcasecmp(StrType,"PointerBlock") == 0){
            PointerBlock* Point_B = (PointerBlock*)Str;
            fwrite(Point_B,sizeof(PointerBlock),1,Fl);

            // < < < B L O C K   B I T M A P   U P D A T E > > >
            char ch = '1';
            int StartByte = Omni->SBinuse->s_bm_block_start + Bit_ID;
            fseek(Fl,StartByte,SEEK_SET);
            fwrite(&ch,sizeof(char),1,Fl);
        }
        else{
            fclose(Fl);
            return 0;
        }
        fclose(Fl);
        return 1;
    }
    return 0;
}

char BinRead_SingleChar(int StartByte){
    char ch = '0';
    FILE* Fl = fopen(Omni->CompletePathDir_of_Disk_inUse,"rb+");
    if(Fl){
        fseek(Fl,StartByte,SEEK_SET);
        fread(&ch,sizeof(char),1, Fl);
        fclose(Fl);
        return ch;   
    }
    return '\0';
}

void BinWrite_withChar(int StartByte,char ch,int nTimes){
    FILE* Fl = fopen(Omni->CompletePathDir_of_Disk_inUse,"rb+");
    if(Fl){
        int cnt = 0;
        while(cnt < nTimes){
            fseek(Fl,StartByte + cnt,SEEK_SET);
            fwrite(&ch,sizeof(char),1,Fl);
            cnt++;
        }
        fclose(Fl);
    }
}

void setOmni(char* mID){
    Locat* lcat = vdTransform(mID);
    Omni->CompletePathDir_of_Disk_inUse = UsingDisk_List[lcat->Letter].CompletePathDir;

    Mounted_Part* mP = getPartMounted_By_vID(mID);
    
    MBR* Disk = LoadMBR(Omni->CompletePathDir_of_Disk_inUse);
    Batch* bt = getBatch_By_PartName(Omni->CompletePathDir_of_Disk_inUse,Disk,mP->ParName);
    Omni->PartBatch_inUse = bt;

    Omni->SBinuse = LoadSuperBlock(bt->StartByte);
}



//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ I N O D E
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

int getFirst_InodeBit_Free(){

    char ch;
    int StartByte = Omni->SBinuse->s_bm_inode_start;
    int EndByte   = Omni->SBinuse->s_bm_block_start - 1;
    int CubeSize  = (EndByte - StartByte + 1); 

    FILE* Fl = fopen(Omni->CompletePathDir_of_Disk_inUse,"rb+");
    int cnt = 0;

    if(Fl){
        while(cnt < CubeSize){
            fseek(Fl,StartByte + cnt,SEEK_SET);
            fread(&ch,sizeof(char),1,Fl);
            if(ch == '0'){
                fclose(Fl);
                return cnt;
            }
            cnt++;
        }
    }
    fclose(Fl);
    return -1;
}

void clear_inodeBits(){
    int StartByte = Omni->SBinuse->s_bm_inode_start;
    int EndByte = Omni->SBinuse->s_bm_block_start - 1;
    int CubeSize = (EndByte - StartByte + 1); 
    BinWrite_withChar(StartByte,'0',CubeSize);
}

int FreeInodeCounting(int iN){
    int cnt = 0;
    int Fc  = 0;
    char Tmp;
    while(cnt < (iN * 3)){
        Tmp = BinRead_SingleChar(Omni->SBinuse->s_bm_inode_start + cnt);
        if(Tmp == '0'){
            Fc++;
        }
        cnt++;
    }
    return Fc;
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ B L O C K
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

int getFirst_BlockBit_Free(){

    char ch;
    int StartByte = Omni->SBinuse->s_bm_block_start;
    int EndByte = Omni->SBinuse->s_inode_start - 1;
    int CubeSize = (EndByte - StartByte + 1);

    FILE* Fl = fopen(Omni->CompletePathDir_of_Disk_inUse,"rb+");
    int cnt = 0;

    if(Fl){
        while(cnt < CubeSize){
            fseek(Fl,StartByte + cnt,SEEK_SET);
            fread(&ch,sizeof(char),1,Fl);
            if(ch == '0'){
                fclose(Fl);
                return cnt;
            }
            cnt++;
        }
    }
    fclose(Fl);
    return -1;
}

void clear_blockBits(){
    int StartByte = Omni->SBinuse->s_bm_block_start;
    int EndByte = Omni->SBinuse->s_inode_start - 1;
    int CubeSize = (EndByte - StartByte + 1); 
    BinWrite_withChar(StartByte,'0',CubeSize);
}

int FreeBlockCounting(int iN){
    int cnt = 0;
    int Fc  = 0;
    char Tmp;
    while(cnt < iN){
        Tmp = BinRead_SingleChar(Omni->SBinuse->s_bm_block_start + cnt);
        if(Tmp == '0'){
            Fc++;
        }
        cnt++;
    }
    return Fc;
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ FS C R U D
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

char* getDefault_txtContent(int _size){

    char* txtDefault = newString(_size);
    int cnt = 0;
    while(cnt < _size){

        int tmp = (cnt - (10 *  (cnt/10)));
        txtDefault[cnt] = '0' + tmp;

        cnt++;
    }

    int Ln = strlen(txtDefault);
    return txtDefault;
}

SeekInfo* CompleteSeeker(int iNodeCurent_Bit_ID,char* FileName){

    SeekInfo* nwSI = newSeekInfo();
    Inode* tIn = (Inode*)BinLoad_Str(iNodeCurent_Bit_ID,"Inode");

    int i_tIn = 0;
    while(i_tIn < 12){

        if(tIn->i_block[i_tIn] > -1){
            int tFB_Bit_ID = tIn->i_block[i_tIn];
            if(tFB_Bit_ID == 8){
                int s = 5;
            }
            FolderBlock* tFB = (FolderBlock*)BinLoad_Str(tFB_Bit_ID,"FolderBlock");

            if(i_tIn == 0){
                int iNodeFather_Bit_ID = tFB->b_content[0].b_inodo;
                nwSI->iNodeFather_Bit_ID = iNodeFather_Bit_ID;
            }

            int i_tFB = 0;
            while(i_tFB < 4){
                if(strcasecmp(tFB->b_content[i_tFB].b_name,FileName) == 0){
                    nwSI->FB_Bit_ID = tFB_Bit_ID;
                    nwSI->FB_Index  = i_tFB;
                    nwSI->iNode_Bit_ID = tFB->b_content[i_tFB].b_inodo;
                    nwSI->iCurent_Bit_ID = iNodeCurent_Bit_ID;
                    return nwSI;
                }
                else{

                    int nextIn_Bit_ID = tFB->b_content[i_tFB].b_inodo;
                    Inode* nextIn = (Inode*)BinLoad_Str(nextIn_Bit_ID,"Inode");
                    if(nextIn->i_type == 0 && strcasecmp(tFB->b_content[i_tFB].b_name,"iNodeFather") != 0 && strcasecmp(tFB->b_content[i_tFB].b_name,"iNodeCurent") != 0){
                        SeekInfo* tmp = CompleteSeeker(nextIn_Bit_ID,FileName);
                        if(tmp != NULL){
                            return tmp;
                        }
                        //return CompleteSeeker(nextIn_Bit_ID,FileName);
                    }
                }
                i_tFB++;
            }
            
            
        }
        i_tIn++;
    }

    return NULL;

}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ F O L D E R
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

int Produce_newFolderBlock(FolderBlock* newStr,int iNodeFather_Bit_ID,int iNodeCurrent_Bit_ID){
    if(iNodeFather_Bit_ID > -1 && iNodeCurrent_Bit_ID > -1){
        strcpy(newStr->b_content[0].b_name,"iNodeFather");
        newStr->b_content[0].b_inodo = iNodeFather_Bit_ID;

        strcpy(newStr->b_content[1].b_name,"iNodeCurent");
        newStr->b_content[1].b_inodo = iNodeCurrent_Bit_ID;
    }
    int newStr_Bit_ID = getFirst_BlockBit_Free();
    BinWrite_Struct(newStr,newStr_Bit_ID,"FolderBlock");
    return newStr_Bit_ID;
}

int Produce_newFileBlock(FileBlock* newStr){
    int newStr_Bit_ID = getFirst_BlockBit_Free();
    BinWrite_Struct(newStr,newStr_Bit_ID,"FileBlock");
    return newStr_Bit_ID;
}

int Produce_newPointerBlock(PointerBlock* newStr){
    int newStr_Bit_ID = getFirst_BlockBit_Free();
    BinWrite_Struct(newStr,newStr_Bit_ID,"PointerBlock");
    return newStr_Bit_ID;
}

int Produce_newInode_FolderType(int Permissions,int i_uid, int i_gid){
    int nInode_Bit_ID = getFirst_InodeBit_Free();
    Inode* nInode = newInode();

    nInode->i_uid = i_uid;
    nInode->i_gid = i_gid;
    nInode->i_size = 0;
    //nInode->i_atime;
    //nInode->i_ctime;
    //nInode->i_mtime;
    nInode->i_type = 0;
    nInode->i_perm = Permissions;

    BinWrite_Struct(nInode,nInode_Bit_ID,"Inode");
    return nInode_Bit_ID;
}

int iNode_FolderDirectCore(FolderBlock* tFB,int tPtr,int tFB_Index,int iNodeFather_Bit_ID,char* FolderName,int Permits,int i_uid, int i_gid){
    // ------- ------- ------- New iNode_Folder   
    int nInodeFolder_Bit_ID = Produce_newInode_FolderType(Permits,i_uid,i_gid);
    Inode* nInodeFolder = (Inode*)BinLoad_Str(nInodeFolder_Bit_ID,"Inode");
    // ------- ------- ------- ------- -------

    // ------- ------- ------- New Folder    
    FolderBlock* newFB = newFolderBlock();
    int newFB_Bit_ID = Produce_newFolderBlock(newFB,iNodeFather_Bit_ID,nInodeFolder_Bit_ID);
    newFB = (FolderBlock*)BinLoad_Str(newFB_Bit_ID,"FolderBlock");
    // ------- ------- ------- ------- -------

    // ------- ------- ------- New iNode_Folder Linking
    nInodeFolder->i_block[0] = newFB_Bit_ID;
    BinWrite_Struct(nInodeFolder,nInodeFolder_Bit_ID,"Inode");
    // ------- ------- ------- ------- -------

    // ------- ------- ------- Folder Linking & Update
    strcpy(tFB->b_content[tFB_Index].b_name,FolderName);
    tFB->b_content[tFB_Index].b_inodo = nInodeFolder_Bit_ID;

    BinWrite_Struct(tFB,tPtr,"FolderBlock");

    return nInodeFolder_Bit_ID;
}

int Folder_Direct_BlockCheck(int tPtr,Inode* iNodeFather,int iNodeFather_DirectIndex,int iNodeFather_Bit_ID,char* FolderName,int Permits,int i_uid,int i_gid){
    if(tPtr > -1){
        FolderBlock* tFB = (FolderBlock*)BinLoad_Str(tPtr,"FolderBlock");
        int i = 0;
        while(i < 4){
            if(tFB->b_content[i].b_inodo == -1){
                int nwInodeFolder_Bit_ID = iNode_FolderDirectCore(tFB,tPtr,i,iNodeFather_Bit_ID,FolderName,Permits,i_uid,i_gid);
                return nwInodeFolder_Bit_ID;
            }
            i++;
        }
        return -1;

    }
    // ******* FirstTime
    else{

        // ------- ------- ------- New Folder    
        FolderBlock* newFB = newFolderBlock();
        int newFB_Bit_ID   = Produce_newFolderBlock(newFB,-1,-1);
        // ------- ------- ------- ------- -------

        // ------- ------- ------- iNodeFather Linking & Update
        iNodeFather->i_block[iNodeFather_DirectIndex] = newFB_Bit_ID;
        BinWrite_Struct(iNodeFather,iNodeFather_Bit_ID,"Inode");
        return Folder_Direct_BlockCheck(newFB_Bit_ID,iNodeFather,iNodeFather_DirectIndex,iNodeFather_Bit_ID,FolderName,Permits,i_uid,i_gid);
        //return allocate_NewFolder(iNodeFather_Bit_ID,FolderName,Permits,i_uid,i_gid);
        // ------- ------- ------- ------- ------- 

    }
}

int Folder_Indirect_BlockCheck(PointerBlock* tPB,int tPB_Bit_ID,int tPB_index,char* FolderName,int Permits,int i_uid,int i_gid){
    int tPtr = tPB->b_pointers[tPB_index];
    if(tPtr > -1){
        FolderBlock* tFB = (FolderBlock*)BinLoad_Str(tPtr,"FolderBlock");
        int i = 0;
        while(i < 4){
            if(tFB->b_content[i].b_inodo == -1){
                int nwInodeFolder_Bit_ID = iNode_FolderDirectCore(tFB,tPtr,i,-1,FolderName,Permits,i_uid,i_gid);
                return nwInodeFolder_Bit_ID;
            }
        i++;
    }
    return -1;
    }
    // ******* FirstTime
    else{

        // ------- ------- ------- New Folder    
        FolderBlock* newFB = newFolderBlock();
        int newFB_Bit_ID   = Produce_newFolderBlock(newFB,-1,-1);
        // ------- ------- ------- ------- -------

        // ------- ------- ------- BlockPointer Linking & Update
        tPB->b_pointers[tPB_index] = newFB_Bit_ID;
        BinWrite_Struct(tPB,tPB_Bit_ID,"PointerBlock");
        return Folder_Indirect_BlockCheck(tPB,tPB_Bit_ID,tPB_index,FolderName,Permits,i_uid,i_gid);
        // ------- ------- ------- ------- ------- 

    }

}

int allocate_NewFolder(int iNodeFather_Bit_ID,char* FolderName,int Permits,int i_uid,int i_gid){
    Inode* iNodeFather = (Inode*)BinLoad_Str(iNodeFather_Bit_ID,"Inode");

    // ******* D I R E C T S *******
    int cnt = 0;
    while(cnt < 12){
        int tPtr = iNodeFather->i_block[cnt];
        int nwInodeFolder_Bit_ID = Folder_Direct_BlockCheck(tPtr,iNodeFather,cnt,iNodeFather_Bit_ID,FolderName,Permits,i_uid,i_gid);
        if(nwInodeFolder_Bit_ID > -1){
            return nwInodeFolder_Bit_ID;
        }
        cnt++;
    }

    // ******* I N D I R E C T S *******
    
    while(cnt < 15){
        int tPtr_3 = iNodeFather->i_block[cnt];
        // ******* PointersCheck
        if(tPtr_3 > -1){
            PointerBlock* tPB = (PointerBlock*)BinLoad_Str(tPtr_3,"PointerBlock");
            int z = 0;
            while(z < 16){
                int tPter_2 = tPB->b_pointers[z];
                int nwInodeFolder_Bit_ID = Folder_Indirect_BlockCheck(tPB,tPtr_3,z,FolderName,Permits,i_uid,i_gid);
                if(nwInodeFolder_Bit_ID > -1){
                    return nwInodeFolder_Bit_ID;
                }
                z++;
            }
            
        }
        else{
            // ------- ------- ------- New PointerBlock    
            PointerBlock* newPB = newPointerBlock();
            int newPB_Bit_ID   = Produce_newPointerBlock(newPB);
            // ------- ------- ------- ------- -------

            // ------- ------- ------- iNodeFather Linking & Update
            iNodeFather->i_block[cnt] = newPB_Bit_ID;
            BinWrite_Struct(iNodeFather,iNodeFather_Bit_ID,"Inode");
            return allocate_NewFolder(iNodeFather_Bit_ID,FolderName,Permits,i_uid,i_gid);
            // ------- ------- ------- ------- ------- 
        }

        cnt++;
    }

}

int make_newFolder(InfoCatcher* nwInf){
    FileFolderInfo* ffInf = get_FFInfo(nwInf);
    return 0;
}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ F I L E
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

int fill_FilePointerBlock(DoublyGenericList** List_64_Char){
    int Pb_Bit_ID = -1;
    PointerBlock* Pb = newPointerBlock();

    int i = 0;
    while(i < 16 && (*List_64_Char)->Length > 0){
        char* tS = (char*)DeQueue((*List_64_Char));
        int tFB_Bit_ID = getFirst_BlockBit_Free();
        FileBlock* tFB = newFileBlock();
        strcpy(tFB->b_content,tS);
        BinWrite_Struct(tFB,tFB_Bit_ID,"FileBlock");

        // ******* L I N K I N G *******
        Pb->b_pointers[i] = tFB_Bit_ID;
        i++;
    }

    // ******* PointerBlock Writing *******
    Pb_Bit_ID = getFirst_BlockBit_Free();
    BinWrite_Struct(Pb,Pb_Bit_ID,"PointerBlock");

    return Pb_Bit_ID;
}

int place_at_newFile(int FatherFolderBLock_Bit_ID,int FolderBlock_Pos,char* FileName,char* Content, int Permits,int i_uid,int i_gid){
    // ******* Father FolderBlock *******
    int FatherFolderBlock_StartByte = Bit_to_StartByte(FatherFolderBLock_Bit_ID,"Block");
    FolderBlock* FatherFolderBlock  = (FolderBlock*)BinLoad_Str(FatherFolderBLock_Bit_ID,"FolderBlock");

    // ******* New iNode *******
    
    Inode* new_iN = newInode();

    new_iN->i_uid = i_uid;
    new_iN->i_gid = i_gid;
    new_iN->i_size = (strlen(Content) * sizeof(char));

    //new_iN->i_atime;
    //new_iN->i_ctime;
    //new_iN->i_mtime;

    //FileType -> 1
    new_iN->i_type = 1;
    new_iN->i_perm = Permits;

    // ******* File Content Sharing *******
    DoublyGenericList* List_64_Char = getSubstring_64CharList(Content);
    int nFBlock = List_64_Char->Length;

    int cnt = 0;
    char* tS;
    while(List_64_Char->Length > 0){

        if(cnt < 12){
            tS = (char*)DeQueue(List_64_Char);
            FileBlock* FB_temp = newFileBlock();
            strcpy(FB_temp->b_content,tS);

            if(strcasecmp(FileName,"users.txt") == 0){
                FolderBlock* Tmp = (FolderBlock*)BinLoad_Str(2,"FolderBlock");
                int asdf = 5;
            }

            int FB_tmp_Bit_ID = getFirst_BlockBit_Free();
            //int FB_tmp_StartByte = Bit_to_StartByte(FB_tmp_Bit_ID,"Block");
            BinWrite_Struct(FB_temp,FB_tmp_Bit_ID,"FileBlock");

            if(strcasecmp(FileName,"users.txt") == 0){
                FolderBlock* Tmp = (FolderBlock*)BinLoad_Str(2,"FolderBlock");
                int asdf = 5;
            }

            
            

            new_iN->i_block[cnt] = FB_tmp_Bit_ID;
        }

        if(cnt >= 12 && cnt < 15){
            int ptr_Bit_ID = fill_FilePointerBlock(&List_64_Char);
            new_iN->i_block[cnt] = ptr_Bit_ID;
        }
        cnt++;
    }

    int iNode_Bit_ID = getFirst_InodeBit_Free();
    int iNode_StartByte = Bit_to_StartByte(iNode_Bit_ID,"Inode");

    BinWrite_Struct(new_iN,iNode_Bit_ID,"Inode");

    // ******* FatherLinking *******
    strcpy(FatherFolderBlock->b_content[FolderBlock_Pos].b_name,FileName);
    FatherFolderBlock->b_content[FolderBlock_Pos].b_inodo =  iNode_Bit_ID;

    BinWrite_Struct(FatherFolderBlock,FatherFolderBLock_Bit_ID,"FolderBlock");
}

int allocate_newFile(int iNode_Folder_Bit_ID,char* FileName,char* Content,int Permits,int i_uid,int i_gid){
    // ******* Folder Load *******
    Inode* iNode_Folder = (Inode*)BinLoad_Str(iNode_Folder_Bit_ID,"Inode");
    int i = 0;
    while(i < 12 ){

        if(iNode_Folder->i_block[i] > -1){
            // ******* Direct Tour *******
            if(i < 12){
                int tFolderBlock_Bit_ID = iNode_Folder->i_block[i];
                FolderBlock* tFolderBlock = (FolderBlock*)BinLoad_Str(tFolderBlock_Bit_ID,"FolderBlock");
                int j = 0;
                while(j < 4){
                    if(tFolderBlock->b_content[j].b_inodo == -1){
                        place_at_newFile(tFolderBlock_Bit_ID,j,FileName,Content,Permits,i_uid,i_gid);
                        return 1;
                    }
                    j++;
                }

                if((i + 1) < 12){

                    if(iNode_Folder->i_block[i + 1] > -1){
                        i++;
                        continue;
                    }

                    int nFB_Bit_ID = getFirst_BlockBit_Free();
                    FolderBlock* nFB = newFolderBlock();
                    BinWrite_Struct(nFB,nFB_Bit_ID,"FolderBlock");
                    place_at_newFile(nFB_Bit_ID,0,FileName,Content,Permits,i_uid,i_gid);
                    iNode_Folder->i_block[i + 1] = nFB_Bit_ID;
                    BinWrite_Struct(iNode_Folder,iNode_Folder_Bit_ID,"Inode");
                    return 1; 
                }
                //i++;
            }
        }
        i++;
    }


    while(i < 15){
        // ******* Indirect Tour *******
        if(iNode_Folder->i_block[i] == -1){

            int nFB_Bit_ID = getFirst_BlockBit_Free();
            FolderBlock* nFB = newFolderBlock();
            BinWrite_Struct(nFB,nFB_Bit_ID,"FolderBlock");
            place_at_newFile(nFB_Bit_ID,0,FileName,Content,Permits,i_uid,i_gid);

            int tPB_Bit_ID = getFirst_BlockBit_Free();
            PointerBlock* tPB = newPointerBlock();
            tPB->b_pointers[0] = nFB_Bit_ID;
            BinWrite_Struct(tPB,tPB_Bit_ID,"PointerBlock");

            iNode_Folder->i_block[i] = tPB_Bit_ID;
            BinWrite_Struct(iNode_Folder,iNode_Folder_Bit_ID,"Inode");
            return 1; 
        }
        else{

            
            int tPB_Bit_ID = iNode_Folder->i_block[i];
            PointerBlock* tPB = (PointerBlock*)BinLoad_Str(tPB_Bit_ID,"PointerBlock");

            int cnt = 0;
            while(cnt < 16){
                if(tPB->b_pointers[cnt] >= 0){
                    int tFB_Bit_ID = tPB->b_pointers[cnt];
                    if(tFB_Bit_ID == 74){
                        int kss = tFB_Bit_ID;
                    }
                    FolderBlock* tFB = (FolderBlock*)BinLoad_Str(tFB_Bit_ID,"FolderBlock");
                    
                    int k = 0;
                    while(k < 4){
                        if(tFB->b_content[k].b_inodo == -1){
                            place_at_newFile(tFB_Bit_ID,k,FileName,Content,Permits,i_uid,i_gid);
                            return 1; 
                        }
                        k++;
                    }

                    if((cnt + 1) < 16){
                        if(tPB->b_pointers[cnt + 1] == -1){
                            int nFB_Bit_ID = getFirst_BlockBit_Free();
                            FolderBlock* nFB = newFolderBlock();
                            BinWrite_Struct(nFB,nFB_Bit_ID,"FolderBlock");
                            place_at_newFile(nFB_Bit_ID,0,FileName,Content,Permits,i_uid,i_gid);

                            //int tPB_Bit_ID = getFirst_BlockBit_Free();
                            //PointerBlock* tPB = newPointerBlock();
                            tPB->b_pointers[cnt + 1] = nFB_Bit_ID;
                            BinWrite_Struct(tPB,tPB_Bit_ID,"PointerBlock");
                            return 1; 
                        }
                        
                    } 
                    else{
                        return 0;
                    }
                }
                else{

                }
                cnt++;
            }
        }
        i++;
    }
    
}

int make_newFile(InfoCatcher* nwInf){
    FileFolderInfo* ffInf = get_FFInfo(nwInf);

    if(nwInf->_cont == NULL){
        if(nwInf->txtData == NULL){
            nwInf->txtData = getDefault_txtContent(nwInf->_size);
        }
    }
    else{
        nwInf->txtData = getString_from_File(nwInf->_cont);
        if(nwInf->txtData ==  NULL){
            printf("\n");
            printf("MKDIR -Cont ERROR: Archivo %s No Encontrado\n",nwInf->_cont);
            return -1;
        }
    }

    int DeepestFolder_Bit_ID = make_newFolder(nwInf);
    if(DeepestFolder_Bit_ID > -1){
        int newFile_Bit_ID = allocate_newFile(DeepestFolder_Bit_ID,ffInf->FileName,ffInf->txtData,664,0,0);
        return newFile_Bit_ID;
    }
    return - 1;
}


//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ F O R M A T
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............


void Fast_PartFormat(){
    clear_blockBits();
    clear_inodeBits();
}

void Full_PartFormat(){
    Fast_PartFormat();
}

int Calc_iN(int PartSize){
    int iN = (PartSize - sizeof(SuperBlock)) / (4 + sizeof(Journaling) + sizeof(Inode) + (3 * sizeof(FileBlock)));
    return iN;
}

void Format_to_EXT2(){
}

void Format_to_EXT3(){

    int iN = Calc_iN(Omni->PartBatch_inUse->Size);

    SuperBlock* SB = newSuperBlock();

    SB->s_filesystem_type = 3;
    SB->s_inodes_count = iN;
    SB->s_blocks_count = (3 * iN);
    SB->s_free_inodes_count = iN;
    SB->s_free_blocks_count = (3 * iN);

    //strcpy(SB->s_mtime,getDateTime());
    //strcpy(SB->s_umtime,getDateTime());
    //SB->s_mnt_count
    //SB->s_magic

    SB->s_inode_size = sizeof(Inode);
    SB->s_block_size = sizeof(FileBlock);

    //SuperBlock StartByte
    int Z = Omni->PartBatch_inUse->StartByte;

    //Inode BitMap StartByte
    int Y = Z + sizeof(SuperBlock) + sizeof(Journaling);

    //Block BitMap StartByte
    int X = Y + iN;

    //Inode StartByte
    int W = X + (3 * iN);

    //BlockStartByte
    int V = W + (iN * sizeof(Inode));

    SB->s_bm_inode_start = Y;
    SB->s_bm_block_start = X;

    SB->s_inode_start = W;
    SB->s_block_start = V;

    SB->s_first_ino = 0;
    SB->s_first_blo = 0;

    Omni->SBinuse = SB;

    Full_PartFormat();

    UpdateSuperBlock();
}

void Load_Defaut_txt(InfoCatcher* nwInf){

    int iN = Omni->SBinuse->s_inodes_count;
    Inode* rt = newInode();

    rt->i_uid  = 1;
    rt->i_gid  = 1;
    rt->i_size = 0;

    //strcpy(rt->i_atime,getDateTime());
    //strcpy(rt->i_ctime,getDateTime());
    //strcpy(rt->i_mtime,getDateTime());

    //Folder = 0
    rt->i_type = 0;
    rt->i_perm = 664;

    //BitMap
    int InodeID = getFirst_InodeBit_Free();
    int BlockID = getFirst_BlockBit_Free();

    //Linking
    rt->i_block[0] = BlockID;

    //I N O D E >>> Writing
    InodeID = getFirst_InodeBit_Free();
    int StartByte = Omni->SBinuse->s_inode_start + (InodeID * sizeof(Inode));
    BinWrite_Struct(rt,InodeID,"Inode");


    //(^< ............ ............ ............ ............ ............

    FolderBlock* Fold_B = newFolderBlock();
    
    //Father Set
    strcpy(Fold_B->b_content[0].b_name,"iNodeFather");
    Fold_B->b_content[0].b_inodo = 0;
    
    //Current Set
    strcpy(Fold_B->b_content[1].b_name,"iNodeCurent");
    Fold_B->b_content[1].b_inodo = 0;

    //Name Set
    //strcpy(Fold_B->b_content[2].b_name,"users.txt");
    //Fold_B->b_content[2].b_inodo = 1;
    
    //B L O C K >>> Writing
    BlockID = getFirst_BlockBit_Free();
    StartByte = Omni->SBinuse->s_block_start + (BlockID * sizeof(FolderBlock));
    BinWrite_Struct(Fold_B,BlockID,"FolderBlock");

    //(^< ............ ............ SuperBlock UPDATE
    InodeID = getFirst_InodeBit_Free();
    BlockID = getFirst_BlockBit_Free();
    Omni->SBinuse->s_first_ino = InodeID;
    Omni->SBinuse->s_first_blo = BlockID;
    Omni->SBinuse->s_free_inodes_count = FreeInodeCounting(iN);
    Omni->SBinuse->s_free_blocks_count = FreeBlockCounting(iN);
    UpdateSuperBlock();

    //(^< ............ ............ Default txt
    nwInf->_path   = newString("/users.txt");
    nwInf->txtData = newString("1,g,root\n1,U,root,root,123\n");
    make_newFile(nwInf);

}

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ L O G I N
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............


int EraseFile(char* FileName){
    SeekInfo* nwSI = CompleteSeeker(0,FileName);

    if(nwSI != NULL){
        int iNode_Bit_ID = nwSI->iNode_Bit_ID;
        Inode* iNode = (Inode*)BinLoad_Str(iNode_Bit_ID,"Inode");

        int i = 0;
        while(i < 12){
            int tFileB_Bit_ID = iNode->i_block[i];
            if(tFileB_Bit_ID > -1){
                BinWriteUpdate_BlockBit('0',tFileB_Bit_ID);
                iNode->i_block[i] = -1;
            }
            i++;
        }

        while(i < 15){
            int tPB_Bit_ID = iNode->i_block[i];
            if(tPB_Bit_ID > -1){
                PointerBlock* tPB = (PointerBlock*)BinLoad_Str(tPB_Bit_ID,"PointerBlock");
                int j = 0;
                while(j < 16){
                    int tFileB_Bit_ID = tPB->b_pointers[j];
                    if(tFileB_Bit_ID > -1){
                        BinWriteUpdate_BlockBit('0',tFileB_Bit_ID);
                        tPB->b_pointers[j] = -1;
                    }
                    j++;
                }
                BinWriteUpdate_BlockBit('0',tPB_Bit_ID);
                iNode->i_block[i] = -1;
            }
            i++;
        }
        BinWriteUpdate_InodeBit('0',iNode_Bit_ID);

        FolderBlock* tFoldB = (FolderBlock*)BinLoad_Str(nwSI->FB_Bit_ID,"FolderBlock");
        //strcpy(tFoldB->b_content[nwSI->FB_Index].b_name,New_Char_Init(12));
        memset(tFoldB->b_content[nwSI->FB_Index].b_name,'\0 ',11);
        tFoldB->b_content[nwSI->FB_Index].b_inodo = -1;

        BinWrite_Struct(tFoldB,nwSI->FB_Bit_ID,"FolderBlock");
        return 1;
    }
    return -1;
}

char* ReadFile(char* FileName){

    SeekInfo* nwSI = CompleteSeeker(0,FileName);

    if(nwSI == NULL){
        return NULL;
    }

    int iNode_Bit_ID = nwSI->iNode_Bit_ID;
    Inode* tInode = (Inode*)BinLoad_Str(iNode_Bit_ID,"Inode");
    char*  tmp = NULL;

    int cnt = 0;
    while(cnt < 12){
        if(tInode->i_block[cnt] > -1){
            
            int tFB_Bit_ID = tInode->i_block[cnt];
            FileBlock* tFB = (FileBlock*)BinLoad_Str(tFB_Bit_ID,"FileBlock");
            if(cnt == 0){
                tmp = tFB->b_content;
            }
            else{
                tmp = Concat_Izq_with_Der(tmp,&tFB->b_content[0],'s','s');  
            }
        }
        cnt++;
    }

    while(cnt < 15){
        if(tInode->i_block[cnt] > 0){
            int tPB_Bit_ID = tInode->i_block[cnt];
            PointerBlock* tPB = (PointerBlock*)BinLoad_Str(tPB_Bit_ID,"PointerBlock");
            int z = 0;
            while (z < 16){
                if(tPB->b_pointers[z] > -1){
                    int tFB_Bit_ID = tPB->b_pointers[z];
                    FileBlock* tFB = (FileBlock*)BinLoad_Str(tFB_Bit_ID,"FileBlock");

                    tmp = Concat_Izq_with_Der(tmp,&tFB->b_content[0],'s','s'); 
                }
                z++;
            }
        }
        cnt++;
    }

    int Lm = tInode->i_size;
    tmp[Lm - 1] = '\0';
    return tmp;
}

int EditFile(char* FileName,char* txtNewContent){
    SeekInfo* nwSI = CompleteSeeker(0,FileName);
    if(nwSI != NULL){
        EraseFile(FileName);
        allocate_newFile(nwSI->iNodeFather_Bit_ID,FileName,txtNewContent,664,1,1);
        return 1;
    }
    return -1;
}

DoublyGenericList* getGroupsList(){

    char* txt = ReadFile("users.txt");

    DoublyGenericList* grpList = new_DoublyGenericList();
    GroupUserInfo* gu = newGrus();
    
    char* tmp = newString(txt);
    char* token = strtok(tmp, ",\n");
    char* Prev = "0";
    int iG = 0;
    while(token != NULL ) {
        
        if(strcasecmp(token,"g") == 0){
            gu->ID = atoi(Prev);
            gu->Type = 'g';
            gu->GrpName = strtok(NULL, ",\n");
            EnQueue(grpList,gu);
            gu = newGrus();
            int ews = 0;
        }
        Prev = newString(token);
        token = strtok(NULL, ",\n");
    }
    
    if(grpList->Length > 0){
        return grpList;
    }
    
    return NULL;
}

DoublyGenericList* getUsersList(){

    char* txt = ReadFile("users.txt");

    DoublyGenericList* usrList = new_DoublyGenericList();
    GroupUserInfo* gu = newGrus();
    
    char* tmp = newString(txt);
    char* token = strtok(tmp, ",\n");
    char* Prev = "0";
    int iG = 0;
    while(token != NULL ) {
        
        if(strcasecmp(token,"u") == 0){
            gu->ID = atoi(Prev);
            gu->Type = 'u';
            gu->GrpName = strtok(NULL, ",\n");
            gu->UsrName = strtok(NULL, ",\n");
            gu->Password = strtok(NULL, ",\n");
            EnQueue(usrList,gu);
            gu = newGrus();
            int ews = 0;
        }
        Prev = newString(token);
        token = strtok(NULL, ",\n");
    }
    
    if(usrList->Length > 0){
        return usrList;
    }
    
    return NULL;
}

char* UsersList_to_String(DoublyGenericList*  usrList){
    char* tmp = newString(25);
    int cnt = 0;
    if(usrList->Length > 0){
        while(usrList->Length > 0){
            GroupUserInfo* gu = (GroupUserInfo*)DeQueue(usrList);
            tmp = Concat_Izq_with_Der(tmp,&gu->ID,'s','i');
            tmp = Concat_Izq_with_Der(tmp,newString(",u,"),'s','s');
            tmp = Concat_Izq_with_Der(tmp,gu->GrpName,'s','s');
            tmp = Concat_Izq_with_Der(tmp,newString(","),'s','s');
            tmp = Concat_Izq_with_Der(tmp,gu->UsrName,'s','s');
            tmp = Concat_Izq_with_Der(tmp,newString(","),'s','s');
            tmp = Concat_Izq_with_Der(tmp,gu->Password,'s','s');
            tmp = Concat_Izq_with_Der(tmp,newString("\n"),'s','s');
            cnt++;
        }
        return tmp;
        
    }
    return NULL;
}

int usrExists(char* Name,DoublyGenericList* Lst){
    int Lm = Lst->Length;
    int cnt = 0;
    while(cnt < Lm){
        GroupUserInfo* tmp = (GroupUserInfo*)getNodebyIndex(Lst,cnt)->Dt;
        if(strcasecmp(Name,tmp->UsrName) == 0){
            return cnt;
        }
        cnt++;
    }
    
    return -1;
}

int grpExists(char* Name,DoublyGenericList* Lst){
    int Lm = Lst->Length;
    int cnt = 0;
    while(cnt < Lm){
        GroupUserInfo* tmp = (GroupUserInfo*)getNodebyIndex(Lst,cnt)->Dt;
        if(strcasecmp(Name,tmp->GrpName) == 0){
            return cnt;
        }
        cnt++;
    }
    
    return -1;
}

char* GroupsList_to_String(DoublyGenericList* grpList){
    char* tmp = newString(25);
    int cnt = 0;
    if(grpList->Length > 0){
        while(grpList->Length > 0){
            GroupUserInfo* gu = (GroupUserInfo*)DeQueue(grpList);
            tmp = Concat_Izq_with_Der(tmp,&gu->ID,'s','i');
            tmp = Concat_Izq_with_Der(tmp,newString(",g,"),'s','s');
            tmp = Concat_Izq_with_Der(tmp,gu->GrpName,'s','s');
            tmp = Concat_Izq_with_Der(tmp,newString("\n"),'s','s');
            cnt++;
        }
        return tmp;
        
    }
    return NULL;
}

GroupUserInfo* getUSR_by_ID(int ID){

    DoublyGenericList*  Lst = getUsersList();
    int cnt = 0;
    while(cnt < Lst->Length){
        GroupUserInfo* gu = (GroupUserInfo*)getNodebyIndex(Lst,cnt)->Dt;
        if(gu->ID == ID){
            return gu;
        }
        cnt++;
    }

    return NULL;
}

GroupUserInfo* getGRP_by_ID(int ID){

    DoublyGenericList*  Lst = getGroupsList();

    int cnt = 0;
    while(cnt < Lst->Length){
        GroupUserInfo* gu = (GroupUserInfo*)getNodebyIndex(Lst,cnt)->Dt;
        if(gu->ID == ID){
            return gu;
        }
        cnt++;
    }

    return NULL;
}

GroupUserInfo* getUSR_by_Name(char* Name,DoublyGenericList* Lst){

    int cnt = 0;
    while(cnt < Lst->Length){
        GroupUserInfo* gu = (GroupUserInfo*)getNodebyIndex(Lst,cnt)->Dt;
        if(strcasecmp(Name,gu->UsrName) == 0){
            return gu;
        }
        cnt++;
    }

    return NULL;
}

GroupUserInfo* getGRP_by_Name(char* Name,DoublyGenericList* Lst){

    int cnt = 0;
    while(cnt < Lst->Length){
        GroupUserInfo* gu = (GroupUserInfo*)getNodebyIndex(Lst,cnt)->Dt;
        if(strcasecmp(Name,gu->GrpName) == 0){
            return gu;
        }
        cnt++;
    }

    return NULL;
}
void txtUsers_Update(DoublyGenericList*  grpList,DoublyGenericList*  usrList){
    char* tmp   = GroupsList_to_String(grpList);
    char* tmp_2 = UsersList_to_String(usrList);
    char* tmp_3 = Concat_Izq_with_Der(tmp,tmp_2,'s','s');
    EditFile("users.txt",tmp_3);
    //Write_txtFile("/home/wrm/Desktop/users_view.txt",tmp_3);
}







#endif // BELONG_H
