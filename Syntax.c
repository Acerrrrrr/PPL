#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SampleTokens.h"
#include "Sampletokens.c"



char* parse(Token_Nodes** current);
char* parseProgram(Token_Nodes** current);
char* parseStatement(Token_Nodes** current);
char* parseAssignment(Token_Nodes** current);
char* parseArithmetic(Token_Nodes** current);
char* parseTerm(Token_Nodes** current);
char* parseFactor(Token_Nodes** current);

Token_Nodes* advance(Token_Nodes** current);

Token_Nodes* current;

char* parse(Token_Nodes** current){

    int i = 1;
    printf("->PROGRAM\n");
    while (*current != NULL){

        printf("ITERATION [%d]\n", i++);
        
        
        printf(parseProgram(current));
        //printf("---Last pointed to: %s %s\n\n", (*current)->tokenType, (*current)->tokenValue);
        *current = advance(current);
    }

    return ""; //parseProgram(&current);
}

char* parseProgram(Token_Nodes** current){

    //printf("---Last pointed to: %s %s\n\n", (*current)->tokenType, (*current)->tokenValue);
    printf("--->STATEMENT\n--");
    return parseStatement(current);
}

char* parseStatement(Token_Nodes** current){

    char* stmt;
    Token_Nodes* lookAhead = advance(current);

    if (!(strcmp(lookAhead->tokenType, "ADDITION_OPERATOR")) || !(strcmp(lookAhead->tokenType, "SUBTRACTION_OPERATOR")) || !(strcmp(lookAhead->tokenType, "MULTIPLICATION_OPERATOR"))
    || !(strcmp(lookAhead->tokenType, "DIVISION_OPERATOR")) || !(strcmp(lookAhead->tokenType, "EXPONENTIAL_OPERATOR")) || !(strcmp(lookAhead->tokenType, "MODULO_OPERATOR"))
    || !(strcmp(lookAhead->tokenType, "FLOOR_DIVISION_OPERATOR"))) {
        
        printf("--->ARITHMETIC\n--");
        stmt = (parseArithmetic(current));
        return stmt;

    }

    else if (!(strcmp(lookAhead->tokenType, "ASS_OP")) || !(strcmp(lookAhead->tokenType, "ADD_ASS_OP")) || !(strcmp(lookAhead->tokenType, "SUB_ASS_OP"))
    || !(strcmp(lookAhead->tokenType, "MULTIPLY_ASS_OP")) || !(strcmp(lookAhead->tokenType, "DIVISION_ASS_OP")) || !(strcmp(lookAhead->tokenType, "EXPONENTIATION_ASS_OP"))
    || !(strcmp(lookAhead->tokenType, "MODULO_ASS_OP")) || !(strcmp(lookAhead->tokenType, "FLOOR_DIVISION_ASS_OP"))) {

        printf("--->ASSIGNMENT\n----");
        stmt = (parseAssignment(current));
        return stmt;
    }

    /*if ((stmt = parseAssignment(current))!= NULL){
        printf("%s", stmt);
        return stmt;
    }*/

    
    return "";
}

char* parseArithmetic(Token_Nodes** current){
    //Arithmetic Statment Grammar Rule:
    //<arithmetic> ::= <term> {("+" | "-") <term>}*
    char* stmt;

    printf("--PARSING ARITHMETIC STATEMENT\n");





    //Display current pointer
    //printf("\nCurrently Pointing at: %s %s\n", (*current)->tokenType, (*current)->tokenValue);


    //Check left token if it is <term>
    printf("---->PARSING TERM: %s (%s)\n", (*current)->tokenType, (*current)->tokenValue);
    if ((stmt = parseTerm(current)) != NULL){
        
        //LookAhead to check  for '+' or '-' operator
        Token_Nodes* lookAhead = advance(current);

        //Check if next token is NULL
        //**Can change to while(?)
        if (lookAhead->tokenType != NULL){
            //printf("\nCurrently Pointing at: %s %s\n", lookAhead->tokenType, lookAhead->tokenValue);

            //Lookahead to check if operator is ADDITION or SUBTRACTION
            if (!(strcmp (lookAhead->tokenType, "ADDITION_OPERATOR")) || !(strcmp (lookAhead->tokenType, "SUBTRACTION_OPERATOR"))){
                printf("PARSED OPERATOR: %s\n", lookAhead->tokenValue);
                
                *current = advance(&lookAhead);

                //Check if current token is <term>
                printf("\nCurrently Pointing at: %s %s\n", (*current)->tokenType, (*current)->tokenValue);
                printf("--PARSING TERM\n");
                //If not, return Syntax Error
                if ((stmt = parseTerm(current)) != NULL){
                    return "ARITHMETIC STATEMENT PARSED\n\n";
                }

                else{
                    return "SYNTAX ERROR\n\n";
                }
            
            }
        }

        return stmt;
        
    }

    return NULL;
}

