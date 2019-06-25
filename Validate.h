#ifndef VALIDATE_H
#define VALIDATE_H

#include "Container.h"
#include "Belong.h"

void ErrorPrinter(char* CMD,char* MsgType,char* ParamName,char* ParamValue,char* Msg){
    printf("\n");
    printf("%s %s: Parametro: %s   Valor:   -> %s <-   %s\n",CMD,MsgType,ParamName,ParamValue,Msg);
}

int _id_Val(char* CMD,char* _id){
    //(^< ............ ............ ............ ............ ............ -id: Mandatory
    if(_id ==  NULL){
        ErrorPrinter(CMD,"ERROR","-id","NULL","Es Obligatorio");
        return 1;
    }
    else{
        Mounted_Part* mP = getPartMounted_By_vID(_id);
        if(mP == NULL){
            ErrorPrinter(CMD,"ERROR","-id",_id,"No es una Particion Montada");
            return 1;
        }
    }
    return 0;
}


/*
int Check_If_FullPath_Exists(char* Path){
    DoublyGenericList* PathList = PathSeparate(Path);
    int i = 0;
    int ptr = 0;
    while (i < PathList->Length){
        char* tmp = (char*)(getNodebyIndex(PathList,i)->Dt);
        SeekInfo* sk = CompleteSeeker(ptr,tmp);
        int isFile = Check_If_Is_txtFile(tmp);
        int isTrueFalse = (strcasecmp(tmp,"true") == 0 || strcasecmp(tmp,"false") == 0);
        if(sk == NULL && isTrueFalse == 0){
            return 0;
        }
        else{
            ptr = sk->iNode_Bit_ID;
        }
        i++;
    }
    return 1;
}
*/

