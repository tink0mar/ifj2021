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
    
char id_frame[3][3] = { "LF@", "GF@", "TF@"};

char *code = NULL;
char *main_code = NULL;
int label_index[1000] = {0};
int zero_cnt = 1;


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

void erase(char **src, int len, int index){
    for (int i = 0; i < len; i++){
        (*src)[index + i] = '\0';
    }
}

int concat_str_float(char **src, double i){
    char *ptr;
    char str[41];
    sprintf(str, "%a", i);

    int len_str = my_strlen(str);
    int len_src = my_strlen(*src);
    
    if ( *src == NULL ) {
        ptr = malloc(len_str + 1);
    } else {
        ptr = (char *) realloc(*src, len_src + len_str + 1);
    }

    if (ptr == NULL){
        return 1;
    }

    erase(&ptr, len_str, len_src);
    strcat(ptr,str);

    ptr[len_src + len_str] = '\0';
    *src = ptr; 

}

int concat_src_int(char **src, int i){
    int len_i = int_len(i);

    int len_src = my_strlen(*src);
    
    char tmp[42];  
    
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
    char *ptr = NULL;
    
    if ( *src == NULL ) {
        ptr = malloc(len_str + 1);
    } else {
        ptr = (char *) realloc(*src, len_src + len_str + 1);
    }

    if (ptr == NULL){
        return 1;
    }

    erase(&ptr, len_str, len_src);
    strcat(ptr,str);
    //fprintf(stderr, "%d", len_src + len_str);
    //fprintf(stderr, "%s\n\n\n\n", ptr);

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

void start(){

    gen_main();
    gen_header();

}

char *get_frame_term_str(Token *token, FrameType frame){
    char *ptr = NULL;
    
    switch(token->type){
        
        case TT_STRING:
            concat_src_str(&ptr, "string@");
            concat_src_str(&ptr, token->attribs.string);
            break;
        case TT_INTEGER:
            concat_src_str(&ptr, "int@");
            concat_src_int(&ptr, token->attribs.integer);
            break;
        case TT_NUMBER:
            concat_src_str(&ptr, "float@");
            concat_str_float(&ptr, token->attribs.number);
            break;
        case TT_IDENTIFIER:
            concat_src_str(&ptr, "LF@");
            concat_src_str(&ptr, token->attribs.string);
            return ptr;    
    }
    return ptr;
}


 /************ GENERATOR *****************/
/***************************************/

/** BASICS */
// generate main
bool gen_main(){
    M_PRTC_N();
    M_PRTC_N();
    M_PRTC_N("LABEL $__main");
    M_PRTC_N();
}

// generate header of code
bool gen_header(){
    PRTC_N();
    PRTC_N(".IFJcode21");
    PRTC_N("#autors Denis Adásek, Martin Kozák, Hugo Hežel, Adam Juliš")
    PRTC_N()
    PRTC_N("DEFVAR GF@\%swap1")
    PRTC_N("DEFVAR GF@\%swap2")
    PRTC_N("DEFVAR GF@\%tmp1")
    PRTC_N("DEFVAR GF@\%tmp2")
    PRTC_N("DEFVAR GF@\%tmp3")
    PRTC_N("JUMP $__main");
    PRTC_N()
    PRTC_N();
    //gen_reads();
    //gen_readn();
    //gen_readi();
    
}

bool gen_reads(){
    PRTC_N()
    PRTC_N("#READ FUNCION")
    PRTC_N("LABEL $reads");
    PRTC_N("PUSHFRAME");
    PRTC_N("DEFVAR LF@\%_ret1")
    PRTC_N("MOVE LF@\%_ret1 nil@nil")
    PRTC_N("READ LF@\%_ret1")
    
    PRTC_N("DEFVAR LF@\%type")
    PRTC_N("TYPE LF@\%type LF@\%_ret1")
    PRTC_N("JUMPIFEQ $ok string LF@\%type")

    PRTC_N("EXIT int@4")

    PRTC_N("LABEL $ok")
    PRTC_N("POPFRAME")
    PRTC_N("RETURN")
}

bool gen_readi(){
    PRTC_N()
    PRTC_N("#READ FUNCION")
    PRTC_N("LABEL $readi");
    PRTC_N("PUSHFRAME");
    PRTC_N("DEFVAR LF@\%_ret1")
    PRTC_N("MOVE LF@\%_ret1 nil@nil")
    PRTC_N("READ LF@\%_ret1")

    PRTC_N("DEFVAR LF@\%type")
    PRTC_N("TYPE LF@\%type LF@\%_ret1")
    PRTC_N("JUMPIFEQ $ok int LF@\%type")

    PRTC_N("EXIT int@4")

    PRTC_N("LABEL $ok")

    PRTC_N("POPFRAME")
    PRTC_N("RETURN")
}

bool gen_readn(){
    PRTC_N()
    PRTC_N("#READ FUNCION")
    PRTC_N("LABEL $reads");
    PRTC_N("PUSHFRAME");
    PRTC_N("DEFVAR LF@\%_ret1")
    PRTC_N("MOVE LF@\%_ret1 nil@nil")
    PRTC_N("READ LF@\%_ret1")

    PRTC_N("DEFVAR LF@\%type")
    PRTC_N("TYPE LF@\%type LF@\%_ret1")
    PRTC_N("JUMPIFEQ $ok float LF@\%type")

    PRTC_N("EXIT int@4")

    PRTC_N("LABEL $ok")

    PRTC_N("POPFRAME")
    PRTC_N("RETURN")
    PRTC_N()
}

bool gen_write(Token *token, int topindex){

    PRTC("WRITE ")
    char *ptr = get_frame_term_str(token, T_LF);
    PRTC(ptr)

    if (token->type == TT_IDENTIFIER){
        PRTC_INT(topindex)
    }

    PRTC_N()
}

bool gen_tointeger(){
    PRTC_N()
    PRTC_N("#ToInteger FUNCION")
    PRTC_N("LABEL $tointeger");
    PRTC_N("PUSHFRAME");
    
    PRTC_N("DEFVAR LF@\%_ret1")
    PRTC_N("MOVE LF@\%_ret1 nil@nil")

    PRTC_N("JUMPIFEQ $toint_ok nil@nil LF@\%1")
    
    PRTC_N("FLOAT2INT LF@\%_ret1 LF@\%1")

    PRTC_N("LABEL $toint_ok")
    PRTC_N("POPFRAME")
    PRTC_N("RETURN")
}

bool gen_ord(){
    PRTC_N()
    return true;
}

bool gen_chr(){
    PRTC_N()
    return true;
}

// GENERATE CODE FOR PSA 

// generate pushs for psa
bool gen_push_E(Token token, int index){
    char *term = get_frame_term_str(&token, T_LF);
    PRTC("PUSHS ");
    
    PRTC(term)
    if (token.type == TT_IDENTIFIER){
        PRTC_INT(index)
    }
    PRTC_N();
    return true;
}

bool gen_clears(){
    PRTC("CLEARS")
    PRTC_N()
    return true;
}

// generate instructions for retype
bool gen_op_retype(int i){

    switch(i){
        case 0:
            break;

        case 1:
            PRTC_N("POPS GF@\%tmp1")
            PRTC_N("INT2FLOATS")
            PRTC_N("PUSHS GF@\%tmp1")
            break;

        case 2:
            PRTC_N("INT2FLOATS")
            break;

        case 3:
            PRTC_N("POPS GF@\%tmp1")
            PRTC_N("INT2FLOATS")
            PRTC_N("PUSHS GF@\%tmp1")
            PRTC_N("INT2FLOATS")
            break;
        default: 
            break;
    }
    
    return true;
}

bool gen_zero_check(){

    PRTC_N("POPS GF@\%tmp1")
    PRTC_N("POPS GF@\%tmp2")
    PRTC_N("EQ GF@\%tmp3 GF@\%tmp2 float@0x0p+0");
    
    PRTC("JUMPIFNEQ LABEL $ok")
    PRTC_INT(zero_cnt)
    PRTC_N(" GF@\%tmp3 bool@true")

    PRTC("LABEL $ok")
    PRTC_INT(zero_cnt)
    PRTC_N()

    PRTC_N("PUSHS GF@\%tmp1")
    PRTC_N("PUSHS GF@\%tmp2")
    zero_cnt++;
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
bool gen_op_divs(int i){
    if (!gen_op_retype(i)){ return false;}
    gen_zero_check();
    PRTC_N("DIV")
    return true;
}

//IDIV
bool gen_op_idivs(int i){
    if (!gen_op_retype(i)){ return false;}
    gen_zero_check();
    PRTC_N("IDIV")
    return true;
}

bool gen_op_concat(){
    PRTC_N("POPS GF@tmp1")
    PRTC_N("POPS GF@tmp2")
    PRTC_N("CONCAT GF@tmp3 GF@tmp2 GF@tmp1")
    PRTC_N("PUSHS GF@tmp3")
    return true;
}

bool gen_op_hashtag(){
    PRTC_N("POPS GF@tmp1")
    PRTC_N("STRLEN GF@tmp2 GF@tmp1")
    PRTC_N("PUSHS GF@tmp2")
    return true;
}

/** GENERATE CODE FOR PARSER */

bool gen_nil_nil(){
    PRTC_N("nil@nil")
}

// generate term, but if identifier generate also frame


/** FUNCTION CALL **/

//generate frame for function call
bool gen_frame_global(bool global){
    if (global){
        M_PRTC_N("CREATEFRAME")
    } else {
        PRTC_N("CREATEFRAME")
    }
    return true;
}

//generate parameters to be sent
bool gen_fun_par_to_be_sent(Token *token, int index, bool global){
    // DEFVAR TF@_index_
    // MOVE TF@_index_ term
    if (global){
        char *term = get_frame_term_str(token, T_GF);
        M_PRTC("DEFVAR TF@\%") M_PRTC_INT(index) M_PRTC_N()
        M_PRTC("MOVE TF@\%") M_PRTC_INT(index) M_PRTC(" ")  M_PRTC(term) M_PRTC_N()
        return true;
    } else {
        char *term = get_frame_term_str(token, T_LF);
        PRTC("DEFVAR TF@\%") PRTC_INT(index) PRTC_N()
        PRTC("MOVE TF@\%") PRTC_INT(index) PRTC(" ") PRTC(term) PRTC_N()
        return true;
    }
}

//generate function call global
bool gen_fun_call(char *fun_id, bool global){
    if (global){
        M_PRTC("CALL $")
        M_PRTC(fun_id)
        M_PRTC_N()
        return true;
    } else {
        PRTC("CALL $")
        PRTC(fun_id)
        PRTC_N()
        return true;
    }
}

//move return to variable
bool gen_move_ret_to_val(char *id, int topindex, int index){
    PRTC("MOVE LF@")
    PRTC(id)
    PRTC_INT(topindex)
    PRTC(" TF@\%_ret")
    PRTC_INT(index)
    PRTC_N()
    return true;
}


/** FUNCTION DEFINITION */


//generate function label
bool gen_fun_label(char *label_id){
    PRTC_N()
    PRTC_N("#FUNKCIA ZACIATOK")
    PRTC_N()
    PRTC("LABEL $");
    PRTC(label_id)
    PRTC_N()

    PRTC_N("PUSHFRAME")
    return true;
}


// generate function parameters,which recieve data from call
// index is poradie
bool gen_fun_par(char *id, int index, int topindex){
    // DEFVAR LF@id
    PRTC("DEFVAR LF@")
    PRTC(id)
    PRTC_INT(topindex)
    PRTC_N()

    // MOVE LF@id LF@_index_
    PRTC("MOVE LF@")
    PRTC(id)
    PRTC_INT(topindex)
    PRTC(" LF@\%")
    PRTC_INT(index);
    PRTC_N()
    PRTC_N()
    return true;
}

//generate return variable
bool gen_return_var(int index){
    PRTC("DEFVAR LF@\%_ret")
    PRTC_INT(index)
    PRTC_N()

    PRTC("MOVE LF@\%_ret")
    PRTC_INT(index)
    PRTC_N(" nil@nil")
}

//move value to return variables
bool gen_move_val_to_ret(int index){
    
    PRTC("POPS LF@\%_ret")
    PRTC_INT(index)
    PRTC_N()
    return true;

}

//generate return
bool gen_return(){
    PRTC_N("POPFRAME")
    PRTC_N("RETURN")
    return true;
}

/** END FUNCTION DEFINITION */




/** LOCAL */

// generate variable
bool gen_var(char *id, int top_index){
    PRTC("DEFVAR LF@")
    PRTC(id)
    PRTC_INT(top_index)
    PRTC_N()

    PRTC("MOVE LF@")
    PRTC(id)
    PRTC_INT(top_index)
    PRTC_N(" nil@nil")
    return true;
}

bool gen_pop_var(char *id, int top_index){
    PRTC("POPS LF@")
    PRTC(id)
    PRTC_INT(top_index)
    PRTC_N()
    return true;
}


/** IF */


//generate name for label without \n
bool gen_if_label_name(bool else_end, int top_index){
    PRTC("$_")
    PRTC_INT(top_index)
    PRTC("_")
    PRTC_INT(label_index[top_index])
    PRTC("_")
    if (else_end){
        PRTC("else")
    } else {
        PRTC("end")
    }
}

bool gen_if(int top_index, Token token){
    PRTC_N()
    PRTC_N("# IF")
    switch(token.type){

        case TT_GREATER:            // >
            PRTC_N("GTS")
            PRTC_N("PUSHS bool@false")
            PRTC("JUMPIFEQS ")
            gen_if_label_name(true, top_index);
            PRTC_N()
            break;

        case TT_LESS:
            PRTC_N("LTS")
            PRTC_N("PUSHS bool@false")
            PRTC("JUMPIFEQS ")
            gen_if_label_name(true, top_index);
            PRTC_N()
            break;

        case TT_GREATER_OR_EQ:

            PRTC_N("POPS GF@\%swap1")
            PRTC_N("POPS GF@\%swap2")
            PRTC_N("PUSH GF@\%swap1")
            PRTC_N("PUSH GF@\%swap2")

            PRTC_N("LTS")
            PRTC_N("PUSHS bool@false")
            PRTC("JUMPIFEQS ")
            gen_if_label_name(true, top_index);
            PRTC_N()
            break;

        case TT_LESS_OR_EQ:
            PRTC_N("POPS GF@\%swap1")
            PRTC_N("POPS GF@\%swap2")
            PRTC_N("PUSH GF@\%swap1")
            PRTC_N("PUSH GF@\%swap2")

            PRTC_N("GTS")
            PRTC_N("PUSHS bool@false")
            PRTC("JUMPIFEQS ")
            gen_if_label_name(true, top_index);
            PRTC_N()
            break;

        case TT_NOT_EQ:
            PRTC_N("EQS")        
            PRTC_N("PUSHS bool@false")
            PRTC("JUMPIFNEQS ")
            gen_if_label_name(true, top_index);
            PRTC_N()
            break;

        case TT_EQ:
            PRTC_N("EQS")
            PRTC_N("PUSHS bool@false")
            PRTC("JUMPIFEQS ")
            gen_if_label_name(true, top_index);
            PRTC_N()
            break;
    }
    return true;
}

bool gen_if_jump_end(int top_index){
    PRTC("JUMP ")
    gen_if_label_name(false, top_index);
    PRTC_N()

}

bool gen_else(int top_index){
    
    PRTC("LABEL ")
    gen_if_label_name(true, top_index);
    PRTC_N()

}

bool gen_if_end(int top_index){
    PRTC("LABEL ")
    gen_if_label_name(false, top_index);
    PRTC_N()
    PRTC_N("#end if")
    label_index[top_index]++;
}

/** WHILE */

bool gen_while_label_name(bool start_end, int top_index){
    PRTC("$_")

    //depth
    if (top_index == 0){
        PRTC("0")
    } else {
        PRTC_INT(top_index);
    }
    
    //width
    PRTC("_")
    int i = label_index[top_index];
    PRTC_INT(i)

    PRTC("_")
    if (start_end){
        PRTC("w_start")
    } else {
        PRTC("w_end")
    }
}


bool gen_while(int top_index, Token token){
    PRTC_N()
    PRTC_N("# WHILE")
    
    PRTC("LABEL ")
    gen_while_label_name(true, top_index);
    PRTC_N()

    switch(token.type){

        case TT_GREATER:            // >
            PRTC_N("GTS")
            PRTC_N("PUSHS bool@false")
            PRTC("JUMPIFEQS ")
            gen_while_label_name(false, top_index);
            PRTC_N()
            break;

        case TT_LESS:
            PRTC_N("LTS")
            PRTC_N("PUSHS bool@false")
            PRTC("JUMPIFEQS ")
            gen_while_label_name(false, top_index);
            PRTC_N()
            break;

        case TT_GREATER_OR_EQ:
            
            PRTC_N("POPS GF@\%swap1")
            PRTC_N("POPS GF@\%swap2")
            PRTC_N("PUSH GF@\%swap1")
            PRTC_N("PUSH GF@\%swap2")

            PRTC_N("LTS")
            PRTC_N("PUSHS bool@false")
            PRTC("JUMPIFEQS ")
            gen_while_label_name(false, top_index);
            PRTC_N()
            break;

        case TT_LESS_OR_EQ:
            PRTC_N("POPS GF@\%swap1")
            PRTC_N("POPS GF@\%swap2")
            PRTC_N("PUSH GF@\%swap1")
            PRTC_N("PUSH GF@\%swap2")

            PRTC_N("GTS")
            PRTC_N("PUSHS bool@false")
            PRTC("JUMPIFEQS ")
            gen_while_label_name(false, top_index);
            PRTC_N()
            break;

        case TT_NOT_EQ:
            PRTC_N("EQS")        
            PRTC_N("PUSHS bool@false")
            PRTC("JUMPIFNEQS ")
            gen_while_label_name(true, top_index);
            PRTC_N()
            break;

        case TT_EQ:
            PRTC_N("EQS")
            PRTC_N("PUSHS bool@false")
            PRTC("JUMPIFEQS ")
            gen_while_label_name(true, top_index);
            PRTC_N()
            break;


    }
}

bool gen_while_end(int top_index){
    
    PRTC("JUMP ")
    gen_while_label_name(true, top_index);
    PRTC_N()

    PRTC("LABEL ")
    gen_while_label_name(false, top_index);
    PRTC_N()
        
    PRTC_N("#end while")
    
    label_index[top_index]++;
    
}