char* parseTerm(Token_Nodes** current){
    char* stmt;


    //Check if current token is <factor>
    printf("------>PARSING FACTOR: %s (%s)\n", (*current)->tokenType, (*current)->tokenValue);
    if ((stmt = parseFactor(current)) != NULL){

        Token_Nodes* lookAhead = advance(current);
        printf("\nCurrently Pointing at: %s %s\n", lookAhead->tokenType, lookAhead->tokenValue);


        if (!(strcmp (lookAhead->tokenType, "MULTIPLICATION_OPERATOR")) || !(strcmp (lookAhead->tokenType, "DIVISION_OPERATOR"))){
            //printf("PARSED OPERATOR: %s\n", lookAhead->tokenValue);

            while (lookAhead != NULL){
                printf("PARSED OPERATOR: %s\n", lookAhead->tokenValue);
                
                *current = advance(&lookAhead);

                printf("\nCurrently Pointing at: %s %s\n", (*current)->tokenType, (*current)->tokenValue);

                if ((stmt = parseFactor(current)) == NULL){
                    return "SYNTAX ERROR\n\n";
                }

                
                lookAhead = advance(current);
                /*if (lookAhead == NULL){
                    printf("%s", lookAhead->tokenType);
                    return "END";
                }
                //printf("\nCurrently Pointing at: %s\n", lookAhead->tokenType);*/
        }

        return "ARITHMETIC STATEMENT PARSED\n\n";
        }
        

        
        return "";
    }

    return NULL;
}

char* parseFactor(Token_Nodes** current){

    char* stmt;
    if (!(strcmp((*current)->tokenType, "IDENTIFIER"))){
        printf("-------->PARSED FACTOR: %s\n\n", (*current)->tokenValue);

        return "";
    }

    else if (!(strcmp((*current)->tokenType, "NUMBER_LITERAL"))){
        printf("-------->PARSED FACTOR: %s\n", (*current)->tokenValue);

        return "";
    }

    else if (!(strcmp((*current)->tokenType, "OPEN_PARENTHESIS"))){
        Token_Nodes* lookAhead = advance(current);
        //printf("Currently Pointing at: %s %s\n", lookahead->tokenType, lookahead->tokenValue);

        while (strcmp((*current)->tokenType, "CLOSED_PARENTHESIS")){
            if ((stmt = parseArithmetic(&lookAhead)) != NULL){
                *current = advance(&lookAhead);
            }
        }
        
        printf("PARSED CLOSED_PARENTHESIS\n\n");
        return "";
    }

    /*else if (!((stmt = parseArithmetic(current)) == NULL)) {
        return "";
    }*/

    return NULL;
}

char* parseAssignment(Token_Nodes** current){
    //Production Rule for Assignment Statement
    // <assignment> ::= (<identifier> "=" (<arithmetic>|<string> | <boolean>)) | (<identifier> ("+=" | "-=" | "*=" | "/=" | "^*=" | "%=" | "//=") (<arithmetic>))

    //Allocate memory for stmt
    char* stmt = malloc(100);

    //Check first token for <IDENTIFIER>
    if (!(strcmp((*current)->tokenType, "IDENTIFIER"))){

        //Print parsed token
        printf("--->PARSED %s: %s\n----", (*current)->tokenType, (*current)->tokenValue);

        //Print into stmt string
        sprintf(stmt, "--->PARSED ASSIGNMENT STATEMENT: %s", (*current)->tokenValue);

        //Advance Token
        *current = advance(current);

        //Check if <ASS_OP>
        if (!(strcmp((*current)->tokenType, "ASS_OP"))){

            //Print parsed token
            printf("--->PARSED %s: %s\n----", (*current)->tokenType, (*current)->tokenValue);

            //Print into stmt string
            sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

            //Advance Token
            *current = advance(current);

            //Check if <IDENTIFIER>
            if (!(strcmp((*current)->tokenType, "IDENTIFIER"))){
                printf("--->PARSED %s: %s\n------", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);
                return stmt;
            }

            //Or check if <BOOLEAN>
            else if (!(strcmp((*current)->tokenType, "BOOLEAN"))){
                printf("--->PARSED %s: %s\n------", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);
                return stmt;
            }

            //Or check if <ARITHMETIC>
            //Insert Arithmetic Rule

            //If checks fail, return Error
            else{
                return "--->SYNTAX ERROR";
            }
        }

        //Or check if Compound Assignment
        else if (!(strcmp((*current)->tokenType, "ADD_ASS_OP")) || !(strcmp((*current)->tokenType, "SUB_ASS_OP")) || !(strcmp((*current)->tokenType, "MULTIPLY_ASS_OP"))
        || !(strcmp((*current)->tokenType, "DIVISION_ASS_OP")) || !(strcmp((*current)->tokenType, "EXPONENTIATION_ASS_OP"))|| !(strcmp((*current)->tokenType, "MODULO_ASS_OP"))
        || !(strcmp((*current)->tokenType, "FLOOR_DIVISION_ASS_OP"))) {

            //Print parsed token
            printf("--->PARSED %s: %s\n----", (*current)->tokenType, (*current)->tokenValue);

            //Print into stmt string
            sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

            //Advance Token
            *current = advance(current);

            //Check if <IDENTIFIER>
            if (!(strcmp((*current)->tokenType, "IDENTIFIER"))){
                printf("--->PARSED %s: %s\n------", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);
                return stmt;
            }

            //Or check if <BOOLEAN>
            else if (!(strcmp((*current)->tokenType, "BOOLEAN"))){
                printf("--->PARSED %s: %s\n------", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);
                return stmt;
            }

            //Insert Arithmetic Rule

            else{
                return "SYNTAX ERROR";
            }

        }

    }

    return NULL;
}



Token_Nodes* advance(Token_Nodes** current){
    
    
    return (*current)->next;
}

int main (){
    SampleTokens();
    current = head;
    //Token_Nodes *currentptr = &current;

    printf("%s", parse(&current));

    printf("\n\nEND");

    return 0;
}