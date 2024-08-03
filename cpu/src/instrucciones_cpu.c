#include <../include/instrucciones_cpu.h>

int deCharAEnum(char *registro)
{
    if (strcmp(registro, "AX") == 0)
    {
        return AX;
    }
    if (strcmp(registro, "BX") == 0)
    {
        return BX;
    }
    if (strcmp(registro, "CX") == 0)
    {
        return CX;
    }
    if (strcmp(registro, "DX") == 0)
    {
        return DX;
    }
    if (strcmp(registro, "EAX") == 0)
    {
        return EAX;
    }
    if (strcmp(registro, "EBX") == 0)
    {
        return EBX;
    }
    if (strcmp(registro, "ECX") == 0)
    {
        return ECX;
    }
    if (strcmp(registro, "EDX") == 0)
    {
        return EDX;
    }
    if (strcmp(registro, "PC") == 0)
    {
        return PC;
    }
    if (strcmp(registro, "SI") == 0)
    {
        return SI;
    }
    if (strcmp(registro, "DI") == 0)
    {
        return DI;
    }
}

void ejecutarSentencia(int socket_kernel, int socket_cliente_memoria)
{

    char **sentenciasSplit = string_split(instruccion_exec, " ");
    if (strcmp(sentenciasSplit[0], "SET") == 0)
        set(sentenciasSplit[1], sentenciasSplit[2]);
    if (strcmp(sentenciasSplit[0], "MOV_IN") == 0)
        mov_in(sentenciasSplit[1], sentenciasSplit[2], socket_cliente_memoria);
    if (strcmp(sentenciasSplit[0], "MOV_OUT") == 0)
        mov_out(sentenciasSplit[1], sentenciasSplit[2], socket_cliente_memoria);
    if (strcmp(sentenciasSplit[0], "SUM") == 0)
        sum(sentenciasSplit[1], sentenciasSplit[2]);
    if (strcmp(sentenciasSplit[0], "SUB") == 0)
        sub(sentenciasSplit[1], sentenciasSplit[2]);
    if (strcmp(sentenciasSplit[0], "JNZ") == 0)
        jnz(sentenciasSplit[1], sentenciasSplit[2]);
    if (strcmp(sentenciasSplit[0], "RESIZE") == 0)
        resize(sentenciasSplit[1], socket_cliente_memoria);
    if (strcmp(sentenciasSplit[0], "COPY_STRING") == 0)
        copy_string(sentenciasSplit[1], socket_cliente_memoria);
    if (strcmp(sentenciasSplit[0], "WAIT") == 0)
        wait(sentenciasSplit[1]);
    if (strcmp(sentenciasSplit[0], "SIGNAL") == 0)
        signal_instruccion(sentenciasSplit[1]);
    if (strcmp(sentenciasSplit[0], "IO_GEN_SLEEP") == 0)
        io_gen_sleep(sentenciasSplit[1], sentenciasSplit[2], socket_kernel);
    if (strcmp(sentenciasSplit[0], "IO_STDIN_READ") == 0)
        io_stdin_read(sentenciasSplit[1], sentenciasSplit[2], sentenciasSplit[3], socket_kernel);
    if (strcmp(sentenciasSplit[0], "IO_STDOUT_WRITE") == 0)
        io_stdout_write(sentenciasSplit[1], sentenciasSplit[2], sentenciasSplit[3], socket_kernel);
    if (strcmp(sentenciasSplit[0], "IO_FS_CREATE") == 0)
        io_fs_create(sentenciasSplit[1], sentenciasSplit[2], socket_kernel);
    if (strcmp(sentenciasSplit[0], "IO_FS_DELETE") == 0)
        io_fs_delete(sentenciasSplit[1], sentenciasSplit[2], socket_kernel);
    if (strcmp(sentenciasSplit[0], "IO_FS_TRUNCATE") == 0)
        io_fs_truncate(sentenciasSplit[1], sentenciasSplit[2], sentenciasSplit[3], socket_kernel);
    if (strcmp(sentenciasSplit[0], "IO_FS_WRITE") == 0)
        io_fs_write(sentenciasSplit[1], sentenciasSplit[2], sentenciasSplit[3], sentenciasSplit[4], sentenciasSplit[5], socket_kernel);
    if (strcmp(sentenciasSplit[0], "IO_FS_READ") == 0)
        io_fs_read(sentenciasSplit[1], sentenciasSplit[2], sentenciasSplit[3], sentenciasSplit[4], sentenciasSplit[5], socket_kernel);
    if (strcmp(sentenciasSplit[0], "EXIT") == 0)
        exit_inst(socket_kernel);
}

