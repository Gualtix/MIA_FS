#ifndef STRINGHANDLER_H
#define STRINGHANDLER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* newString(int Ln){
    // +1 for '\0' character
    char* Str = (char*) malloc((Ln + 1) * sizeof(char)); 
    memset(Str,'\0',Ln);
    Str[Ln] = '\0';
    return Str;
}

char* newString(char* Str){
    int Ln = strlen(Str);
    char* Rs = newString(Ln);
    int cnt = 0;
    while(cnt < Ln){
        Rs[cnt] = Str[cnt];
        cnt++;
    }
    return Rs;
}

char* newString(const char* Str){
    int Ln = strlen(Str);
    char* Rs = newString(Ln);
    int cnt = 0;
    while(cnt < Ln){
        Rs[cnt] = Str[cnt];
        cnt++;
    }
    return Rs;
}

char* toString(void* _Izq, char It){

    char* Izq = newString(100);

    if(_Izq != NULL){
        //------------ I Z Q
        if(It == 'c'){
            char* tmp = (char*) _Izq;
            Izq = newString(1);
            Izq[0] = tmp[0];
        }

        if(It == 's'){
            Izq = (char*) _Izq;
        }

        if(It == 'i'){
            int *tmp = (int*) _Izq;
            sprintf(Izq,"%d",*tmp);
        }

        if(It == 'd'){
            double* tmp = (double*) _Izq;
            sprintf(Izq,"%0.2f",*tmp);
        }
    }

    if(_Izq == NULL){
        return NULL;
    }

    int Ln = strlen(Izq);
    char* Str = newString(Ln);
    strcpy(Str,Izq);
    return Str;

}

char* Concat_Izq_with_Der(void* _Izq, void* _Der, char It, char Dt){

    char* Izq = newString(100);
    char* Der = newString(100);

    if(_Izq != NULL){
        //------------ I Z Q
        if(It == 'c'){
            char* tmp = (char*) _Izq;
            Izq = newString(1);
            Izq[0] = tmp[0];
        }

        if(It == 's'){
            Izq = (char*) _Izq;
        }

        if(It == 'i'){
            int *tmp = (int*) _Izq;
            sprintf(Izq,"%d",*tmp);
        }

        if(It == 'd'){
            double* tmp = (double*) _Izq;
            sprintf(Izq,"%0.2f",*tmp);
        }
    }

    if(_Der != NULL){
        //------------ D E R
        if(Dt == 'c'){
            char* tmp = (char*) _Der;
            Der = newString(1);
            Der[0] = tmp[0];
        }

        if(Dt == 's'){
            Der = (char*) _Der;
        }

        if(Dt == 'i'){
            int *tmp = (int*) _Der;
            sprintf(Der,"%d",*tmp);
        }

        if(Dt == 'd'){
            double *tmp = (double*) _Der;
            sprintf(Der,"%0.2f",*tmp);
        }
    }

    if(_Izq == NULL){
        return Der;
    }

    if(_Der == NULL){
        return Izq;
    }

    int Ln = strlen(Izq) + strlen(Der);
    char* Str = newString(Ln);
    strcpy(Str,Izq);
    strcat(Str,Der);
    return Str;

}

void String_ByRef_ArrayClear(char** Str){

}

void String_ByRef_AvoidQuotationMarks(char** Str){

}

void String_ByRef_toLower(char** MStr){
    int Ln = strlen(*MStr);
    int cnt = 0;
    while (cnt < Ln) {
        char Current = *(*MStr + cnt);
        *(*MStr + cnt) = putchar(tolower(Current));
        cnt++;
    }
}

int String_IsEmpty(char* MStr){
    int Ln = strlen(MStr);
    int cnt = 0;
    while(cnt < Ln){
        char Tmp = *(MStr + cnt);
        if(Tmp != '^'){
            return 0;
        }
        cnt++;
    }
    return 1;
}

char* StringCloneWithOut(char* SRC,char Sym){

    int Nm = 0;
    char Tmp;

    char* CD = newString(SRC);
    //Destruye la Cadena original
    while ((Tmp = *CD++), ((Tmp == Sym)? ++Nm : 0), Tmp);

    if(Nm == 0){
        return SRC;
    }

    char* Rs = newString(strlen(SRC) - Nm);

    int i = 0;
    int cnt = 0;
    
    while(SRC[cnt] != '\0'){
        if(SRC[cnt] != Sym){
            Rs[i] = SRC[cnt];
            i++;
        }
        cnt++;
    }

    return Rs;
}

int String_isNumber(char* Str){
    int cnt = 0;
    while(Str[cnt] != '\0'){
        if(!isdigit(Str[cnt])){
            return 0;
        }
        cnt++;
    }

    if(atoi(Str) < 0){
        return 0;
    }
    return 1;
}

int String_Has_this_Char(char* Str,char ch){
    
    int ln = strlen(Str);
    
    int i = 0;
    while (i < ln){
        if(Str[i] == ch){
            return i;
        }
        i++;
    }
    
    return - 1;
}

#endif // STRINGHANDLER_H