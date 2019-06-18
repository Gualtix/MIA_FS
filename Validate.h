#ifndef VALIDATE_H
#define VALIDATE_H

#include "Container.h"
#include "Belong.h"

void ErrorPrinter(char* CMD,char* MsgType,char* ParamName,char* ParamValue,char* Msg){
    printf("\n");
    printf("%s %s: Parametro: %s   Valor:   -> %s <-   %s\n",CMD,MsgType,ParamName,ParamValue,Msg);
}

int _id_Val(char* _id){
    //(^< ............ ............ ............ ............ ............ -id: Mandatory
    if(_id ==  NULL){
        ErrorPrinter("MKFS","ERROR","-id","NULL","Es Obligatorio");
        return 1;
    }
    else{
        Mounted_Part* mP = getPartMounted_By_vID(_id);
        if(mP == NULL){
            ErrorPrinter("MKFS","ERROR","-id",_id,"No es una Particion Montada");
            return 1;
        }
    }
    return 0;
}

int ErrorManager(InfoCatcher* nwInf,char* CMD){
    // 0 = No Error
    //MKFS   ****************************************************************************************************** 
    if(strcasecmp(CMD,"MKFS") == 0){
        //(^< ............ ............ ............ ............ ............ -id: Mandatory
        _id_Val(nwInf->_id);
        //(^< ............ ............ ............ ............ ............ -type: Optional
        if(nwInf->_type == NULL){
            ErrorPrinter("MKFS","WARNING","-type","NULL","Usando Default FULL");
            nwInf->_type = newString("full");
        }
        //(^< ............ ............ ............ ............ ............ -fs: Optional
        if(nwInf->_fs == NULL){
            ErrorPrinter("MKFS","WARNING","-fs","NULL","Usando Default EXT3");
            nwInf->_fs = newString("3fs");
            return 0;
        }
    }
    //LOGIN   ***************************************************************************************************** 
    else if(strcasecmp(CMD,"LOGIN") == 0){

        if(Omni->LoggedUser != NULL){
            printf("\n");
            printf("LOGIN ERROR: Ya existe una Sesion Iniciada...\n");
            return 1;
        }

        //(^< ............ ............ ............ ............ ............ -id: Mandatory
        int Rt = _id_Val(nwInf->_id);
        if(Rt == 1){
            return 1;
        }

        //(^< ............ ............ ............ ............ ............ -usr: Mandatory
        if(nwInf->_usr == NULL){
            ErrorPrinter("LOGIN","ERROR","-usf","NULL","Es Obligatorio");
            return 1;
        }

        setOmni(nwInf->_id);

        DoublyGenericList* usrList = getUsersList();
        DoublyGenericList* grpList = getGroupsList();

        GroupUserInfo* gu = getUSR_by_Name(nwInf->_usr,usrList);

        int usr = usrExists(nwInf->_usr,usrList);
        //int grp = grpExists(nwInf->_grp,grpList);
        
        if(usr == -1){
            ErrorPrinter("LOGIN","ERROR","-usr",nwInf->_usr,"El Usuario no Existe");
            return 1;
        }
        
        //(^< ............ ............ ............ ............ ............ -pwd: Mandatory
        if(nwInf->_pwd == NULL){
            ErrorPrinter("LOGIN","ERROR","-pwd","NULL","Es Obligatorio");
            return 1;
        }
        
        if(strcmp(nwInf->_pwd,gu->Password) != 0){
            ErrorPrinter("LOGIN","ERROR","-pwd",nwInf->_pwd,"Password Incorrecto");
            return 1;
        }

        Omni->LoggedUser = gu;
    }
    //LOGOUT   **************************************************************************************************** 
    //MKGRP   ***************************************************************************************************** 
    //RMGRP   ***************************************************************************************************** 
    //MKUSR   ***************************************************************************************************** 
    //RMUSR   ***************************************************************************************************** 
    //CHMOD   ***************************************************************************************************** 
    //CAT   ******************************************************************************************************* 
    //REM   ******************************************************************************************************* 
    //EDIT   ******************************************************************************************************
    //REN   ******************************************************************************************************* 
    //MKDIR   ***************************************************************************************************** 
    //CP   ********************************************************************************************************
    //MV   ********************************************************************************************************
    //FIND   ******************************************************************************************************
    //CHOWN   ***************************************************************************************************** 
    //CHGRP   *****************************************************************************************************
    //MV   ********************************************************************************************************
    //REP   *******************************************************************************************************
    return 0;
}

int Validate_MKFS(InfoCatcher* nwInf){

}

#endif // VALIDATE_H
