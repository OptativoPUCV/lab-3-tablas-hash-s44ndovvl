#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    
    if(map->size >= map->capacity) {
        enlarge(map);
        enlarge_called = 1; //no borrar (testing purposes)
    }

    long pos = hash(key, map->capacity);
    map->current = pos;
    
    while(map->buckets[pos] != NULL && map->buckets[pos]->key != NULL && is_equal(map->buckets[pos]->key, key) == 0) {
        pos += 1;
        pos %= map->capacity;

        if(map->current == pos) {
            printf("mapa lleno\n");
            return;
        }
    }

    map->buckets[pos] = createPair(key, value);
    map->size += 1;

    map->current = pos;
}

void enlarge(HashMap * map) {

    enlarge_called = 1; //no borrar (testing purposes)

    Pair ** old_buckets = map->buckets;
    long old_capacity = map->capacity;
    map->capacity = 2 * old_capacity;

    map->buckets = (Pair **) calloc (map->capacity,sizeof(Pair *));
    if(map->buckets == NULL) exit(EXIT_FAILURE);

    map->size = 0;

    for(int i = 0; i < old_capacity; i++) {
        if(old_buckets[i] != NULL) {
            insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
            free(old_buckets[i]);
        }
    }

    map->current = old_capacity;
    free(old_buckets);
}

/*Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido*/


HashMap * createMap(long capacity) {

    HashMap * map = (HashMap *)malloc(sizeof(HashMap));
    if(map == NULL) exit(EXIT_FAILURE); 

    map->buckets = (Pair **) calloc (capacity,sizeof(Pair *));
    if(map->buckets == NULL) exit(EXIT_FAILURE); 

    map->capacity = capacity;
    map->size = 0;

    map->current = -1;
    return map;
}

void eraseMap(HashMap * map,  char * key) {    
    long pos = hash(key, map->capacity);
    map->current = pos;

    while(map->buckets[pos] != NULL) {
        
        if(is_equal(map->buckets[pos]->key, key) == 1)
        {
            map->buckets[pos]->key = NULL;
            map->size -= 1;
            map->current = pos;
        }
        pos += 1;
        pos %= map->capacity; 

        if(map->current == pos) return;
    }

    
}

Pair * searchMap(HashMap * map,  char * key) {   
    long buscado = hash(key, map->capacity);
    map->current = buscado;
    
    while(map->buckets[buscado] != NULL) {
        if(is_equal(map->buckets[buscado]->key, key)) {
            map->current = buscado;
            return map->buckets[buscado];
        }
        
        buscado += 1;
        buscado %= map->capacity; 

        if(map->current == buscado) return NULL;
    }

    map->current = buscado;
    return NULL;
}

Pair * firstMap(HashMap * map) {
    long primero = 0;
    while(map->buckets[primero] == NULL || map->buckets[primero]->key == NULL){
        
        primero += 1;
        if(primero == map->capacity) 
        {
            map->current = -1;
            return NULL;
        }

    }
    map->current = primero;
    return map->buckets[primero];
}

Pair * nextMap(HashMap * map) {

    long siguiente = map->current + 1;
    siguiente %= map->capacity;
    /*while(map->buckets[siguiente] == NULL || map->buckets[siguiente]->key == NULL){
        
        siguiente += 1;
        siguiente %= map->capacity;
        if(siguiente == map->current + 1) 
        {
            map->current = map->current + 1;
            return NULL;
        }

    }*/
    map->current = siguiente;
    return map->buckets[siguiente];
}
