//
// Created by Kiku on 12/5/2022.
//

struct treeNode;
struct cross;
struct arm;

int tokens();
bool findSides(char *input, cross * c);
char * readUntil(char * input, char until);
char * readInput();
char * getBetween(char delim, char ndelim, char * inp);
bool isNumber(char * in );
treeNode * generateUniverse(cross c, treeNode root);
#ifndef ZVONY_CPP_TOKENS_H
#define ZVONY_CPP_TOKENS_H

#endif //ZVONY_CPP_TOKENS_H
