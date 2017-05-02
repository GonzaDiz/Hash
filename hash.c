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
   *                  HASHING			 	    	   *
  *****************************************************/
unsigned long hash(unsigned char *str, size_t tam) {
        unsigned long hash = 5381;
        int c;
        while (c = *str++)
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash % tam;
    }

 /******************************************************
   *             FUNCIONES AUXILIARES  	    		   *
  *****************************************************/

campo_hash_t* crear_tabla (size_t tam){

	campo_hash_t* tabla = malloc((tam)*sizeof(campo_hash_t));
	if (tabla == NULL) return NULL;

	for (size_t i=0;i<tam;i++){
		campo_hash_t* campo = malloc (sizeof(campo_hash));
		if (campo == NULL) return NULL;
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

	// TODO redimensionar en caso de ser necesario.
	// TODO crear alguna forma de obtener la posicion.


	if (hash->tabla[posicion] == NULL) return false;

	if (hash->tabla[posicion]->estado == OCUPADO){
		hash->destruir_dato(hash->tabla[posicion]->dato);
		hash->tabla[posicion]->dato = dato;
		return true;
	}
	if (hash->tabla[posicion]->estado == VACIO){
		hash->tabla[posicion]->clave = clave;
		hash->tabla[posicion]->dato = dato;
		hash->tabla[posicion]->estado = OCUPADO;
		hash->cantidad++;
		return true;
	}
	return false;

}
void *hash_borrar(hash_t *hash, const char *clave){

	//TODO redimensionar en caso de ser necesario.
	//TODO crear alguna forma de obtener la posicion.

	if (hash->tabla[posicion]->estado == OCUPADO){
		void* dato = hash->tabla[posicion]->dato;

		hash->tabla[posicion]->clave = NULL;
		hash->tabla[posicion]->dato = NULL;
		hash->tabla[posicion]->estado = BORRADO;
		hash->cantidad--;
		return dato;
	}
	return NULL;

}
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