void set(char *registro, char *valor)
{
    int valorASetear = atoi(valor);
    switch (deCharAEnum(registro))
    {
    case AX:
        reg->AX = valorASetear;
        break;
    case BX:
        reg->BX = valorASetear;
        break;
    case CX:
        reg->CX = valorASetear;
        break;
    case DX:
        reg->DX = valorASetear;
        break;
    case EAX:
        reg->EAX = valorASetear;
        break;
    case EBX:
        reg->EBX = valorASetear;
        break;
    case ECX:
        reg->ECX = valorASetear;
        break;
    case EDX:
        reg->EDX = valorASetear;
        break;
    case PC:
        reg->PC = valorASetear;
        break;
    case SI:
        reg->SI = valorASetear;
        break;
    case DI:
        reg->DI = valorASetear;
        break;
    default:
        break;
    }
    log_info(logger, "PID: %d - Ejecutando: SET - %s %s", contexto->pid, registro, valor);
}

void sum(char *registroDestino, char *registroOrigen)
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
    log_info(logger, "PID: %d - Ejecutando: SUM - %s %s", contexto->pid, registroDestino, registroOrigen);
}

void sub(char *registroDestino, char *registroOrigen)
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
    log_info(logger, "PID: %d - Ejecutando: SUB - %s %s", contexto->pid,registroDestino, registroOrigen);
}

void jnz(char *registro, char *instruccion)
{
    int valorASetear = atoi(instruccion);
    switch (deCharAEnum(registro))
    {
    case AX:
        if (reg->AX != 0)
            reg->PC = valorASetear;
        break;
    case BX:
        if (reg->BX != 0)
            reg->PC = valorASetear;
        break;
    case CX:
        if (reg->CX != 0)
            reg->PC = valorASetear;
        break;
    case DX:
        if (reg->DX != 0)
            reg->PC = valorASetear;
        break;
    case EAX:
        if (reg->EAX != 0)
            reg->PC = valorASetear;
        break;
    case EBX:
        if (reg->EBX != 0)
            reg->PC = valorASetear;
        break;
    case ECX:
        if (reg->ECX != 0)
            reg->PC = valorASetear;
        break;
    case EDX:
        if (reg->EDX != 0)
            reg->PC = valorASetear;
        break;
    case PC:
        if (reg->PC != 0)
            reg->PC = valorASetear;
        break;
    case SI:
        if (reg->SI != 0)
            reg->PC = valorASetear;
        break;
    case DI:
        if (reg->DI != 0)
            reg->PC = valorASetear;
        break;
    default:
        break;
    }
    log_info(logger, "PID: %d - Ejecutando: JNZ - %s %s", contexto->pid, registro, instruccion);
}

void io_gen_sleep(char *interfaz, char *unidadesDeTrabajo, int socket_kernel)
{
    char *mensaje = string_new();
    // Paramos contador
    temporal_stop(temporizador);
    int64_t tiempo_transcurrido = temporal_gettime(temporizador);
    if ((contexto->quantum - (int)tiempo_transcurrido) <= 0)
    {
        contexto->quantum = 0;
    }
    else
    {
        contexto->quantum = contexto->quantum - (int)tiempo_transcurrido;
    }

    string_append(&mensaje, "IO_GEN_SLEEP ");
    string_append(&mensaje, interfaz);
    string_append(&mensaje, " ");
    string_append(&mensaje, unidadesDeTrabajo);
    string_append(&mensaje, " ");
    string_append(&mensaje, string_itoa(contexto->pid));
    enviar_mensaje(mensaje, socket_kernel);
    log_info(logger, "PID: %d - Ejecutando: IO_GEN_SLEEP - %s %s", contexto->pid, interfaz, unidadesDeTrabajo);
}

void io_stdin_read(char *interfaz, char *registro_direccion, char *registro_tamanio, int socket_kernel)
{
    char *mensaje = string_new();
    // Paramos contador
    temporal_stop(temporizador);
    int64_t tiempo_transcurrido = temporal_gettime(temporizador);
    if ((contexto->quantum - (int)tiempo_transcurrido) <= 0)
    {
        contexto->quantum = 0;
    }
    else
    {
        contexto->quantum = contexto->quantum - (int)tiempo_transcurrido;
    }
    int direccion = get_valor_registro(registro_direccion);
    int tamanio = get_valor_registro(registro_tamanio);
    string_append(&mensaje, "IO_STDIN_READ ");
    string_append(&mensaje, interfaz);
    string_append(&mensaje, " ");
    string_append(&mensaje, string_itoa(contexto->pid));
    string_append(&mensaje, " ");
    char* marcos = iterar_lista_mensaje(lista_marcos,direccion,tamanio);
    string_append(&mensaje, marcos);
    enviar_mensaje(mensaje, socket_kernel);
    log_info(logger, "PID: %d - Ejecutando: IO_STDIN_READ - %s %s %s", contexto->pid, interfaz, registro_direccion, registro_tamanio);
}

