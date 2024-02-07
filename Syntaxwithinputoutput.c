#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SampleTokens.h"
#include "Sampletokens.c"

// Function prototypes
char* parse(Token_Nodes** current);
char* parseProgram(Token_Nodes** current);
char* parseStatement(Token_Nodes** current);
char* parseAssignment(Token_Nodes** current);
char* parseArithmetic(Token_Nodes** current);
char* parseTerm(Token_Nodes** current);
char* parseFactor(Token_Nodes** current);
char* parseOutput(Token_Nodes** current);
char* parseInput(Token_Nodes** current);
Token_Nodes* advance(Token_Nodes** current);



char* parse(Token_Nodes** current){
    char* stmt;

    printf("->PROGRAM\n");
    while ((*current) != NULL){

        printf("%s\n\n", parseProgram(current));
    }

    return ""; 
}

char* parseProgram(Token_Nodes** current){
    printf("--->STATEMENT\n--");
    return parseStatement(current);
}

char* parseStatement(Token_Nodes** current){
    char* stmt;
    Token_Nodes* lookAhead = advance(current);

    // Check for Output production rule
    if (!(strcmp(lookAhead->tokenType, "OUTPUT_KEYWORD"))) {
        printf("--->OUTPUT\n----");
        stmt = parseOutput(current);
        return stmt;
    }

    // Check for Input production rule
    else if (!(strcmp(lookAhead->tokenType, "INPUT_KEYWORD"))) {
        printf("--->INPUT\n----");
        stmt = parseInput(current);
        return stmt;
    }

    // Check for Arithmetic or Assignment
    else if (!(strcmp(lookAhead->tokenType, "ADDITION_OPERATOR")) || !(strcmp(lookAhead->tokenType, "SUBTRACTION_OPERATOR")) || !(strcmp(lookAhead->tokenType, "MULTIPLICATION_OPERATOR"))
    || !(strcmp(lookAhead->tokenType, "DIVISION_OPERATOR")) || !(strcmp(lookAhead->tokenType, "EXPONENTIAL_OPERATOR")) || !(strcmp(lookAhead->tokenType, "MODULO_OPERATOR"))
    || !(strcmp(lookAhead->tokenType, "FLOOR_DIVISION_OPERATOR"))) {
        
        printf("--->ARITHMETIC\n----");
        stmt = parseArithmetic(current);
        return stmt;

    }

    else if (!(strcmp(lookAhead->tokenType, "ASS_OP")) || !(strcmp(lookAhead->tokenType, "ADD_ASS_OP")) || !(strcmp(lookAhead->tokenType, "SUB_ASS_OP"))
    || !(strcmp(lookAhead->tokenType, "MULTIPLY_ASS_OP")) || !(strcmp(lookAhead->tokenType, "DIVISION_ASS_OP")) || !(strcmp(lookAhead->tokenType, "EXPONENTIATION_ASS_OP"))
    || !(strcmp(lookAhead->tokenType, "MODULO_ASS_OP")) || !(strcmp(lookAhead->tokenType, "FLOOR_DIVISION_ASS_OP"))) {

        printf("--->ASSIGNMENT\n----");
        stmt = parseAssignment(current);
        return stmt;
    }
    
    return stmt;
}

char* parseArithmetic(Token_Nodes** current){
    char* stmt = malloc(100);
    char* arithStmt;

    printf("--->TERM\n------");
    if ((arithStmt = parseTerm(current)) != NULL){
        sprintf(stmt, arithStmt);
        
        printf("--");
        while (((*current)->tokenType) != NULL && strcmp((*current)->tokenType, "NEWLINE")){
            if (!(strcmp ((*current)->tokenType, "ADDITION_OPERATOR")) || !(strcmp ((*current)->tokenType, "SUBTRACTION_OPERATOR"))){
                printf("----->PARSED %s: %s\n----", (*current)->tokenType,(*current)->tokenValue);
                sprintf(stmt + strlen(stmt), "%s ", (*current)->tokenValue);
                    
                *current = advance(current);
                
                printf("--->TERM\n------");
                if ((arithStmt = parseTerm(current))){
                    sprintf(stmt + strlen(stmt), arithStmt);
                        
                }
                    
                else{
                    printf("SYNTAX ERROR: UNEXPECTED TOKEN TYPE: %s\n\n", (*current)->tokenType);
                    *current = advance(current);
                    return "";
                }   
            }
                
            if ((*current)->tokenType != NULL){
                if (!(strcmp((*current)->tokenType, "CLOSED_PARENTHESIS"))){
                    return stmt;
                }
            }
        }
        
        if ((*current) != NULL){
            *current = advance(current);
        }

        printf("------->ARITHMETIC STATEMENT PARSED: ");
        return stmt;  
    }

    return NULL;
}

