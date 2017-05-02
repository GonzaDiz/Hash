#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "hash.h"

typedef struct hash hash_t;
typedef struct hash_iter hash_iter_t;
typedef void (*hash_destruir_dato_t)(void *);

struct hash{
	size_t cant;
	size_t tam;
	//tabla decidir si cerrado o abierto
	hash_destruir_dato_t destructor;
}

enum estado {
	OCUPADO;
	VACIO;
	BORRADO;
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){}
bool hash_guardar(hash_t *hash, const char *clave, void *dato){}
void *hash_borrar(hash_t *hash, const char *clave){}
void *hash_obtener(const hash_t *hash, const char *clave){}
bool hash_pertenece(const hash_t *hash, const char *clave){}
size_t hash_cantidad(const hash_t *hash){}
void hash_destruir(hash_t *hash){}

