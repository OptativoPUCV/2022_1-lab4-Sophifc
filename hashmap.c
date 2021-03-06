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
  long aux=hash(key,map->capacity);
  int cont=1;
  while(cont!=0){
    if(map->buckets[aux]==NULL){
      Pair *dato=createPair(key,value);
      map->buckets[aux]=dato;
      map->current=aux;
      map->size++;
      break;
    }
    else{
      if(is_equal(key, (map->buckets[aux]->key))==1){
        break;
      }
    }
    aux++;
  }

}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  HashMap *mapa= createMap(map->capacity *2);
  //map->capacity=(map->capacity)*2;
  //map->size=0;
  for(int i=0; i<(map->capacity); i++){
    if(map->buckets[i]!=NULL){
      insertMap(mapa,map->buckets[i]->key,map->buckets[i]->value);
    }
  }
  *map=*mapa;
}


HashMap * createMap(long capacity) {
  HashMap *mapa=(HashMap*)calloc(capacity,sizeof(HashMap));
  mapa->current=-1;
  mapa->capacity=capacity;
  mapa->size=0;
  mapa->buckets= (Pair**) calloc (capacity,sizeof(Pair*));
  return mapa;
   // return NULL;
}

void eraseMap(HashMap * map,  char * key) {    
long pos=hash(key, map->capacity);
int elim=1;
  do{
    if((map->buckets[pos])==NULL){
      break;
    }
    else{
        if(is_equal((map->buckets[pos]->key),key)){
        map->buckets[pos]->key=NULL;
        map->size--;
        elim=0;
      }
    }
    pos++;
  }while(elim!=0);

}

Pair * searchMap(HashMap * map,  char * key) {   
long pos=hash(key, map->capacity);
  int ret=1;
  do{
    if((map->buckets[pos])==NULL){
      return NULL;
    }
    else{
      if(is_equal(map->buckets[pos]->key, key)==1){
        map->current=pos;
        return map->buckets[pos];
      }
    }
    pos++;
  }while(ret!=0);
  return NULL;

}

Pair * firstMap(HashMap * map) {
  
  for(long i=0; i<map->size; i++){
    if(map->buckets[i]!=NULL && map->buckets[i]->key!=NULL){
      map->current=i;
      return map->buckets[i];
    }
  }
  return NULL;
}

Pair * nextMap(HashMap * map) {

  for(long i=map->current+1; i<map->capacity; i++){
    if(map->buckets[i]!=NULL && map->buckets[i]->key !=NULL){
      map->current=i;
      return map->buckets[i];
    }
  }
  return NULL;
}