void io_stdout_write(char *interfaz, char *registro_direccion, char *registro_tamanio, int socket_kernel)
{
    char *mensaje = string_new();
    // Paramos contador
    temporal_stop(temporizador);
    int64_t tiempo_transcurrido = temporal_gettime(temporizador);
    if ((contexto->quantum - (int)tiempo_transcurrido) <= 0)
    {
        contexto->quantum = 0;
    }
    else
    {
        contexto->quantum = contexto->quantum - (int)tiempo_transcurrido;
    }
    int direccion = get_valor_registro(registro_direccion);
    int tamanio = get_valor_registro(registro_tamanio);
    string_append(&mensaje, "IO_STDOUT_WRITE ");
    string_append(&mensaje, interfaz);
    string_append(&mensaje, " ");
    string_append(&mensaje, string_itoa(contexto->pid));
    string_append(&mensaje, " ");
    char* marcos = iterar_lista_mensaje(lista_marcos,direccion,tamanio);
    string_append(&mensaje, marcos);
    enviar_mensaje(mensaje, socket_kernel);
    log_info(logger, "PID: %d - Ejecutando: IO_STDOUT_WRITE - %s %s %s", contexto->pid, interfaz, registro_direccion, registro_tamanio);
}

void exit_inst(int socket_kernel)
{
    temporal_stop(temporizador);
    int64_t tiempo_transcurrido = temporal_gettime(temporizador);
    if ((contexto->quantum - (int)tiempo_transcurrido) <= 0)
    {
        contexto->quantum = 0;
    }
    else
    {
        contexto->quantum = contexto->quantum - (int)tiempo_transcurrido;
    }
    log_info(logger, "PID: %d - Ejecutando: EXIT", contexto->pid);
}

