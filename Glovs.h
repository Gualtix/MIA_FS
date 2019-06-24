#ifndef GLOVS_H
#define GLOVS_H

#include "Container.h"
#include "Container2.h"

typedef struct GLS{
    char*          CompletePathDir_of_Disk_inUse;
    SuperBlock*    SBinuse;
    Batch*         PartBatch_inUse;
    GroupUserInfo* LoggedUser;
}GLS;

GLS* Omni;
char* COMMS;

GLS* newGLS(){
    
    GLS* Omni = (GLS*)malloc(sizeof(GLS));

    Omni->CompletePathDir_of_Disk_inUse = NULL;
    Omni->SBinuse = NULL;
    Omni->PartBatch_inUse = NULL;
    Omni->LoggedUser = NULL;

    return Omni;
}





#endif // GLOVS_H




