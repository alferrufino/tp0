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

	logger = iniciar_logger();

	log_info(logger,"Soy un Log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// guardamos los valores en "valor","ip" y "puerto"

	valor = config_get_string_value(config,"CLAVE");

	ip = config_get_string_value(config,"IP");

	puerto = config_get_string_value(config,"PUERTO");

	// Loggeamos el valor de config

	log_info(logger,"%s",valor);

	log_info(logger,"%s",ip);

	log_info(logger,"%s",puerto);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Creamos una conexión hacia el servidor

	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	enviar_mensaje(valor,conexion);

	// Armamos y enviamos el paqueteS

	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("tp0.log","TP0",true,LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("cliente.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío

	while(strcmp(leido,"") != 0)
	{
		log_info(logger,"%s",leido);
		free(leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las lineas antes de regresar!

	free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");

	while(strcmp(leido,"") != 0){

		agregar_a_paquete(paquete,leido,strlen(leido) + 1);
		
		free(leido);

		leido = readline("> ");
	}

	enviar_paquete(paquete,conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);

	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	con las funciones de las commons y del TP mencionadas en el enunciado */
	
	log_destroy(logger);

	config_destroy(config);

	liberar_conexion(conexion);
}
