#ifndef HASH_H
#define HASH_H

/******************************************************
  *            			 HASH    		 			  *
  *****************************************************/

typedef struct hash hash_t;
typedef struct hash_iter hash_iter_t;
typedef void (*hash_destruir_dato_t)(void *);

/******************************************************
  *             PRIMITIVAS DEL HASH 	    		  *
  *****************************************************/


// Pre: debe existir una funcion de destruccion, asi el hash sabe como destruir el dato previo.
//Post: devuelve una tabla de hash, en caso contario devuelve NULL;
hash_t *hash_crear(hash_destruir_dato_t destruir_dato);

//Pre: hash creado
//Post: guarda en el hash el dato, si la clave ya existe el nuevo dato debe ser remplazado
//en la posicion donde esta la misma clave. Si la clave no existe se debe guardar el dato
//en una posicion con el estado vacio y cambia el estado vacio a ocupado. 
//Si pudo guardar el dato devuelve true en caso contario devuelve false.
bool hash_guardar(hash_t *hash, const char *clave, void *dato);

//Pre: hash creado
//Post: si la clave pertenece al hash, devuelvo el dato y reemplazo el estado a borrado. Si la clave
//no existe devuelvo NULL:
void *hash_borrar(hash_t *hash, const char *clave);

//Pre: hash creado
//Post: se busca el dato segun la clave y lo devuelve. Si el dato no existe
//devuelve NULL:
void *hash_obtener(const hash_t *hash, const char *clave);

//Pre: hash creado
//Post: se fija si existe la clave en el hash, para eso busca la posicion con la funcion de hash
//y va comparando hasta encontrar un lugar vacio. Si encontro la clave devuelve true, en caso contario
//devuelve false.
bool hash_pertenece(const hash_t *hash, const char *clave);

//Pre: hash creado
//Post: devuelve la cantidad de claves que hay en la tabla
size_t hash_cantidad(const hash_t *hash);

//Pre: hash creado
//Post: elimina el hash, destruyendo todos los datos de la tabla y liberando la memoria.
void hash_destruir(hash_t *hash);



/******************************************************
  *             PRIMITIVAS DE ITERACION      		  *
  *****************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash);

bool hash_iter_avanzar(hash_iter_t *iter);

const char *hash_iter_ver_actual(const hash_iter_t *iter);

bool hash_iter_al_final(const hash_iter_t *iter);

void hash_iter_destruir(hash_iter_t* iter);


#endif