void mov_in(char *registro_dato, char *registro_direccion, int socket_cliente_memoria)
{
    int valor_registro = get_valor_registro(registro_direccion);
    int num_pag = calcular_num_pagina(valor_registro);
    int tamanio_registro = get_tamanio_registro(registro_dato);
    int bytes_hasta_final = tamanio_registro;
    int offset = calcular_desplazamiento(valor_registro, num_pag);
    char *mensaje = string_new();
    void *valor_leido = malloc(tamanio_registro);
    char* valor_leido_prueba;
    if (list_size(lista_marcos) > 1)
    {
        for (int i = 0; i < list_size(lista_marcos); i++)
        {
            int dir_fisica;
            int tamanio_lectura;
            if (i == 0)
            {
                bytes_hasta_final = bytes_hasta_final - (tamanio_pagina - offset);
                int marco = list_get(lista_marcos, i);
                dir_fisica = (marco * tamanio_pagina) + offset;
                tamanio_lectura = tamanio_pagina - offset;
                mensaje = generar_mensaje_lectura(dir_fisica, tamanio_lectura);
                enviar_mensaje(mensaje, socket_cliente_memoria);
                //void *resultado = malloc(tamanio_lectura);
                //resultado = recibir_desde_memoria(socket_cliente_memoria);
                valor_leido_prueba = recibir_desde_memoria(socket_cliente_memoria);
                //memcpy(valor_leido, resultado, tamanio_lectura);
                //char* valor_mostrar = convertir_void_a_char(tamanio_lectura, resultado);
                log_info(logger, "PID: %d - Acción: LEER - Dirección Física: %d - Valor: %s", contexto->pid, dir_fisica, valor_leido_prueba);
            }
            else if (i > 0 && i < (list_size(lista_marcos) - 1))
            {
                bytes_hasta_final = bytes_hasta_final - tamanio_pagina;
                int marco = list_get(lista_marcos, i);
                dir_fisica = (marco * tamanio_pagina);
                tamanio_lectura = tamanio_pagina;
                mensaje = generar_mensaje_lectura(dir_fisica, tamanio_lectura);
                enviar_mensaje(mensaje, socket_cliente_memoria);
                //void *resultado = malloc(tamanio_lectura);
                //resultado = recibir_desde_memoria(socket_cliente_memoria);
                valor_leido_prueba = recibir_desde_memoria(socket_cliente_memoria);
                //int aux = (tamanio_pagina - offset) + (tamanio_pagina * (i - 1));
                //memcpy(valor_leido + aux, resultado, tamanio_lectura);
                //char* valor_mostrar = convertir_void_a_char(tamanio_lectura, resultado);
                log_info(logger, "PID: %d - Acción: LEER - Dirección Física: %d - Valor: %s", contexto->pid, dir_fisica, valor_leido_prueba);
            }
            else if (i == (list_size(lista_marcos) - 1))
            {
                int marco = list_get(lista_marcos, i);
                dir_fisica = (marco * tamanio_pagina);
                tamanio_lectura = bytes_hasta_final;
                mensaje = generar_mensaje_lectura(dir_fisica, tamanio_lectura);
                enviar_mensaje(mensaje, socket_cliente_memoria);
                //void *resultado = malloc(tamanio_lectura);
                //resultado = recibir_desde_memoria(socket_cliente_memoria);
                valor_leido_prueba = recibir_desde_memoria(socket_cliente_memoria);
                //int aux = tamanio_registro - tamanio_lectura;
                //memcpy(valor_leido + aux, resultado, tamanio_lectura);
                //char* valor_mostrar = convertir_void_a_char(tamanio_lectura, resultado);
                log_info(logger, "PID: %d - Acción: LEER - Dirección Física: %d - Valor: %s", contexto->pid, dir_fisica, valor_leido_prueba);
            }
        }
    }
    else
    {
        int marco = list_get(lista_marcos, 0);
        int dir_fisica = (marco * tamanio_pagina) + offset;
        int tamanio_lectura = bytes_hasta_final;
        mensaje = generar_mensaje_lectura(dir_fisica, tamanio_lectura);
        enviar_mensaje(mensaje, socket_cliente_memoria);
        //valor_leido = recibir_desde_memoria(socket_cliente_memoria);
        valor_leido_prueba = recibir_desde_memoria(socket_cliente_memoria);
        //char* valor_mostrar = convertir_void_a_char(tamanio_lectura, valor_leido);
        log_info(logger, "PID: %d - Acción: LEER - Dirección Física: %d - Valor: %s", contexto->pid, dir_fisica, valor_leido_prueba);
    }

    set_valor_registro(registro_dato, atoi(valor_leido_prueba));
    log_info(logger, "PID: %d - Ejecutando: MOV_IN - %s %s", contexto->pid, registro_dato, registro_direccion);
}

