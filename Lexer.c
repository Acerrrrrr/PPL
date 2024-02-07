#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

// Token types
enum TokenType {
    STRING_LITERAL, NUMBER_LITERAL, IDENTIFIER, KEYWORD, ADDITION_OPERATOR, SUBTRACTION_OPERATOR, MULTIPLICATION_OPERATOR, DIVISION_OPERATOR,
    EXPONENTIATION_OPERATOR, MODULO_OPERATOR, FLOOR_DIVISION_OPERATOR, ASSIGNMENT_OPERATOR, ADDITION_ASSIGNMENT_OPERATOR, SUBTRACTION_ASSIGNMENT_OPERATOR,
    MULTIPLICATION_ASSIGNMENT_OPERATOR, DIVISION_ASSIGNMENT_OPERATOR, EXPONENTIATION_ASSIGNMENT_OPERATOR, MODULUS_ASSIGNMENT_OPERATOR, FLOOR_DIVISION_ASSIGNMENT_OPERATOR,
    IS_EQUAL_OPERATOR, NOT_EQUAL_OPERATOR, GREATER_THAN_OPERATOR, LESS_THAN_OPERATOR, GREATER_EQUAL_OPERATOR, LESS_EQUAL_OPERATOR,
    AND_OPERATOR, OR_OPERATOR, NOT_OPERATOR, KEYWORDS, RESERVE_WORD, CONSTANT_NUMBER, CONSTANT_DECIMAL, CONSTANT_TEXT, CONSTANT_COLLECTION, CONSTANT_SWITCH,
    NOISE_WORD, COMMENT, SEMICOLON, COLON, OPEN_PARENTHESIS, CLOSED_PARENTHESIS, OPEN_CURLY, CLOSED_CURLY,
    QUOTATION_MARKS, COMMAS, OPEN_BRACKET, CLOSED_BRACKET, INVALID, END_OF_FILE,
};

// Token structure
struct Token {
    enum TokenType type;
    char lexeme[100];
};

// Function to check if a character is a valid identifier character
int isValidIdentifierChar(char ch) {
    return isalnum(ch) || ch == '_';
}

// Function to check if a character is a valid numeric literal character
int isValidNumberChar(char ch) {
    return isdigit(ch) || ch == '.';
}

char prevChar;
int quoteCount = 0;

// Check if the given string 'str' matches the specified keyword.
bool isKeyword(char *str, const char *keyword) {
    for (int i = 0; keyword[i] != '\0'; i++) {
        if (str[i] != keyword[i]) {
            return false;
        }
    }
    return !isalnum(str[strlen(keyword)]);
}

bool isKeywordExtended(char *str) {
    return isKeyword(str, "during") ||
           isKeyword(str, "unless") ||
           isKeyword(str, "next")   || 
           isKeyword(str, "enough") ||
           isKeyword(str, "otherwise") ||
           isKeyword(str, "when") ||
           isKeyword(str, "lin") ||
           isKeyword(str, "del") ||
           isKeyword(str, "lout") ||
           isKeyword(str, "pass");
}

bool isReserveWord(char *str) {
    return isKeyword(str, "on") ||
           isKeyword(str, "off") ||
           isKeyword(str, "none");
}

bool isNoiseWord(char *str) {
    return isKeyword(str, "ete") ||
           isKeyword(str, "delete") ||
           isKeyword(str, "es")   || 
           isKeyword(str, "passes") ||
           isKeyword(str, "ber") ||
           isKeyword(str, "number") ||
           isKeyword(str, "imal") ||
           isKeyword(str, "decimal") ||
           isKeyword(str, "e") ||
           isKeyword(str, "text") ||
           isKeyword(str, "lect") ||
           isKeyword(str, "collect") ||
           isKeyword(str, "itc") ||
           isKeyword(str, "switch");
}

bool isConstantNum(char *str) {
    return isKeyword(str, "num");
}

bool isConstantDec(char *str) {
    return isKeyword(str, "dec");
}

