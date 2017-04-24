int countNeighbours(Tree ****hash, localization x, int n);

void preOrder(Node *root, Tree ****hash, int i, int j, Row *insert, Row *delete, int n);

void nextGen(Tree ****hash, Row **insert, Row **delete, int n);

int readFile(Tree ****hash, char *file);