void mov_out(char *registro_direccion, char *registro_dato, int socket_cliente_memoria)
{
    int valor_registro = get_valor_registro(registro_direccion);
    int valor_registro_dato = get_valor_registro(registro_dato);
    int num_pag = calcular_num_pagina(valor_registro);
    int tamanio_registro = get_tamanio_registro(registro_dato);
    int bytes_hasta_final = tamanio_registro;
    int offset = calcular_desplazamiento(valor_registro, num_pag);
    char *mensaje = string_new();
    if (list_size(lista_marcos) > 1)
    {
        for (int i = 0; i < list_size(lista_marcos); i++)
        {
            int dir_fisica;
            int tamanio_escritura;
            if (i == 0)
            {
                int marco = list_get(lista_marcos, i);
                bytes_hasta_final = bytes_hasta_final - (tamanio_pagina - offset);
                dir_fisica = (marco * tamanio_pagina) + offset;
                tamanio_escritura = tamanio_pagina - offset;//escritura
                void* valor_a_escribir = malloc(tamanio_escritura);
                //memcpy(valor_a_escribir, valor_registro_dato, tamanio_escritura);
                //mensaje = generar_mensaje_escritura(dir_fisica, tamanio_escritura, valor_a_escribir);
                mensaje = generar_mensaje_escritura_prueba(dir_fisica, tamanio_escritura, string_itoa(valor_registro_dato));
                enviar_mensaje(mensaje, socket_cliente_memoria);
                char* resultado = recibir_desde_memoria(socket_cliente_memoria);
                //char* valor_mostrar = convertir_void_a_char(tamanio_escritura, valor_a_escribir);
                log_info(logger, "PID: %d - Acción: ESCRIBIR - Dirección Física: %d - Valor: %s", contexto->pid, dir_fisica, string_itoa(valor_registro_dato));
            }
            else if (i > 0 && i < (list_size(lista_marcos) - 1))
            {
                int marco = list_get(lista_marcos, i);
                dir_fisica = (marco * tamanio_pagina);
                tamanio_escritura = tamanio_pagina;
                void* valor_a_escribir = malloc(tamanio_escritura);
                //memcpy(valor_a_escribir, valor_registro_dato+tamanio_registro-bytes_hasta_final, tamanio_escritura);
                bytes_hasta_final = bytes_hasta_final - tamanio_pagina;
                //mensaje = generar_mensaje_escritura(dir_fisica, tamanio_escritura, valor_a_escribir);
                mensaje = generar_mensaje_escritura_prueba(dir_fisica, tamanio_escritura, string_itoa(valor_registro_dato));
                enviar_mensaje(mensaje, socket_cliente_memoria);
                char* resultado = recibir_desde_memoria(socket_cliente_memoria);
                //char* valor_mostrar = convertir_void_a_char(tamanio_escritura, valor_a_escribir);
                log_info(logger, "PID: %d - Acción: ESCRIBIR - Dirección Física: %d - Valor: %s", contexto->pid, dir_fisica, string_itoa(valor_registro_dato));
            }
            else if (i == (list_size(lista_marcos) - 1))
            {
                int marco = list_get(lista_marcos, i);
                dir_fisica = (marco * tamanio_pagina);
                tamanio_escritura = bytes_hasta_final;
                void* valor_a_escribir = malloc(tamanio_escritura);
                //memcpy(valor_a_escribir, valor_registro_dato+tamanio_registro-bytes_hasta_final, tamanio_escritura);
                bytes_hasta_final = bytes_hasta_final - bytes_hasta_final;
                //mensaje = generar_mensaje_escritura(dir_fisica, tamanio_escritura, valor_a_escribir);
                mensaje = generar_mensaje_escritura_prueba(dir_fisica, tamanio_escritura, string_itoa(valor_registro_dato));
                enviar_mensaje(mensaje, socket_cliente_memoria);
                char* resultado = recibir_desde_memoria(socket_cliente_memoria);
                //char* valor_mostrar = convertir_void_a_char(tamanio_escritura, valor_a_escribir);
                log_info(logger, "PID: %d - Acción: ESCRIBIR - Dirección Física: %d - Valor: %s", contexto->pid, dir_fisica, string_itoa(valor_registro_dato));
            }
        }
    }
    else
    {
        int marco = list_get(lista_marcos, 0);
        int dir_fisica = (marco * tamanio_pagina) + offset;
        int tamanio_escritura = bytes_hasta_final;
        void* valor_a_escribir = malloc(tamanio_escritura);
        //memcpy(valor_a_escribir, valor_registro_dato, tamanio_escritura);
        valor_a_escribir = valor_registro_dato;
        //mensaje = generar_mensaje_escritura(dir_fisica, tamanio_escritura, valor_a_escribir);
        mensaje = generar_mensaje_escritura_prueba(dir_fisica, tamanio_escritura, string_itoa(valor_registro_dato));
        enviar_mensaje(mensaje, socket_cliente_memoria);
        char* resultado = recibir_desde_memoria(socket_cliente_memoria);
        //char* valor_mostrar = convertir_void_a_char(tamanio_escritura, valor_a_escribir);
        //log_info(logger, "PID: %d - Acción: ESCRIBIR - Dirección Física: %d - Valor: %s", contexto->pid, dir_fisica, valor_mostrar);
        log_info(logger, "PID: %d - Acción: ESCRIBIR - Dirección Física: %d - Valor: %s", contexto->pid, dir_fisica, string_itoa(valor_registro_dato));
    }
    log_info(logger, "PID: %d - Ejecutando: MOV_OUT - %s %s", contexto->pid, registro_direccion, registro_dato);
}