bool isConstantCol(char *str) {
    return isKeyword(str, "col");
}

bool isConstantTxt(char *str) {
    return isKeyword(str, "txt");
}

bool isConstantSwtch(char *str) {
    return isKeyword(str, "swh");
}

// Function to analyze lexemes and determine tokens
struct Token getNextToken(FILE* file) {
    struct Token token;
    char currentChar;

    

    // Skip whitespaces and comments
    do {
        currentChar = fgetc(file);
    } while (isspace(currentChar));

    // Check for end of file
    if (currentChar == EOF) {
        token.type = END_OF_FILE;
        strcpy(token.lexeme, "EOF");
        return token;
    }

    char comment[100];
    int commentEnd = 0;
    int commentCount = 0;

    // Check for Comments
    if (currentChar == '/') {

        char nextChar = fgetc(file);

        if (nextChar == '*') {

            // Reset commentCount and clear contents of comment array
            commentCount = 0;
            memset(comment, 0, sizeof(comment));


            comment[commentCount++] = currentChar;
            comment[commentCount++] = nextChar;

            // While loop until end of file
            while ((currentChar = fgetc(file)) != EOF) {
                comment[commentCount++] = currentChar;

                if (currentChar == '*') {
                    char potentialSlash = fgetc(file);

                    if (potentialSlash == '/') {
                        commentEnd = 1;

                        comment[commentCount++] = potentialSlash;  // Include the '/'
                        comment[commentCount] = '\0';

                        token.type = COMMENT;
                        strcpy(token.lexeme, comment);

                        return token;
                    } else {
                        comment[commentCount++] = potentialSlash;  // Include the '*' in the comment
                    }
                }
            }

            // Check if there is no closing '*/' or if an error occurred
            if (commentEnd == 0) {
                token.type = INVALID;
                strcpy(token.lexeme, comment);
                return token;
            }
            
        }

        // Use ungetc to retract, if the lexeme wasn't a comment
        ungetc(nextChar, file);
    }

    char stringText[100];
    int i = 0;
    

    if ((strchr("\"", prevChar)) && (quoteCount == 1)){

            stringText[i++] = currentChar;
            prevChar = currentChar;

        while ((currentChar = fgetc(file)) != EOF){

            if (strchr("\"", currentChar) != NULL){

                ungetc(currentChar, file);
                stringText[i] = '\0';
                
                strcpy(token.lexeme, stringText);
                token.type = STRING_LITERAL;
                
                return token;
            }
            
            stringText[i++] = currentChar;

        
        }
        token.type = INVALID;
        return token;
    }

    char buffer[10];
    i = 0;
    while (((isalpha(currentChar) || currentChar == '_' || currentChar == '.') && i < 99) || (isalnum(currentChar) && buffer[0] != '\0' && isdigit(buffer[0]) != 1)) {
        buffer[i++] = currentChar;
        currentChar = fgetc(file);
    }

    // Check for keywords
    buffer[i] = '\0';
    if (isKeywordExtended(buffer)) {
        token.type = KEYWORD;
        strcpy(token.lexeme, buffer);
        ungetc(currentChar, file);
        return token;
    }

