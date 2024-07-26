#include <../include/instrucciones_cpu.h>

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

void ejecutarSentencia(int socket_kernel, int socket_cliente_memoria){

    char ** sentenciasSplit = string_split(instruccion_exec, " ");
    if(strcmp(sentenciasSplit[0],"SET")==0) set(sentenciasSplit[1],sentenciasSplit[2]);
    if(strcmp(sentenciasSplit[0],"MOV_IN")==0) mov_in();
    if(strcmp(sentenciasSplit[0],"MOV_OUT")==0) mov_out();
    if(strcmp(sentenciasSplit[0],"SUM")==0) sum(sentenciasSplit[1],sentenciasSplit[2]);
    if(strcmp(sentenciasSplit[0],"SUB")==0) sub(sentenciasSplit[1],sentenciasSplit[2]);
    if(strcmp(sentenciasSplit[0],"JNZ")==0) jnz(sentenciasSplit[1],sentenciasSplit[2]);
    if(strcmp(sentenciasSplit[0],"RESIZE")==0) resize();
    if(strcmp(sentenciasSplit[0],"COPY_STRING")==0) copy_string();
    if(strcmp(sentenciasSplit[0],"WAIT")==0) wait();
    if(strcmp(sentenciasSplit[0],"SIGNAL")==0) signal_instruccion();
    if(strcmp(sentenciasSplit[0],"IO_GEN_SLEEP")==0) io_gen_sleep(sentenciasSplit[1],sentenciasSplit[2], socket_kernel);
    if(strcmp(sentenciasSplit[0],"IO_STDIN_READ")==0) io_stdin_read();
    if(strcmp(sentenciasSplit[0],"IO_STDOUT_WRITE")==0) io_stdout_write();
    if(strcmp(sentenciasSplit[0],"IO_FS_CREATE")==0) io_fs_create();
    if(strcmp(sentenciasSplit[0],"IO_FS_TRUNCATE")==0) io_fs_truncate();
    if(strcmp(sentenciasSplit[0],"IO_FS_WRITE")==0) io_fs_write();
    if(strcmp(sentenciasSplit[0],"IO_FS_READ")==0) io_fs_read();
    if(strcmp(sentenciasSplit[0],"EXIT")==0) exit_inst();
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
    switch (deCharAEnum(registroDestino))
    {
    case AX:
        switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->AX += reg->AX;
        break;
    case BX:
        reg->AX += reg->BX;
        break;
    case CX:
        reg->AX += reg->CX;
        break;
    case DX:
        reg->AX += reg->DX;
        break;
    case EAX:
        reg->AX += reg->EAX;
        break;
    case EBX:
        reg->AX += reg->EBX;
        break;
    case ECX:
        reg->AX += reg->ECX;
        break;
    case EDX:
        reg->AX += reg->EDX;
        break;
    case PC:
        reg->AX += reg->PC;
        break;
    case SI:
        reg->AX += reg->SI;
        break;
    case DI:
        reg->AX += reg->DI;
        break;
    default:
        break;
    }
        break;
    case BX:
            switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->BX += reg->AX;
        break;
    case BX:
        reg->BX += reg->BX;
        break;
    case CX:
        reg->BX += reg->CX;
        break;
    case DX:
        reg->BX += reg->DX;
        break;
    case EAX:
        reg->BX += reg->EAX;
        break;
    case EBX:
        reg->BX += reg->EBX;
        break;
    case ECX:
        reg->BX += reg->ECX;
        break;
    case EDX:
        reg->BX += reg->EDX;
        break;
    case PC:
        reg->BX += reg->PC;
        break;
    case SI:
        reg->BX += reg->SI;
        break;
    case DI:
        reg->BX += reg->DI;
        break;
    default:
        break;
    }
        break;
    case CX:
                    switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->CX += reg->AX;
        break;
    case BX:
        reg->CX += reg->BX;
        break;
    case CX:
        reg->CX += reg->CX;
        break;
    case DX:
        reg->CX += reg->DX;
        break;
    case EAX:
        reg->CX += reg->EAX;
        break;
    case EBX:
        reg->CX += reg->EBX;
        break;
    case ECX:
        reg->CX += reg->ECX;
        break;
    case EDX:
        reg->CX += reg->EDX;
        break;
    case PC:
        reg->CX += reg->PC;
        break;
    case SI:
        reg->CX += reg->SI;
        break;
    case DI:
        reg->CX += reg->DI;
        break;
    default:
        break;
    }
        break;
    case DX:
                    switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->DX += reg->AX;
        break;
    case BX:
        reg->DX += reg->BX;
        break;
    case CX:
        reg->DX += reg->CX;
        break;
    case DX:
        reg->DX += reg->DX;
        break;
    case EAX:
        reg->DX += reg->EAX;
        break;
    case EBX:
        reg->DX += reg->EBX;
        break;
    case ECX:
        reg->DX += reg->ECX;
        break;
    case EDX:
        reg->DX += reg->EDX;
        break;
    case PC:
        reg->DX += reg->PC;
        break;
    case SI:
        reg->DX += reg->SI;
        break;
    case DI:
        reg->DX += reg->DI;
        break;
    default:
        break;
    }
        break;
    case EAX:
                switch (deCharAEnum(registroOrigen))
        {
    case AX:
        reg->EAX += reg->AX;
        break;
    case BX:
        reg->EAX += reg->BX;
        break;
    case CX:
        reg->EAX += reg->CX;
        break;
    case DX:
        reg->EAX += reg->DX;
        break;
    case EAX:
        reg->EAX += reg->EAX;
        break;
    case EBX:
        reg->EAX += reg->EBX;
        break;
    case ECX:
        reg->EAX += reg->ECX;
        break;
    case EDX:
        reg->EAX += reg->EDX;
        break;
    case PC:
        reg->EAX += reg->PC;
        break;
    case SI:
        reg->EAX += reg->SI;
        break;
    case DI:
        reg->EAX += reg->DI;
        break;
    default:
        break;
    }
        break;
    case EBX:
switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->EBX += reg->AX;
        break;
    case BX:
        reg->EBX += reg->BX;
        break;
    case CX:
        reg->EBX += reg->CX;
        break;
    case DX:
        reg->EBX += reg->DX;
        break;
    case EAX:
        reg->EBX += reg->EAX;
        break;
    case EBX:
        reg->EBX += reg->EBX;
        break;
    case ECX:
        reg->EBX += reg->ECX;
        break;
    case EDX:
        reg->EBX += reg->EDX;
        break;
    case PC:
        reg->EBX += reg->PC;
        break;
    case SI:
        reg->EBX += reg->SI;
        break;
    case DI:
        reg->EBX += reg->DI;
        break;
    default:
        break;
    }
        break;
    case ECX:
                            switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->ECX += reg->AX;
        break;
    case BX:
        reg->ECX += reg->BX;
        break;
    case CX:
        reg->ECX += reg->CX;
        break;
    case DX:
        reg->ECX += reg->DX;
        break;
    case EAX:
        reg->ECX += reg->EAX;
        break;
    case EBX:
        reg->ECX += reg->EBX;
        break;
    case ECX:
        reg->ECX += reg->ECX;
        break;
    case EDX:
        reg->ECX += reg->EDX;
        break;
    case PC:
        reg->ECX += reg->PC;
        break;
    case SI:
        reg->ECX += reg->SI;
        break;
    case DI:
        reg->ECX += reg->DI;
        break;
    default:
        break;
    }
        break;
    case EDX:
        switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->EDX += reg->AX;
        break;
    case BX:
        reg->EDX += reg->BX;
        break;
    case CX:
        reg->EDX += reg->CX;
        break;
    case DX:
        reg->EDX += reg->DX;
        break;
    case EAX:
        reg->EDX += reg->EAX;
        break;
    case EBX:
        reg->EDX += reg->EBX;
        break;
    case ECX:
        reg->EDX += reg->ECX;
        break;
    case EDX:
        reg->EDX += reg->EDX;
        break;
    case PC:
        reg->EDX += reg->PC;
        break;
    case SI:
        reg->EDX += reg->SI;
        break;
    case DI:
        reg->EDX += reg->DI;
        break;
    default:
        break;
    }
        break;
    case PC:
        switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->PC += reg->AX;
        break;
    case BX:
        reg->PC += reg->BX;
        break;
    case CX:
        reg->PC += reg->CX;
        break;
    case DX:
        reg->PC += reg->DX;
        break;
    case EAX:
        reg->PC += reg->EAX;
        break;
    case EBX:
        reg->PC += reg->EBX;
        break;
    case ECX:
        reg->PC += reg->ECX;
        break;
    case EDX:
        reg->PC += reg->EDX;
        break;
    case PC:
        reg->PC += reg->PC;
        break;
    case SI:
        reg->PC += reg->SI;
        break;
    case DI:
        reg->PC += reg->DI;
        break;
    default:
        break;
    }
        break;
    case SI:
        switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->SI += reg->AX;
        break;
    case BX:
        reg->SI += reg->BX;
        break;
    case CX:
        reg->SI += reg->CX;
        break;
    case DX:
        reg->SI += reg->DX;
        break;
    case EAX:
        reg->SI += reg->EAX;
        break;
    case EBX:
        reg->SI += reg->EBX;
        break;
    case ECX:
        reg->SI += reg->ECX;
        break;
    case EDX:
        reg->SI += reg->EDX;
        break;
    case PC:
        reg->SI += reg->PC;
        break;
    case SI:
        reg->SI += reg->SI;
        break;
    case DI:
        reg->SI += reg->DI;
        break;
    default:
        break;
    }
        break;
    case DI:
        switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->DI += reg->AX;
        break;
    case BX:
        reg->DI += reg->BX;
        break;
    case CX:
        reg->DI += reg->CX;
        break;
    case DX:
        reg->DI += reg->DX;
        break;
    case EAX:
        reg->DI += reg->EAX;
        break;
    case EBX:
        reg->DI += reg->EBX;
        break;
    case ECX:
        reg->DI += reg->ECX;
        break;
    case EDX:
        reg->DI += reg->EDX;
        break;
    case PC:
        reg->DI += reg->PC;
        break;
    case SI:
        reg->DI += reg->SI;
        break;
    case DI:
        reg->DI += reg->DI;
        break;
    default:
        break;
    }
        break;
    default:
        break;
    }
    log_info(logger, "Se ejecuto la instrucción SUM con los parametros registro destino %s y registro origen %s", registroDestino, registroOrigen);
}