void resize(char *tamanio, int socket_cliente_memoria)
{

    char *mensaje = string_new();
    string_append(&mensaje, "RESIZE ");
    string_append(&mensaje, tamanio);
    string_append(&mensaje, " ");
    string_append(&mensaje, string_itoa(contexto->pid));
    enviar_mensaje(mensaje, socket_cliente_memoria);
    log_info(logger, "PID: %d - Ejecutando: RESIZE - %s", contexto->pid, tamanio);
    int cod_op = recibir_operacion(socket_cliente_memoria);
    switch (cod_op)
    {
    case MENSAJE:
        int size;
        char *buffer = recibir_buffer(&size, socket_cliente_memoria);
        log_info(logger, "Me llego el mensaje %s", buffer);
        char **mensaje_split = string_split(buffer, " ");
        if (strcmp(mensaje_split[0], "RESIZE") == 0 && strcmp(mensaje_split[1], "OUT_OF_MEMORY") == 0)
        {
            desalojo_out_of_memory = 1;
            temporal_stop(temporizador);
            int64_t tiempo_transcurrido = temporal_gettime(temporizador);
            if ((contexto->quantum - (int)tiempo_transcurrido) <= 0)
            {
                contexto->quantum = 0;
            }
            else
            {
                contexto->quantum = contexto->quantum - (int)tiempo_transcurrido;
            }
        }
        if (strcmp(mensaje_split[0], "RESIZE") == 0 && strcmp(mensaje_split[1], "OK") == 0)
        {
            log_info(logger, "Se ejecuto correctamente RESIZE");
        }
        free(buffer);
        break;

    case -1:
        log_error(logger, "el cliente se desconecto.");
        return EXIT_FAILURE;
    default:
        log_warning(logger, "Operacion desconocida. No quieras meter la pata");
        break;
    }
}

void copy_string(char *tamanio, int socket_cliente_memoria)
{
    int numero_tamanio = atoi(tamanio);
    unsigned int valor_registro_si = get_valor_registro("SI");
    unsigned int valor_registro_di = get_valor_registro("DI");
    int numero_pagina_si = floor(valor_registro_si / tamanio_pagina);
    int desplazamiento_si = valor_registro_si - numero_pagina_si * tamanio_pagina;
    int numero_pagina_di = floor(valor_registro_di / tamanio_pagina);
    int desplazamiento_di = valor_registro_di - numero_pagina_di * tamanio_pagina;
    int bytes_hasta_final = numero_tamanio;

    // Almacena lo que tiene origen
    //void *origen_leido = malloc(numero_tamanio);
    char* origen_leido;
    char* valor_a_grabar = string_new();
    for (int i = 0; i < list_size(lista_marcos); i++)
    {

        int aux_origen = list_get(lista_marcos, i);
        char *mensaje = string_new();
        int df_origen;
        int tamanio_lectura;
        if (i == 0)
        {
            df_origen = aux_origen * tamanio_pagina + desplazamiento_si;
            if (list_size(lista_marcos) == 1)
            {
                tamanio_lectura = numero_tamanio;
            }
            else
            {
                tamanio_lectura = tamanio_pagina - desplazamiento_si;
            }
            mensaje = generar_mensaje_lectura(df_origen, tamanio_lectura);
        }
        else if (i > 0 && i < (list_size(lista_marcos) - 1))
        {
            df_origen = aux_origen * tamanio_pagina;
            tamanio_lectura = tamanio_pagina;
            mensaje = generar_mensaje_lectura(df_origen, tamanio_lectura);
        }
        else if (i == (list_size(lista_marcos) - 1))
        {
            tamanio_lectura = bytes_hasta_final;
            df_origen = aux_origen * tamanio_pagina;
            mensaje = generar_mensaje_lectura(df_origen, tamanio_lectura);
        }
        enviar_mensaje(mensaje, socket_cliente_memoria);
        //void *resultado = malloc(tamanio_lectura);
        origen_leido = recibir_desde_memoria(socket_cliente_memoria);
        //char* valor_mostrar = convertir_void_a_char(tamanio_lectura, resultado);
        log_info(logger, "PID: %d - Acción: LEER - Dirección Física: %d - Valor: %s", contexto->pid, df_origen, origen_leido);

        //memcpy(origen_leido + (numero_tamanio - bytes_hasta_final), resultado, tamanio_lectura);
        bytes_hasta_final = bytes_hasta_final - tamanio_lectura;
        string_append(&valor_a_grabar, origen_leido);
    }
    void *valor_a_escribir;
    int bytes_a_escribir = numero_tamanio;

    for (int i = 0; i < list_size(lista_marcos_destino); i++)
    {

        int aux_destino = list_get(lista_marcos_destino, i);
        void *mensaje;
        int df_destino;
        int tamanio_escritura;
        if (i == 0)
        {
            df_destino = aux_destino * tamanio_pagina + desplazamiento_di;
            if (list_size(lista_marcos_destino) == 1)
            {
                tamanio_escritura = numero_tamanio;
            }
            else
            {
                tamanio_escritura = tamanio_pagina - desplazamiento_di;
            }
            valor_a_escribir = malloc(tamanio_escritura);
            memcpy(valor_a_escribir, valor_a_grabar + (numero_tamanio - bytes_a_escribir), tamanio_escritura);
            mensaje = generar_mensaje_escritura(df_destino, tamanio_escritura, valor_a_escribir);
        }
        else if (i > 0 && i < (list_size(lista_marcos_destino) - 1))
        {
            df_destino = aux_destino * tamanio_pagina;
            tamanio_escritura = tamanio_pagina;
            valor_a_escribir = realloc(valor_a_escribir, tamanio_escritura);
            memcpy(valor_a_escribir, valor_a_grabar + (numero_tamanio - bytes_a_escribir), tamanio_escritura);
            mensaje = generar_mensaje_escritura(df_destino, tamanio_escritura, valor_a_escribir);
        }
        else if (i == (list_size(lista_marcos_destino) - 1))
        {
            tamanio_escritura = bytes_a_escribir;
            df_destino = aux_destino * tamanio_pagina;
            valor_a_escribir = realloc(valor_a_escribir, tamanio_escritura);
            memcpy(valor_a_escribir, valor_a_grabar + (numero_tamanio - bytes_a_escribir), tamanio_escritura);
            mensaje = generar_mensaje_escritura(df_destino, tamanio_escritura, valor_a_escribir);
        }
        enviar_mensaje(mensaje, socket_cliente_memoria);
        char* valor_mostrar = convertir_void_a_char(tamanio_escritura, valor_a_escribir);
        log_info(logger, "PID: %d - Acción: ESCRIBIR - Dirección Física: %d - Valor: %s", contexto->pid, df_destino, valor_mostrar);

        bytes_a_escribir = bytes_a_escribir - tamanio_escritura;
    }
    log_info(logger, "PID: %d - Ejecutando: COPY_STRING - %s", contexto->pid, tamanio);
}

