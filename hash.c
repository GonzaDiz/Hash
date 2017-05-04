#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

#define TAMANIO_INICIAL 133

typedef enum estado{OCUPADO, VACIO ,BORRADO} estado_t;

typedef struct campo_hash{
	char* clave;
	void* dato;
	estado_t estado;
}campo_hash_t;

struct hash{
	size_t cantidad;
	size_t tam;
	campo_hash_t* tabla;
	hash_destruir_dato_t destructor;
};

struct hash_iter{
	size_t posicionActual;
	size_t recorridos;
	const hash_t* hash;
};
 /******************************************************
   *                  HASHING			 	    	   *
  *****************************************************/
size_t hashing (const char *str, size_t tam) {
        size_t hash = 5381;
        int c;
        while ((c = (*str++)))
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash % tam;
}

 /******************************************************
   *             FUNCIONES AUXILIARES  	    		   *
  *****************************************************/

campo_hash_t* crear_tabla (size_t tam){

	campo_hash_t* tabla = malloc((tam)*sizeof(campo_hash_t));
	if (tabla == NULL) return NULL;
	campo_hash_t campo;
	for (size_t i=0;i<tam;i++){
		//campo_hash_t campo = malloc (sizeof(campo_hash_t));
		//if (campo == NULL) return NULL;
		campo.clave = NULL;
		campo.dato = NULL;
		campo.estado = VACIO;
		tabla[i] = campo;
	}

	return tabla;
}


//Pre: el hash fue creado.
//Post: si la clave ya existia en la tabla devuelve la posicion en la que estaba, si la clave no existia devuelve
//una posicion vacia.
size_t obtener_posicion(campo_hash_t* tabla, size_t posicion, const char *clave){
	while (tabla[posicion].estado != VACIO){
		if (tabla[posicion].estado == OCUPADO){
			if (strcmp(tabla[posicion].clave,clave) == 0){
			return posicion;
			}
		}
		posicion++;
	}
	return posicion;
}

size_t obtener_posicion_ocupada(const hash_t* hash, size_t posicion){
	while (hash->tabla[posicion].estado !=OCUPADO && (posicion < hash->tam)){
		posicion++;
	}
	return posicion;
}

/*hash_t *hash_redimensionar(hash_t *hash,size_t tam_nuevo){
	campo_hash_t* tabla = crear_tabla(tam_nuevo);
}*/

 /******************************************************
   *             PRIMITIVAS DEL HASH 	    		   *
  *****************************************************/


 /******************************************************
   *             PRIMITIVAS HASH   		    		   *
  *****************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if (hash == NULL) return NULL;

	hash->cantidad = 0;
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

	//Obtengo posicion
	//size_t posInicial = hash(clave,hash->tam);
	size_t posicion = obtener_posicion(hash->tabla,hashing(clave,hash->tam),clave);
	//if (hash->tabla[posicion] == NULL) return false;

	//Si la clave ya existia en la tabla, destruyo el dato y clo cambio por el nuevo dato
	if (hash->tabla[posicion].estado == OCUPADO){
		if(hash->destructor) hash->destructor(hash->tabla[posicion].dato);
		hash->tabla[posicion].dato = dato;
		return true;
	}
	//Si la clave no existia en la tabla, la guardo en la posicion vacia que tenia.
	if (hash->tabla[posicion].estado == VACIO){
		size_t len = strlen(clave);
		hash->tabla[posicion].clave = malloc(1+len);
		memcpy(hash->tabla[posicion].clave,clave,len);
		hash->tabla[posicion].clave[len] = '\0';
		hash->tabla[posicion].dato = dato;
		hash->tabla[posicion].estado = OCUPADO;
		hash->cantidad++;
		return true;
	}
	return false;

}
void hash_destruir(hash_t *hash){
	for(size_t i=0;i < hash->tam;i++){
		if(hash->tabla[i].estado == OCUPADO){ 
			free(hash->tabla[i].clave);
			if(hash->destructor) hash->destructor(hash->tabla[i].dato);
		}
	}
	free(hash->tabla);
	free(hash);
}
void *hash_borrar(hash_t *hash, const char *clave){

	//TODO redimensionar en caso de ser necesario.
	
	//Obtengo posicion
	size_t posicion = obtener_posicion(hash->tabla,hashing(clave,hash->tam),clave);

	//Si la posicion estaba ocupada, guardo el dato, cambio el estado a BORRADO y borro la clave y el dato.
	if (hash->tabla[posicion].estado == OCUPADO){
		void* dato = hash->tabla[posicion].dato;
		free(hash->tabla[posicion].clave);
		hash->tabla[posicion].clave = NULL;
		hash->tabla[posicion].dato = NULL;
		hash->tabla[posicion].estado = BORRADO;
		hash->cantidad--;
		return dato;
	}
	return NULL;

}
void *hash_obtener(const hash_t *hash, const char *clave){
	//TODO crear alguna forma de obtener la posicion.
	//Obtengo posicion.
	size_t posicion = obtener_posicion(hash->tabla,hashing(clave,hash->tam),clave);
	//En caso de que la posicion este ocupada devuelvo el dato de esa posicion.
	if (hash->tabla[posicion].estado == OCUPADO) return hash->tabla[posicion].dato;
	return NULL;

}

bool hash_pertenece(const hash_t *hash, const char *clave){
	size_t posicion = obtener_posicion(hash->tabla,hashing(clave,hash->tam),clave);
	if (hash->tabla[posicion].estado != OCUPADO) return false;
	return true;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cantidad;
}


 /******************************************************
   *             PRIMITIVAS ITERADOR 	    		   *
  *****************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if (iter == NULL) return NULL;
	iter->posicionActual = 0;
	iter->recorridos = 0;
	iter->hash = hash;
	return iter;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if (hash_iter_al_final(iter)) return NULL;
	return iter->hash->tabla[iter->posicionActual].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
	if (iter->recorridos == iter->hash->cantidad) return true;
	return false;
}

bool hash_iter_avanzar(hash_iter_t *iter){
	if (hash_iter_al_final(iter)) return false;
	iter->posicionActual = obtener_posicion_ocupada(iter->hash,iter->posicionActual +1);
	iter->recorridos++;
	return true;
}

void hash_iter_destruir(hash_iter_t* iter){
	free(iter);
}