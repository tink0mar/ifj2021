/**
 * @file generator.c
 * @brief Code generator
 * @author Denis Adásek
 *
 * Project: IFJ compiler
 * Date: 24.11.2021
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error.h"
#include "scanner.h"
#include "generator.h"
#include "parser.h"

#define SIZE 100

#define PRTC(_code_)\
    if ( concat_src_str( &code ,_code_) == 1){ \
        set_error(99); \
        return false;\
    }\


#define PRTC_N(_coden_) \
    if ( concat_src_str(&code, _coden_ "\n") == 1){\
        set_error(99); \
        return false;\
    }\

//
#define PRTC_INT(_int_)\
    if ( concat_src_int( &code ,_int_) == 1){ \
        set_error(99); \
        return false;\
    }\

//
#define M_PRTC(_code_)\
    if ( concat_src_str(&main_code, _code_) == 1){ \
        set_error(99); \
        return false;\
    }\


#define M_PRTC_N(_coden_) \
    if ( concat_src_str(&main_code, _coden_ "\n") == 1){\
        set_error(99); \
        return false;\
    }\

//

#define M_PRTC_INT(_int_)\
    if ( concat_src_int( &main_code ,_int_) == 1){ \
        set_error(99); \
        return false;\
    }\

#define RET\
    return true;\

typedef enum {
    T_LF,
    T_GF,
    T_TF
} FrameType;

char id_frame[3][3] = { "LF@", "GF@", "TF@"};

char *code = NULL;
char *main_code = NULL;
int label_index[1000] = {0};
int tmp_cnt = 1;


int my_strlen(char *str){
    int i = 0;

    if (str == NULL){
        return 0;
    }
    
    while (str[i] != '\0'){
        i++;
    }
    return i;

}

int int_len(int i){
    int len = 1;

    for (i ; i > 0; i = i / 10){
        len++;
    }

    return len;
}

int concat_src_int(char **src, int i){
    int len_i = int_len(i);
    
    int len_src = my_strlen(*src);
    
    char tmp[41];  
    
    sprintf(tmp, "%d", i);
    
    if ( *src == NULL ) {
        *src = (char *) malloc( len_i + 1);
    } else {
        *src = (char *) realloc(*src, len_src + len_i + 1);
    }
    
    strcat(*src,tmp);

    return 0;
}


int concat_src_str(char **src, char *str ){
    int len_str = my_strlen(str);
    int len_src = my_strlen(*src);
    char *ptr;

    if ( *src == NULL ) {
        ptr = (char *) malloc(len_str + 1);
    } else {
        ptr = (char *) realloc(*src, len_src + len_str + 1);
    }

    if (ptr == NULL){
        return 1;
    }

    //fprintf(stderr, "%d", len_src + len_str);
    strcat(ptr,str);
    ptr[len_src + len_str] = '\0';
    *src = ptr; 

    return 0;
}

void print_code(){

    printf("%s", code);
    printf("%s", main_code);
    free(code);
    free(main_code);
}

 /************ GENERATOR *****************/
/***************************************/

/** BASICS */
// generate main
bool gen_main(){
    M_PRTC_N();
    M_PRTC_N();
    M_PRTC_N("LABEL main");
}

// generate header of code
bool gen_header(){

    PRTC_N(".IFJcode21");
    PRTC_N("#autors Denis Adásek, Martin Kozák, Hugo Hežel, Adam Juliš")
    PRTC_N("JUMP main");
    PRTC_N()
}

// GENERATE CODE FOR PSA 

// generate pushs for psa
bool gen_push_E(Token token){
    
    switch(token.type){
        case TT_IDENTIFIER:
            PRTC("PUSHS");
            PRTC(token.attribs.string);
            break;

        case TT_STRING:  
            PRTC("PUSHS string@");
            PRTC(token.attribs.string);      
            break;

        case TT_INTEGER:
            PRTC("PUSHS int@")
            PRTC(token.attribs.string);
            break;

        case TT_NUMBER:
            PRTC("PUSHS float@")
            PRTC(token.attribs.string);
            break;

    }
    PRTC_N();
    return true;
}

bool gen_clears(){
    PRTC("CLEARS")
    PRTC_N()
    return true;
}

// TODO vyriesit ramce
// generate instructions for retype
bool gen_op_retype(int i){

    switch(i){
        case 1:
            PRTC("DEFVAR TF@tmp") 
            PRTC_INT(tmp_cnt)
            PRTC_N()

            PRTC("POPS ")
            PRTC_INT(tmp_cnt)
            PRTC_N()
            break;

        case 2:
            PRTC_N("INT2FLOATS")
            break;

        case 3:
            break;
            // copy
    }
    return true;
}

//ADDS
bool gen_op_adds(int i){
    if (!gen_op_retype(i)){return false;}
    PRTC_N("ADDS")
    return true;
}