void wait(char* recurso)
{       temporal_stop(temporizador);
    int64_t tiempo_transcurrido = temporal_gettime(temporizador);
    if ((contexto->quantum - (int)tiempo_transcurrido) <= 0)
    {
        contexto->quantum = 0;
    }
    else
    {
        contexto->quantum = contexto->quantum - (int)tiempo_transcurrido;
    }
    log_info(logger, "PID: %d - Ejecutando: WAIT - %s", contexto->pid, recurso);
}

void signal_instruccion(char* recurso)
{       temporal_stop(temporizador);
    int64_t tiempo_transcurrido = temporal_gettime(temporizador);
    if ((contexto->quantum - (int)tiempo_transcurrido) <= 0)
    {
        contexto->quantum = 0;
    }
    else
    {
        contexto->quantum = contexto->quantum - (int)tiempo_transcurrido;
    }
    log_info(logger, "PID: %d - Ejecutando: SIGNAL - %s", contexto->pid, recurso);
}

void io_fs_create(char *interfaz, char *nombre_archivo, int socket_kernel)
{
    char *mensaje = string_new();
    // Paramos contador
    temporal_stop(temporizador);
    int64_t tiempo_transcurrido = temporal_gettime(temporizador);
    if ((contexto->quantum - (int)tiempo_transcurrido) <= 0)
    {
        contexto->quantum = 0;
    }
    else
    {
        contexto->quantum = contexto->quantum - (int)tiempo_transcurrido;
    }

    string_append(&mensaje, "IO_FS_CREATE ");
    string_append(&mensaje, interfaz);
    string_append(&mensaje, " ");
    string_append(&mensaje, nombre_archivo);
    string_append(&mensaje, " ");
    string_append(&mensaje, string_itoa(contexto->pid));
    enviar_mensaje(mensaje, socket_kernel);
    log_info(logger, "PID: %d - Ejecutando: IO_FS_CREATE - %s %s", contexto->pid, interfaz, nombre_archivo);
}

void io_fs_delete(char *interfaz, char *nombre_archivo, int socket_kernel)
{
    char *mensaje = string_new();
    // Paramos contador
    temporal_stop(temporizador);
    int64_t tiempo_transcurrido = temporal_gettime(temporizador);
    if ((contexto->quantum - (int)tiempo_transcurrido) <= 0)
    {
        contexto->quantum = 0;
    }
    else
    {
        contexto->quantum = contexto->quantum - (int)tiempo_transcurrido;
    }

    string_append(&mensaje, "IO_FS_DELETE ");
    string_append(&mensaje, interfaz);
    string_append(&mensaje, " ");
    string_append(&mensaje, nombre_archivo);
    string_append(&mensaje, " ");
    string_append(&mensaje, string_itoa(contexto->pid));
    enviar_mensaje(mensaje, socket_kernel);
    log_info(logger, "PID: %d - Ejecutando: IO_FS_DELETE - %s %s", contexto->pid, interfaz, nombre_archivo);
}

