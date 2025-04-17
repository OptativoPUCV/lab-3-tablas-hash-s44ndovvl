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
    
    while(map->buckets[pos] != NULL && map->buckets[pos]->key != NULL) {
        pos += 1;
        pos %= map->capacity; 
    }

    map->buckets[pos] = createPair(key, value);
    map->size += 1;

    map->current = pos;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    //HashMap * old_map = map;
    //int old_size = map->size;


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

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
