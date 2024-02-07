#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SampleTokens.h"



/*typedef struct Token_Nodes{
    char tokenType[50];
    char* tokenValue;
    struct Token_Nodes *next;
} Token_Nodes;*/

Token_Nodes* createNode(char* tokenType, char* tokenValue){
    Token_Nodes* newNode = malloc(sizeof(Token_Nodes));

    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    strcpy(newNode->tokenType, tokenType);
    newNode->tokenValue = strdup(tokenValue);
    newNode->next = NULL;
    return newNode;
}

void freeList(Token_Nodes* head) {

    while (head != NULL) {
        Token_Nodes* temp = head;
        head = head->next;

        free(temp);
    }
}

int SampleTokens () {

    /*head = createNode("NUMBER_LITERAL", "1");
    head->next = createNode("SUBTRACTION_OPERATOR", "-");
    head->next->next = createNode("OPEN_PARENTHESIS", "(");
    head->next->next->next = createNode("IDENTIFIER", "var2");
    head->next->next->next->next = createNode("ADDITION_OPERATOR", "+");
    head->next->next->next->next->next = createNode("NUMBER_LITERAL", "5");
    head->next->next->next->next->next->next = createNode("CLOSED_PARENTHESIS", ")");
    //head->next->next->next->next->next->next->next = createNode("NEWLINE", " \\n ");
    
    Token_Nodes* temp = head->next->next->next->next->next->next;
    temp->next = createNode("IDENTIFIER", "var3");
    temp->next->next = createNode("MULTIPLICATION_OPERATOR", "*");
    temp->next->next->next = createNode("NUMBER_LITERAL", "25");
    


    /*for (Token_Nodes* current = head; current != NULL; current = current->next){
        printf("%s: %s\n", current->tokenType, current->tokenValue);
    }*/

    head = createNode("IDENTIFIER", "var2");
    head->next = createNode("ASS_OP", "=");
    head->next->next = createNode("BOOLEAN", "TRUE");

    //freeList(head);
    return 0;

}