#ifndef CONTAINER_H
#define CONTAINER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
