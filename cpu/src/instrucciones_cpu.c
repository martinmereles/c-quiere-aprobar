#include <instrucciones_cpu.h>


void set (char * registro, char * valor)
{
    enum_registro var = * 
    switch (pasar_a_numero(registro))
    {
    case AX:
        
        break;
    
    default:
        break;
    }
}

enum_registro pasar_a_numero (char * valor_registro)
{
    if(strcmp(valor_registro, "AX"))
    {
        return AX;
    }
    if(strcmp(valor_registro, "BX"))
    {
        return BX;
    }
}