#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include <string.h>


TREE parseTree; //holds the result of the parse
char* nextTerminal;//input array
int valid = 0; //variable to see if a valid parse tree has been generated

/*****Recursive Descent starts here******/
TREE E() {
    TREE treeC = C();
    if (treeC != FAILED) {
        TREE treeET = ET();
        if (treeET != FAILED) { //if ET hasn't failed and entire input has been read
            return makeNode2('E', treeC, treeET);
        } else {
            return FAILED;
        }
    } else {
        return FAILED;
    }
}

TREE ET() { //also called F
    if (*nextTerminal == '|') { //first production
        nextTerminal++;
        TREE treeE = E();
        if (treeE != FAILED) {
            return makeNode2('F', makeNode0('|'), treeE);
        } else {
            return FAILED;
        }
    } else { //second production
        return makeNode1('F', makeNode0('0'));
    }
}

//Concat
TREE C() {
    TREE treeS = S();
    if (treeS != FAILED){
        TREE treeCT = CT();
        if (treeCT != FAILED) {
            return makeNode2('C', treeS, treeCT);
        } else {
            return FAILED;
        }
    } else {
        return FAILED;
    }
}

//Closure
TREE S() {
    TREE treeA = A();
    if (treeA != FAILED) {
        TREE treeST = ST();
        if (treeST != FAILED) {
            return makeNode2('S', treeA, treeST);
        } else {
            return FAILED;
        }
    } else {
        return FAILED;
    }
}

//Concat helper
TREE CT() { //also called G
    if (*nextTerminal == '.') { //first production
        nextTerminal++; //move pointer to next
        TREE treeC = C();
        if (treeC != FAILED) {
            return makeNode2('G', makeNode0('.'), treeC);
        } else {
            return FAILED;
        }
    } else { //second production
        return makeNode1('G', makeNode0('0')); //epsilon is represented by 0
    }
}

TREE A() {
    TREE treeE;
    if(*nextTerminal == '(') { //first production
        nextTerminal++;
        treeE = E();
        if(treeE != FAILED && *nextTerminal==')') {
            nextTerminal++;
            return makeNode3('A', makeNode0('('), treeE, makeNode0(')'));
        } else {
            return FAILED;
        }
    } else { //second production
        TREE treeX = X();
        if (treeX != FAILED) {
            return makeNode1('A', treeX);
        } else {
            return FAILED;
        }
    }
}

TREE ST() {//also called H
    if(*nextTerminal == '*') {
        nextTerminal++;
        TREE treeST = ST();
        if (treeST != FAILED) {
            return makeNode2('H', makeNode0('*'), ST());
        } else {
            return FAILED;
        }
    } else  {
        return makeNode1('H', makeNode0('0'));
    }
}

TREE X() {
    if (*nextTerminal >= 'a' && *nextTerminal <= 'z') {
        char x = *nextTerminal;
        nextTerminal++;
        return makeNode1('X', makeNode0(x));
    }
    return FAILED;
}

void runRecursiveDescent() {
    parseTree = E(); //starts with E symbol
    if(parseTree==0 || *nextTerminal != 0) { //if null parsetree or entire input hasn't been read
        valid = 0;
        printf("\nInvalid Expression - Could not make parse tree! \n");
    } else {
        valid = 1;
        printf("\nSuccessful Recursive Descent Parsing. Below is the PARSE TREE:\n");
        printParseTree(parseTree, -1);
    }
}

char* concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

//Method that interprets the parse tree as an expression tree and evaluates the expression
char* evaluate(TREE t) {
    if(t->label == 'E') {
        char* str1 = evaluate(t->leftmostChild);
        char* str2 = evaluate(t->leftmostChild->rightSibling);
        if (strcmp(str1, "0") == 0) {
            return str2;
        } 
        if (strcmp(str2, "0") == 0) {
            return str1;
        }
        return concat(concat(concat(concat("(UNION ", str1), " "), str2), ")");
    }
    if(t->label >= 97 && t->label <= 122) { //for X
        char label[2]; //converting char to string
        label[0] = t->label;
        label[1] = '\0';
        return concat(concat("(ATOMIC ", label), ")");
    }
    if(t->label == 'S') { //will call A and H
        char* str1 = evaluate(t->leftmostChild);
        char* str2;
        if(t->leftmostChild->rightSibling) str2 = evaluate(t->leftmostChild->rightSibling);
        if(strcmp(str2, "*") != 0) { //if there is a star
            return str1; //if no star just return normal expression
        } 
        return concat(concat("(CLOSURE ", str1), ")");
    }
    if(t->label == 'C') {
        char* str1 = evaluate(t->leftmostChild);
        char* str2;
        if(t->leftmostChild->rightSibling) str2 = evaluate(t->leftmostChild->rightSibling); //incorrect value comes here
        if (strcmp(str1, "0") == 0) {
            return str2;
        } 
        if (strcmp(str2, "0") == 0) {
            return str1;
        }
        return concat(concat(concat(concat("(CONCAT ", str1), " "), str2), ")");
    }
    if(t->label == 'G') {
        char* str1 = evaluate(t->leftmostChild);
        char* str2;
        if(t->leftmostChild->rightSibling) str2 = evaluate(t->leftmostChild->rightSibling);;
        if (strcmp(str1, ".") == 0) {
            return str2;
        } 
        return "0"; //if left child is not '.' it will be '0'
    }
    if(t->label == 'F') {
        char* str1 = evaluate(t->leftmostChild);
        char* str2;
        if(t->leftmostChild->rightSibling) str2 = evaluate(t->leftmostChild->rightSibling);
        if (strcmp(str1, "|") == 0) {
            return str2;
        } 
        return "0"; //if left child is not '|' it will be '0'
    }
    if(t->label == 'H') {
        char* str1 = evaluate(t->leftmostChild);
        if (strcmp(str1, "*") == 0) { //if there is one star, keep pushing it up
            return str1;
        } 
        return "0"; //if left child is not '.' it will be '0'
    }
    if(t->label == 'A') {
        char* str1 = evaluate(t->leftmostChild);
        char* str2;
        if(t->leftmostChild->rightSibling) str2 = evaluate(t->leftmostChild->rightSibling); 
        if(strcmp(str1, "(") == 0) { //brackets are ignored in this case
            return str2; //this will be E
        }
        return str1; //otherwise str1 will be a letter
    }
    if(t->label == 'X') {
        char* str1 = evaluate(t->leftmostChild);
        return str1;
    }
    if(t->label == '(') return "(";
    if(t->label == '0') return "0";
    if(t->label == '*') return "*";
    if(t->label == '|') return "|";
    if(t->label == '.') return ".";
    return "0";
}

/**
 * Driver function for solving the tree, this will only run if valid = 1
 */
void solveTree() {
    if(valid==1) {
        printf("\nSolved Expression: %s", evaluate(parseTree));
    } else {
        printf("\nInvalid Expression - Could not evaluate expression!");
    }
}

int main() {
    nextTerminal = (char*)malloc(sizeof(char)*100); //array of 100 characters

    printf("Please Enter a RegEx input: ");
    scanf("%s", nextTerminal);
    //starts recursive descent
    runRecursiveDescent();

    //solves tree
    solveTree();

    printf("\n\nEXIT PROGRAM");
}