char* parseTerm(Token_Nodes** current){
    char* stmt = malloc(100);
    char* termStmt = malloc(100);

    printf("--->FACTOR\n--------");
    if ((termStmt = parseFactor(current)) != NULL){
        sprintf(stmt, "%s ", termStmt); 

        while ((*current) != NULL){
            if (!(strcmp ((*current)->tokenType, "MULTIPLICATION_OPERATOR")) || !(strcmp ((*current)->tokenType, "DIVISION_OPERATOR"))
            || !(strcmp ((*current)->tokenType, "EXPNOENTIATION_OPERATOR")) || !(strcmp ((*current)->tokenType, "MODULO_OPERATOR"))
            || !(strcmp ((*current)->tokenType, "FLOOR_DIVISION_OPERATOR"))){

                printf("----->PARSED %s: %s\n----", (*current)->tokenType,(*current)->tokenValue);
                sprintf(stmt + strlen(stmt), "%s ", (*current)->tokenValue);

                *current = advance(current);
                if ((termStmt = parseFactor(current))){
                        sprintf(stmt + strlen(stmt), termStmt);
                        
                }

                else{
                        printf("SYNTAX ERROR: UNEXPECTED TOKEN TYPE: %s\n\n", (*current)->tokenType);
                        *current = advance(current);
                        return "";
                }
            }
            
            else{
                return stmt;
            }
        }

        return stmt;
    }

    return NULL;
}

char* parseFactor(Token_Nodes** current){

    char* stmt = malloc(100);
    char* factorStmt = malloc(100);
    if (!(strcmp((*current)->tokenType, "IDENTIFIER"))){
        printf("--->PARSED FACTOR: %s\n", (*current)->tokenValue);
        sprintf(stmt, (*current)->tokenValue);

        *current = advance(current);

        return stmt;
    }

    else if (!(strcmp((*current)->tokenType, "NUMBER_LITERAL"))){
        printf("--->PARSED FACTOR: %s\n", (*current)->tokenValue);
        sprintf(stmt, (*current)->tokenValue);

        *current = advance(current);

        return stmt;
    }

    else if (!(strcmp((*current)->tokenType, "OPEN_PARENTHESIS"))){
        printf("--->PARSED %s\n----------", (*current)->tokenValue);
        sprintf(stmt, (*current)->tokenValue);

        *current = advance(current);

        if ((factorStmt = parseArithmetic(current)) == NULL){
            printf("SYNTAX ERROR: EXPECTED ARITHMETIC STATEMENT");
            return "";
        }

        sprintf(stmt + strlen(stmt), "%s ", factorStmt);

        if ((strcmp((*current)->tokenType, "CLOSED_PARENTHESIS"))){
            printf("SYNTAX ERROR: EXPECTED "")\"");
            return "";
        }

        printf("--->PARSED %s\n", (*current)->tokenValue);
        sprintf(stmt + strlen(stmt), "%s ", (*current)->tokenValue);

        *current = advance(current);

        return stmt;
    }

    return NULL;
}