void sub (char * registroDestino, char * registroOrigen)    
{
        switch (deCharAEnum(registroDestino))
    {
    case AX:
        switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->AX -= reg->AX;
        break;
    case BX:
        reg->AX -= reg->BX;
        break;
    case CX:
        reg->AX -= reg->CX;
        break;
    case DX:
        reg->AX -= reg->DX;
        break;
    case EAX:
        reg->AX -= reg->EAX;
        break;
    case EBX:
        reg->AX -= reg->EBX;
        break;
    case ECX:
        reg->AX -= reg->ECX;
        break;
    case EDX:
        reg->AX -= reg->EDX;
        break;
    case PC:
        reg->AX -= reg->PC;
        break;
    case SI:
        reg->AX -= reg->SI;
        break;
    case DI:
        reg->AX -= reg->DI;
        break;
    default:
        break;
    }
        break;
    case BX:
            switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->BX -= reg->AX;
        break;
    case BX:
        reg->BX -= reg->BX;
        break;
    case CX:
        reg->BX -= reg->CX;
        break;
    case DX:
        reg->BX -= reg->DX;
        break;
    case EAX:
        reg->BX -= reg->EAX;
        break;
    case EBX:
        reg->BX -= reg->EBX;
        break;
    case ECX:
        reg->BX -= reg->ECX;
        break;
    case EDX:
        reg->BX -= reg->EDX;
        break;
    case PC:
        reg->BX -= reg->PC;
        break;
    case SI:
        reg->BX -= reg->SI;
        break;
    case DI:
        reg->BX -= reg->DI;
        break;
    default:
        break;
    }
        break;
    case CX:
                    switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->CX -= reg->AX;
        break;
    case BX:
        reg->CX -= reg->BX;
        break;
    case CX:
        reg->CX -= reg->CX;
        break;
    case DX:
        reg->CX -= reg->DX;
        break;
    case EAX:
        reg->CX -= reg->EAX;
        break;
    case EBX:
        reg->CX -= reg->EBX;
        break;
    case ECX:
        reg->CX -= reg->ECX;
        break;
    case EDX:
        reg->CX -= reg->EDX;
        break;
    case PC:
        reg->CX -= reg->PC;
        break;
    case SI:
        reg->CX -= reg->SI;
        break;
    case DI:
        reg->CX -= reg->DI;
        break;
    default:
        break;
    }
        break;
    case DX:
                    switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->DX -= reg->AX;
        break;
    case BX:
        reg->DX -= reg->BX;
        break;
    case CX:
        reg->DX -= reg->CX;
        break;
    case DX:
        reg->DX -= reg->DX;
        break;
    case EAX:
        reg->DX -= reg->EAX;
        break;
    case EBX:
        reg->DX -= reg->EBX;
        break;
    case ECX:
        reg->DX -= reg->ECX;
        break;
    case EDX:
        reg->DX -= reg->EDX;
        break;
    case PC:
        reg->DX -= reg->PC;
        break;
    case SI:
        reg->DX -= reg->SI;
        break;
    case DI:
        reg->DX -= reg->DI;
        break;
    default:
        break;
    }
        break;
    case EAX:
                switch (deCharAEnum(registroOrigen))
        {
    case AX:
        reg->EAX -= reg->AX;
        break;
    case BX:
        reg->EAX -= reg->BX;
        break;
    case CX:
        reg->EAX -= reg->CX;
        break;
    case DX:
        reg->EAX -= reg->DX;
        break;
    case EAX:
        reg->EAX -= reg->EAX;
        break;
    case EBX:
        reg->EAX -= reg->EBX;
        break;
    case ECX:
        reg->EAX -= reg->ECX;
        break;
    case EDX:
        reg->EAX -= reg->EDX;
        break;
    case PC:
        reg->EAX -= reg->PC;
        break;
    case SI:
        reg->EAX -= reg->SI;
        break;
    case DI:
        reg->EAX -= reg->DI;
        break;
    default:
        break;
    }
        break;
    case EBX:
switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->EBX -= reg->AX;
        break;
    case BX:
        reg->EBX -= reg->BX;
        break;
    case CX:
        reg->EBX -= reg->CX;
        break;
    case DX:
        reg->EBX -= reg->DX;
        break;
    case EAX:
        reg->EBX -= reg->EAX;
        break;
    case EBX:
        reg->EBX -= reg->EBX;
        break;
    case ECX:
        reg->EBX -= reg->ECX;
        break;
    case EDX:
        reg->EBX -= reg->EDX;
        break;
    case PC:
        reg->EBX -= reg->PC;
        break;
    case SI:
        reg->EBX -= reg->SI;
        break;
    case DI:
        reg->EBX -= reg->DI;
        break;
    default:
        break;
    }
        break;
    case ECX:
                            switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->ECX -= reg->AX;
        break;
    case BX:
        reg->ECX -= reg->BX;
        break;
    case CX:
        reg->ECX -= reg->CX;
        break;
    case DX:
        reg->ECX -= reg->DX;
        break;
    case EAX:
        reg->ECX -= reg->EAX;
        break;
    case EBX:
        reg->ECX -= reg->EBX;
        break;
    case ECX:
        reg->ECX -= reg->ECX;
        break;
    case EDX:
        reg->ECX -= reg->EDX;
        break;
    case PC:
        reg->ECX -= reg->PC;
        break;
    case SI:
        reg->ECX -= reg->SI;
        break;
    case DI:
        reg->ECX -= reg->DI;
        break;
    default:
        break;
    }
        break;
    case EDX:
        switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->EDX -= reg->AX;
        break;
    case BX:
        reg->EDX -= reg->BX;
        break;
    case CX:
        reg->EDX -= reg->CX;
        break;
    case DX:
        reg->EDX -= reg->DX;
        break;
    case EAX:
        reg->EDX -= reg->EAX;
        break;
    case EBX:
        reg->EDX -= reg->EBX;
        break;
    case ECX:
        reg->EDX -= reg->ECX;
        break;
    case EDX:
        reg->EDX -= reg->EDX;
        break;
    case PC:
        reg->EDX -= reg->PC;
        break;
    case SI:
        reg->EDX -= reg->SI;
        break;
    case DI:
        reg->EDX -= reg->DI;
        break;
    default:
        break;
    }
        break;
    case PC:
        switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->PC -= reg->AX;
        break;
    case BX:
        reg->PC -= reg->BX;
        break;
    case CX:
        reg->PC -= reg->CX;
        break;
    case DX:
        reg->PC -= reg->DX;
        break;
    case EAX:
        reg->PC -= reg->EAX;
        break;
    case EBX:
        reg->PC -= reg->EBX;
        break;
    case ECX:
        reg->PC -= reg->ECX;
        break;
    case EDX:
        reg->PC -= reg->EDX;
        break;
    case PC:
        reg->PC -= reg->PC;
        break;
    case SI:
        reg->PC -= reg->SI;
        break;
    case DI:
        reg->PC -= reg->DI;
        break;
    default:
        break;
    }
        break;
    case SI:
        switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->SI -= reg->AX;
        break;
    case BX:
        reg->SI -= reg->BX;
        break;
    case CX:
        reg->SI -= reg->CX;
        break;
    case DX:
        reg->SI -= reg->DX;
        break;
    case EAX:
        reg->SI -= reg->EAX;
        break;
    case EBX:
        reg->SI -= reg->EBX;
        break;
    case ECX:
        reg->SI -= reg->ECX;
        break;
    case EDX:
        reg->SI -= reg->EDX;
        break;
    case PC:
        reg->SI -= reg->PC;
        break;
    case SI:
        reg->SI -= reg->SI;
        break;
    case DI:
        reg->SI -= reg->DI;
        break;
    default:
        break;
    }
        break;
    case DI:
        switch (deCharAEnum(registroOrigen))
    {
    case AX:
        reg->DI -= reg->AX;
        break;
    case BX:
        reg->DI -= reg->BX;
        break;
    case CX:
        reg->DI -= reg->CX;
        break;
    case DX:
        reg->DI -= reg->DX;
        break;
    case EAX:
        reg->DI -= reg->EAX;
        break;
    case EBX:
        reg->DI -= reg->EBX;
        break;
    case ECX:
        reg->DI -= reg->ECX;
        break;
    case EDX:
        reg->DI -= reg->EDX;
        break;
    case PC:
        reg->DI -= reg->PC;
        break;
    case SI:
        reg->DI -= reg->SI;
        break;
    case DI:
        reg->DI -= reg->DI;
        break;
    default:
        break;
    }
        break;
    default:
        break;
    }
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

