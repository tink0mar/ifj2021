/**
 * @file scanner.c
 * @brief Scanner for ifj compiler
 * @author Adam Juliš
 * 
 * Project: IFJ compiler
 * Date: 24.11.2021
 */ 

#include "scanner.h" 
#include "stdio.h"
#include "string.h"
#include "error.h"


#define EOL '\n'
#define EXPAND_STR if ( expand_string(&(token->attribs.string), c) == 1){ \
                        set_error(INTERNAL_ERR);\
                        return;\
                    } \

int expand_string(char **string, char c){
    int str_len;

    if (*string == NULL) {
        str_len = 1;
    } else {    
        str_len = (int)strlen(*string);
    }

    char *ptr = (char *)realloc( *string , sizeof(char) * (str_len+1) );
    
    if (ptr == NULL){

        return 1;
    } else {
        
        ptr[str_len] = c;
        ptr[str_len+1] = '\0'; 
        
        *string = ptr;
        return 0;
    }
}
/**
 *@brief Init token to next use 
 * @param 
 */
void init_token(Token *token){
    free(token->attribs.string);
    free(token->attribs.string);
    
    return;
}

/**
 *  @brief Load the char/string to token
 *  @param  
 */
void alphabet(Token *token, char c){
    EXPAND_STR;

    while (1){

        c = getchar();
        
        if ( (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || ( c >= 'a' && c <= 'z') || c == '_'){
            /**CHECK IT MORE*//*VYPRAZDNIT ATTRIBS OR NOT MORE*/
            if (strcmp(token->attribs.string, "do")) { token->type = TT_KW_DO; return;}
            if (strcmp(token->attribs.string, "else")){ token->type = TT_KW_ELSE; return; }
            if (strcmp(token->attribs.string, "end")){ token->type = TT_KW_END; return; }
            if (strcmp(token->attribs.string, "function")){ token->type = TT_KW_FUNCTION; return; }
            if (strcmp(token->attribs.string, "global")){ token->type = TT_KW_GLOBAL; return; }
            if (strcmp(token->attribs.string, "if")){ token->type = TT_KW_IF; return; }
            if (strcmp(token->attribs.string, "integer")){ token->type = TT_KW_INTEGER; return; }
            if (strcmp(token->attribs.string, "local")){ token->type = TT_KW_LOCAL; return }
            if (strcmp(token->attribs.string, "nil")){ token->type = TT_KW_NIL; return; }
            if (strcmp(token->attribs.string, "number")){ token->type = TT_KW_NUMBER; return; }
            if (strcmp(token->attribs.string, "require")){ token->type = TT_KW_REQUIRE; return; }
            if (strcmp(token->attribs.string, "return")){ token->type = TT_KW_RETURN; return; }
            if (strcmp(token->attribs.string, "string")){ token->type = TT_KW_STRING; return; }
            if (strcmp(token->attribs.string, "then")){ token->type = TT_KW_THEN; return; }
            if (strcmp(token->attribs.string, "while")){ token->type = TT_KW_WHILE; return;  }
            
            EXPAND_STR;
            return;
            }
        else {
            ungetc(c, stdin);
            return; 
        }
    }
}

// komentare vseobecne
void dash_minus(Token *token){
    char c;
    StateType state = S_MINUS_COMMENT;

    while(1) {
        c = getchar();
        
        switch (state){

            case S_MINUS_COMMENT:
                if (c == '-') {
                    state = S_LINE_COMMENT;
                } else {
                    token->type = TT_MINUS;
                    ungetc(c, stdin);
                    return;
                }
                break;

            case S_LINE_COMMENT:
                if (c == '[') {
                    c = getchar();
                    if (c == '['){
                        state = S_MULT_COMMENT;
                        break;
                    } else {
                        set_error(LEXICAL_ERR);
                        return;
                    }
                } else if (c == EOL || c == EOF) {
                    ungetc(c, stdin);
                    return;
                }else {
                    state = S_LINE_COMMENT;
                    break;
                }
            
            case S_MULT_COMMENT:
                if (c == EOL || c == EOF) {
                    ungetc(c,stdin);
                    return;
                } else {
                    state = S_MULT_COMMENT;
                    break;
                }

        }

    }

}

void number(Token *token) {
    char c;
    StateType state = S_INTEGER;

    while (1) {

        c = getchar();

        switch(state){
            
            case S_INTEGER:
                if ( c >= '0' && c <= '9'){
                    EXPAND_STR;
                    state = S_INTEGER;
                    break;
                } else if (c == 'E' || c == 'e'){
                    EXPAND_STR;
                    state = S_EXPONENT;
                    break;   
                } else if (c == '.') {
                    EXPAND_STR;
                    state = S_DOT;
                    break;
                }else {
                    char *junk = NULL;
                    token->attribs.integer =  (int) strtol( token->attribs.string, &junk, 10);
                    if (junk == NULL) {
                        token->type = TT_NUMBER;
                        ungetc(c, stdin);
                    } else {
                        set_error(LEXICAL_ERR);
                    }
                    return;
                }
            
            case S_DOT:
                if (c >= '0' && c <= '9'){
                    EXPAND_STR;
                    state = S_NUMBER_NO_EX;
                    break;
                } else {
                    set_error(LEXICAL_ERR);
                    return;
                } 
            
            case S_NUMBER_NO_EX:
                if (c >= '0' <= '9') {
                    EXPAND_STR;
                    state = S_NUMBER_NO_EX;
                    break;
                } else if (c == 'E' || c == 'e') {
                    EXPAND_STR;
                    state = S_EXPONENT;
                    break;
                } else {
                    char *junk = NULL;
                    token->attribs.number = strtod( token->attribs.string, &junk);
                    if (junk == NULL) {
                        token->type = TT_NUMBER;
                        ungetc(c, stdin);
                    } else {
                        set_error(LEXICAL_ERR);
                    }
                    return;
                }
            
            case S_EXPONENT:
                if (c == '+' || c == '-') {
                    EXPAND_STR;
                    state = S_EX_PLUS_MINUS;
                    break;
                } else if ( c >= '0' && c <= '9') {
                    EXPAND_STR;
                    state = S_NUMBER;
                    break;
                }else {
                    set_error(LEXICAL_ERR);
                    break;
                }
            
            case S_EX_PLUS_MINUS:
                if (c >= '0' && c <= '9') {
                    EXPAND_STR;
                    state = S_NUMBER;
                    break;
                } else {
                    set_error(LEXICAL_ERR);
                    break;
                }
            
            case S_NUMBER:
                if (c >= '0' && c <= '9') {
                    EXPAND_STR;
                    state = S_NUMBER;
                    break;
                } else {
                    char *junk;
                    token->attribs.number = strtod( token->attribs.string, &junk);
                    if (junk == NULL) {
                        token->type = TT_NUMBER;
                        ungetc(c, stdin);
                    } else {
                        set_error(LEXICAL_ERR);
                    }
                    return;
                }
        
        }
    }

}


void string(Token *token){
    char c;
    StateType state = S_STRING;

    while(1){

        c = getchar();
        
        switch (state){
            
            case S_STRING:
                
                if (c == '"'){
                    token->attribs.string = "";
                    state = S_STRING_END;
                    break;
                } else if (c == '\\'){
                    state = S_ESCAPE;
                    break;
                } else if (c > 32 && c <= 255) {
                    state = S_STRING;
                    EXPAND_STR;
                    break;
                } else { 
                    set_error(LEXICAL_ERR);
                    return;
                } 

            case S_ESCAPE:

                if (c == '\\' || c == 'n' || c == 't' || c == '"') {
                    state = S_STRING;
                    EXPAND_STR;
                    break;
                } else if ( c == '0') {
                    EXPAND_STR;
                    state = S_ESC_NUM_ZERO;
                    break;
                } else if (c == '1') {
                    EXPAND_STR;
                    state = S_ESC_NUM_ONE;
                    break;
                } else if (c == '2') {
                    EXPAND_STR;
                    state = S_ESC_NUM_TWO;
                    break;
                } else {
                    set_error(LEXICAL_ERR);
                    return;
                }
            
            case S_ESC_NUM_ZERO:

                if (c >= '1' && c <= '9') {
                    EXPAND_STR;
                    state = S_ESC_NUM_REST;
                    break;
                } else if ( c == '0' ) {
                    EXPAND_STR;
                    state = S_ESC_NUM_ZERO_ZERO;
                    break;
                } else {
                    set_error(LEXICAL_ERR);
                    return;
                }

            case S_ESC_NUM_ZERO_ZERO:

                if (c >= '1' && c <= '9'){
                    EXPAND_STR;
                    state = S_STRING;
                    break;
                } else {
                    set_error(LEXICAL_ERR);
                    return;
                }

            case S_ESC_NUM_ONE:
                if (c >= '1' && c <= '9') {
                    EXPAND_STR;
                    state = S_ESC_NUM_REST;
                    break;
                } else {
                    set_error(LEXICAL_ERR);
                    return;
                }
            
            case S_ESC_NUM_TWO:
                if (c == '5') {
                    EXPAND_STR;
                    state = S_ESC_NUM_TWO_FIVE;
                    break;
                } else if ( c >= '0' && c <= '4'){
                    EXPAND_STR;
                    state = S_ESC_NUM_REST;
                    break;
                } else {
                    set_error(LEXICAL_ERR);
                    return;
                }
            
            case S_ESC_NUM_TWO_FIVE:
                if (c=='5') {
                    EXPAND_STR;
                    state = S_STRING;
                    break;
                } else {
                    set_error(LEXICAL_ERR);
                    return;
                }

            case S_ESC_NUM_REST:
                if (c >= '0' && c <= '9') {
                    EXPAND_STR;
                    state = S_STRING;
                    break;
                }
            
            case S_STRING_END:
                if (c == '"') {
                    token->type = TT_STRING;
                    return;
                }

        }


    }

}

void get_token(Token *token) {

    char c = getchar();
    StateType state = S_START;

    init_token(token);     

    while (1){

        switch(state) {

            case S_START:
                    switch (c) {
                        if (c == EOF) {
                            token->type = TT_EOF;    /*staci doufam*/
                            return; 
                        } else if (c == '-') {                   /**HEY A MUZE BYT NECO JINEHO -- JAKO TREBA a-- ???????????*/
                            dash_minus(token);
                            return;
                        } else if (c == '"') {
                            string(token);
                            return;
                        } else if   {

                        } else {
                            set_error(LEXICAL_ERR);
                            return;
                        }
                        

        }
        
}

}





