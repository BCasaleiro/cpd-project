#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void printList(Row *row){
  List *aux;
  for(aux=row->first;aux!=NULL;aux=aux->next){
    printf("%d,%d,%d\n", aux->data.x, aux->data.y, aux->data.z);
  }
}

void freeList(Row *row){
  List * aux = row->first;
  List *aux1=aux;
  if(aux==NULL){
    return;
  }else{
    for (aux = row->first; aux1!=NULL; aux=aux1) {
      aux1=aux->next;
      free(aux);
    }
  }
}

void init(Row *row){
  row->first=NULL;
  row->size=0;
}

List *newNodeList(localization x){
  List *new=calloc(1, sizeof(List));
  if(new==NULL){
    printf("erro a alocar\n");
    exit(-1);
  }
  new->data = x;
  new->next = NULL;
  return new;
}

void add(Row *row, localization x){
  List *new;
  new=newNodeList(x);
  if(row->size==0){
    row->first=new;
  }else{
    new->next=row->first;
    row->first=new;
  }
  row->size=(row->size)+1;
}
