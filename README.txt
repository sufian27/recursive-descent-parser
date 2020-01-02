Author: Sufian Mushtaq

>>>>>> DETAILS <<<<<<

The project is a recursive descent parser written in C for the following grammar:

<E> --> <C><F>
<F> --> \<E>|0
<C> --> <S><G>
<G> --> .<C>|0
<S> --> <A><H>
<H> --> *<H>|0
<A> --> (<E>)|<X>
<X> --> a|b|...|z

Note: Terminal symbol \ represents union operator | in regular expressions. The 0 represents an epsilon (empty string). 

The parser outputs a parse tree of the expression that's entered by the user and "solves" it by outputting a simplified equivalent expression in english. The parse tree has its root on the left most and increasing depth towards the right with every tab level indicating the depth level. For example, the expression a.b|c would output the following tree and solved expression:
Tree:
E
  C
    S
      A
        X
          a
      H
        0
    G
      .
      C
        S
          A
            X
              b
          H
            0
        G
          0
  F
    |
    E
      C
        S
          A
            X
              c
          H
            0
        G
          0
      F
        0
Solved Expression: (UNION (CONCAT (ATOMIC a) (ATOMIC b)) (ATOMIC c))

>>>>>> INPUT <<<<<<
Input a regular expression with a period symbol (.) to represent concatenation, star (*) to represent closure, and pipe (|) to represent union. Also, for now, the program accepts terminal symbols from lower case a-z inclusive, and parentheses (). 

>>>>>> COMPILE AND RUN <<<<<<

Compile from the terminal using command "gcc -Wall -Werror -std=c99 rexp.c tree.c" and run the .exe file.

>>>>>> FILES <<<<<<
rexp.c --> Main file that contains all the computation.
tree.c --> Contains the definition of the tree data structure.
tree.h --> Header file for tree.c.


