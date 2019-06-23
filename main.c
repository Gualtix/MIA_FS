#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "Fw/StringHandler.h"
#include "Fw/Helper.h"
#include "Analizer.h"
#include "Glovs.h"

int main()
{
    //Check_If_FullPath_Exists("/home/wrm/Desktop/Alaska.txt");
    newLine_Flag = 0;
    cmdString    = NULL;

    Omni = newGLS();
    
    int i = 0;
    while (i < 25){
        UsingDisk_List[i] = *newDisk_in_Use();
        i++;
    }

    while (1){
        printf("\n");        
        printf("(^< ............ ............ ............\n");
        printf("Ready to Execute:\n");
        //char *InputString = CatchCommandLine();
        char *InputString = newString("exec -path~:~/home/wrm/Desktop/EXEC_FILES/EntradaF2.sh");
        int Rs = strcasecmp(InputString, "exit\n");
        ExecuteComand(InputString);
        if (Rs == 0){
            break;
        }
        break;
    }   
    
    printf("(^< ............ S A L I E N D O ............\n");
    return 0;
}