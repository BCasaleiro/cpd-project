#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"tree.h"
#include"list.h"

int countNeighbours(Tree ****hash, localization x, int n){
  int i=0;
  if(x.x==0){
    if(find(x.z, (*hash)[n-1][x.y]->root)!=NULL){
      i++;
    }
  }else{
    if(find(x.z, (*hash)[x.x-1][x.y]->root)!=NULL){
      i++;
    }
  }
  if (x.x==n-1) {
    if(find(x.z, (*hash)[0][x.y]->root)!=NULL){
      i++;
    }
  }else{
    if(find(x.z, (*hash)[x.x+1][x.y]->root)!=NULL){
      i++;
    }
  }
  if (x.y==0) {
    if(find(x.z, (*hash)[x.x][n-1]->root)!=NULL){
      i++;
    }
  }else{
    if(find(x.z, (*hash)[x.x][x.y-1]->root)!=NULL){
      i++;
    }
  }
  if (x.y==n-1) {
    if(find(x.z, (*hash)[x.x][0]->root)!=NULL){
      i++;
    }
  }else{
    if(find(x.z, (*hash)[x.x][x.y+1]->root)!=NULL){
      i++;
    }
  }
  if (x.z==0) {
    if(find(n-1, (*hash)[x.x][x.y]->root)!=NULL){
      i++;
    }
  }else{
    if(find(x.z-1, (*hash)[x.x][x.y]->root)!=NULL){
      i++;
    }
  }
  if (x.z==n-1) {
    if(find(0, (*hash)[x.x][x.y]->root)!=NULL){
      i++;
    }
  }else{
    if(find(x.z+1, (*hash)[x.x][x.y]->root)!=NULL){
      i++;
    }
  }
  return i;
}


void preOrder(Node *root, Tree ****hash, int i, int j, Row *insert, int n)
{
    localization x;
    int c;
    if(root != NULL)
    {
        x.x=i;x.y=j;x.z=root->data;
        c = countNeighbours(hash,x, n);
        if(c==4){
          add(insert, x);
        }
        if (i==n-1) {
          x.x=0;x.y=j;x.z=root->data;
        }else{
          x.x=i + 1;x.y=j;x.z=root->data;
        }
        c = countNeighbours(hash,x, n);
          if (c==2||c==3) {
            add(insert, x);
          }
        if (i==0) {
          x.x=n-1;x.y=j;x.z=root->data;
        }else{
          x.x=i - 1;x.y=j;x.z=root->data;
        }
        c = countNeighbours(hash,x, n);
          if (c==2||c==3) {
            add(insert, x);
          }
        if (j==n-1) {
          x.x=i;x.y=0;x.z=root->data;
        }else{
          x.x=i;x.y=j+1;x.z=root->data;
        }
        c = countNeighbours(hash,x, n);
          if (c==2||c==3) {
            add(insert, x);
          }
        if (j==0) {
          x.x=i;x.y=n-1;x.z=root->data;
        }else{
          x.x=i;x.y=j-1;x.z=root->data;
        }
        c = countNeighbours(hash,x, n);
          if (c==2||c==3) {
            add(insert, x);
          }
        if (root->data==n-1) {
          x.x=i;x.y=j;x.z=0;
        }else{
          x.x=i;x.y=j;x.z=root->data+1;
        }
        c = countNeighbours(hash,x, n);
          if (c==2||c==3) {
            add(insert, x);
          }
        if (root->data==0) {
          x.x=i;x.y=j;x.z=n-1;
        }else{
          x.x=i;x.y=j;x.z=root->data-1;
        }
        c = countNeighbours(hash,x, n);
          if (c==2||c==3) {
            add(insert, x);
          }
        preOrder(root->left, hash, i, j, insert, n);
        preOrder(root->right, hash, i, j, insert, n);
    }
}

void nextGen(Tree ****hash, Row *insert, int n, Tree ****hash1){
  int i,j;
  List *aux;
  (*hash1) = (Tree***)malloc(n*n*sizeof(Tree**));
  for (i = 0; i < n; i++) {
    (*hash1)[i]=(Tree**)malloc(n*sizeof(Tree*));
    for (j = 0; j < n; j++) {
      (*hash1)[i][j]=(Tree*)malloc(sizeof(Tree));
      initTree((*hash1)[i][j]);
    }
  }
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      preOrder((*hash)[i][j]->root, hash, i, j, insert, n);
    }
  }
  for (aux = insert->first; aux!=NULL; aux=aux->next) {
    (*hash1)[aux->data.x][aux->data.y]->root = insertTree(aux->data.z, (*hash1)[aux->data.x][aux->data.y]->root);
    (*hash1)[aux->data.x][aux->data.y]->size=((*hash1)[aux->data.x][aux->data.y]->size) + 1;
  }
}

int readFile(Tree ****hash, char *file){
  FILE *fp;
  int x,y,z,n;
  int i,j;
  fp=fopen(file,"r");
  fscanf(fp, "%d", &n);
  (*hash) = (Tree***)malloc(n*n*sizeof(Tree**));
  for (i = 0; i < n; i++) {
    (*hash)[i]=(Tree**)malloc(n*sizeof(Tree*));
    for (j = 0; j < n; j++) {
      (*hash)[i][j]=(Tree*)malloc(sizeof(Tree));
      initTree((*hash)[i][j]);
    }
  }
  while (fscanf(fp,"%d %d %d", &x, &y, &z)==3) {
    (*hash)[x][y]->root=insertTree(z,(*hash)[x][y]->root);
    (*hash)[x][y]->size=((*hash)[x][y]->size) + 1;
  }
  fclose(fp);
  return n;
}