    if (isReserveWord(buffer)){
        token.type = RESERVE_WORD;
        strcpy(token.lexeme, buffer);
        ungetc(currentChar, file);
        return token;
    }
    if (isNoiseWord(buffer)){
        token.type = NOISE_WORD;
        strcpy(token.lexeme, buffer);
        ungetc(currentChar, file);
        return token;
    }
    if (isConstantNum(buffer)){
        token.type = CONSTANT_NUMBER;
        strcpy(token.lexeme, buffer);
        ungetc(currentChar, file);
        return token;
    }
    if (isConstantDec(buffer)){
        token.type = CONSTANT_DECIMAL;
        strcpy(token.lexeme, buffer);
        ungetc(currentChar, file);
        return token;
    }
    if (isConstantCol(buffer)){
        token.type = CONSTANT_COLLECTION;
        strcpy(token.lexeme, buffer);
        ungetc(currentChar, file);
        return token;
    }
    if (isConstantTxt(buffer)){
        token.type = CONSTANT_TEXT;
        strcpy(token.lexeme, buffer);
        ungetc(currentChar, file);
        return token;
    }
    if (isConstantSwtch(buffer)){
        token.type = CONSTANT_SWITCH;
        strcpy(token.lexeme, buffer);
        ungetc(currentChar, file);
        return token;
    }
    // Check for identifiers
if (isalpha(buffer[0]) || buffer[0] == '_') {
    int hasUnderscore = (buffer[0] == '_');
    int hasDot = 0;
    int endsWithUnderscore = 0;
    int endsWithDot = 0;

    for (int j = 0; j < i; ++j) {
        token.lexeme[j] = buffer[j];

        if (buffer[j] == '_') {
            hasUnderscore = 1;
        } else if (buffer[j] == '.') {
            hasDot = 1;
        }

        // Check if the identifier ends with an underscore or dot
        if (j == i - 1 && (buffer[j] == '_' || buffer[j] == '.')) {
            if (buffer[j] == '_') {
                endsWithUnderscore = 1;
            } else {
                endsWithDot = 1;
            }
        }
    }

    token.lexeme[i] = '\0';

    // Check if the identifier starts with an underscore
    if (token.lexeme[0] == '_') {
        token.type = INVALID;
        return token;
    }

    // Check if the identifier contains both underscore and dot
    if (hasUnderscore && hasDot) {
        token.type = INVALID;
        return token;
    }

    // Check if the identifier ends with an underscore or dot
    if (endsWithUnderscore || endsWithDot) {
        token.type = INVALID;
        return token;
    }

    if (isalnum(buffer[i]) != 0) {
        token.type = INVALID;
        return token;
    }

    token.type = IDENTIFIER;
    ungetc(currentChar, file);
    return token;
    }
    

    // Check for numeric literals
    if (isdigit(currentChar) || currentChar == '.') {
        int i = 0;
        while (isValidNumberChar(currentChar)) {
            token.lexeme[i++] = currentChar;
            currentChar = fgetc(file);
        }
        token.lexeme[i] = '\0';
        token.type = NUMBER_LITERAL;
        ungetc(currentChar, file);
        return token;
    }

    // Check for add operators
    if (strchr("+", currentChar) != NULL) {

        char nextChar = fgetc(file);

        if (strchr("=", nextChar) != NULL){
            token.lexeme[0] = currentChar;
            token.lexeme[1] = nextChar;
            token.lexeme[2] = '\0';
            token.type = ADDITION_ASSIGNMENT_OPERATOR;
            return token;
        }

        else {
            ungetc(nextChar, file);
            token.lexeme[0] = currentChar;
            token.lexeme[1] = '\0';
            token.type = ADDITION_OPERATOR;
            return token;
        }
    }

    // Check for subtract operators
    if (strchr("-", currentChar) != NULL) {

        char nextChar = fgetc(file);

        if (strchr("=", nextChar) != NULL){
            token.lexeme[0] = currentChar;
            token.lexeme[1] = nextChar;
            token.lexeme[2] = '\0';
            token.type = SUBTRACTION_ASSIGNMENT_OPERATOR;
            return token;
        }

        else {
            ungetc(nextChar, file);
            token.lexeme[0] = currentChar;
            token.lexeme[1] = '\0';
            token.type = SUBTRACTION_OPERATOR;
            return token;
        }
    }

    // Check for multiply operators
    if (strchr("*", currentChar) != NULL) {

        char nextChar = fgetc(file);

        if (strchr("=", nextChar) != NULL){
            token.lexeme[0] = currentChar;
            token.lexeme[1] = nextChar;
            token.lexeme[2] = '\0';
            token.type = MULTIPLICATION_ASSIGNMENT_OPERATOR;
            return token;
        }

        else {
            ungetc(nextChar, file);
            token.lexeme[0] = currentChar;
            token.lexeme[1] = '\0';
            token.type = MULTIPLICATION_OPERATOR;
            return token;
        }
    }