int ErrorManager(InfoCatcher* nwInf,char* CMD){
    // 0 = No Error
    //MKFS   ****************************************************************************************************** 
    if(strcasecmp(CMD,"MKFS") == 0){
        //(^< ............ ............ ............ ............ ............ -id: Mandatory
        if(_id_Val("MKFS",nwInf->_id) == 1) return 1;
        
        //(^< ............ ............ ............ ............ ............ -type: Optional
        if(nwInf->_type == NULL){
            if(isRecovery == 0){
                ErrorPrinter("MKFS","WARNING","-type","NULL","Usando Default FULL");
            }
            
            nwInf->_type = newString("full");
        }

        //(^< ............ ............ ............ ............ ............ -fs: Optional
        if(nwInf->_fs == NULL){
            if(isRecovery == 0){
                ErrorPrinter("MKFS","WARNING","-fs","NULL","Usando Default EXT3");
            }
            nwInf->_fs = newString("3fs");
        }
        return 0;
    }
    //LOGIN   ***************************************************************************************************** 
    else if(strcasecmp(CMD,"LOGIN") == 0){

        if(Omni->LoggedUser != NULL){
            printf("\n");
            printf("LOGIN ERROR: Ya existe una Sesion Iniciada...\n");
            return 1;
        }

        //(^< ............ ............ ............ ............ ............ -id: Mandatory
        if(_id_Val("LOGIN",nwInf->_id) == 1) return 1;

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
        return 0;
    } 
    //MKGRP   ***************************************************************************************************** 
    else if(strcasecmp(CMD,"MKGRP") == 0){
    
        //(^< ............ ............ ............ ............ ............ root use only
        if (strcasecmp(Omni->LoggedUser->UsrName,"root") != 0){
            ErrorPrinter("MKGRP","ERROR","Usuario",Omni->LoggedUser->UsrName,"Solo root puede usar MKGRP");
            return 1;
        }
    
        //(^< ............ ............ ............ ............ ............ -name: Mandatory
        if(nwInf->_name == NULL){
            ErrorPrinter("MKGRP","ERROR","-name","NULL,","Es Obligatorio");
            return 1;
        }

        //(^< ............ ............ ............ ............ ............ -name: Unique
        DoublyGenericList*  grpList = getGroupsList();
        GroupUserInfo* gu = newGrus();
        int grpEx = grpExists(nwInf->_name,grpList);
        if(grpEx > -1){
            ErrorPrinter("MKGRP","ERROR","-name",nwInf->_name,"Este Grupo ya Existe");
            return 1;
        }

        return 0;
    }
    //RMGRP   ***************************************************************************************************** 
    else if(strcasecmp(CMD,"RMGRP") == 0){
        //(^< ............ ............ ............ ............ ............ root use only
        if (strcasecmp(Omni->LoggedUser->UsrName,"root") != 0){
            ErrorPrinter("RMGRP","ERROR","Usuario",Omni->LoggedUser->UsrName,"Solo root puede usar MKGRP");
            return 1;
        }

        //(^< ............ ............ ............ ............ ............ -name: Mandatory
        if(nwInf->_name == NULL){
            ErrorPrinter("RMGRP","ERROR","-name","NULL,","Es Obligatorio");
            return 1;
        }

        //(^< ............ ............ ............ ............ ............ -name: Non-existent
        DoublyGenericList*  grpList = getGroupsList();
        GroupUserInfo* gu = newGrus();
        int grpEx = grpExists(nwInf->_name,grpList);

        if(grpEx == -1){
            ErrorPrinter("RMGRP","ERROR","-name",nwInf->_name,"Este Grupo No Existe");
            return 1;
        }

        return 0;
    }
    //MKUSR   ***************************************************************************************************** 
    else if(strcasecmp(CMD,"MKUSR") == 0){
        //(^< ............ ............ ............ ............ ............ root use only
        if (strcasecmp(Omni->LoggedUser->UsrName,"root") != 0){
            ErrorPrinter("MKUSR","ERROR","Usuario",Omni->LoggedUser->UsrName,"Solo root puede usar MKUSR");
            return 1;
        }

        //(^< ............ ............ ............ ............ ............ -usr: Mandatory
        if(nwInf->_usr == NULL){
            ErrorPrinter("MKUSR","ERROR","-usr","NULL,","Es Obligatorio");
            return 1;
        }

        //(^< ............ ............ ............ ............ ............ -pwd: Mandatory
        if(nwInf->_pwd == NULL){
            ErrorPrinter("MKUSR","ERROR","-pwd","NULL","Es Obligatorio");
            return 1;
        }

        //(^< ............ ............ ............ ............ ............ -grp: Mandatory
        if(nwInf->_grp == NULL){
            ErrorPrinter("MKUSR","ERROR","-grp","NULL,","Es Obligatorio");
            return 1;
        }

        //(^< ............ ............ ............ ............ ............ -usr: Unique
        DoublyGenericList*  usrList = getUsersList();
        GroupUserInfo* gu = newGrus();
        int usrEx = usrExists(nwInf->_usr,usrList);

        if(usrEx > -1){
            ErrorPrinter("MKUSR","ERROR","-usr",nwInf->_usr,"Este Usuario ya Existe");
            return 1;
        }

        return 0;

    }
    //RMUSR   ***************************************************************************************************** 
    else if(strcasecmp(CMD,"RMUSR") == 0){
        //(^< ............ ............ ............ ............ ............ root use only
        if (strcasecmp(Omni->LoggedUser->UsrName,"root") != 0){
            ErrorPrinter("RMUSR","ERROR","Usuario",Omni->LoggedUser->UsrName,"Solo root puede usar RMUSR");
            return 1;
        }

        //(^< ............ ............ ............ ............ ............ -usr: Mandatory
        if(nwInf->_usr == NULL){
            ErrorPrinter("RMUSR","ERROR","-usr","NULL,","Es Obligatorio");
            return 1;
        }

        //(^< ............ ............ ............ ............ ............ -usr: Non-existent
        DoublyGenericList*  usrList = getUsersList();
        GroupUserInfo* gu = newGrus();
        int usrEx = usrExists(nwInf->_usr,usrList);

        if(usrEx == -1){
            ErrorPrinter("RMUSR","ERROR","-usr",nwInf->_usr,"Este Usuario No Existe");
            return 1;
        }

        return 0;
    }

    //CHMOD   ***************************************************************************************************** 
    //CAT   ******************************************************************************************************* 
    //REM   ******************************************************************************************************* 
    else if(strcasecmp(CMD,"REM") == 0){
        //(^< ............ ............ ............ ............ ............ -path: Mandatory
        if(nwInf->_path == NULL){
            ErrorPrinter("REM","ERROR","-path","NULL","Es Obligatorio");
            return 1;
        }
        //(^< ............ ............ ............ ............ ............ -path: Unique
        DoublyGenericList* Ph = PathSeparate(nwInf->_path);
        Pop(Ph);
        char* tmp = (char*)Pop(Ph);
        int  istxt = Check_If_Is_txtFile(tmp);
        SeekInfo* SF = CompleteSeeker(0,tmp);
        if(SF == NULL){
            if(istxt == 1){
                ErrorPrinter("REM","ERROR","-path",tmp,"El Archivo No Existe");
                return 1;
            }
            else{
                ErrorPrinter("REM","ERROR","-path",tmp,"El Folder No Existe");
                return 1;
            }
        }
        return 0;
    }
    //EDIT   ******************************************************************************************************
    //REN   *******************************************************************************************************
    //MKFILE   ****************************************************************************************************
    else if(strcasecmp(CMD,"MKFILE") == 0){
        //(^< ............ ............ ............ ............ ............ -path: Mandatory
        if(nwInf->_path == NULL){
            ErrorPrinter("MKFILE","ERROR","-path","NULL","Es Obligatorio");
            return 1;
        }
        char* FileName = Path_Get_FileName(nwInf->_path);

        //(^< ............ ............ ............ ............ ............ -cont: Optional
        if(nwInf->_cont != NULL){
            char* txtData = getString_from_File(nwInf->_cont);
            if(txtData == NULL){
                ErrorPrinter("MKFILE","ERROR","-cont",nwInf->_cont,"Archivo No Encontrado");
                return 1;
            }
            else{
                nwInf->txtData = txtData;
                return 0;
            }
        }

        //(^< ............ ............ ............ ............ ............ -size: Optional
        if(nwInf->_size <= 0){
            ErrorPrinter("MKFILE","ERROR","-size","Menor a Cero","Valor Invalido");
            return 1;
        }
        else{
            char* txtData = getDefault_txtContent(nwInf->_size);
            nwInf->txtData = txtData;
            return 0;
        }
    } 
    //MKDIR   *****************************************************************************************************
    else if(strcasecmp(CMD,"MKDIR") == 0){
        //(^< ............ ............ ............ ............ ............ -path: Unique
        char* nwFolderName = Path_get_Last_FolderName(nwInf->_path);
        SeekInfo* SF = CompleteSeeker(0,nwFolderName);
        if(SF != NULL){
            ErrorPrinter("MKDIR","ERROR","-path",nwFolderName,"Ya existe un Directorio con ese Nombre");
            return 1;
        }
        //(^< ............ ............ ............ ............ ............ -path: Mandatory
        if(nwInf->_path == NULL){
            ErrorPrinter("MKDIR","ERROR","-path","NULL","Es Obligatorio");
            return 1;
        }
        return 0;
    }
    //CP   ********************************************************************************************************
    //LOSS   ******************************************************************************************************
    else if(strcasecmp(CMD,"LOSS") == 0){
        //(^< ............ ............ ............ ............ ............ -id: Mandatory
        if(_id_Val("LOSS",nwInf->_id) == 1) return 1;
        return 0;
    }

    //RECOVERY   **************************************************************************************************
    else if(strcasecmp(CMD,"RECOVERY") == 0){
        //(^< ............ ............ ............ ............ ............ -id: Mandatory
        if(_id_Val("RECOVERY",nwInf->_id) == 1) return 1;
        return 0;
        }
    //FIND   ******************************************************************************************************
    //CHOWN   ***************************************************************************************************** 
    //CHGRP   *****************************************************************************************************
    //MV   ********************************************************************************************************
    else if(strcasecmp(CMD,"MV") == 0){

   
        //(^< ............ ............ ............ ............ ............ -path: Mandatory
        if(nwInf->_path == NULL){
            ErrorPrinter("MV","ERROR","-path","NULL","Es Obligatorio");
            return 1;
        }
        //(^< ............ ............ ............ ............ ............ -dest: Mandatory
        if(nwInf->_dest == NULL){
            ErrorPrinter("MV","ERROR","-dest","NULL","Es Obligatorio");
            return 1;
        }

        DoublyGenericList* Ph = PathSeparate(nwInf->_path);
        Pop(Ph);
        char* OrgName = (char*)Pop(Ph);
        int  istxt    = Check_If_Is_txtFile(OrgName);

        Ph = PathSeparate(nwInf->_dest);
        Pop(Ph);
        char* DestName = (char*)Pop(Ph);

        SeekInfo* Origin = CompleteSeeker(0,OrgName);
        SeekInfo* Dest   = CompleteSeeker(0,DestName);
        
        if(Origin == NULL){
            if(istxt == 1){
                ErrorPrinter("MV","ERROR","-ruta",OrgName,"El Archivo Origen No Existe");
                return 1;
            }
            else{
                ErrorPrinter("MV","ERROR","-ruta",OrgName,"El Folder Origen No Existe");
                return 1;
            }
        }
        if(Origin == NULL){
            ErrorPrinter("MV","ERROR","-ruta",DestName,"El Folder Destino No Existe");
            return 1;
        }
        return 0;
        
    }
    //REP   *******************************************************************************************************
    else if(strcasecmp(CMD,"REP") == 0){

        //(^< ............ ............ ............ ............ ............ -id: Mandatory
        if(_id_Val("REP",nwInf->_id) == 1) return 1;

        //(^< ............ ............ ............ ............ ............ -name: Mandatory
        if(nwInf->_name == NULL){
            ErrorPrinter("REP","ERROR","-name","NULL","Es Obligatorio");
            return 1;
        }

        //(^< ............ ............ ............ ............ ............ -path: Mandatory
        if(nwInf->_path == NULL){
            ErrorPrinter("REP","ERROR","-path","NULL","Es Obligatorio");
            return 1;
        }
        //(^< ............ ............ ............ ............ ............ -ruta: Mandatory
        if(strcasecmp(nwInf->_name,"ls") != 0 && strcasecmp(nwInf->_name,"file") != 0){
            return 0;
        }
        if(nwInf->_ruta == NULL && strcasecmp(nwInf->_name,"mbr") != 0 && strcasecmp(nwInf->_name,"disk") != 0){
            ErrorPrinter("REP","ERROR","-ruta","NULL","Es Obligatorio");
            return 1;
        }

        if(strcasecmp(nwInf->_name,"file") == 0){
            char* FileName = Path_get_Last_FolderName(nwInf->_ruta);
            setOmni(nwInf->_id);
            SeekInfo* SF = CompleteSeeker(0,FileName);
            Omni = newGLS();
            if(SF == NULL){
                ErrorPrinter("REP","ERROR","-ruta",FileName,"El Archivo No Existe");
                return 1;
            }
        }

        if(strcasecmp(nwInf->_name,"ls") == 0){

            DoublyGenericList* Ph = PathSeparate(nwInf->_ruta);
            Pop(Ph);
            char* tmp = (char*)Pop(Ph);
            int  istxt = Check_If_Is_txtFile(tmp);
            setOmni(nwInf->_id);
            SeekInfo* SF = CompleteSeeker(0,tmp);
            Omni = newGLS();
            if(SF == NULL){
                if(istxt == 1){
                    ErrorPrinter("REP","ERROR","-ruta",tmp,"El Archivo No Existe");
                    return 1;
                }
                else{
                    ErrorPrinter("REP","ERROR","-ruta",tmp,"El Folder No Existe");
                    return 1;
                }
            }
            return 0;
        }
    }
    return 0;
}



#endif // VALIDATE_H
