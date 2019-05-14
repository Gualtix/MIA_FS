#ifndef F1_REP
#define F1_REP

#include "../Fw/Helper.h"
#include "../Container.h"
#include "../Fw/StringHandler.h"
#include "../Belong.h"

void AddBlock(FILE* DotFl,Batch* GB,char* BlockName,int DiskSize){
    fprintf(DotFl,"\t\t\t\t\t<TD width = \"150\">\n");
        fprintf(DotFl,"\t\t\t\t\t\t- %s -<br/>\n",BlockName);
        double PRC = ((double)GB->Size/DiskSize)*100;
        double BlockSize = GB->Size/1024;
        fprintf(DotFl,"\t\t\t\t\t\t%.3f (Kb)<br/>\n",BlockSize);
        fprintf(DotFl,"\t\t\t\t\t\t%.3f%% del Disco<br/>\n",PRC);
        fprintf(DotFl,"\t\t\t\t\t\tStartByte:%d<br/>\n",GB->StartByte);
        fprintf(DotFl,"\t\t\t\t\t\tEndByte:%d<br/>\n",GB->EndByte);
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
            AddBlock(DotFl,Bt,Nm,DiskSize);   
        }

        if(Bt->Type == 'm'){
            AddBlock(DotFl,Bt,"MBR",DiskSize);   
        }
    }
}


void GenerateDiskRender(char* CompletePathDir,char* ReportPath,char* ReportName)
{

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
                        DoublyGenericList* btList = getBatchList_FromDisk(Disk);
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