//SUBS
bool gen_op_subs(int i){
    if (!gen_op_retype(i)){return false;}
    PRTC_N("SUBS")
    return true;
}

//MULS
bool gen_op_muls(int i){
    if (!gen_op_retype(i)){return false;}
    PRTC_N("MULS")
    return true;
}

//DIV
bool gen_op_div(int i){
    if (!gen_op_retype(i)){ return false;}
    PRTC_N("DIV")
    return true;
}

//IDIV
bool gen_op_idiv(int i){
    if (!gen_op_retype(i)){ return false;}
    PRTC_N("IDIV")
    return true;
}


/** GENERATE CODE FOR PARSER */

//generate frame for function call
bool gen_frame(){
    PRTC_N("CREATEFRAME")
    return true;
}

//generate popframe

bool gen_popframe(){
    PRTC_N("POPFRAME")
    return true;
}

// get string to be printed
char *get_term_str(Token *token){
    char *ptr = NULL;
    switch(token->type){

        case TT_STRING:
            concat_src_str(&ptr, "string@");
            break;
        case TT_INTEGER:
            concat_src_str(&ptr, "int@");
            break;
        case TT_NUMBER:
            concat_src_str(&ptr, "float@");
            break;
        case TT_IDENTIFIER:
            concat_src_str(&ptr, token->attribs.string);
            return ptr;    
    }
    concat_src_str(&ptr, token->attribs.string);
}

// generate term, but if identifier generate also frame
char *get_frame_term_str(Token *token, FrameType frame){
    char *ptr = NULL;
    switch(token->type){

        case TT_STRING:
            concat_src_str(&ptr, "string@");
            break;
        case TT_INTEGER:
            concat_src_str(&ptr, "int@");
            break;
        case TT_NUMBER:
            concat_src_str(&ptr, "float@");
            break;
        case TT_IDENTIFIER:
            concat_src_str(&ptr, id_frame[frame]);
            concat_src_str(&ptr, token->attribs.string);
            return ptr;    
    }
    concat_src_str(&ptr, token->attribs.string);
}

/** FUNCTION CALL **/

//generate parameters to be sent
bool gen_fun_par_to_be_sent(Token *token, int index, bool global){
    // DEFVAR TF@_index_
    // MOVE TF@_index_ term
    if (global){
        char *term = get_term_str(token);
        M_PRTC("DEFVAR TF@") M_PRTC_INT(index) M_PRTC_N()
        M_PRTC("MOVE TF@") M_PRTC_INT(index) M_PRTC(term) M_PRTC_N()
        return true;
    } else {
        char *term = get_term_str(token);
        PRTC("DEFVAR TF@") PRTC_INT(index) PRTC_N()
        PRTC("MOVE TF@") PRTC_INT(index) PRTC(term) PRTC_N()
        return true;
    }
}

//generate function call global
bool gen_fun_call(char *fun_id, bool global){
    if (global){
        PRTC("CALL ")
        PRTC(fun_id)
        PRTC_N()
        return true;
    } else {
        M_PRTC("CALL ")
        M_PRTC(fun_id)
        M_PRTC_N()
        return true;
    }
}

/** FUNCTION DEFINITION */

//generate function label
bool gen_fun_label(char *id){
    PRTC("LABEL");
    PRTC(id)
    PRTC_N()
    return true;
}

// generate function parameters,which recieve data from call
bool gen_fun_par(char *id, int index){
    // DEFVAR LF@par_index_
    PRTC("DEFVAR LF@par")
    PRTC_INT(index)
    PRTC_N()

    // MOVE LF@par_index_ LF@_index_
    PRTC("MOVE LF@par")
    PRTC_INT(index)
    PRTC(" LF@")
    PRTC_INT(index);
    return true;
}

//generate return
bool gen_return(){
    PRTC_N("RETURN")
    return true;
}

//generate return variable
bool gen_return_var(int index, Token *token){
    PRTC("DEFVAR LF@_ret")
    PRTC_INT(index)
    PRTC_N()
    return true;
}

bool gen_move_to_return(int index){
    PRTC("POPS LF@_ret")
    PRTC_INT(index)
    PRTC_N()
    return true;
}


/** LOCAL */

// generate variable
bool gen_var(char *id){
    PRTC("DEFVAR TF@")
    PRTC(id)
    PRTC_N()
    return true;
}

bool gen_pop_var(char *id){
    PRTC("POPS ")
    PRTC(id)
    PRTC_N()
    return true;
}


int main(){
    Token token;

    token.attribs.string = "aa";
    token.type = TT_STRING;

    //fprintf(stderr, "%d", TT_STRING);
    
    gen_main();
    
    print_code();
    char *a = NULL;

    
}
