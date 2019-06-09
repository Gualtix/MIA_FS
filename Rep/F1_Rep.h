#ifndef F1_REP
#define F1_REP

#include "../Fw/Helper.h"
#include "../Container.h"
#include "../Fw/StringHandler.h"
#include "../Belong.h"

void AddBlock(FILE* DotFl,Batch* GB,char* BlockName,int DiskSize){
    if(GB->Type != 'e'){

        if(GB->Type == 'm'){
            fprintf(DotFl,"\t\t\t\t\t<TD bgcolor=\"#00cc99\" width = \"150\">\n");
        }
        else if(GB->Type == 'p'){
            fprintf(DotFl,"\t\t\t\t\t<TD bgcolor=\"#66ccff\" width = \"150\">\n");
        }
        else if(GB->Type == 's'){
            fprintf(DotFl,"\t\t\t\t\t<TD bgcolor=\"#eff5f5\" width = \"200\">\n");
        }
        else{
            fprintf(DotFl,"\t\t\t\t\t<TD width = \"150\">\n");
        }
    }
    
        fprintf(DotFl,"\t\t\t\t\t\t- %s -<br/>\n",BlockName);
        double PRC = ((double)GB->Size/DiskSize)*100;
        double BlockSize = GB->Size/1024;
        fprintf(DotFl,"\t\t\t\t\t\t%.3f (Kb)<br/>\n",BlockSize);
        fprintf(DotFl,"\t\t\t\t\t\t%.3f%% del Disco<br/>\n",PRC);
        fprintf(DotFl,"\t\t\t\t\t\tStartByte:%d<br/>\n",GB->StartByte);
        fprintf(DotFl,"\t\t\t\t\t\tEndByte:%d<br/>\n",GB->EndByte);
    
    if(GB->Type != 'e'){
        fprintf(DotFl,"\t\t\t\t\t</TD>\n");
    }
}

void Add_EBR_Block(FILE* DotFl,Batch* GB,char* BlockName,int DiskSize,int Next){
    if(GB->Type != 'e'){
        fprintf(DotFl,"\t\t\t\t\t<TD bgcolor=\"#ff9900\" width = \"75\">\n");
    }
    
        fprintf(DotFl,"\t\t\t\t\t\t- EBR -<br/>\n");
        fprintf(DotFl,"\t\t\t\t\t\t %s <br/>\n",BlockName);
        fprintf(DotFl,"\t\t\t\t\t\Next:%d<br/>\n",Next);
    
    if(GB->Type != 'e'){
        fprintf(DotFl,"\t\t\t\t\t</TD>\n");
    }
}

void Add_LogicPart_Block(FILE* DotFl,Batch* GB,char* BlockName,int DiskSize){
    if(GB->Type != 'e'){
        fprintf(DotFl,"\t\t\t\t\t<TD bgcolor=\"#c6ff1a\" width = \"350\">\n");
    }
    
        fprintf(DotFl,"\t\t\t\t\t\t- Logic -<br/>\n");
        fprintf(DotFl,"\t\t\t\t\t\t %s <br/>\n",BlockName);
        double PRC = ((double)GB->Size/DiskSize)*100;
        int BlockSize = GB->Size;
        fprintf(DotFl,"\t\t\t\t\t\t %d (Bytes)<br/>\n",BlockSize);
        fprintf(DotFl,"\t\t\t\t\t\t%.3f%% del Disco<br/>\n",PRC);
        fprintf(DotFl,"\t\t\t\t\t\tStartByte:%d<br/>\n",GB->StartByte);
        fprintf(DotFl,"\t\t\t\t\t\tEndByte:%d<br/>\n",GB->EndByte);
    
    if(GB->Type != 'e'){
        fprintf(DotFl,"\t\t\t\t\t</TD>\n");
    }
}