    // Check for division operators
    if (strchr("/", currentChar) != NULL) {

        char nextChar = fgetc(file);

        if (strchr("/", nextChar) != NULL) {

            char thirdChar = fgetc(file);

            if (strchr("=", thirdChar) != NULL){
                token.lexeme[0] = currentChar;
                token.lexeme[1] = nextChar;
                token.lexeme[2] = thirdChar;
                token.lexeme[3] = '\0';
                token.type = FLOOR_DIVISION_ASSIGNMENT_OPERATOR;
                return token;
            }

            else {
                ungetc(thirdChar, file);
                token.lexeme[0] = currentChar;
                token.lexeme[1] = nextChar;
                token.lexeme[2] = '\0';
                token.type = FLOOR_DIVISION_OPERATOR;
                return token;
            }
        }

        else if (strchr("=", nextChar) != NULL){
            token.lexeme[0] = currentChar;
            token.lexeme[1] = nextChar;
            token.lexeme[2] = '\0';
            token.type = DIVISION_ASSIGNMENT_OPERATOR;
            return token;
        }

        else {
            ungetc(nextChar, file);
            token.lexeme[0] = currentChar;
            token.lexeme[1] = '\0';
            token.type = DIVISION_OPERATOR;
            return token;
        }
    }

    // Check for exponentiation
    if (strchr("^", currentChar) != NULL) {

        char nextChar = fgetc(file);

        if (strchr("*", nextChar) != NULL) {

            char thirdChar = fgetc(file);

            if (strchr("=", thirdChar) != NULL) {
                token.lexeme[0] = currentChar;
                token.lexeme[1] = nextChar;
                token.lexeme[2] = thirdChar;
                token.lexeme[3] = '\0';
                token.type = EXPONENTIATION_ASSIGNMENT_OPERATOR;
                return token;
            }
            
            else {
                ungetc(thirdChar, file);
                token.lexeme[0] = currentChar;
                token.lexeme[1] = nextChar;
                token.lexeme[2] = '\0';
                token.type = EXPONENTIATION_OPERATOR;
                return token;
            }
        }
        
        else {
            ungetc(nextChar, file);
            token.lexeme[0] = currentChar;
            token.lexeme[1] = '\0';
            token.type = INVALID;
            return token;
        }
    }

    // Check for modulo operators
    if (strchr("%", currentChar) != NULL) {

        char nextChar = fgetc(file);

        if (strchr("=", nextChar) != NULL){
            token.lexeme[0] = currentChar;
            token.lexeme[1] = nextChar;
            token.lexeme[2] = '\0';
            token.type = MODULUS_ASSIGNMENT_OPERATOR;
            return token;
        }

        else {
            ungetc(nextChar, file);
            token.lexeme[0] = currentChar;
            token.lexeme[1] = '\0';
            token.type = MODULO_OPERATOR;
            return token;
        }
    }

    // Check for assignment operators
    if (strchr("=", currentChar) != NULL) {

        char nextChar = fgetc(file);

        if (strchr("=", nextChar) != NULL){
                token.lexeme[0] = currentChar;
                token.lexeme[1] = nextChar;
                token.lexeme[2] = '\0';
                token.type = IS_EQUAL_OPERATOR;
                return token;
        }

        else {
            ungetc(nextChar, file);
            token.lexeme[0] = currentChar;
            token.lexeme[1] = '\0';
            token.type = ASSIGNMENT_OPERATOR;
            return token;
        }
    }


    // Check for greater than operators
    if (strchr(">", currentChar) != NULL) {

        char nextChar = fgetc(file);

        if (strchr("=", nextChar) != NULL) {
            token.lexeme[0] = currentChar;
            token.lexeme[1] = nextChar;
            token.lexeme[2] = '\0';
            token.type = GREATER_EQUAL_OPERATOR;
            return token;
        }
        
        else {
            ungetc(nextChar, file);
            token.lexeme[0] = currentChar;
            token.lexeme[1] = '\0';
            token.type = GREATER_THAN_OPERATOR;
            return token;
        }
    }

