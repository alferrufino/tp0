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

	/* ---------------- LOGGING ---------------- */

	logger = log_create("tp0.log", "CLIENTE", true, LOG_LEVEL_INFO);
	// Usando el logger creado previamente
	log_info(logger,"Hola Soy un log");
	// Escribi: "Hola! Soy un log"

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = config_create("cliente.config");
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config,"PUERTO");
	valor = config_get_string_value(config,"CLAVE");
	// Loggeamos el valor de config
	log_info(logger,"IP leida: %s",ip);
	log_info(logger,"PUERTO leido: %s",puerto);
	log_info(logger,"CLAVE leida: %s",valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);
	log_info(logger,"Termina la parte 2 del tp0");

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete

	enviar_paquete_consola(conexion, logger);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
}

void leer_consola(t_log* logger)
{
	char* leido;

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	
	while (1) {
    	leido = readline("> ");

    	// Si es string vacío, termina el programa
    	if (strcmp(leido, "") == 0) {
        	free(leido);
        	break;
    	}
    	// Logueamos la línea ingresada
    	log_info(logger, "Linea ingresada: %s", leido);
    	free(leido);
	}
	// ¡No te olvides de liberar las lineas antes de regresar!
}

void enviar_paquete_consola(int conexion, t_log* logger)
{
	char* leido;
	t_paquete* paquete = crear_paquete();

	while (1) {
		leido = readline("> ");

		// Si se ingresa línea vacía, se termina
		if (strcmp(leido, "") == 0) {
			free(leido);
			break;
		}

		log_info(logger, "Linea enviada: %s", leido);

		agregar_a_paquete(paquete, leido, strlen(leido) + 1);

		free(leido);
	}

	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