void AddExtended_Block(FILE* DotFl,Batch* GB,char* BlockName,int DiskSize,int ColSpan){
    
    fprintf(DotFl,"\t\t\t\t\t<TD bgcolor=\"#ff5050\" width = \"150\">\n");

        fprintf(DotFl,"\t\t\t\t\t\t<TABLE border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"0\">\n");
        fprintf(DotFl,"\t\t\t\t\t\t\t<TR>\n");
            fprintf(DotFl,"\t\t\t\t\t\t\t\t<TD colspan=\"%d\">\n",100);
            AddBlock(DotFl,GB,BlockName,DiskSize);
            fprintf(DotFl,"\t\t\t\t\t\t\t\t</TD>\n");
        fprintf(DotFl,"\t\t\t\t\t\t\t</TR>\n");

        fprintf(DotFl,"\t\t\t\t\t\t\t<TR>\n");
            while(GB->LgParts->Length > 0){
                Batch* tB = (Batch*)(DeQueue(GB->LgParts));
                
                if(tB->Type == 'q'){
                    //char* Nm = Concat_Izq_with_Der(newString("EBR: "),tB->PartName,'s','s');
                    char* Nm = tB->PartName;
                    
                    Add_EBR_Block(DotFl,tB,Nm,DiskSize,tB->Next);

                    int Cmp = strcasecmp(tB->PartName,"UNDEFINED");
                    if(Cmp != 0){
                        Batch* Lg = newBatch();
                        Lg->StartByte = tB->StartByte;
                        Lg->Size =  tB->Size;
                        Lg->EndByte = tB->EndByte;
                        Lg->Next = tB->Next;
                        Lg->PartName = tB->PartName;
                        Lg->Type = 'l';

                        char* Ps = tB->PartName;
                        Add_LogicPart_Block(DotFl,Lg,Ps,DiskSize);
                    }
                    
                }
                else if(tB->Type == 's'){
                    AddBlock(DotFl,tB,"Space",DiskSize);
                }
                else{
                    char* Nm = Concat_Izq_with_Der(newString("Logic: "),tB->PartName,'s','s');
                    AddBlock(DotFl,tB,Nm,DiskSize);
                }
            }
        fprintf(DotFl,"\t\t\t\t\t\t\t</TR>\n");

    fprintf(DotFl,"\t\t\t\t\t\t</TABLE>\n");
    fprintf(DotFl,"\t\t\t\t\t</TD>\n");
}

void AddBatch(FILE* DotFl,DoublyGenericList* btList,int DiskSize){
    while(btList->Length > 0){
        Batch* Bt = (Batch*)DeQueue(btList);
        if(Bt->Type == 's'){
            AddBlock(DotFl,Bt,"Space",DiskSize);
        }
        if(Bt->Type == 'p'){
            char* Nm = Concat_Izq_with_Der(newString("Primary: "),Bt->PartName,'s','s');
            AddBlock(DotFl,Bt,Nm,DiskSize);   
        }

        if(Bt->Type == 'e'){
            char* Nm = Concat_Izq_with_Der(newString("Extended: "),Bt->PartName,'s','s');
            AddExtended_Block(DotFl,Bt,Nm,DiskSize,Bt->LgParts->Length);   
        }

        if(Bt->Type == 'm'){
            AddBlock(DotFl,Bt,"MBR",DiskSize);   
        }
    }
}

void GenerateDiskRender(char* CompletePathDir,char* ReportPath,char* ReportName){
    //(^< ............ ............ ............   J O I N :   R E P O R T P A T H   &   R E P O R T N A M E
    char* CompleteReportPathDir = Concat_Izq_with_Der(ReportPath,ReportName,'s','s');

    //(^< ............ ............ ............   D I S K   L O A D
    MBR* Disk = LoadMBR(CompletePathDir);

    //(^< ............ ............ ............   R E P O R T
    CreatePathDir(ReportPath);

    FILE* DotFl = fopen(CompleteReportPathDir,"w+");

    //(^< ............ ............ ...........   D O T   F I L E
    if(DotFl){
        fprintf(DotFl,"digraph test {\n");
            fprintf(DotFl,"\tnode[shape=plaintext];\n");
            fprintf(DotFl,"\tDiskStruct\n");
            fprintf(DotFl,"\t[\n");

            fprintf(DotFl,"\t\tlabel = \n");

                fprintf(DotFl,"\t\t<\n");

                    fprintf(DotFl,"\t\t\t<TABLE border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"0\">\n");
                        fprintf(DotFl,"\t\t\t\t<TR>\n");
                        //(^< ............ ............ ........... R E C O R R I D O
                        DoublyGenericList* btList = getBatchList_FromDisk(CompletePathDir,Disk);
                        
                        AddBatch(DotFl,btList,Disk->mbr_tamano);
                        
                        fprintf(DotFl,"\t\t\t\t</TR>\n");

                    fprintf(DotFl,"\t\t\t</TABLE>\n");

                fprintf(DotFl,"\t\t>,\n");

            fprintf(DotFl,"\t];\n");

        fprintf(DotFl,"}");
    }

    fclose(DotFl);
    char* Tmp = newString("xdg-open ");
    Tmp = Concat_Izq_with_Der(Tmp,CompleteReportPathDir,'s','s');
    system(Tmp);

}

#endif // F1_REP