    // Check for less than operators
    if (strchr("<>>", currentChar) != NULL) {

        char nextChar = fgetc(file);

        if (strchr("=", nextChar) != NULL) {
            token.lexeme[0] = currentChar;
            token.lexeme[1] = nextChar;
            token.lexeme[2] = '\0';
            token.type = LESS_EQUAL_OPERATOR;
            return token;
        }
        
        else {
            ungetc(nextChar, file);
            token.lexeme[0] = currentChar;
            token.lexeme[1] = '\0';
            token.type = LESS_THAN_OPERATOR;
            return token;
        }
    }

    
    // Check for and operators
    if (strchr("&", currentChar) != NULL) {
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0';
        token.type = AND_OPERATOR;
        return token;
    }

    
    // Check for not and not equal operators
    if (strchr("~", currentChar) != NULL) {

        char nextChar = fgetc(file);

        if (strchr("=", nextChar) != NULL) {
            token.lexeme[0] = currentChar;
            token.lexeme[1] = nextChar;
            token.lexeme[2] = '\0';
            token.type = NOT_EQUAL_OPERATOR;
            return token;
        }

        else {
            ungetc(nextChar, file);
            token.lexeme[0] = currentChar;
            token.lexeme[1] = '\0';
            token.type = NOT_OPERATOR;
            return token;
        }
    }

    // Check for or operators
    if (strchr("|", currentChar) != NULL) {

        char nextChar = fgetc(file);

        if (strchr("|", nextChar) != NULL) {
            token.lexeme[0] = currentChar;
            token.lexeme[1] = nextChar;
            token.lexeme[2] = '\0';
            token.type = OR_OPERATOR;
            return token;
        }

        else {
            ungetc(nextChar, file);
            token.lexeme[0] = currentChar;
            token.lexeme[1] = '\0';
            token.type = INVALID;
            return token;
        }
    }

    // Check for open parentheses
    if (strchr("(", currentChar) != NULL) {
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0';
        token.type = OPEN_PARENTHESIS;
        return token;
    }

    // Check for close parentheses
    if (strchr(")", currentChar) != NULL) {
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0';
        token.type = CLOSED_PARENTHESIS;
        return token;
    }

    // Check for open open brackets
    if (strchr("[", currentChar) != NULL) {
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0';
        token.type = OPEN_BRACKET;
        return token;
    }

    // Check for closed curly brackets
    if (strchr("]", currentChar) != NULL) {
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0';
        token.type = CLOSED_BRACKET;
        return token;
    }

    // Check for open curly brackets
    if (strchr("{", currentChar) != NULL) {
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0';
        token.type = OPEN_CURLY;
        return token;
    }

    // Check for closed curly brackets
    if (strchr("}", currentChar) != NULL) {
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0';
        token.type = CLOSED_CURLY;
        return token;
    }

    // Check for semicolon
    if (strchr(";", currentChar) != NULL) {
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0';
        token.type = SEMICOLON;
        return token;
    }
    
    // Check for colon
    if (strchr(":", currentChar) != NULL) {
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0';
        token.type = COLON;
        return token;
    }

    // Check for quotation marks
    if (strchr("\"", currentChar) != NULL) {
        
        if (quoteCount == 1){
            quoteCount --;
        }

        else {
            quoteCount++;
            prevChar = currentChar;
        }

        //prevChar = currentChar;
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0';
        token.type = QUOTATION_MARKS;
        return token;
    }


    // Check for comma
    if (strchr(",", currentChar) != NULL) {
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0';
        token.type = COMMAS;
        return token;
    }


    // Invalid character
    token.lexeme[0] = currentChar;
    token.lexeme[1] = '\0';
    token.type = INVALID;
    return token;
}