void io_fs_truncate(char *interfaz, char *nombre_archivo, char *registro_tamanio, int socket_kernel)
{
    char *mensaje = string_new();
    // Paramos contador
    temporal_stop(temporizador);
    int64_t tiempo_transcurrido = temporal_gettime(temporizador);
    if ((contexto->quantum - (int)tiempo_transcurrido) <= 0)
    {
        contexto->quantum = 0;
    }
    else
    {
        contexto->quantum = contexto->quantum - (int)tiempo_transcurrido;
    }
    int tamanio = get_valor_registro(registro_tamanio);
    string_append(&mensaje, "IO_FS_TRUNCATE ");
    string_append(&mensaje, interfaz);
    string_append(&mensaje, " ");
    string_append(&mensaje, nombre_archivo);
    string_append(&mensaje, " ");
    string_append(&mensaje, string_itoa(tamanio));
    string_append(&mensaje, " ");
    string_append(&mensaje, string_itoa(contexto->pid));
    enviar_mensaje(mensaje, socket_kernel);
    log_info(logger, "PID: %d - Ejecutando: IO_FS_TRUNCATE - %s %s %s", contexto->pid, interfaz, nombre_archivo, registro_tamanio);
}

void io_fs_write(char *interfaz, char *nombre_archivo, char *registro_direccion, char *registro_tamanio, char *registro_puntero_archivo, int socket_kernel)
{
    char *mensaje = string_new();
    // Paramos contador
    temporal_stop(temporizador);
    int64_t tiempo_transcurrido = temporal_gettime(temporizador);
    if ((contexto->quantum - (int)tiempo_transcurrido) <= 0)
    {
        contexto->quantum = 0;
    }
    else
    {
        contexto->quantum = contexto->quantum - (int)tiempo_transcurrido;
    }
    int tamanio = get_valor_registro(registro_tamanio);
    int puntero_archivo = get_valor_registro(registro_puntero_archivo);
    int direccion = get_valor_registro(registro_direccion);
    string_append(&mensaje, "IO_FS_WRITE ");
    string_append(&mensaje, interfaz);
    string_append(&mensaje, " ");
    string_append(&mensaje, nombre_archivo);
    string_append(&mensaje, " ");
    string_append(&mensaje, puntero_archivo);
    string_append(&mensaje, " ");
    string_append(&mensaje, string_itoa(contexto->pid));
    string_append(&mensaje, " ");
    char* marcos = iterar_lista_mensaje(lista_marcos,direccion,tamanio);
    string_append(&mensaje, marcos);
    enviar_mensaje(mensaje, socket_kernel);
    log_info(logger, "PID: %d - Ejecutando: IO_FS_WRITE - %s %s %s %s %s", contexto->pid, interfaz, nombre_archivo, registro_direccion, registro_tamanio, registro_puntero_archivo);
}

void io_fs_read(char *interfaz, char *nombre_archivo, char *registro_direccion, char *registro_tamanio, char *registro_puntero_archivo, int socket_kernel)
{
    char *mensaje = string_new();
    // Paramos contador
    temporal_stop(temporizador);
    int64_t tiempo_transcurrido = temporal_gettime(temporizador);
    if ((contexto->quantum - (int)tiempo_transcurrido) <= 0)
    {
        contexto->quantum = 0;
    }
    else
    {
        contexto->quantum = contexto->quantum - (int)tiempo_transcurrido;
    }
    int tamanio = get_valor_registro(registro_tamanio);
    int puntero_archivo = get_valor_registro(registro_puntero_archivo);
    int direccion = get_valor_registro(registro_direccion);
    string_append(&mensaje, "IO_FS_READ ");
    string_append(&mensaje, interfaz);
    string_append(&mensaje, " ");
    string_append(&mensaje, nombre_archivo);
    string_append(&mensaje, " ");
    string_append(&mensaje, puntero_archivo);
    string_append(&mensaje, " ");
    string_append(&mensaje, string_itoa(contexto->pid));
    string_append(&mensaje, " ");
    char* marcos = iterar_lista_mensaje(lista_marcos,direccion,tamanio);
    string_append(&mensaje, marcos);
    enviar_mensaje(mensaje, socket_kernel);
    log_info(logger, "PID: %d - Ejecutando: IO_FS_READ - %s %s %s %s %s", contexto->pid, interfaz, nombre_archivo, registro_direccion, registro_tamanio, registro_puntero_archivo);
}