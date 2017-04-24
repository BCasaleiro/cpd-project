typedef struct _localization {
  int x,y,z;
} localization;

typedef struct _List {
  localization data;
  struct _List *next;
} List;

typedef struct _Row {
  List *first;
  int size;
} Row;

void printList(Row *row);

void freeList(Row *row);

void init(Row *row);

List *newNodeList(localization x);

void add(Row *row, localization x);
