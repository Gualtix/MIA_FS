#ifndef F2_REP
#define F2_REP

#ifndef F2REPTS_H
#define F2REPTS_H

#include "../Fw/Helper.h"
#include "../Fw/StringHandler.h"

#include "../Glovs.h"
#include "../Analizer.h"
#include "../Belong.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



void AddSuper(FILE* DotFl,char* Color,char* TopTittle,int ColSpan){
    fprintf(DotFl,"\t\t\t\t\t\t<TR> <TD BGCOLOR = \"%s\" width = \"150\" colspan = \"%d\">- %s -<br/></TD></TR>\n",Color,ColSpan,TopTittle);
}

void AddRow(FILE* DotFl,char* Type,char* Value){
    fprintf(DotFl,"\t\t\t\t\t\t<TR><TD>%s</TD><TD>%s</TD></TR>\n",Type,Value);
}

void SuperBlockDot(FILE* DotFl){

    char* tS = newString(20);
    
    fprintf(DotFl,"\t\t\tSuperBlock\n");
    fprintf(DotFl,"\t\t\t[label =\n");
        fprintf(DotFl,"\t\t\t\t<\n");
            fprintf(DotFl,"\t\t\t\t\t<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">\n");
                AddSuper(DotFl,"#79a6d2","SuperBlock",2);
                AddRow(DotFl,"Name","Value");

                sprintf(tS, "%d",Omni->SBinuse->s_filesystem_type);
                AddRow(DotFl,"s_filesystem_type",tS);
                
                sprintf(tS, "%d",Omni->SBinuse->s_inodes_count);
                AddRow(DotFl,"s_inodes_count",tS);

                sprintf(tS, "%d",Omni->SBinuse->s_blocks_count);
                AddRow(DotFl,"s_blocks_count",tS);

                sprintf(tS, "%d",Omni->SBinuse->s_free_blocks_count);
                AddRow(DotFl,"s_free_blocks_count",tS);

                sprintf(tS, "%d",Omni->SBinuse->s_free_inodes_count);
                AddRow(DotFl,"s_free_inodes_count",tS);

                sprintf(tS, "%d",Omni->SBinuse->s_mtime);
                AddRow(DotFl,"s_mtime",tS);

                sprintf(tS, "%d",Omni->SBinuse->s_umtime);
                AddRow(DotFl,"s_umtime",tS);

                sprintf(tS, "%d",Omni->SBinuse->s_mnt_count);
                AddRow(DotFl,"s_mnt_count",tS);

                sprintf(tS, "%d",Omni->SBinuse->s_magic);
                AddRow(DotFl,"s_magic",tS);

                sprintf(tS, "%d",Omni->SBinuse->s_inode_size);
                AddRow(DotFl,"s_inode_size",tS);

                sprintf(tS, "%d",Omni->SBinuse->s_block_size);
                AddRow(DotFl,"s_block_size",tS);

                sprintf(tS, "%d",Omni->SBinuse->s_first_ino);
                AddRow(DotFl,"s_first_ino",tS);

                sprintf(tS, "%d",Omni->SBinuse->s_first_blo);
                AddRow(DotFl,"s_first_blo",tS);

                sprintf(tS, "%d",Omni->SBinuse->s_bm_inode_start);
                AddRow(DotFl,"s_bm_inode_start",tS);

                sprintf(tS, "%d",Omni->SBinuse->s_bm_block_start);
                AddRow(DotFl,"s_bm_block_start",tS);

                sprintf(tS, "%d",Omni->SBinuse->s_inode_start);
                AddRow(DotFl,"s_inode_start",tS);

                sprintf(tS, "%d",Omni->SBinuse->s_block_start);
                AddRow(DotFl,"s_block_start",tS);
                
            fprintf(DotFl,"\t\t\t\t\t</TABLE>\n");
        fprintf(DotFl,"\t\t\t\t>\n");
        fprintf(DotFl,"\t\t\t]\n");
}

void inodeBitMapDot(FILE* DotFl,int iN){
    
    char* tS = newString(20);
    
    fprintf(DotFl,"\t\tInodeBitMap\n");
    fprintf(DotFl,"\t\t\t[label =\n");
        fprintf(DotFl,"\t\t\t\t<\n");
            fprintf(DotFl,"\t\t\t\t\t<TABLE BGCOLOR = \"#ff8080\" BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">\n");
                AddSuper(DotFl,"#ff3333",":: InodeBitMap ::",20);

                int extra = (iN % 20);
                int nRow =  iN - extra;

                int cnt = 0;
                char ch;
                while(cnt < nRow){

                    if(cnt % 20 == 0 ){
                        fprintf(DotFl,"\t\t\t\t\t\t<TR>\n");
                        fprintf(DotFl,"\t\t\t\t\t\t\t");
                    }

                    ch = BinRead_SingleChar(Omni->SBinuse->s_bm_inode_start + cnt);
                    fprintf(DotFl,"<TD>%c</TD>",ch);

                    if( (cnt + 1) >= 5 && (cnt + 1) % 5 == 0 ){
                        fprintf(DotFl,"\n");
                        fprintf(DotFl,"\t\t\t\t\t\t\t");
                    }

                    if( (cnt + 1) >= 20 && (cnt + 1) % 20 == 0 ){
                        fprintf(DotFl,"</TR>\n");
                    }
                    cnt++;
                }

                if(extra > 0){
                    fprintf(DotFl,"\t\t\t\t\t\t<TR>\n");
                    fprintf(DotFl,"\t\t\t\t\t\t\t");

                    while(cnt < iN){
                        ch = BinRead_SingleChar(Omni->SBinuse->s_bm_inode_start + cnt);
                        fprintf(DotFl,"<TD>%c</TD>",ch);
                        if( (cnt + 1) >= 5 && (cnt + 1) % 5 == 0 ){
                            fprintf(DotFl,"\n");
                            fprintf(DotFl,"\t\t\t\t\t\t\t");
                        }
                        cnt++;
                    }
                    fprintf(DotFl,"\n");
                    fprintf(DotFl,"\t\t\t\t\t\t</TR>\n");

                }

            fprintf(DotFl,"\t\t\t\t\t</TABLE>\n");
        fprintf(DotFl,"\t\t\t\t>\n");
    fprintf(DotFl,"\t\t\t]\n");
}

void blockBitMapDot(FILE* DotFl,int iN){
    char* tS = newString(20);
    
    fprintf(DotFl,"\t\tBlockBitMap\n");
    fprintf(DotFl,"\t\t\t[label =\n");
        fprintf(DotFl,"\t\t\t\t<\n");
            fprintf(DotFl,"\t\t\t\t\t<TABLE BGCOLOR = \"#66ffb3\" BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">\n");
                AddSuper(DotFl,"#00cc66",":: BlockBitMap ::",20);

                int extra = ((iN * 3) % 20);
                int nRow  =  (iN * 3) - extra;

                int cnt = 0;
                char ch;
                while(cnt < nRow){

                    if( (cnt + 20) % 20 == 0 ){
                        fprintf(DotFl,"\t\t\t\t\t\t<TR>\n");
                        fprintf(DotFl,"\t\t\t\t\t\t\t");
                    }

                    ch = BinRead_SingleChar(Omni->SBinuse->s_bm_block_start + cnt);
                    fprintf(DotFl,"<TD>%c</TD>",ch);

                    if( (cnt + 1) >= 5 && (cnt + 1) % 5 == 0 ){
                        fprintf(DotFl,"\n");
                        fprintf(DotFl,"\t\t\t\t\t\t\t");
                    }

                    if( (cnt + 1) >= 20 && (cnt + 1) % 20 == 0 ){
                        fprintf(DotFl,"</TR>\n");
                    }
                    cnt++;
                }

                if(extra > 0){
                    fprintf(DotFl,"\t\t\t\t\t\t<TR>\n");
                    fprintf(DotFl,"\t\t\t\t\t\t\t");

                    while(cnt < (iN * 3)){
                        ch = BinRead_SingleChar(Omni->SBinuse->s_bm_block_start + cnt);
                        fprintf(DotFl,"<TD>%c</TD>",ch);
                        if( (cnt + 1) >= 5 && (cnt + 1) % 5 == 0 ){
                            fprintf(DotFl,"\n");
                            fprintf(DotFl,"\t\t\t\t\t\t\t");
                        }
                        cnt++;
                    }
                    fprintf(DotFl,"\n");
                    fprintf(DotFl,"\t\t\t\t\t\t</TR>\n");
                }

            fprintf(DotFl,"\t\t\t\t\t</TABLE>\n");
        fprintf(DotFl,"\t\t\t\t>\n");
    fprintf(DotFl,"\t\t\t]\n");
}

void AddInode(FILE* DotFl,int StartByte){

    int ID = StartByte_to_InodeBit(StartByte);

    char* tS = newString(20);

    Inode* Tinp = (Inode*)BinLoad_Str(ID,"Inode");

    sprintf(tS, "%d",ID);

    fprintf(DotFl,"\t\tInode_%s\n",tS);
    fprintf(DotFl,"\t\t\t[label =\n");
        fprintf(DotFl,"\t\t\t\t<\n");
            fprintf(DotFl,"\t\t\t\t\t<TABLE BGCOLOR = \"#99c2ff\" BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">\n");
                AddSuper(DotFl,"#0066ff",Concat_Izq_with_Der(newString("Inode: "),tS,'s','s'),2);

                sprintf(tS, "%d",Tinp->i_uid);
                AddRow(DotFl,"i_uid",tS);

                sprintf(tS, "%d",Tinp->i_gid);
                AddRow(DotFl,"i_gid",tS);

                sprintf(tS, "%d",Tinp->i_size);
                AddRow(DotFl,"i_size",tS);

                sprintf(tS, "%d",Tinp->i_atime);
                AddRow(DotFl,"i_atime",tS);

                sprintf(tS, "%d",Tinp->i_ctime);
                AddRow(DotFl,"i_ctime",tS);

                sprintf(tS, "%d",Tinp->i_mtime);
                AddRow(DotFl,"i_mtime",tS);

                sprintf(tS, "%d",Tinp->i_type);
                AddRow(DotFl,"i_type",tS);

                sprintf(tS, "%d",Tinp->i_perm);
                AddRow(DotFl,"i_perm",tS);

                int cnt = 0;
                while(cnt < 12){

                    if(Tinp->i_block[cnt] == -1){
                        cnt++;
                        continue;
                    }
                    sprintf(tS, "%d",Tinp->i_block[cnt]);

                    char Tp[5];
                    sprintf(Tp, "%d",cnt);

                    char* Ident = Concat_Izq_with_Der(newString("direct_"),Tp,'s','s');

                    AddRow(DotFl,Ident,tS);
                    cnt++;
                }

                if(Tinp->i_block[12] >= 0){
                    sprintf(tS, "%d",Tinp->i_block[12]);
                    AddRow(DotFl,"indirect_12",tS);
                }

                if(Tinp->i_block[13] >= 0){
                    sprintf(tS, "%d",Tinp->i_block[13]);
                    AddRow(DotFl,"indirect_13",tS);
                }

                if(Tinp->i_block[14] >= 0){
                    sprintf(tS, "%d",Tinp->i_block[14]);
                    AddRow(DotFl,"indirect_14",tS);
                }

                fprintf(DotFl,"\t\t\t\t\t</TABLE>\n");
        fprintf(DotFl,"\t\t\t\t>\n");
    fprintf(DotFl,"\t\t\t]\n");
}

void AddFileBlock(FILE* DotFl,int StartByte){

    int ID = StartByte_to_BlockBit(StartByte);

    char* tS = newString(20);

    FileBlock* Tmp = (FileBlock*)BinLoad_Str(ID,"FileBlock");

    sprintf(tS, "%d",ID);

    fprintf(DotFl,"\t\tFileBlock_%s\n",tS);

    fprintf(DotFl,"\t\t\t[label =\n");
        fprintf(DotFl,"\t\t\t\t<\n");
            fprintf(DotFl,"\t\t\t\t\t<TABLE BGCOLOR = \"#ffe066\" BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">\n");
                AddSuper(DotFl,"#ffcc00",Concat_Izq_with_Der(newString("FileBlock: "),tS,'s','s'),2);

                char Clone[64];
                memset(Clone,'\0',64);

                int i = 0;
                int index = 0;
                while(i < 64){
                    char w = Tmp->b_content[i];
                    int r = isalnum(w); 
                    if(r > 0 || w == ','){
                        Clone[index] = w;
                        index++;
                    }
                    else{
                        if(w == '\0'){
                            Clone[index] = w;
                            index++;
                            break;
                        }
                        int ad = 0;
                        int sdfas = 5;
                    }
                    i++;
                }
                
                AddRow(DotFl,"b_content",Clone);

            fprintf(DotFl,"\t\t\t\t\t</TABLE>\n");
        fprintf(DotFl,"\t\t\t\t>\n");
    fprintf(DotFl,"\t\t\t]\n");
}

void AddFolderBlock(FILE* DotFl,int StartByte){

    int ID = StartByte_to_BlockBit(StartByte);
    char* tS = newString(20);

    FolderBlock* Tmp = (FolderBlock*)BinLoad_Str(ID,"FolderBlock");

    sprintf(tS, "%d",ID);

    fprintf(DotFl,"\t\tFolderBlock_%s\n",tS);
    fprintf(DotFl,"\t\t\t[label =\n");
        fprintf(DotFl,"\t\t\t\t<\n");
            fprintf(DotFl,"\t\t\t\t\t<TABLE BGCOLOR = \"#ffb380\" BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">\n");
                AddSuper(DotFl,"#ff660",Concat_Izq_with_Der(newString("FileBlock: "),tS,'s','s'),2);

                AddRow(DotFl,"name_0",Tmp->b_content[0].b_name);
                sprintf(tS, "%d",Tmp->b_content[0].b_inodo);
                AddRow(DotFl,"inodePtr_0",tS);

                AddRow(DotFl,"name_1",Tmp->b_content[1].b_name);
                sprintf(tS, "%d",Tmp->b_content[1].b_inodo);
                AddRow(DotFl,"inodePtr_1",tS);

                AddRow(DotFl,"name_2",Tmp->b_content[2].b_name);
                sprintf(tS, "%d",Tmp->b_content[2].b_inodo);
                AddRow(DotFl,"inodePtr_2",tS);

                AddRow(DotFl,"name_3",Tmp->b_content[3].b_name);
                sprintf(tS, "%d",Tmp->b_content[3].b_inodo);
                AddRow(DotFl,"inodePtr_3",tS);
 
                
            fprintf(DotFl,"\t\t\t\t\t</TABLE>\n");
        fprintf(DotFl,"\t\t\t\t>\n");
    fprintf(DotFl,"\t\t\t]\n");
    
}

void AddPointerBlock(FILE* DotFl,int StartByte){
    int ID = StartByte_to_BlockBit(StartByte);
    char* tS = newString(20);

    PointerBlock* Tmp = (PointerBlock*)BinLoad_Str(ID,"PointerBlock");

    sprintf(tS, "%d",ID);

    fprintf(DotFl,"\t\tPointerBlock_%s\n",tS);
    fprintf(DotFl,"\t\t\t[label =\n");
        fprintf(DotFl,"\t\t\t\t<\n");
            fprintf(DotFl,"\t\t\t\t\t<TABLE BGCOLOR = \"#80ff80\" BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">\n");
                AddSuper(DotFl,"#00cc00",Concat_Izq_with_Der(newString("PointerBlock: "),tS,'s','s'),2);

                int cnt = 0;
                while(cnt < 16){
                    sprintf(tS, "%d",Tmp->b_pointers[cnt]);

                    char Tp[5];
                    sprintf(Tp, "%d",cnt);

                    char* Ident = Concat_Izq_with_Der(newString("pointer_"),Tp,'s','s');

                    AddRow(DotFl,Ident,tS);
                    cnt++;
                }

    
            fprintf(DotFl,"\t\t\t\t\t</TABLE>\n");
        fprintf(DotFl,"\t\t\t\t>\n");
    fprintf(DotFl,"\t\t\t];\n");

}

void AddLink(FILE* DotFl,char* Izq,char* Der){
    fprintf(DotFl,"\t\t%s->%s;\n",Izq,Der);
}

char* getGraphStructName(char* Type,int ID){
    char* tS = newString(20);
    sprintf(tS, "%s_%d",Type,ID);
    return tS;
}

void FileSystemTree(FILE* DotFl,int StartByte){

    //Type 0 ---> Folder
    //Type 1 ---> File
    int Father_ID_Bit = StartByte_to_InodeBit(StartByte);
    Inode* Rt = (Inode*)BinLoad_Str(Father_ID_Bit,"Inode");
    AddInode(DotFl,StartByte);

    //Father Link Name
    char* FatherName = getGraphStructName("Inode",Father_ID_Bit);

    int cnt = 0;
    
    //Direct
    while(cnt < 12){
        if(Rt->i_block[cnt] >= 0){

            //Folder Type
            if(Rt->i_type == 0){
                
                int Son_ID_Bit = Rt->i_block[cnt];
                int Son_StartByte = Bit_to_StartByte(Son_ID_Bit,"Block");
                AddFolderBlock(DotFl,Son_StartByte);

                //Son Link Name
                char* SonName = getGraphStructName("FolderBlock",Son_ID_Bit);

                //Linking
                AddLink(DotFl,FatherName,SonName);

                //******* FolderBlock Tour ******
                FolderBlock* Fb = (FolderBlock*)BinLoad_Str(Son_ID_Bit,"FolderBlock");
                int i = 0;
                while(i < 4){

                    char* iName  = newString(Fb->b_content[i].b_name);
                    int Next_ID_Bit = Fb->b_content[i].b_inodo; 
                    int Next_StartByte = Bit_to_StartByte(Next_ID_Bit,"Inode");
                    char* NextName = getGraphStructName("Inode",Next_ID_Bit);

                    if(Next_ID_Bit > 0 && strcasecmp(iName,"iNodeFather") != 0 && strcasecmp(iName,"iNodeCurent")){
                        //Next Linking
                        AddLink(DotFl,SonName,NextName);
                        FileSystemTree(DotFl,Next_StartByte);
                    }
                    i++;
                }
            }
            
            //File Type
            if(Rt->i_type == 1){
                
                int Son_ID_Bit = Rt->i_block[cnt];
                int Son_StartByte = Bit_to_StartByte(Son_ID_Bit,"Block");
                AddFileBlock(DotFl,Son_StartByte);

                //Son Link Name
                char* SonName = getGraphStructName("FileBlock",Son_ID_Bit);

                //Linking
                AddLink(DotFl,FatherName,SonName);
            
            }
        }
        cnt++;
    }

    //Indirect
    while(cnt < 15){

        if(Rt->i_block[cnt] < 0){
            cnt++;
            continue;
        }

        //Folder Type
        if(Rt->i_type == 0){
            int Son_ID_Bit = Rt->i_block[cnt];
            int Son_StartByte = Bit_to_StartByte(Son_ID_Bit,"Block");
            //AddFolderBlock(DotFl,Son_StartByte);
            AddPointerBlock(DotFl,Son_StartByte);

            //Son Link Name
            char* SonName = getGraphStructName("PointerBlock",Son_ID_Bit);

            //Linking
            AddLink(DotFl,FatherName,SonName);

            //******* PointerBlock Tour ******
            PointerBlock* Pb = (PointerBlock*)BinLoad_Str(Son_ID_Bit,"PointerBlock");
            int z = 0;
            while(z < 16){
                int Next_ID_Bit = Pb->b_pointers[z];
                int Next_StartByte = Bit_to_StartByte(Next_ID_Bit,"Block");
                char* NextName = getGraphStructName("FolderBlock",Next_ID_Bit);

                //AddFolderBlock(DotFl,Next_StartByte);
                //Linking
                //AddLink(DotFl,SonName,NextName);
            
                if(Next_ID_Bit > 0){

                    AddFolderBlock(DotFl,Next_StartByte);
                    //Linking
                    AddLink(DotFl,SonName,NextName);
                    
                    //******* FolderBlock Tour ******
                    FolderBlock* Fb = (FolderBlock*)BinLoad_Str(Next_ID_Bit,"FolderBlock");
                    int i = 0;
                    while(i < 4){
                        char* iName  = newString(Fb->b_content[i].b_name);
                        int SuperNext_ID_Bit = Fb->b_content[i].b_inodo; 
                        int SuperNext_StartByte = Bit_to_StartByte(SuperNext_ID_Bit,"Inode");
                        char* SuperNextName = getGraphStructName("Inode",SuperNext_ID_Bit);

                        if(SuperNext_ID_Bit > 0 && strcasecmp(iName,"iNodeFather") != 0 && strcasecmp(iName,"iNodeCurrent")){
                            //Next Linking
                            AddLink(DotFl,NextName,SuperNextName);
                            FileSystemTree(DotFl,SuperNext_StartByte);
                        }
                        i++;
                    }

                }
                z++;
            }
        }

        //File Type
        if(Rt->i_type == 1){
            
            int Son_ID_Bit = Rt->i_block[cnt];
            int Son_StartByte = Bit_to_StartByte(Son_ID_Bit,"Block");
            AddPointerBlock(DotFl,Son_StartByte);

            //Son Link Name
            char* SonName = getGraphStructName("PointerBlock",Son_ID_Bit);

            //Linking
            AddLink(DotFl,FatherName,SonName);

            //******* PointerBlock Tour ******
            PointerBlock* Pb = (PointerBlock*)BinLoad_Str(Son_ID_Bit,"PointerBlock");
            int z = 0;
            while(z < 16){

                int Next_ID_Bit = Pb->b_pointers[z];
                if(Next_ID_Bit > 0){
                    //******* FileBlock Tour ******
                    int Next_StartByte = Bit_to_StartByte(Next_ID_Bit,"Block");
                    char* NextName = getGraphStructName("FileBlock",Next_ID_Bit);
                    AddFileBlock(DotFl,Next_StartByte);

                    //Linking
                    AddLink(DotFl,SonName,NextName);

                    }
                z++;
            }   
        }
        cnt++;
    }


    
    //AddInode(DotFl,5520);
    //AddInode(DotFl,5522+112);
    //AddFolderBlock(DotFl,9104);
    //AddFileBlock(DotFl,9104+64);
    
}

void InodeOnlyDot(FILE* DotFl,int iN){

    int Base = Omni->SBinuse->s_bm_inode_start;
    int cnt = 0;
    char* lsName = NULL;
    while(cnt < iN){
        //int Bit_ID = cnt;
        int StartByte = Bit_to_StartByte(cnt,"Inode");
        char chBit    = BinRead_SingleChar(Omni->SBinuse->s_bm_inode_start + cnt);
        Inode* tmp = (Inode*)BinLoad_Str(cnt,"Inode");
        if(chBit == '1'){
            Inode* tmp = (Inode*)BinLoad_Str(cnt,"Inode");
            AddInode(DotFl,StartByte);
            char* Izq_Name = getGraphStructName("Inode",cnt);
            if(lsName != NULL && (cnt + 1 != iN)){
                AddLink(DotFl,lsName,Izq_Name);
            }
            lsName = Izq_Name;
        }
        cnt++;
    }
    
}

int CheckRank(int N){
    if(N > -2 && N < 1500){
        return 1;
    }
    return 0;
}

int isFolderBlock(int Bit_ID){
    FolderBlock* Folder = (FolderBlock*)BinLoad_Str(Bit_ID,"FolderBlock");

    int cnt = 0;
    while(cnt < 4){
        int tmp = Folder->b_content[cnt].b_inodo;
        if(CheckRank(tmp) == 0){
            return 0;
        }
        cnt++;
    }

    return 1;
}

int isPointerBlock(int Bit_ID){
    PointerBlock* Pointer = (PointerBlock*)BinLoad_Str(Bit_ID,"PointerBlock");

    int cnt = 0;
    while(cnt < 16){
        int tmp = Pointer->b_pointers[cnt];
        if(CheckRank(tmp) == 0){
            return 0;
        }
        cnt++;
    }
    return 1;
}

void BlockOnlyDot(FILE* DotFl,int iN){

    int Base = Omni->SBinuse->s_bm_block_start;
    int cnt = 0;
    char* lsName = NULL;
    while(cnt < (iN * 3)){
        //int Bit_ID = cnt;
        int StartByte = Bit_to_StartByte(cnt,"Block");
        char chBit    = BinRead_SingleChar(Omni->SBinuse->s_bm_block_start + cnt);

        if(chBit == '1'){

            if(cnt == 73){
                int asdf = 584;
            }

            
            if(isPointerBlock(cnt) == 1){

                PointerBlock* tmp = (PointerBlock*)BinLoad_Str(cnt,"PointerBlock");
                AddPointerBlock(DotFl,StartByte);
                char* Izq_Name = getGraphStructName("PointerBlock",cnt);
                if(lsName != NULL && (cnt + 1 != iN)){
                    AddLink(DotFl,lsName,Izq_Name);
                }
                lsName = Izq_Name;
            }
            else if(isFolderBlock(cnt) == 1){

                FolderBlock* tmp = (FolderBlock*)BinLoad_Str(cnt,"FolderBlock");
                AddFolderBlock(DotFl,StartByte);
                char* Izq_Name = getGraphStructName("FolderBlock",cnt);
                if(lsName != NULL && (cnt + 1 != iN)){
                    AddLink(DotFl,lsName,Izq_Name);
                }
                lsName = Izq_Name;

            }
            else{

                FileBlock* tmp = (FileBlock*)BinLoad_Str(cnt,"FileBlock");
                AddFileBlock(DotFl,StartByte);
                char* Izq_Name = getGraphStructName("FileBlock",cnt);
                if(lsName != NULL && (cnt + 1 != iN)){
                    AddLink(DotFl,lsName,Izq_Name);
                }
                lsName = Izq_Name;

            }
        }
        cnt++;
    }
}

//(^< ............ ............ ...........   txt_InodeBitMap
void txt_InodeBitMap(char* CompleteReportPathDir){

    int iN = Calc_iN(Omni->PartBatch_inUse->Size);

    /*
    int iN = -1;
    if(Usr_inUse->SBinuse->s_filesystem_type == 3){
        iN = Usr_inUse->Ext3_iN;
    }
    else{
        iN = Usr_inUse->Ext2_iN;
    }
    */

    int Base = Omni->SBinuse->s_bm_inode_start;
    int cnt = 0;
    int Fp  = 0;
   
    char txtContent[500000];
    memset(txtContent,'\0',500000);
    
    
    while(cnt < iN){

        char chBit = BinRead_SingleChar(Omni->SBinuse->s_bm_inode_start + cnt);
        
        if(cnt != 0 && ((cnt + 1) % 20) == 0){
            txtContent[Fp] = chBit;
            Fp++;
            
            txtContent[Fp] = '\n';
            Fp++;
        }
        else{
        
            txtContent[Fp] = chBit;
            Fp++;

            txtContent[Fp] = ' ';
            Fp++;

            txtContent[Fp] = ' ';
            Fp++;
        
        }
        cnt++;
    }

    char* Ext_ReportName = Path_Get_FileName(CompleteReportPathDir);
    char* Ext_ReportPath = Path_Get_Isolated(CompleteReportPathDir);

    CreatePathDir(Ext_ReportPath);
    Write_txtFile(CompleteReportPathDir,txtContent);
    
}

//(^< ............ ............ ...........   txt_BlockBitMap
void txt_BlockBitMap(char* CompleteReportPathDir){

    int iN = Calc_iN(Omni->PartBatch_inUse->Size);
    /*
    int iN = -1;
    if(Usr_inUse->SBinuse->s_filesystem_type == 3){
        iN = Usr_inUse->Ext3_iN;
    }
    else{
        iN = Usr_inUse->Ext2_iN;
    }
    */

    int Base = Omni->SBinuse->s_bm_block_start;
    int cnt = 0;
    int Fp  = 0;

    int Tot = (3 * iN);
   
    char txtContent[Tot];
    memset(txtContent,'\0',Tot);
    
    while(cnt < (3 * iN)){

        char chBit = BinRead_SingleChar(Omni->SBinuse->s_bm_block_start + cnt);
        
        if(cnt != 0 && ((cnt + 1) % 20) == 0){
            txtContent[Fp] = chBit;
            Fp++;
            
            txtContent[Fp] = '\n';
            Fp++;
        }
        else{
        
            txtContent[Fp] = chBit;
            Fp++;

            txtContent[Fp] = ' ';
            Fp++;

            txtContent[Fp] = ' ';
            Fp++;
        
        }
        cnt++;
    }

    char* Ext_ReportName = Path_Get_FileName(CompleteReportPathDir);
    char* Ext_ReportPath = Path_Get_Isolated(CompleteReportPathDir);

    CreatePathDir(Ext_ReportPath);
    Write_txtFile(CompleteReportPathDir,txtContent);
}

char* int_to_OctalPerm(char tS){
    char* Sp = newString(3);

    if(tS == '0'){
        Sp = newString("---");
    }
    else if(tS == '1'){
        Sp = newString("--x");
    }
    else if(tS == '2'){
        Sp = newString("-w-");
    }
    else if(tS == '3'){
        Sp = newString("-wx");
    }
    else if(tS == '4'){
        Sp = newString("r--");
    }
    else if(tS == '5'){
        Sp = newString("r-x");
    }
    else if(tS == '6'){
        Sp = newString("rw-");
    }
    else if(tS == '7'){
        Sp = newString("rwx");
    }

    return Sp;
}

char* getPermitString(int Perm){
    char* tS = newString(3);
    sprintf(tS,"%d",Perm);

    char* _1 = int_to_OctalPerm(tS[0]);
    char* _2 = int_to_OctalPerm(tS[1]);
    char* _3 = int_to_OctalPerm(tS[2]);

    char* Cmp = Concat_Izq_with_Der(_1,newString(" "),'s','s');
    Cmp = Concat_Izq_with_Der(Cmp,_2,'s','s');
    Cmp = Concat_Izq_with_Der(Cmp,newString(" "),'s','s');
    Cmp = Concat_Izq_with_Der(Cmp,_3,'s','s');

    return Cmp;
}

DoublyGenericList* getFolderContent_Tour(char* Name){
    SeekInfo* iNode = CompleteSeeker(0,Name);
}

void LsRep(char* CompleteReportPathDir,char* Name,char* Type){

    
    DoublyGenericList* iList = new_DoublyGenericList();
    char* Cmp = NULL;

    if(strcasecmp(Type,"File") == 0){
        SeekInfo* nwSI = CompleteSeeker(0,Name);
        if(nwSI == NULL){
            printf("\n");
            printf("LS REP ERROR: Archivo: %s No Encontrado\n",Name);
            return;
        }
        Inode* nwInode = (Inode*)BinLoad_Str(nwSI->iNode_Bit_ID,"Inode");
        Cmp = getPermitString(nwInode->i_perm);
        EnQueue(iList,nwInode);
    }

    if(strcasecmp(Type,"Folder") == 0){
        SeekInfo* nwSI = CompleteSeeker(0,Name);
        if(nwSI == NULL){
            printf("\n");
            printf("LS REP ERROR: Folder: %s No Encontrado\n",Name);
            return;
        }
        Inode* nwInode = (Inode*)BinLoad_Str(nwSI->iNode_Bit_ID,"Inode");
        Cmp = getPermitString(nwInode->i_perm);
        EnQueue(iList,nwInode);
    }

    char* ReportName = Path_Get_FileName(CompleteReportPathDir);
    char* ReportPath = Path_Get_Isolated(CompleteReportPathDir);

    int LsNm = strlen(ReportName);
    ReportName[LsNm - 1] = 't';
    ReportName[LsNm - 2] = 'o';
    ReportName[LsNm - 3] = 'd';

    CreatePathDir(ReportPath);

    CompleteReportPathDir = Concat_Izq_with_Der(ReportPath,ReportName,'s','s');
    
    FILE* DotFl = fopen(CompleteReportPathDir,"w+");
    

    //(^< ............ ............ ...........   D O T   F I L E
    if(DotFl){
        fprintf(DotFl,"digraph LsRep {\n");
        fprintf(DotFl,"\trankdir = LR;\n");
        fprintf(DotFl,"\tnode [shape = plaintext];\n");
        fprintf(DotFl,"\t\tsubgraph cluster_OutLook {\n");
        fprintf(DotFl,"\t\t\tlabel = \"LS :: R E P O R T\";\n");
        fprintf(DotFl,"\t\t\tgraph[style = dotted];\n");

            fprintf(DotFl,"\t\tLs_Report\n");
            fprintf(DotFl,"\t\t\t[label =\n");
                fprintf(DotFl,"\t\t\t\t<\n");
                    fprintf(DotFl,"\t\t\t\t\t<TABLE BGCOLOR = \"#99c2ff\" BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\">\n");
                        
                        AddSuper(DotFl,"#0066ff",Concat_Izq_with_Der(newString("From: "),Name,'s','s'),8);
                        fprintf(DotFl,"\t\t\t\t\t\t<TR><TD>Permision</TD><TD>Owner</TD><TD>Group</TD><TD>Size_in_Bytes</TD><TD>Date</TD><TD>Type</TD><TD>Name</TD></TR>\n");
                        while(iList->Length > 0){
                            
                            Inode* tmp = (Inode*)DeQueue(iList);
                            char* Permision = newString(Cmp);
                            //Grus* Usr = getUSR_by_ID(tmp->i_uid);
                            //Grus* Grp = getGRP_by_ID(tmp->i_gid);
                            //char* Owner = newString(Usr->UsrName);
                            //char* Group = newString(Usr->GrpName);
                            char* Owner = newString("UUUU");
                            char* Group = newString("KKKK");

                            char* Size_in_Bytes = newString(25);
                            sprintf(Size_in_Bytes,"%d",tmp->i_size);

                            char* Date = newString(25);
                            sprintf(Date,"%d",tmp->i_mtime);

                            fprintf(DotFl,"\t\t\t\t\t\t<TR><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD></TR>\n",Permision,Owner,Group,Size_in_Bytes,Date,Type,Name);
                        
                        }
                        
                        fprintf(DotFl,"\t\t\t\t\t</TABLE>\n");
                fprintf(DotFl,"\t\t\t\t>\n");
            fprintf(DotFl,"\t\t\t]\n");


        fprintf(DotFl,"\t\t}\n");
        fprintf(DotFl,"}\n");
        fclose(DotFl);
        char* Tmp = newString("xdg-open ");
        Tmp = Concat_Izq_with_Der(Tmp,CompleteReportPathDir,'s','s');
        system(Tmp);

    }
    
}

//void FullViewRender(char* CompletePathDir,char* ReportPath,char* ReportName,int iN){
void FullViewRender(char* CompleteReportPathDir,char* Type){
   
    int iN = Calc_iN(Omni->PartBatch_inUse->Size);

    /*
    if(Usr_inUse->SBinuse->s_filesystem_type == 3){
        iN = Usr_inUse->Ext3_iN;
    }
    else{
        iN = Usr_inUse->Ext2_iN;
    }
    */

    //(^< ............ ............ ............   D I S K   L O A D
    //MBR* Disk = LoadMBR(CompletePathDir_inUse);



    //(^< ............ ............ ............   R E P O R T
    char* ReportName = Path_Get_FileName(CompleteReportPathDir);
    char* ReportPath = Path_Get_Isolated(CompleteReportPathDir);

    int LsNm = strlen(ReportName);
    ReportName[LsNm - 1] = 't';
    ReportName[LsNm - 2] = 'o';
    ReportName[LsNm - 3] = 'd';

    CreatePathDir(ReportPath);

    CompleteReportPathDir = Concat_Izq_with_Der(ReportPath,ReportName,'s','s');
    
    FILE* DotFl = fopen(CompleteReportPathDir,"w+");

    //(^< ............ ............ ...........   D O T   F I L E
    if(DotFl){
        fprintf(DotFl,"digraph FullView {\n");
        fprintf(DotFl,"\trankdir = LR;\n");
        fprintf(DotFl,"\tnode [shape = plaintext];\n");
        fprintf(DotFl,"\t\tsubgraph cluster_OutLook {\n");
        fprintf(DotFl,"\t\t\tlabel = \"Ext3 :: FileSystem\";\n");
        fprintf(DotFl,"\t\t\tgraph[style = dotted];\n");

        //(^< ............ ............ ...........   SuperBlock
        if(strcasecmp(Type,"sb") == 0){
            SuperBlockDot(DotFl);
        }
        

        //(^< ............ ............ ...........   Journaling
        if(strcasecmp(Type,"journaling") == 0){
            int StartByte = Bit_to_StartByte(0,"journaling");

        }

        //(^< ............ ............ ...........   FileSystemTree
        if(strcasecmp(Type,"tree") == 0){
            int StartByte = Bit_to_StartByte(0,"Inode");
            FileSystemTree(DotFl,StartByte);
            //(^< ............ ............ ...........   inodeBitMap
            //inodeBitMapDot(DotFl,iN);

            //(^< ............ ............ ...........   blockBitMap
            //blockBitMapDot(DotFl,iN);
        }

        //(^< ............ ............ ...........   InodeOnlyDot
        if(strcasecmp(Type,"inode") == 0){
            
            InodeOnlyDot(DotFl,iN);
        }
        
        //(^< ............ ............ ...........   BlockOnlyDot
        if(strcasecmp(Type,"block") == 0){
            
            BlockOnlyDot(DotFl,iN);
        }

        fprintf(DotFl,"\t\t}\n");
        fprintf(DotFl,"}\n");
        fclose(DotFl);
        char* Tmp = newString("xdg-open ");
        Tmp = Concat_Izq_with_Der(Tmp,CompleteReportPathDir,'s','s');
        system(Tmp);
    }
}

#endif // F2REPTS_H



#endif // F2_REP