int main() {
    // Open the input file
    FILE* inputFile = fopen("test.txt", "r");

    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    // Open the output file for writing
    FILE* outputFile = freopen("Symbol Table.txt", "w", stdout);

    if (outputFile == NULL) {
        perror("Error opening output file");
        fclose(inputFile);
        return 1;
    }

    struct Token token;

    printf("+-----------------------------+------------------------------------+\n");
    printf("| %-27s | %-34s |\n", "Token", "Lexeme");
    printf("+-----------------------------+------------------------------------+\n");

    do {
        token = getNextToken(inputFile);

        switch (token.type) {

            case KEYWORD:
                printf("| %-27s | %-34s |\n", "KEYWORD", token.lexeme);
                break;

            case IDENTIFIER:
                printf("| %-27s | %-34s |\n", "IDENTIFIER", token.lexeme);
                break;

            case RESERVE_WORD:
                printf("| %-27s | %-34s |\n", "RESERVE_WORD", token.lexeme);
                break;

            case NOISE_WORD:
                printf("| %-27s | %-34s |\n", "NOISE_WORD", token.lexeme);
                break;

            case STRING_LITERAL:
                printf("| %-27s | %-34s |\n", "STRING_LITERAL", token.lexeme);
                break;

            case NUMBER_LITERAL:
                printf("| %-27s | %-34s |\n", "NUMBER_LITERAL", token.lexeme);
                break;

            case COMMENT:
                printf("| %-27s | %-34s |\n", "COMMENT", token.lexeme);
                break;
                
            case CONSTANT_NUMBER:
                printf("| %-27s | %-34s |\n", "CONSTANT_NUMBER", token.lexeme);
                break;

            case CONSTANT_DECIMAL:
                printf("| %-27s | %-34s |\n", "CONSTANT_DECIMAL", token.lexeme);
                break;

            case CONSTANT_TEXT:
                printf("| %-27s | %-34s |\n", "CONSTANT_TEXT", token.lexeme);
                break;

            case CONSTANT_COLLECTION:
                printf("| %-27s | %-34s |\n", "CONSTANT_COLLECTION", token.lexeme);
                break;

            case CONSTANT_SWITCH:
                printf("| %-27s | %-34s |\n", "CONSTANT_SWITCH", token.lexeme);
                break;


            //Arithmetic Operator Cases
            case ADDITION_OPERATOR:
                printf("| %-27s | %-34s |\n", "ADDITION_OPERATOR", token.lexeme);
                break;

            case SUBTRACTION_OPERATOR:
                printf("| %-27s | %-34s |\n", "SUBTRACTION_OPERATOR", token.lexeme);
                break;

            case MULTIPLICATION_OPERATOR:
                printf("| %-27s | %-34s |\n", "MULTIPLICATION_OPERATOR", token.lexeme);
                break;

            case FLOOR_DIVISION_OPERATOR:
                printf("| %-27s | %-34s |\n", "FLOOR_DIVISION_OPERATOR", token.lexeme);
                break;

            case DIVISION_OPERATOR:
                printf("| %-27s | %-34s |\n", "DIVISION_OPERATOR", token.lexeme);
                break;
            
            case EXPONENTIATION_OPERATOR:
                printf("| %-27s | %-34s |\n", "EXPONENTIATION_OPERATOR", token.lexeme);
                break;

            case MODULO_OPERATOR:
                printf("| %-27s | %-34s |\n", "MODULO_OPERATOR", token.lexeme);
                break;


            //Assignment Operator Cases
            case ASSIGNMENT_OPERATOR:
                printf("| %-27s | %-34s |\n", "ASS_OP", token.lexeme);
                break;
            
            case ADDITION_ASSIGNMENT_OPERATOR:
                printf("| %-27s | %-34s |\n", "ADD_ASS_OP", token.lexeme);
                break;
            
            case SUBTRACTION_ASSIGNMENT_OPERATOR:
                printf("| %-27s | %-34s |\n", "SUB_ASS_OP", token.lexeme);
                break;
            
            case MULTIPLICATION_ASSIGNMENT_OPERATOR:
                printf("| %-27s | %-34s |\n", "MULTIPLY_ASS_OP", token.lexeme);
                break;
            
            case DIVISION_ASSIGNMENT_OPERATOR:
                printf("| %-27s | %-34s |\n", "DIV_ASS_OP", token.lexeme);
                break;

            case EXPONENTIATION_ASSIGNMENT_OPERATOR:
                printf("| %-27s | %-34s |\n", "EXPONENTIATION_ASS_OP", token.lexeme);
                break;
            
            case MODULUS_ASSIGNMENT_OPERATOR:
                printf("| %-27s | %-34s |\n", "MODULUS_ASS_OP", token.lexeme);
                break;

            case FLOOR_DIVISION_ASSIGNMENT_OPERATOR:
                printf("| %-27s | %-34s |\n", "FLOOR_DIV_ASST_OP", token.lexeme);
                break;
            

            //Boolean Relational Operators
            case IS_EQUAL_OPERATOR:
                printf("| %-27s | %-34s |\n", "IS_EQUAL_OP", token.lexeme);
                break;

            case NOT_EQUAL_OPERATOR:
                printf("| %-27s | %-34s |\n", "NOT_EQUAL_OP", token.lexeme);
                break;
            
            case GREATER_THAN_OPERATOR:
                printf("| %-27s | %-34s |\n", "GREATER_THAN_OP", token.lexeme);
                break;

            case LESS_THAN_OPERATOR:
                printf("| %-27s | %-34s |\n", "LESS_THAN_OP", token.lexeme);
                break;
            
            case GREATER_EQUAL_OPERATOR:
                printf("| %-27s | %-34s |\n", "GREATER_EQUAL_OP", token.lexeme);
                break;
            
            case LESS_EQUAL_OPERATOR:
                printf("| %-27s | %-34s |\n", "LESS_EQUAL_OP", token.lexeme);
                break;


            //Boolean Logical Operators
            case AND_OPERATOR:
                printf("| %-27s | %-34s |\n", "AND_OP", token.lexeme);
                break;

            case OR_OPERATOR:
                printf("| %-27s | %-34s |\n", "OR_OP", token.lexeme);
                break;

            case NOT_OPERATOR:
                printf("| %-27s | %-34s |\n", "NOT_OP", token.lexeme);
                break;
            

            //Delimiters and Brackets
            case SEMICOLON:
                printf("| %-27s | %-34s |\n", "SEMICOLON", token.lexeme);
                break;

            case COLON:
                printf("| %-27s | %-34s |\n", "COLON", token.lexeme);
                break;

            case OPEN_PARENTHESIS:
                printf("| %-27s | %-34s |\n", "OPEN_PARENTHESIS", token.lexeme);
                break;

            
            case CLOSED_PARENTHESIS:
                printf("| %-27s | %-34s |\n", "CLOSED_PARENTHESIS", token.lexeme);
                break;

            case OPEN_BRACKET:
                printf("| %-27s | %-34s |\n", "OPEN_BRACKET", token.lexeme);
                break;

            case CLOSED_BRACKET:
                printf("| %-27s | %-34s |\n", "CLOSED_BRACKET", token.lexeme);
                break;

            case OPEN_CURLY:
                printf("| %-27s | %-34s |\n", "OPEN_CURLY", token.lexeme);
                break;

            
            case CLOSED_CURLY:
                printf("| %-27s | %-34s |\n", "CLOSED_CURLY", token.lexeme);
                break;
            
            
            case QUOTATION_MARKS:
                printf("| %-27s | %-34s |\n", "QUOTATION_MARKS", token.lexeme);
                break;

            
            case COMMAS:
                printf("| %-27s | %-34s |\n", "COMMAS", token.lexeme);
                break;

            
            case INVALID:
                printf("| %-27s | %-34s |\n", "INVALID", token.lexeme);
                break;

            
            case END_OF_FILE:
                printf("Token: END_OF_FILE\n");
                break;
        }
        printf("+-----------------------------+------------------------------------+\n");
    } while (token.type != END_OF_FILE);

    // Close the files
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
