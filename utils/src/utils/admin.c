#include <utils/admin.h>



void cerrar_log_config (t_log* logger, t_config* config){
    log_destroy(logger);
    config_destroy(config);
}