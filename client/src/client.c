#include "client.h"

int main(void)
{
    /*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
    int conexion;
    char* ip;
    char* puerto;
    char* valor;

    t_log* logger;
    t_config* config;

    logger = log_create("cliente.log", "CLIENTE", true, LOG_LEVEL_INFO);
    log_info(logger, "Hola! Soy un log");

    config = config_create("cliente.config");
    if (config == NULL) {
        log_error(logger, "No se pudo leer el archivo de config");
        log_destroy(logger);
        return EXIT_FAILURE;
    }

    ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTO");
    valor = config_get_string_value(config, "CLAVE");

    log_info(logger, "IP: %s - PUERTO: %s - CLAVE: %s", ip, puerto, valor);

    leer_consola(logger);

    /*---------------------------------------------------PARTE 3-------------------------------------------------------------*/
    
    conexion = crear_conexion(ip, puerto);

    // SEGURIDAD: Si no hay conexión, terminamos el programa con elegancia
    if (conexion == -1) {
        log_error(logger, "No se pudo conectar al servidor. ¡Asegurate de que esté prendido!");
        terminar_programa(-1, logger, config);
        return EXIT_FAILURE;
    }

    // IMPORTANTE: Revisar el orden de los parámetros según tus commons.
    // Generalmente es: enviar_mensaje(texto, socket);
    enviar_mensaje(valor, conexion);

    enviar_paquete_consola(conexion, logger);

    terminar_programa(conexion, logger, config);

    return EXIT_SUCCESS;
}

void leer_consola(t_log* logger)
{
    char* leido;
    while (1) {
        leido = readline("> ");
        if (leido == NULL || strcmp(leido, "") == 0) {
            free(leido);
            break;
        }
        log_info(logger, "Linea ingresada: %s", leido);
        free(leido);
    }
}

void enviar_paquete_consola(int conexion, t_log* logger)
{
    char* leido;
    t_paquete* paquete = crear_paquete();

    while (1) {
        leido = readline("> ");
        if (leido == NULL || strcmp(leido, "") == 0) {
            free(leido);
            break;
        }
        agregar_a_paquete(paquete, leido, strlen(leido) + 1);
        free(leido);
    }

    enviar_paquete(paquete, conexion);
    eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
    // Solo cerramos la conexión si el socket es válido
    if (conexion != -1) {
        liberar_conexion(conexion);
    }
    log_destroy(logger);
    config_destroy(config);
}