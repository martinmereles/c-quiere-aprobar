#include <../include/main_kernel.h>

t_log* logger;
int GLOBAL_PID;
t_list* QUEUE_NEW;
t_list* QUEUE_READY;
t_list* QUEUE_READY_PLUS;
t_list* QUEUE_RUNNING;
t_list* QUEUE_BLOCKED;
t_list* QUEUE_TERMINATED;
t_list* INTERFACES;
sem_t sem_grado_multiprog;
sem_t sem_multiprocesamiento;
t_sem_estados sem_array_estados[6]; /*
                        0 = New
                        1 = Ready
                        2 = Running
                        3 = Blocked
                        4 = Terminated
                        5 = Ready+*/
sem_t sem_sincro_cpu;
int socket_memoria;
int socket_cpu_interrupt;
sem_t mutex_lista_interfaces;
sem_t mutex_lista_recursos;
sem_t detencion_planificador_corto;
sem_t detencion_planificador_largo;
sem_t llegada_desalojo_io;
bool detuvo_planificacion;
char* algoritmo;
char* instruccion;

t_list* lista_recursos;

int main(int argc, char* argv[]) {
    QUEUE_NEW = list_create();
    QUEUE_READY = list_create();
    QUEUE_RUNNING = list_create();
    QUEUE_BLOCKED = list_create();
    QUEUE_TERMINATED = list_create();
    QUEUE_READY_PLUS = list_create();
    INTERFACES = list_create();
    lista_recursos = list_create();
    logger = iniciar_logger("./kernel-log.log", "kernel");
    t_config* config = iniciar_config(logger, "./kernel.config");
    char* quantum = config_get_string_value(config, "QUANTUM");
    int grado_multiprog = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");
    algoritmo = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    sem_init(&sem_grado_multiprog, 0, grado_multiprog);
    sem_init(&sem_sincro_cpu, 0, 0);
    sem_init(&sem_multiprocesamiento, 0, 1);
    sem_init(&mutex_lista_interfaces, 0, 1);
    sem_init(&detencion_planificador_corto, 0, 1);
    sem_init(&detencion_planificador_largo, 0, 1);
    sem_init(&llegada_desalojo_io, 0, 1);
    detuvo_planificacion = 0;
    sem_init(&mutex_lista_recursos, 0, 1);
    for(int i = 0; i < 6; i++){
        sem_init(&sem_array_estados[i].mutex, 0, 1);
        sem_init(&sem_array_estados[i].contador, 0, 0);
    }

    //Se cargan los recursos
    cargar_recursos(config);


    //Incio hilo planificador largo plazo
    char * param_largo_plazo = malloc(sizeof(char));
	pthread_t hilo_plani_largo_plazo;
	pthread_create(&hilo_plani_largo_plazo,
                        NULL,
                        planificador_largo_plazo,
                        param_largo_plazo);
	pthread_detach(hilo_plani_largo_plazo);

    //Inicio hilo server
    char* puerto = config_get_string_value(config, "PUERTO_ESCUCHA");
    char *param = (char*)malloc(sizeof(puerto));
	param=puerto;
	pthread_t hiloServidor;
	pthread_create(&hiloServidor,
                        NULL,
                        iniciar_hilo_server_kernel,
                        param);
	pthread_detach(hiloServidor);
    
    //Inicia conexion con memoria
    char* ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char* puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    log_info(logger, "La IP de MEMORIA es : %s", ip_memoria);
    log_info(logger, "El PUERTO de MEMORIA es : %s", puerto_memoria);
    int socket_cliente_memoria = crear_conexion(ip_memoria,puerto_memoria);
    socket_memoria = socket_cliente_memoria;
    enviar_mensaje("Me conecto desde Kernel",socket_cliente_memoria);

    
    char* ip_cpu = config_get_string_value(config, "IP_CPU");
    log_info(logger, "La IP de CPU es : %s", ip_cpu);

    //Inicia conexion con puerto dispatch de CPU
    char* puerto_cpu_dispatch = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    log_info(logger, "El PUERTO de CPU DISPATCH es : %s", puerto_cpu_dispatch);
    int socket_cpu_dispatch = crear_conexion(ip_cpu, puerto_cpu_dispatch);
    log_info(logger, "El SOCKET de CPU DISPATCH es : %d", socket_cpu_dispatch);
    enviar_mensaje("Me conecto desde Kernel a PUERTO_CPU_DISPATCH", socket_cpu_dispatch);

    //Hilo de escucha con CPU puerto DISPATCH
    pthread_t hilo_atencion_cpu_dispatch;
    int *socket_cliente_hilo = (int)malloc(sizeof(int));
    socket_cliente_hilo = socket_cpu_dispatch;
    pthread_create(&hilo_atencion_cpu_dispatch,
                    NULL,
                    (void*) atender_cliente_kernel,
                    socket_cliente_hilo);
    pthread_detach(hilo_atencion_cpu_dispatch);

    //Inicia conexion con puerto interrupt de CPU
    char* puerto_cpu_interrupt = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
    log_info(logger, "El PUERTO de CPU INTERRUPT es : %s", puerto_cpu_interrupt);
    socket_cpu_interrupt = crear_conexion(ip_cpu, puerto_cpu_interrupt);
    log_info(logger, "El SOCKET de CPU INTERRUPT es : %d", socket_cpu_interrupt);
    enviar_mensaje("Me conecto desde Kernel a PUERTO_CPU_INTERRUPT", socket_cpu_interrupt);

    
    //Incio hilo planificador corto plazo
    socket_info_t* params = malloc(sizeof(socket_info_t));
    params->socket_dispatch = socket_cpu_dispatch;
    params->socket_interrupt = socket_cpu_interrupt;

	pthread_t hilo_plani_corto_plazo;
	pthread_create(&hilo_plani_corto_plazo,
                        NULL,
                        planificador_corto_plazo,
                        params);
	pthread_detach(hilo_plani_corto_plazo);
    
    lanzar_consola(quantum, socket_cliente_memoria, socket_cpu_dispatch, socket_cpu_interrupt, config);
    
    pthread_join(hiloServidor, NULL);
    //Cierre de log y config
    cerrar_log_config (logger,config); 
    list_destroy(QUEUE_NEW);
    list_destroy(QUEUE_READY);
    list_destroy(QUEUE_RUNNING);
    list_destroy(QUEUE_BLOCKED);
    list_destroy(QUEUE_TERMINATED);
    
    return EXIT_SUCCESS;
}