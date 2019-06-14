#ifndef CONTAINER2_H
#define CONTAINER2_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SuperBlock{
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    time_t s_mtime;
    time_t s_umtime;
    int s_mnt_count;
    int s_magic;
    int s_inode_size;
    int s_block_size;
    int s_first_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
}SuperBlock;

typedef struct SeekInfo{
    int FB_Bit_ID;
    int FB_Index;
    int iNode_Bit_ID;
    int iCurent_Bit_ID;
    int iNodeFather_Bit_ID;
}SeekInfo;

SeekInfo* newSeekInfo(){
    SeekInfo* nwSI = (SeekInfo*)malloc(sizeof(SeekInfo));

    nwSI->FB_Bit_ID = -1;
    nwSI->FB_Index  = -1;
    nwSI->iCurent_Bit_ID = -1;
    nwSI->iNodeFather_Bit_ID = -1;

    return nwSI;
}



typedef struct Inode{
    int i_uid;
    int i_gid;
    int i_size;
    time_t i_atime;
    time_t i_ctime;
    time_t i_mtime;
    int i_block [15];
    int i_type;
    int i_perm;

}Inode;

Inode* newInode(){

    Inode* Tmp = (Inode*)malloc(sizeof(Inode));

    Tmp->i_uid   = -1;
    Tmp->i_gid   = -1;
    Tmp->i_size  = -1;
    Tmp->i_atime = -1;
    Tmp->i_ctime = -1;
    Tmp->i_mtime = -1;

    int cnt = 0;
    while(cnt < 15){
        Tmp->i_block[cnt] = -1;
        cnt++;
    }
    
    Tmp->i_type  = -1;
    Tmp->i_perm  = -1;

    return Tmp;
}



typedef struct Content
{
    char b_name [12];
    int  b_inodo;
}Content;

Content newContent(){
    Content Cnt;
    Cnt.b_inodo = - 1;

    int i = 0;
    while(i < 12){
        Cnt.b_name[i] = '\0';
        i++;
    }
    return Cnt;    
}





typedef struct FolderBlock{
    Content b_content [4];
}FolderBlock;

FolderBlock* newFolderBlock(){
    FolderBlock* Fb = (FolderBlock*)malloc(sizeof(FolderBlock));
    int cnt = 0;
    while(cnt < 4){
        Fb->b_content[cnt] = newContent();
        cnt++;
    }
    return Fb;
}






typedef struct FileBlock{
    char b_content [64];
}FileBlock;


FileBlock* newFileBlock(){
    FileBlock* Tmp = (FileBlock*)malloc(sizeof(FileBlock));
    return Tmp;
}




typedef struct PointerBlock{
    int b_pointers [16];
}PointerBlock;

PointerBlock* newPointerBlock(){
    PointerBlock* Pb = (PointerBlock*)malloc(sizeof(PointerBlock));
    int cnt = 0;
    while(cnt < 16){
        Pb->b_pointers[cnt] = -1;
        cnt++;
    }
    return Pb;
}

#endif // CONTAINER2_H
