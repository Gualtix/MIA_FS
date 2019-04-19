#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "Fw/StringHandler.h"
#include "Fw/Helper.h";

int main()
{

    
    char* Path = Path_Get_Isolated("/home/wrm/Desktop/EXEC_FILES/Entrada_F1.sh");
    char* FileName = Path_Get_FileName("/home/wrm/Desktop/EXEC_FILES/Entrada_F1.sh");
    char* LastDir = Path_Get_LastDirName("/home/wrm/Desktop/EXEC_FILES/Entrada_F1.sh");
    char* Us = newString("Numanches");
    char* eee = newString("Siiz");

    double eks = 85.58;

    char soso = 'e';

    int as = -96;

    char* EE = Concat_Izq_with_Der(newString("Sipirri"),newString(" Horas"),'s','s');
    char a = Us[0];
    a = Us[1];
    a = Us[2];
    a = Us[3];
    a = Us[4];
    a = Us[5];
    a = Us[6];
    a = Us[7];
    a = Us[8];
    a = Us[9];
    a = Us[10];
    a = Us[11];
    a = Us[12];

    int k = 55;
    /*

    while (1){
        printf("\n");        
        printf("(^< ............ ............ ............\n");
        printf("Ready to Execute:\n");
        //char *InputString = CatchCommandLine();
        char *InputString = New_Char_Init_Loaded("exec -path~:~/home/wrm/Desktop/EXEC_FILES/Entrada_F1.sh\n");
        int Rs = strcasecmp(InputString, "exit\n");
        ExecuteComand(InputString);
        if (Rs == 0){
            break;
        }
        
        break;
        
    }    
    */
    printf("(^< ............ S A L I E N D O ............\n");
    return 0;
}