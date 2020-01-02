#ifndef TREE_H_
#define TREE_H_
#include <stdlib.h>
#define FAILED NULL
//From Ullman
typedef struct NODE *TREE;
struct NODE {
    char label;
    TREE leftmostChild, rightSibling;
};

TREE makeNode0(char x);
TREE makeNode4(char x, TREE t1, TREE t2, TREE t3, TREE t4);
TREE makeNode2(char x, TREE t1, TREE t2);
TREE makeNode3(char x, TREE t1, TREE t2, TREE t3);
TREE makeNode1(char x, TREE t);

void printParseTree(TREE parseTree, int level);

TREE E();
TREE ET();
TREE C();
TREE CT();
TREE S();
TREE ST();
TREE A();
TREE X();

#endif