void io_gen_sleep (char * interfaz, char * unidadesDeTrabajo, int socket_kernel)
{   char* mensaje = string_new();
    //Paramos contador
    temporal_stop(temporizador);
    int64_t tiempo_transcurrido = temporal_gettime(temporizador);
    if((contexto->quantum - (int) tiempo_transcurrido) <= 0){
        contexto->quantum = 0;
    }else{
        contexto->quantum = contexto->quantum - (int) tiempo_transcurrido;
    }

    string_append(&mensaje, "IO_GEN_SLEEP ");
    string_append(&mensaje, interfaz);
    string_append(&mensaje, " ");
    string_append(&mensaje, unidadesDeTrabajo);
    string_append(&mensaje, " ");
    string_append(&mensaje, string_itoa(contexto->pid));
    enviar_mensaje(mensaje, socket_kernel);
    log_info(logger, "Se ejecuto la instrucción IO_GEN_SLEEP con los parametros interfaz %s y unidades de trabajo %s", interfaz, unidadesDeTrabajo);
}

void io_stdin_read (char * interfaz, char * direccion, char * tamanio, int socket_kernel)
{   char* mensaje = string_new();
    //Paramos contador
    temporal_stop(temporizador);
    int64_t tiempo_transcurrido = temporal_gettime(temporizador);
    if((contexto->quantum - (int) tiempo_transcurrido) <= 0){
        contexto->quantum = 0;
    }else{
        contexto->quantum = contexto->quantum - (int) tiempo_transcurrido;
    }
    
    string_append(&mensaje, "IO_STDIN_READ ");
    string_append(&mensaje, interfaz);
    string_append(&mensaje, " ");
    string_append(&mensaje, direccion);
    string_append(&mensaje, " ");
    string_append(&mensaje, tamanio);
    string_append(&mensaje, " ");
    string_append(&mensaje, string_itoa(contexto->pid));
    enviar_mensaje(mensaje, socket_kernel);
    log_info(logger, "Se ejecuto la instrucción IO_STDIN_READ con los parametros interfaz %s , direccion %s , tamanio %s y pid %d", interfaz, direccion, tamanio, contexto->pid);
}

