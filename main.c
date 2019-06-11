#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "Fw/StringHandler.h"
#include "Fw/Helper.h"
#include "Analizer.h"

int main()
{

    int i = 0;
    while (i < 25){
        UsingDisk_List[i] = *newDisk_in_Use();
    }
    
    while (1){
        printf("\n");        
        printf("(^< ............ ............ ............\n");
        printf("Ready to Execute:\n");
        //char *InputString = CatchCommandLine();
        char *InputString = newString("exec -path~:~/home/wrm/Desktop/EXEC_FILES/Entrada_F1.sh");
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