#ifndef BELONG2_H
#define BELONG2_H

#include "Container2.h"
#include "Fw/Helper.h"
#include "Glovs.h"
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ B I N
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

int Bit_to_StartByte(int Bit,char* BiType){
    if(strcasecmp(BiType,"Inode") == 0){
        return (SBinuse->s_inode_start + (Bit * sizeof(Inode)));
    }
    if(strcasecmp(BiType,"Block") == 0){
        return (SBinuse->s_block_start + (Bit * sizeof(FileBlock)));
    }
    return -1;
}

int StartByte_to_InodeBit(int StartByte){
    return((StartByte - SBinuse->s_inode_start)/(sizeof(Inode)));
}

int StartByte_to_BlockBit(int StartByte){
    return((StartByte - SBinuse->s_block_start)/(sizeof(FileBlock)));
}

void* BinLoad_Str(int Bit_ID,char* StrType){

    int StartByte = -1;
    
    if(strcasecmp(StrType,"Inode") == 0){
        StartByte = Bit_to_StartByte(Bit_ID,"Inode");
    }
    else{
        StartByte = Bit_to_StartByte(Bit_ID,"Block");
    }

    FILE* Fl = fopen(CompletePathDir_of_Disk_inUse,"rb+");
    if(Fl){
        fseek(Fl,StartByte,SEEK_SET);
        //if(strcasecmp(StrType,"SuperBlock") == 0){
        //    SuperBlock* SB = newSuperBlock();
        //    fread(SB,sizeof(SuperBlock),1, Fl);
        //    return SB;
        //}
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

//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............
//(^< ............ ............ ............ ............ ............ File System C R U D
//(^< ............ ............ ............ ............ ............ ............ ............ ............ ............ ............

SeekInfo* CompleteSeeker(int iNodeCurent_Bit_ID,char* FileName){

    SeekInfo* nwSI = newSeekInfo();
    Inode* tIn = (Inode*)BinLoad_Str(iNodeCurent_Bit_ID,"Inode");

    int i = 0;
    while(i < 12){
        int tFB_Bit_ID = tIn->i_block[i];
        if(tFB_Bit_ID > - 1){
            FolderBlock* tFB = (FolderBlock*)BinLoad_Str(tFB_Bit_ID,"FolderBlock");

            if(i == 0){
                int iNodeFather_Bit_ID = tFB->b_content[0].b_inodo;
                nwSI->iNodeFather_Bit_ID = iNodeFather_Bit_ID;
            }

            int j;
            while(j < 4){
                if(strcasecmp(tFB->b_content[j].b_name,FileName) == 0){
                    nwSI->FB_Bit_ID = tFB_Bit_ID;
                    nwSI->FB_Index  = j;
                    nwSI->iNode_Bit_ID = tFB->b_content[j].b_inodo;
                    nwSI->iCurent_Bit_ID = iNodeCurent_Bit_ID;
                    return nwSI;
                }
                else{

                    int nextIn_Bit_ID = tFB->b_content[j].b_inodo;
                    Inode* nextIn = (Inode*)BinLoad_Str(nextIn_Bit_ID,"Inode");
                    if(nextIn->i_type == 0 && strcasecmp(tFB->b_content[j].b_name,"iNodeFather") != 0 && strcasecmp(tFB->b_content[j].b_name,"iNodeCurent") != 0){
                        SeekInfo* tmp = CompleteSeeker(nextIn_Bit_ID,FileName);
                        if(tmp != NULL){
                            return tmp;
                        }
                    }
                }
                j++;
            }
        }
        i++;
    }

    
    //Indirecto Simple
    i++;
    if(i == 12){
        int tBP_Bit_ID = tIn->i_block[i];
        if(tBP_Bit_ID > - 1){
            PointerBlock* tPB = (PointerBlock*)BinLoad_Str(tBP_Bit_ID,"PointerBlock");

            int x = 0;
            while(x < 16){
                int tmp = tPB->b_pointers[x];
                if(tmp > -1){
                    
                }

                x++;
            }

        }
        
    }

    //Indirecto Doble
    i++;
    if(i == 13){
        
    }

    //Indirecto Tiple
    i++;
    if(i == 14){
        
    }




    /*
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

    /*while(i_tIn < 15){
        if(tIn->i_block[i_tIn] > -1){

        }
        i_tIn++;
    }*/
}


#endif // BELONG2_H