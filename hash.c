#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "hash.h"

#define TAMANIO_INICIAL 133

typedef enum estado{
	OCUPADO;
	VACIO;
	BORRADO;
}estado_t;

typedef struct campo_hash{
	char* clave;
	void* dato;
	estado_t estado;
}campo_hash_t;

struct hash{
	size_t cant;
	size_t tam;
	campo_hash_t* tabla;
	hash_destruir_dato_t destructor;
}


 /******************************************************
   *             FUNCIONES AUXILIARES  	    		   *
  *****************************************************/

campo_hash_t* crear_tabla (size_t tam){

	campo_hash_t* tabla = malloc((tam)*sizeof(campo_hash_t));
	if (tabla == NULL) return NULL;

	for (size_t i=0;i<tam;i++){
		campo_hash_t* campo = malloc (sizeof(campo_hash));
		campo->clave = NULL;
		campo->dato = NULL;
		campo->estado = VACIO;
		tabla[i] = campo;
	}
	
	return tabla;
}

 /******************************************************
   *             PRIMITIVAS DEL HASH 	    		   *
  *****************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc (size_t(hash_t));
	if (hash == NULL) return NULL;

	hash->cant = 0;
	hash->tam = TAMANIO_INICIAL;
	hash->destructor = destruir_dato;

	campo_hash_t* tabla = crear_tabla(hash->tam);
	if (tabla == NULL){
		free(hash);
		return NULL;
	}
	hash->tabla = tabla;
	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){

}
void *hash_borrar(hash_t *hash, const char *clave){}
void *hash_obtener(const hash_t *hash, const char *clave){}
bool hash_pertenece(const hash_t *hash, const char *clave){}

size_t hash_cantidad(const hash_t *hash){
	return hash->cantidad;
}
void hash_destruir(hash_t *hash){
	//destuir_dato(hash->)
	free(hash->tabla);
	free(hash);
}