char* parseAssignment(Token_Nodes** current){
    char* stmt = malloc(100);

    if (!(strcmp((*current)->tokenType, "IDENTIFIER"))){
        printf("--->PARSED %s: %s\n----", (*current)->tokenType, (*current)->tokenValue);
        sprintf(stmt, "--->PARSED ASSIGNMENT STATEMENT: %s", (*current)->tokenValue);

        *current = advance(current);

        if (!(strcmp((*current)->tokenType, "ASS_OP"))){
            printf("--->PARSED %s: %s\n----", (*current)->tokenType, (*current)->tokenValue);
            sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

            *current = advance(current);

            if (!(strcmp((*current)->tokenType, "IDENTIFIER"))){
                printf("--->PARSED %s: %s\n------", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

                *current = advance(current);
                return stmt;
            }

            else if (!(strcmp((*current)->tokenType, "BOOLEAN"))){
                printf("--->PARSED %s: %s\n------", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

                *current = advance(current);
                return stmt;
            }

            else{
                return "--->SYNTAX ERROR";
            }
        }

        else if (!(strcmp((*current)->tokenType, "ADD_ASS_OP")) || !(strcmp((*current)->tokenType, "SUB_ASS_OP")) || !(strcmp((*current)->tokenType, "MULTIPLY_ASS_OP"))
        || !(strcmp((*current)->tokenType, "DIVISION_ASS_OP")) || !(strcmp((*current)->tokenType, "EXPONENTIATION_ASS_OP"))|| !(strcmp((*current)->tokenType, "MODULO_ASS_OP"))
        || !(strcmp((*current)->tokenType, "FLOOR_DIVISION_ASS_OP"))) {

            printf("--->PARSED %s: %s\n----", (*current)->tokenType, (*current)->tokenValue);
            sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

            *current = advance(current);

            if (!(strcmp((*current)->tokenType, "IDENTIFIER"))){
                printf("--->PARSED %s: %s\n------", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

                *current = advance(current);
                return stmt;
            }

            else if (!(strcmp((*current)->tokenType, "BOOLEAN"))){
                printf("--->PARSED %s: %s\n------", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

                *current = advance(current);
                return stmt;
            }

            else{
                return "SYNTAX ERROR";
            }
        }

    }

    return NULL;
}

char* parseOutput(Token_Nodes** current) {
    char* stmt = malloc(100);

    if (!(strcmp((*current)->tokenType, "OUTPUT_KEYWORD"))) {
        printf("--->PARSED %s: %s\n----", (*current)->tokenType, (*current)->tokenValue);
        sprintf(stmt, "--->PARSED OUTPUT STATEMENT: %s ", (*current)->tokenValue);

        *current = advance(current);

        if (!(strcmp((*current)->tokenType, "OPEN_PARENTHESIS"))){
            printf("--->PARSED %s: %s\n----", (*current)->tokenType, (*current)->tokenValue);
            sprintf(stmt + strlen(stmt),  "%s", (*current)->tokenValue);

            *current = advance(current);

            // Add parsing for output content
            // You can extend this part based on your grammar

            if (!(strcmp((*current)->tokenType, "NUMBER_LITERAL")) || !(strcmp((*current)->tokenType, "STRING_LITERAL"))
                || !(strcmp((*current)->tokenType, "IDENTIFIER")) || !(strcmp((*current)->tokenType, "RELATIONAL_OPERATOR"))
                || !(strcmp((*current)->tokenType, "LOGICAL_OPERATOR"))){
                
                printf("--->PARSED %s: %s\n----", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  "%s", (*current)->tokenValue);

                *current = advance(current);

                if (!(strcmp((*current)->tokenType, "CLOSED_PARENTHESIS"))){
                    printf("--->PARSED %s: %s\n----", (*current)->tokenType, (*current)->tokenValue);
                    sprintf(stmt + strlen(stmt),  "%s", (*current)->tokenValue);

                    *current = advance(current);
                    return stmt;
                }
            }
        }
    }

    return NULL;
}

char* parseInput(Token_Nodes** current) {
    char* stmt = malloc(100);

    if (!(strcmp((*current)->tokenType, "INPUT_KEYWORD"))) {
        printf("--->PARSED %s: %s\n----", (*current)->tokenType, (*current)->tokenValue);
        sprintf(stmt, "--->PARSED INPUT STATEMENT: %s ", (*current)->tokenValue);

        *current = advance(current);

        if (!(strcmp((*current)->tokenType, "OPEN_PARENTHESIS"))){
            printf("--->PARSED %s: %s\n----", (*current)->tokenType, (*current)->tokenValue);
            sprintf(stmt + strlen(stmt),  "%s", (*current)->tokenValue);

            *current = advance(current);

            // Add parsing for input content
            // You can extend this part based on your grammar

            if (!(strcmp((*current)->tokenType, "STRING_LITERAL"))){
                printf("--->PARSED %s: %s\n----", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  "%s", (*current)->tokenValue);

                *current = advance(current);

                if (!(strcmp((*current)->tokenType, "CLOSED_PARENTHESIS"))){
                    printf("--->PARSED %s: %s\n----", (*current)->tokenType, (*current)->tokenValue);
                    sprintf(stmt + strlen(stmt),  "%s", (*current)->tokenValue);

                    *current = advance(current);
                    return stmt;
                }
            }
        }
    }

    return NULL;
}

Token_Nodes* advance(Token_Nodes** current){
    return (*current)->next;
}
int main() {
    Token_Nodes* tokens = SampleTokens();
    Token_Nodes* current = tokens;

    // Print the tokens
    printf("Sample Tokens:\n");
    while (current != NULL) {
        printf("Token Type: %s, Token Value: %s\n", current->tokenType, current->tokenValue);
        current = current->next;
    }

    // Free the memory allocated for the tokens
    freeList(tokens);

    return 0;
}
