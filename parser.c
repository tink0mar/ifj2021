/**
 * @file parser.c
 * @brief Syntax analyzator 
 * @author Martin Kozák
 * 
 * Project: IFJ compiler
 * Date: 24.11.2021
 */ 


#include "scanner.h"
#include <stdlib.h>
#include <stdbool.h>
#include "error.h"
#include "parser.h"


TToken *get_token_stack(){

}



bool return_types_list(){

    TToken *token = get_token_stack();

    if (token->type == TT_EOF || token->type == TT_KW_GLOBAL || token->type == TT_KW_FUNCTION) {


        return true; 

    } else if () {

    }

}

bool data_types_list(){

    TToken *token = get_token();

    if (token->type == TT_RIGHT_PAR){
        
        return true;
    
    } else {

        switch(token->type) {

            case TT_STRING:

                break;
            case TT_NUMBER:

                break;
            case TT_INTEGER:

                break;
            case TT_KW_NIL:

                break;
            default:
                return false;

        }

    }

    return false;
    
}


bool body(){

    bool body = false;

    TToken *token = get_token();

    if (token->type == TT_EOF) {

        return true;

    // GLOBAL ID : FUNCTION
    } else if (token->type != TT_KW_GLOBAL) {


        token = get_token();
            
        if (token->type != TT_IDENTIFIER){
            
            // IMPORTANT

        }

        token = get_token();

        if (token->type != TT_COLON){

        }

        token = get_token();

        if (token->type != TT_KW_FUNCTION) {

        }

        token = get_token();

        if (token->type != TT_LEFT_PAR) {
                
        }

        if ( data_types_list() == false) {

            return false;
        }

        if ( return_types_list() == false) {

            return false;
        }



    // FUNCTION ID ()
    } else if (token->type == TT_KW_FUNCTION) {

        

    } else if (token->type == TT_IDENTIFIER) {

    }

    return false;

}

bool program(){

    TToken *token  = get_token();
    

    if ( token->type == TT_KW_REQUIRE) {
        token = get_token();
        if (token->type == TT_STRING && strcmp(token->attribs.string, "ifj 21")){
            return true && body();
        }
    }
    return false;
}

void syntactic_analyzator(){

    if ( program() == true ) {

    } 

}

