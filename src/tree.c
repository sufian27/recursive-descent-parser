#include "tree.h"
#include <stdlib.h>
#include <stdio.h>

/*
    x
*/
TREE makeNode0(char x) {
    TREE root;
    root = (TREE) malloc(sizeof(struct NODE));
    root->label = x;
    root->leftmostChild = NULL;
    root->rightSibling = NULL;
    return root;
}

/*
    x
  /
t1
*/
TREE makeNode1(char x, TREE t) {
    TREE root;
    root = makeNode0(x);
    root->leftmostChild = t;
    return root;
}

/*
    x
  /   \
t1 --> t2
*/
TREE makeNode2(char x, TREE t1, TREE t2) {
    TREE root;
    root = makeNode1(x, t1);
    t1->rightSibling = t2;
    return root;
}

/*
    x
  /  \       \
t1 --> t2 --> t3
*/
TREE makeNode3(char x, TREE t1, TREE t2, TREE t3) {
    TREE root;
    root = makeNode1(x, t1);
    t1->rightSibling = t2;
    t2->rightSibling = t3;
    return root;
}

/*
    x
  /  \       \      \
t1 --> t2 --> t3 --> t4
*/
TREE makeNode4(char x, TREE t1, TREE t2, TREE t3, TREE t4) {
    TREE root;
    root = makeNode1(x, t1);
    t1->rightSibling = t2;
    t2->rightSibling = t3;
    t3->rightSibling = t4;
    return root;
}


/**
 * Prints parse tree in the required format.
 */
void printParseTree(TREE parseTree, int level) {
    TREE c;
    for(int i = 0; i<=level; i++) {
        printf("  ");
    }
    printf("%c\n", parseTree->label);
    c = parseTree->leftmostChild;
    while(c!=NULL) {
        printParseTree(c, level+1);
        c = c->rightSibling;
    }
}
