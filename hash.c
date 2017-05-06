
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

#define TAMANIO_INICIAL 199

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
	unsigned long hash = 5831;
    unsigned int c;
    while ((c = (*str++)))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c 

    return hash % tam;
}
 /******************************************************
   *             FUNCIONES AUXILIARES  	    		   *
  *****************************************************/

campo_hash_t* crear_tabla (size_t tam){
	campo_hash_t* tabla = malloc((tam)*sizeof(campo_hash_t));
	if (tabla == NULL) return NULL;
	campo_hash_t campo;
	campo.clave = NULL;
	campo.dato = NULL;
	campo.estado = VACIO;
	for (size_t i=0;i<tam;i++){
		tabla[i] = campo;
	}
	return tabla;
}

size_t obtener_posicion(campo_hash_t* tabla, size_t posicion, const char *clave, size_t tam){
	// Si la clave es la misma devuelvo la misma posicion.
	if (tabla[posicion].estado == OCUPADO){
		if (strcmp(tabla[posicion].clave,clave) == 0) return posicion;
	}

	//Si la clave no es la misma busco la proxima vacia o una ya ocupada con la misma clave.
	while (tabla[posicion].estado != VACIO){
		posicion++;
		posicion = posicion % tam;
		if (tabla[posicion].estado == OCUPADO){
			if (strcmp(tabla[posicion].clave,clave) == 0) return posicion;
		}
	}
	return posicion;
}

hash_t* redim_hash(hash_t* hash, size_t tamanioNuevo){
	hash_iter_t* iter = hash_iter_crear(hash);
	if (iter == NULL) return NULL;
	campo_hash_t* tabla = crear_tabla(tamanioNuevo); 
	if (tabla == NULL) return NULL;

	size_t posicion=0, len=0;
	while (!hash_iter_al_final(iter)){
		posicion = obtener_posicion(tabla,hashing(hash_iter_ver_actual(iter),tamanioNuevo),hash_iter_ver_actual(iter),tamanioNuevo);
		tabla[posicion].estado=OCUPADO;
		tabla[posicion].dato=hash_obtener(hash,hash_iter_ver_actual(iter));
		len = strlen(hash_iter_ver_actual(iter));
		tabla[posicion].clave = malloc(1+len);
		memcpy(tabla[posicion].clave,hash_iter_ver_actual(iter),len);
		tabla[posicion].clave[len] = '\0';
		hash_iter_avanzar(iter);
	}

	for(size_t i=0;i < hash->tam;i++){
		if(hash->tabla[i].estado == OCUPADO){ 
			free(hash->tabla[i].clave);
		}
	}
	free(hash->tabla);
	hash->tam = tamanioNuevo;
	hash->tabla = tabla;
	hash_iter_destruir(iter);
	return hash;
}

size_t obtener_posicion_ocupada(const hash_t* hash, size_t posicion){
	while ((posicion < (hash->tam-1)) && hash->tabla[posicion].estado !=OCUPADO){
		posicion++;
	}
	return posicion;
}

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

	if(hash->cantidad > hash->tam * 70 / 100) redim_hash(hash,hash->tam * 2);
	size_t posicion = obtener_posicion(hash->tabla,hashing(clave,hash->tam),clave,hash->tam);
	
	if (hash->tabla[posicion].estado == OCUPADO){
		if(hash->destructor) hash->destructor(hash->tabla[posicion].dato);
		hash->tabla[posicion].dato = dato;
		return true;
	}
	
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

	size_t posicion = obtener_posicion(hash->tabla,hashing(clave,hash->tam),clave,hash->tam);

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
	size_t posicion = obtener_posicion(hash->tabla,hashing(clave,hash->tam),clave,hash->tam);
	if (hash->tabla[posicion].estado == OCUPADO) return hash->tabla[posicion].dato;
	return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	size_t posicion = obtener_posicion(hash->tabla,hashing(clave,hash->tam),clave,hash->tam);
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
	iter->posicionActual = obtener_posicion_ocupada(hash,0);
	iter->recorridos = 0;
	iter->hash = hash;
	return iter;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if (hash_iter_al_final(iter)) return NULL;
	return iter->hash->tabla[iter->posicionActual].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
	if (iter->recorridos >= iter->hash->cantidad) return true;
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
