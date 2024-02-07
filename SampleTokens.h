#ifndef SAMPLETOKENS_H
#define SAMPLETOKENS_H

typedef struct Token_Nodes {
    char tokenType[50];
    char* tokenValue;
    struct Token_Nodes* next;
} Token_Nodes;

Token_Nodes* createNode(char* tokenType, char* tokenValue);

extern void freeList(Token_Nodes* head);

Token_Nodes* head;


#endif