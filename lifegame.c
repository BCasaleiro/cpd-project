#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"list.h"
#include"tree.h"
#include"aux.h"

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Usage: lifegame <infile> <iterations>.\n");
    exit(-1);
  }

  char name[246];
  strcpy(name, argv[1]);
  int k = strtol(argv[2],NULL,10);

  Tree ****hash = (Tree****)malloc(sizeof(Tree****));
  Tree ****hash1 = (Tree****)malloc(sizeof(Tree****));
  int i;
  int n;
  FILE *fp;
  fp = fopen("result.out", "w");
  Row *insert=(Row*)malloc(sizeof(Row));
  init(insert);
  n = readFile(hash, name);
  for (i = 0; i < k; i++){
    if (i==0) {
    }else{
      hash=hash1;
      hash1 = (Tree****)malloc(sizeof(Tree****));
    }
    init(insert);
    nextGen(hash, insert, n, hash1);
    freeList(insert);
    freeTree(hash, n);
  }
  fprintTree(hash1,n, fp);
  free(insert);
  freeTree(hash1, n);
  fclose(fp);
  return 0;
}