void io_stdout_write (char * interfaz, char * direccion, char * tamanio, int socket_kernel)
{   char* mensaje = string_new();
    //Paramos contador
    temporal_stop(temporizador);
    int64_t tiempo_transcurrido = temporal_gettime(temporizador);
    if((contexto->quantum - (int) tiempo_transcurrido) <= 0){
        contexto->quantum = 0;
    }else{
        contexto->quantum = contexto->quantum - (int) tiempo_transcurrido;
    }

    string_append(&mensaje, "IO_STDOUT_WRITE ");
    string_append(&mensaje, interfaz);
    string_append(&mensaje, " ");
    string_append(&mensaje, direccion);
    string_append(&mensaje, " ");
    string_append(&mensaje, tamanio);
    string_append(&mensaje, " ");
    string_append(&mensaje, string_itoa(contexto->pid));
    enviar_mensaje(mensaje, socket_kernel);
    log_info(logger, "Se ejecuto la instrucción IO_STDOUT_WRITE con los parametros interfaz %s , direccion %s , tamanio %s y pid %d", interfaz, direccion, tamanio, contexto->pid);
}

void exit_inst(int socket_kernel){
    //TODO Ver que implica el exit
    enviar_pcb_contexto(socket_kernel, contexto);
}

void mov_in(char* registro, char * direccion, int socket_cliente_memoria){
    
    int num_pag = calcular_num_pagina(valor_registro);
    int bytes_hasta_final = tamanio_registro;
    int offset = calcular_desplazamiento(valor_registro, num_pag);
    char* mensaje = string_new();//Enviar a Memoria=>"Direccion_fisica valor tamanio"
    void* valor_leido = malloc(tamanio_registro);
    if(list_size(lista_marcos) > 1){
        for(int i= 0; i < list_size(lista_marcos), i++){//[3,4,78]
            int dir_fisica;
            int tamanio_lectura;
            if(i == 0){
                bytes_hasta_final = bytes_hasta_final - (tamanio_pagina - offset);
                dir_fisica = (list_get(lista_marcos, i) * tamanio_pagina) + offset;
                tamanio_lectura = tamanio_pagina - offset;
                mensaje = generar_mensaje_lectura(dir_fisica, tamanio_lectura);
                enviar_mensaje(mensaje ,socket_cliente_memoria);
                void* resultado = malloc(tamanio_lectura);
                resultado = recibir_desde_memoria(socket_cliente_memoria);
                memcpy(valor_leido, resultado, tamanio_lectura);
            }else if(i > 0 && i < (list_size(lista_marcos) -1)){
                bytes_hasta_final = bytes_hasta_final - tamanio_pagina;
                dir_fisica = (list_get(lista_marcos, i) * tamanio_pagina);
                tamanio_lectura = tamanio_pagina;
                mensaje = generar_mensaje_lectura(dir_fisica, tamanio_lectura);
                enviar_mensaje(mensaje ,socket_cliente_memoria);
                void* resultado = malloc(tamanio_lectura);
                resultado = recibir_desde_memoria(socket_cliente_memoria);
                int aux = (tamanio_pagina-offset) + (tamanio_pagina * (i-1));
                memcpy(valor_leido+aux, resultado, tamanio_lectura);
            }else if(i == (list_size(lista_marcos)-1)){
                dir_fisica = (list_get(lista_marcos, i) * tamanio_pagina);
                tamanio_lectura = bytes_hasta_final;
                mensaje = generar_mensaje_lectura(dir_fisica, tamanio_lectura);
                enviar_mensaje(mensaje ,socket_cliente_memoria);
                void* resultado = malloc(tamanio_lectura);
                resultado = recibir_desde_memoria(socket_cliente_memoria);
                int aux = tamanio_registro-tamanio_lectura;
                memcpy(valor_leido+aux, resultado, tamanio_lectura);
            }
            
        }
    }else{
        int dir_fisica = (list_get(lista_marcos, 0) * tamanio_pagina) + offset;
        int tamanio_lectura = bytes_hasta_final;
        mensaje = generar_mensaje_lectura(dir_fisica, tamanio_lectura);
        enviar_mensaje(mensaje ,socket_cliente_memoria);
        valor_leido = recibir_desde_memoria(socket_cliente_memoria);
    }

    set_valor_registro(instruccion_exec_split[1], valor_leido);
    //log_info(logger, "Se ejecuto la instrucción IO_STDOUT_WRITE con los parametros interfaz %s , direccion %s , tamanio %s y pid %d", interfaz, direccion, tamanio, contexto->pid);
}

