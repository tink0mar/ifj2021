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


int expand_string(char **string, char c){
    int str_len;

    if (*string == NULL) {
        str_len = 1;
    } else {    
        str_len = (int)strlen(*string);
    }

    char *ptr = (char *)realloc( *string , sizeof(char) * (str_len+1) );
    
    if (ptr == NULL){
        return 99;
    } else {
        
        ptr[str_len] = c;
        ptr[str_len+1] = '\0'; 
        
        *string = ptr;
        return 0;
    }
}


TToken *get_token() {

    TToken *token = (TToken *)malloc(sizeof(TToken));
    
    token->attribs.string = NULL;



}





