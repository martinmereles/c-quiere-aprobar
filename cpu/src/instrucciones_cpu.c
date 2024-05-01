#include "../include/instrucciones_cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern registros_t *reg;

//void recibirInstruccion(){


//}

int deCharAEnum(char* registro){
    if(strcmp(registro,"AX")==0){
        return AX;
    }
    if(strcmp(registro,"BX")==0){
        return BX;
    }
    if(strcmp(registro,"CX")==0){
        return CX;
    }
    if(strcmp(registro,"DX")==0){
        return DX;
    }
    if(strcmp(registro,"EAX")==0){
        return EAX;
    }
    if(strcmp(registro,"EBX")==0){
        return EBX;
    }
    if(strcmp(registro,"ECX")==0){
        return ECX;
    }
    if(strcmp(registro,"EDX")==0){
        return EDX;
    }
    if(strcmp(registro,"PC")==0){
        return PC;
    }
    /*  PREGUNTAR !! 
    if(strcmp(registro,"SI")==0){
        return SI;
    }
    if(strcmp(registro,"DI")==0){
        return DI;
    } */
    
}
void set (char * registro, char * valor)    
{
    switch (deCharAEnum(registro))
    {
    case AX:
        
        printf("El registro es: %d\n", deCharAEnum(registro));
        //uint8_t valorASetear = 88;
        int valorASetear = atoi(valor);
        //uint8_t* var= reg->reg_generales->AX;
        //*var = (uint8_t)valorASetear;
        //reg->reg_generales->AX=valorASetear;
        printf("El registro es: %d\n", reg->reg_generales->AX);
        
        break;
    default:
        break;
    }
}
