#include "../include/instrucciones_cpu.h"

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
    if(strcmp(registro,"SI")==0){
        return SI;
    }
    if(strcmp(registro,"DI")==0){
        return DI;
    } 
    
}

void set (char * registro, char * valor)    
{   int valorASetear = atoi(valor);
    switch (deCharAEnum(registro))
    {
    case AX:
        reg->AX=valorASetear;
        break;
    case BX:
        reg->BX=valorASetear;
        break;
    case CX:
        reg->CX=valorASetear;
        break;
    case DX:
        reg->DX=valorASetear;
        break;
    case EAX:
        reg->EAX=valorASetear;
        break;
    case EBX:
        reg->EBX=valorASetear;
        break;
    case ECX:
        reg->ECX=valorASetear;
        break;
    case EDX:
        reg->EDX=valorASetear;
        break;
    case PC:
        reg->PC=valorASetear;
        break;
    case SI:
        reg->SI=valorASetear;
        break;
    case DI:
        reg->DX=valorASetear;
        break;
    default:
        break;
    }
    log_info(logger, "Se ejecuto la instrucción SET con los parametros registro %s y valor %s", registro, valor);
}

void sum (char * registroDestino, char * registroOrigen)    
{
    // Falta implementar
    log_info(logger, "Se ejecuto la instrucción SUM con los parametros registro destino %s y registro origen %s", registroDestino, registroOrigen);
}

void sub (char * registroDestino, char * registroOrigen)    
{
    // Falta implementar
    log_info(logger, "Se ejecuto la instrucción SUB con los parametros registro destino %s y registro origen %s", registroDestino, registroOrigen);
}

void jnz (char * registro, char * instruccion)    
{   int valorASetear = atoi(instruccion);
    switch (deCharAEnum(registro))
    {
    case AX:
        if(reg->AX != 0)reg->PC=valorASetear;
        break;
    case BX:
        if(reg->BX != 0)reg->PC=valorASetear;
        break;
    case CX:
        if(reg->CX != 0)reg->PC=valorASetear;
        break;
    case DX:
        if(reg->DX != 0)reg->PC=valorASetear;
        break;
    case EAX:
        if(reg->EAX != 0)reg->PC=valorASetear;
        break;
    case EBX:
        if(reg->EBX != 0)reg->PC=valorASetear;
        break;
    case ECX:
        if(reg->ECX != 0)reg->PC=valorASetear;
        break;
    case EDX:
        if(reg->EDX != 0)reg->PC=valorASetear;
        break;
    case PC:
        if(reg->PC != 0)reg->PC=valorASetear;
        break;
    case SI:
        if(reg->SI != 0)reg->PC=valorASetear;
        break;
    case DI:
        if(reg->DI != 0)reg->PC=valorASetear;
        break;
    default:
        break;
    }
    log_info(logger, "Se ejecuto la instrucción JNZ con los parametros registro %s e instrucción %s", registro, instruccion);
}

void io_gen_sleep (char * interfaz, char * unidadesDeTrabajo)    
{
    // Falta implementar
    log_info(logger, "Se ejecuto la instrucción IO_GEN_SLEEP con los parametros interfaz %s y unidades de trabajo %s", interfaz, unidadesDeTrabajo);
}