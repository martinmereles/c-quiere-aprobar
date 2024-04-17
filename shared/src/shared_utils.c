#include "shared_utils.h"

char *mi_funcion_compartida()
{
    printf("Hice uso de la shared!");
    return 0;
}

int handshakeCliente(int socket_cliente, int modulo)
{
    uint32_t handshake = modulo;
    uint32_t result = -1;
    send(socket_cliente, &handshake, sizeof(uint32_t), 0);
    recv(socket_cliente, &result, sizeof(uint32_t), MSG_WAITALL);

    return result;
}

int handshakeServidor(int socket_cliente, t_log *logger)
{
    uint32_t handshake;
    uint32_t resultOk = 0;
    uint32_t resultError = -1;

    recv(socket_cliente, &handshake, sizeof(uint32_t), MSG_WAITALL);
    sleep(3);

    send(socket_cliente, &resultOk, sizeof(uint32_t), 0);
    log_info(logger, "Handshake con %s\n", op_ModuloToString(handshake));
    return handshake;

    /* if (handshake == )
     {
         send(socket_cliente, &resultOk, sizeof(uint32_t), 0);
         log_info(logger, "Handshake con %s\n", op_ModuloToString(handshake));
     }
     else
     {
         send(socket_cliente, &resultError, sizeof(uint32_t), 0);
         printf("\nHandshake incorrecto. Fue: %d\n", resultError); // Borrar, solo para corroborar
         // char *mensajeError = "ERROR AL HACER EL HANDSHAKE";
         // error_show(mensajeError);
     };  */
}

const char *op_ModuloToString(int opCode)
{
    printf("recibi el valor de opcode: %d\n", opCode);
    {
        switch (opCode)
        {
        case KERNEL:
            return "KERNEL";
        case CPU:
            return "CPU";

        case MEMORIA:
            return "MEMORIA";
        default:
            return "Error";
        }
    }
}

char* buscarEnConfig(t_config* config,char* index){
    char* valorObtenido;
    valorObtenido = config_get_string_value(config, index);
    return valorObtenido;
}