void mov_out(){
    int num_pag = calcular_num_pagina(valor_registro);
    int bytes_hasta_final = tamanio_registro;
    int offset = calcular_desplazamiento(valor_registro, num_pag);
    char* mensaje = string_new();//Enviar a Memoria=>"Direccion_fisica valor tamanio"
    void* valor_leido = malloc(tamanio_registro);
    if(list_size(lista_marcos) > 1){
        for(int i= 0; i < list_size(lista_marcos), i++){//[3,4,78]
            int dir_fisica;
            int tamanio_lectura;
            if(i == 0){
                bytes_hasta_final = bytes_hasta_final - (tamanio_pagina - offset);
                dir_fisica = (list_get(lista_marcos, i) * tamanio_pagina) + offset;
                tamanio_lectura = tamanio_pagina - offset;
                mensaje = generar_mensaje_lectura(dir_fisica, tamanio_lectura);
                enviar_mensaje(mensaje ,socket_cliente_memoria);
                void* resultado = malloc(tamanio_lectura);
                resultado = recibir_desde_memoria(socket_cliente_memoria);
                memcpy(valor_leido, resultado, tamanio_lectura);
            }else if(i > 0 && i < (list_size(lista_marcos) -1)){
                bytes_hasta_final = bytes_hasta_final - tamanio_pagina;
                dir_fisica = (list_get(lista_marcos, i) * tamanio_pagina);
                tamanio_lectura = tamanio_pagina;
                mensaje = generar_mensaje_lectura(dir_fisica, tamanio_lectura);
                enviar_mensaje(mensaje ,socket_cliente_memoria);
                void* resultado = malloc(tamanio_lectura);
                resultado = recibir_desde_memoria(socket_cliente_memoria);
                int aux = (tamanio_pagina-offset) + (tamanio_pagina * (i-1));
                memcpy(valor_leido+aux, resultado, tamanio_lectura);
            }else if(i == (list_size(lista_marcos)-1)){
                dir_fisica = (list_get(lista_marcos, i) * tamanio_pagina);
                tamanio_lectura = bytes_hasta_final;
                mensaje = generar_mensaje_lectura(dir_fisica, tamanio_lectura);
                enviar_mensaje(mensaje ,socket_cliente_memoria);
                void* resultado = malloc(tamanio_lectura);
                resultado = recibir_desde_memoria(socket_cliente_memoria);
                int aux = tamanio_registro-tamanio_lectura;
                memcpy(valor_leido+aux, resultado, tamanio_lectura);
            }
            
        }
    }else{
        int dir_fisica = (list_get(lista_marcos, 0) * tamanio_pagina) + offset;
        int tamanio_lectura = bytes_hasta_final;
        mensaje = generar_mensaje_lectura(dir_fisica, tamanio_lectura);
        enviar_mensaje(mensaje ,socket_cliente_memoria);
        valor_leido = recibir_desde_memoria(socket_cliente_memoria);
    }

    set_valor_registro(instruccion_exec_split[1], valor_leido);
}

void resize(){

}

void copy_string(){
    
}

void wait(){
    //No hace nada
}

void signal_instruccion(){
    //No hace nada
}

void io_fs_create(){
    
}

void io_fs_delete(){
    
}

void io_fs_truncate(){
    
}

void io_fs_write(){
    
}

void io_fs_read(){
    
}