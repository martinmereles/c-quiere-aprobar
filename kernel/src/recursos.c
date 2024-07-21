#include <../include/recursos.h>

void cargar_recursos(t_config* config){
    char** recursos = config_get_array_value(config, "RECURSOS");
    char** instancias_recursos = config_get_array_value(config, "INSTANCIAS_RECURSOS");
    sem_wait(&mutex_lista_recursos);
    for(int i = 0; recursos[i] != NULL ;i++){
        printf("Se carga el siguiente recurso %s - %s instancia\n", recursos[i], instancias_recursos[i]);
        recurso_t* recurso = malloc(sizeof(recurso_t));
        recurso->nombre = string_new();
        string_append(&recurso->nombre, recursos[i]);
        recurso->procesos_bloqueados = list_create();
        recurso->instancias = atoi(instancias_recursos[i]);
        sem_init(&recurso->mutex_recurso, 0, 1);
        list_add(lista_recursos, recurso);
    }
    sem_post(&mutex_lista_recursos);
}

bool existe_recurso(char* nombre_recurso){
    bool existe = false;
    recurso_t * recurso;
    for(int i = 0; i < list_size(lista_recursos); i++){
        recurso = list_get(lista_recursos, i);
        if(strcmp(recurso->nombre, nombre_recurso) == 0){
            existe = true;
        }
    }
    return existe;
}

void wait_instuccion(char* nombre_recurso, int pid){
    if(existe_recurso(nombre_recurso)){

        bool _es_recurso_buscado(void* elemento){
            return es_recurso_buscado(nombre_recurso, elemento);
        }

        recurso_t* recurso = list_find(lista_recursos,_es_recurso_buscado);
        
        sem_wait(&recurso->mutex_recurso);
        recurso->instancias--;
        if(recurso->instancias < 0){
            list_add(recurso->procesos_bloqueados, pid);
            //bloquear proceso
        }

        sem_post(&recurso->mutex_recurso);
        
    }else{
        finalizar_proceso(pid, socket_cpu_interrupt, socket_memoria);
    }
}

void signal_instuccion(char* nombre_recurso, int pid){
    if(existe_recurso(nombre_recurso)){
        
        bool _es_recurso_buscado(void* elemento){
            return es_recurso_buscado(nombre_recurso, elemento);
        }

        recurso_t* recurso = list_find(lista_recursos,_es_recurso_buscado);

        sem_wait(&recurso->mutex_recurso);
        recurso->instancias++;
        if(list_size(recurso->procesos_bloqueados > 0){
            int pid_a_desbloquear = list_remove(recurso->procesos_bloqueados,0);
            //desbloquear proceso
        }

        sem_post(&recurso->mutex_recurso);
        
    }else{
        finalizar_proceso(pid, socket_cpu_interrupt, socket_memoria);
    }
}

bool es_recurso_buscado(char* identificador, void* elemento){
    recurso_t* aux = malloc(sizeof(t_interfaz));
    aux = elemento;
    bool aux2 = (strcmp(aux->nombre, identificador) == 0);
    return (aux2);
}