/**
 * @file parser.c
 * @brief Syntax analyzator
 * @author Martin Kozák
 *
 * Project: IFJ compiler
 * Date: 24.11.2021
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "error.h"
#include "parser.h"
#include "scanner.h"
#include "symtable.h"
#include "enum_list.h"

// SYMTABLE OPRAAAV
DataType find_data_type(TokenType type){

    switch(type){
        case TT_STRING:
            return STR;
        case TT_NUMBER:
            return NUM;
        case TT_INTEGER:
            return INT;
        
        case TT_KW_NIL:

        default:
            return BAD;
    }
}

DataType kw_find_data_type_kw(TokenType type){

    switch(type){
        case TT_KW_STRING:
            return STR;
        case TT_KW_NUMBER:
            return NUM;
        case TT_KW_INTEGER:
            return INT;
        
        case TT_KW_NIL:

        default:
            return BAD;
    }
}


void free_resources(ParserData *p_data){

    // token
    free(p_data->token->attribs.string);
    free(p_data->token);

    // global frame dispose
    bst_dispose(&(p_data->global_frame));

    // stack dispose
    sym_stack_dispose(&(p_data->stack));
    free(p_data);
}

ParserData *parser_init()
{

    ParserData *p_data;

    p_data = malloc(sizeof(ParserData));

    if (p_data == NULL)
    {
        return NULL;
    }

    p_data->token = malloc(sizeof(Token));

    if (p_data->token == NULL)
    {
        free(p_data);
        return NULL;
    }
    p_data->token->attribs.string = NULL;
    
    sym_stack_init(&p_data->stack);

    bst_init(&(p_data->global_frame));
    p_data->get_token = true;
    return p_data;
}

char * copy_str(char *src){
    char *ptr = malloc(sizeof(char) * (strlen(src) + 1) ); \
    strcpy(ptr, src);
}

bool id_list(ParserData *p_data){
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token);




}

/**
 * RETURN
 * <ret_expr_list_others> -> eps
 * <ret_expr_list_others> -> , <expression> <ret_expr_list_others>
 */
bool return_expr_list_others(ParserData *p_data, DataType **return_list, int *return_len){
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token);
    
    if (token->type == TT_COMMA){
        
        switch(token->type){

            case TT_KW_END:
            case TT_KW_ELSE:
                return false;

            case TT_STRING:
                enum_append(return_list, STR, return_len);
                return return_expr_list_others(p_data,return_list, return_len);

            case TT_INTEGER:
                enum_append(return_list, INT, return_len);
                return return_expr_list_others(p_data,return_list, return_len);

            case TT_NUMBER:
                enum_append(return_list, NUM, return_len);
                return return_expr_list_others(p_data,return_list, return_len);
            
            case TT_IDENTIFIER:
            
                enum_append(return_list, , return_len);
                return return_expr_list_others(p_data,return_list, return_len);
                // expression
                return return_expr_list_others(p_data,return_list, return_len);

            default:
                set_error(SYNTACTIC_ERR);
                return false;
        }

    } else {
        set_error(SYNTACTIC_ERR);
        return false;
    }

}


/**
 * RETURN
 * MUSIM SI PAMATAT MENO FUNKCIE
 * <ret_expr_list> -> eps
 * <ret_expr_list> -> <expression> <ret_expr_list_others>
 */
bool return_expr_list(ParserData *p_data){
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token);
    
    int return_len;
    DataType *return_list;

    switch(token->type){
        
        case TT_KW_END:
        case TT_KW_ELSE:
            return true;
            //TOKEN BACK
            
        case TT_STRING:
        case TT_INTEGER:
        case TT_NUMBER:
        case TT_HASHTAG:
        case TT_IDENTIFIER:
            // expression
            return return_expr_list_others(p_data, &return_list ,&return_len);

        default:
            set_error(SYNTACTIC_ERR);
            return false;
    }

}


bool local_expr_2(ParserData *p_data, char *var_id){
    // "=" was loaded
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token);

    switch (token->type){
        case TT_STRING:
        case TT_INTEGER:
        case TT_NUMBER:
        case TT_HASHTAG:
            // expression

        case TT_KW_NIL:
            // idk

        case TT_IDENTIFIER:
            // remember identifier
            char *id_name = copy_str(token->attribs.string);

            GET_TOKEN(token, p_data->get_token);
            
            if ( token->type == TT_LEFT_PAR ){
                //CALL FUNCTION
                bool is_global = false; 
                //FUNCTION CALL
                //return function_call(p_data, id_name, is_global);
                return true;
            } else {
                //expression
                // LINKED LIST
                //genrovanie kodu
                return true;

            }

        default:
            set_error(SYNTACTIC_ERR);
            return false;

    }

}

/**
 * <local_expr> -> eps
 * <lcoal_expr> -> = <local_expr_2>
 */
bool local_expr(ParserData *p_data, char *var_id){
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token);

    switch(token->type){
        // eps
        case TT_IDENTIFIER:
        case TT_KW_LOCAL:
        case TT_KW_RETURN:
        case TT_KW_IF:
        case TT_KW_WHILE:
        case TT_KW_ELSE:
        case TT_KW_END:
            // next token cant be read
            p_data->get_token = false;
            return true;
        
        case TT_EQ:
            return local_expr_2(p_data, var_id);

        default:
            set_error(SYNTACTIC_ERR);
            return false;
    }

}

/**
 * LOCAL
 * <block> -> LOCAL ID : <data_type> <local_expr> <block>
 */
bool local(ParserData *p_data){
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token);
    DataType type;
    // LOCAL was loaded

    if (token->type == TT_IDENTIFIER){
        //remember identifier
        char *var_id = copy_str(token->attribs.string);

        // check colon
        GET_TOKEN(token, p_data->get_token);
        CHECK_VARS(token->type, TT_COLON, SYNTACTIC_ERR);

        GET_TOKEN(token, p_data->get_token);
        type = kw_find_data_type(token->type);
        
        switch (token->type){

            //insert var to sym
            case TT_KW_STRING:
            case TT_KW_NUMBER:
            case TT_KW_INTEGER:
                INSERT_VAR_TO_SYM(var_id, type); 
                break;
            // CO S TYM ??
            case TT_KW_NIL:
            default:
                set_error(SYNTACTIC_ERR);
                return false;
        }
        
        return local_expr(p_data, var_id);

    } else {
        set_error(SYNTACTIC_ERR);
        return false;
    }

}

/**
 * WHILE <expression> DO <block> END <block>
 */
bool while_clause(ParserData *p_data){
    Token *token = p_data->token;

    GET_TOKEN(token, p_data->get_token);
    
    /*
    if ( psa(token) == false ) {  //GENEROVANIE
        return false;
    }
    */

    GET_TOKEN(token, p_data->get_token)
    CHECK_VARS(token->type, TT_KW_DO, SYNTACTIC_ERR);

    bool is_if_block = false;
    
    if (block_fwe(p_data, is_if_block)) {

        //GENROVANIE LABELU NA END
        
        return true;

    } else {
        return false;
    }

       
}


/**
 * IF <expression> THEN <block_if> ELSE <block> <END> 
 */
bool if_clause(ParserData *p_data) {
    Token *token = p_data->token;
    // IF was read
    // HUGO to potrebuje mat nacitane
    GET_TOKEN(token, p_data->get_token);

    // GENEROVANIE PODMIENKY A PRVEHO LABELU
    /*
    if ( psa(token) == false ) {
        return false;
    }
    */

    // check THEN
    GET_TOKEN(token, p_data->get_token)
    CHECK_VARS(token->type, TT_KW_THEN, SYNTACTIC_ERR);

    bool is_if_block = true; // we are in if block
    // BLOCK_IF
    if ( block_fwe(p_data, is_if_block) == false){
        return false;
    } else {
        // == true, ELSE was read
        
        //GENEROVANIE DRUHEHO LABELU

        bool is_if_block = false; // we are in else block which ends with end
        return block_fwe(p_data, is_if_block);
    }
}


/**
 * <block> -> eps
 * <block> -> IF <expression> THEN <block> ELSE <block> end <block>
 * <block -> WHILE <expression> DO <block> END <block>
 * <block> -> LOCAL ID : <data_type> <local_expr> <block>
 * <block> -> <id_list> = <expr_list_and_func> <block>
 * <block> -> ID ( <function_param_list> ) <block>
 * <block> -> RETURN <ret_expr_list> <block> 
 * 
 */

bool block_fwe(ParserData *p_data, bool is_if_block, char *fun_id){
    Token *token = p_data->get_token;
    GET_TOKEN(token, p_data->get_token);

    // novy ramec
    NEW_SYMTABLE_FRAME

    if (token->type == TT_KW_END && is_if_block == false) {
        sym_stack_pop(&p_data->stack);
        //p_data->get_token = false;
        return true;
    } else if (token->type == TT_KW_ELSE && is_if_block == true){
        sym_stack_pop(&p_data->stack);
        //p_data->get_token = false;
        return true;
    }

    switch (token->type){

        case TT_KW_LOCAL:
            return local(p_data) && block_fwe(p_data, is_if_block);

        case TT_KW_IF:
            return if_clause(p_data) && block_fwe(p_data, is_if_block);

        case TT_KW_WHILE:
            return while_cyclus(p_data) && block_fwe(p_data, is_if_block);

        case TT_IDENTIFIER:
            char *id_name = copy_str(token->attribs.string);
            GET_TOKEN(token, p_data->get_token);

            if (token->type == TT_LEFT_PAR){
                bool is_global = false; 
                //FUNCTION CALL // TODO
                return function_call(p_data, id_name, false) && block_fwe(p_data, is_if_block);
            } else {
                p_data->get_token = false;
                // TODO
                return id_list(p_data) && block_fwe(p_data, is_if_block); 

            }

        case TT_KW_RETURN:
            return return_expr_list(p_data) && block_fwe(p_data, is_if_block);

        default:
            set_error(SYNTACTIC_ERR);
            return false;

    }
}



/**
 * <param_list_others> -> eps
 * <param_list_others> -> , ID : <data_type> <param_list_others>
 */
bool param_list_others(ParserData *p_data, DataType **param_list, int *param_len) {
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token);
    char *id;
    DataType type = find_data_type(token->type); 

    switch(token->type) {
        case TT_COMMA:
        
            GET_TOKEN(token, p_data->get_token);

            GET_TOKEN(token, p_data->get_token);
            //check if token is identifier
            CHECK_VARS(token->type, TT_IDENTIFIER ,SYNTACTIC_ERR);
            
            id = copy_str(token->attribs.string);

            // GENERATE CODE PLSKY

            GET_TOKEN(token, p_data->get_token);
            //check if token is colon
            CHECK_VARS(token->type, TT_COLON ,SYNTACTIC_ERR);
            type = kw_find_data_type(token->type);

            switch (token->type){

                case TT_KW_STRING:
                case TT_KW_NUMBER:
                case TT_KW_INTEGER:
                    enum_append(param_list, type, param_len);
                case TT_KW_NIL:
                default:
                    return false;
            }

            return param_list_others(p_data, param_list, param_len);
        
        //eps
        case TT_RIGHT_PAR:
            return true;
        
        default:
            set_error(SYNTACTIC_ERR);
            return false;
    }
}




/**
 * <param_list> -> eps
 * <param_list> -> ID : <data_type> <param_list_others>
 */

bool param_list_func(ParserData *p_data, DataType **param_list, int *param_len){
    Token *token = p_data->token;
    char *id;
    GET_TOKEN(token, p_data->get_token);
    DataType type = find_data_type(token->type);

    switch(token->type){
        //eps
        case TT_RIGHT_PAR:
            return true;

        case TT_IDENTIFIER:
            id = token->attribs.string;

            GET_TOKEN(token, p_data->get_token);
            //check if token is colon
            CHECK_VARS(token->type, TT_COLON ,SYNTACTIC_ERR);
            
            //GENEROVANIE PARAMETRA

            GET_TOKEN(token, p_data->get_token);
            type = kw_find_data_type(token->type);

            switch(token->type){
                
                case TT_KW_STRING:
                case TT_KW_NUMBER:
                case TT_KW_INTEGER:
                    enum_append(param_list, type, param_len);
                // TODO NIL
                default:
                    set_error(SYNTACTIC_ERR);
                    return false;
            }
            return param_list_others(p_data, param_list, param_len);   
        }
}


bool return_types_list_local_others(ParserData *p_data, DataType **return_list, int *return_len){
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token);
    DataType type = find_data_type(token->type);

    switch(token->type){
        //eps
        case TT_KW_IF:
        case TT_KW_WHILE:
        case TT_KW_LOCAL:
        case TT_KW_RETURN:
        case TT_KW_END:
        case TT_IDENTIFIER:
            p_data->get_token = false;
            return true;
        
        case TT_COMMA:
            
            GET_TOKEN(token, p_data->get_token);
            type = kw_find_data_type(token->type);

            switch(token->type) 
            {
                case TT_KW_STRING:
                case TT_KW_NUMBER:
                case TT_KW_INTEGER:
                    enum_append(return_list, type, return_len);
                    return return_types_list_others_local(p_data,return_list,return_len);
                
                case TT_KW_NIL:
                default:
                    set_error(SYNTACTIC_ERR);
                    return false;
            }

        default:
            set_error(SYNTACTIC_ERR);
            return false;
    }

}


/**
 * <return_types_list_local> -> eps
 * <return_types_list_local> -> : <data_type> <return_types_list_others_local>
 */

bool return_types_list_local(ParserData *p_data, DataType **return_list, int *return_len){
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token);
    DataType type;  

    switch(token->type){
        //eps
        case TT_KW_IF:
        case TT_KW_WHILE:
        case TT_KW_LOCAL:
        case TT_KW_RETURN:
        case TT_KW_END:
        case TT_IDENTIFIER:
            p_data->get_token = false;
            return true;
        
        case TT_COLON:

            GET_TOKEN(token, p_data->get_token);
            type = kw_find_data_type(token->type);

            switch(token->type) 
            {
                case TT_KW_STRING:
                case TT_KW_NUMBER:
                case TT_KW_INTEGER:
                    enum_append(return_list, type, return_len);
                    return return_types_list_others_local(p_data,return_list,return_len);
                
                case TT_KW_NIL:
                default:
                    set_error(SYNTACTIC_ERR);
                    return false;
            }

        default:
            set_error(SYNTACTIC_ERR);
            return false;
    }
}

/**
 *  <body> -> FUNCTION ID ( <param_list> ) <return_types_list> <block> END <body>
 */

bool function_def(ParserData *p_data) {
    Token *token = p_data->token;
    
    GET_TOKEN(token, p_data->get_token);

    int return_len = 0;
    int param_len = 0;

    DataType *param_list = NULL;
    DataType *return_list = NULL;

    if (token->type == TT_IDENTIFIER){

        char *fun_id;
        COPY_ID(fun_id, token->attribs.string);

        GET_TOKEN(token, p_data->get_token);

        //check variable if LEFT PARENTHISE
        CHECK_VARS(token->type, TT_LEFT_PAR, SYNTACTIC_ERR);

        GET_TOKEN(token, p_data->get_token);
        
        if ( param_list_func(p_data, &param_list, &param_len) ){
            
            if ( return_types_list(p_data, &return_list, &return_len) ){
                
                // TODO porovnat datove type ak je uz deklarovana
                bool flag =  bst_insert_fun( &(p_data->global_frame), fun_id, FUN, param_len, param_list, return_len, return_list, true);

                if (flag == false)
                    return false;

                // generovanie LABELU

                /* <BLOCK> */
                bool is_if_block = false;
                if (block_fwe(p_data, is_if_block, fun_id) == true) {
                    // bol nacitany end funkcie 

                    //GENEROVANIE RETURNOV

                    return true;
                } else {
                    return false; 
                }


            } else {
                return false;
            }
        } else {
            return false;
        }

    } else {
        set_error(SYNTACTIC_ERR);
        return false;
    }

}

/**
 *  <return_types_list_others> -> eps
 *  <return_types_list_others> -> , <data_types> <data_types_list_others>
 */

bool return_types_list_others_global(ParserData *p_data, DataType **return_list, int *return_len) {

    Token *token = p_data->token;

    GET_TOKEN(token, p_data->get_token); 

    switch(token->type) {
        case TT_COMMA:
        
            GET_TOKEN(token, p_data->get_token);

            switch (token->type){

                case TT_KW_STRING:
                    enum_append(return_list, STR, return_len);
                    break;
                case TT_KW_NUMBER:
                    enum_append(return_list, NUM, return_len);
                    break;
                case TT_KW_INTEGER:
                    enum_append(return_list, INT, return_len);
                    break;
                // TODO NIL
                default:
                    set_error(SYNTACTIC_ERR);
                    return false;
            }

            return return_types_list_others_global(p_data, return_list, return_len);
        
        case TT_EOF:
        case TT_KW_GLOBAL:
        case TT_KW_FUNCTION:
        case TT_IDENTIFIER:
            p_data->get_token = false;
            return true;
        
        default:
            set_error(SYNTACTIC_ERR);
            return false;
    }
}

/**
 * GLOBAL RETURN TYPES
 * <return_types_list> -> eps
 * <return_types_list> -> : <data_type> <data_types_list_others>
 */

bool return_types_list_global(ParserData *p_data, DataType **return_list, int *return_len){
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token);

    switch (token->type)
    {
        case TT_EOF:
        case TT_KW_GLOBAL:
        case TT_KW_FUNCTION:
        case TT_IDENTIFIER:
            // token was loaded and scanner cant load another
            p_data->get_token = false;
            return true;

        case TT_COLON:

            GET_TOKEN(token, p_data->get_token);

            switch(token->type) 
            {
                case TT_KW_STRING:
                    enum_append(return_list, STR, return_len);
                    break;

                case TT_KW_NUMBER:
                    enum_append(return_list, NUM, return_len);
                    break;

                case TT_KW_INTEGER:
                    enum_append(return_list, INT, return_len);
                    break;
                // TODO NIL
                default:
                    set_error(SYNTACTIC_ERR);
                    return false;
            }

            return return_types_list_others_global(p_data,return_list,return_len);

        default:
            set_error(SYNTACTIC_ERR);
            return false;
    }

}

/**
 * <data_types_list_others> -> eps
 * <data_types_list_others> -> , <data_types> <data_types_list_others>
 */

bool data_types_list_others(ParserData *p_data, DataType **param_list, int *param_len) {
    
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token);

    // eps
    if (token->type == TT_RIGHT_PAR){
        return true;
    } else if (token->type == TT_COMMA) { 
        // check comma after data_type

        GET_TOKEN(token, p_data->get_token);

        switch (token->type)
        {

            case TT_KW_STRING:
                enum_append(param_list, STR, param_len);
                break;

            case TT_KW_NUMBER:
                enum_append(param_list, NUM, param_len);
                break;

            case TT_KW_INTEGER:
                enum_append(param_list, INT, param_len);
                break;
            case TT_KW_NIL:
            default:
                set_error(SYNTACTIC_ERR);
                return false;
        }

        return data_types_list_others(p_data, param_list, param_len);
    
    } else {
        set_error(SYNTACTIC_ERR);
        return false;
    }
}


/**
 * <data_types_list> -> eps
 * <data_types_list> -> <data_type> <data_types_list_others>
 */
bool data_types_list(ParserData *p_data, DataType **param_list, int *param_len){
    
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token);

    switch (token->type)
    {
        case TT_RIGHT_PAR:
            // eps
            return true;

        case TT_KW_STRING:
            enum_append(param_list, STR, param_len);
            break;

        case TT_KW_NUMBER:
            enum_append(param_list, NUM, param_len);
            break;

        case TT_KW_INTEGER:
            enum_append(param_list, INT, param_len);
            break;
        // TODO NIL
        default:
            set_error(SYNTACTIC_ERR);
            return false;
    }

    return data_types_list_others(p_data, param_list, param_len);
}

/**
 * <body> -> GLOBAL ID : FUNCTION ( <data_types_list> ) <return_types_list> <body>
 */
bool global(ParserData *p_data)
{
    Token *token = p_data->token;
    // parametre a navratove type
    int return_len = 0;
    int param_len = 0;

    DataType *param_list = NULL;
    DataType *return_list = NULL;

    GET_TOKEN(token, p_data->get_token);

    // check token for identifier
    if (token->type == TT_IDENTIFIER)
    {
        
        if (bst_search(p_data->global_frame, token->attribs.string) != NULL)
        {
            
            set_error(SEM_UNDEFINED_ERR);
            return false;
        }

        // save function ID for later
        char *fun_id;
        COPY_ID(fun_id, token->attribs.string)
        
        // check colon
        GET_TOKEN(token, p_data->get_token);
        CHECK_VARS(token->type, TT_COLON, SYNTACTIC_ERR);

        // check "function"
        GET_TOKEN(token, p_data->get_token);
        CHECK_VARS(token->type, TT_KW_FUNCTION, SYNTACTIC_ERR);

        // check left parenthise
        GET_TOKEN(token, p_data->get_token);
        CHECK_VARS(token->type, TT_LEFT_PAR, SYNTACTIC_ERR);

        if ((data_types_list(p_data, &param_list, &param_len))){
            
            if (return_types_list_global(p_data, &return_list, &return_len)) {
                bool is_defined = false;
                
                bool a = bst_insert_fun(&(p_data->global_frame), fun_id, FUN, param_len, param_list, return_len, return_list, false);
                
                //fprintf(stderr, "_%s_", (p_data->global_frame)->key);
                
                return a;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }

    }
}

bool function_param_list_others(ParserData *p_data, bool global_call, int *i, TreeNode *func)
{
    Token *token = p_data->token;
    *i++;
    DataType type = find_data_type(token->type);
    GET_TOKEN(token, p_data->get_token);

    if (token->type == TT_COMMA){
        switch (token->type)
        {
            case TT_KW_NIL:
                CHECK_VARS(func->fun_extension->param_type[*i], NIL, SEM_FUNC_PARAM_RET_ERR);
                break;

            case TT_STRING:
            case TT_NUMBER:
            case TT_INTEGER:
                //checks type
                CHECK_VARS(func->fun_extension->param_type[*i], type, SEM_FUNC_PARAM_RET_ERR);
                // generate parameter
                return function_param_list_others(p_data, global_call, i, func);
                break;

            case TT_IDENTIFIER:
                char *id_name = copy_str(token->attribs.string);
                TreeNode *node = bst_search_in_stack(&(p_data->stack), id_name);

                if (node == NULL){
                    set_error(SEM_UNDEFINED_ERR);
                    return;
                }

                //checks type
                CHECK_VARS(func->fun_extension->param_type[*i], node->id, SEM_FUNC_PARAM_RET_ERR);

                // generate parameter

                return function_param_list_others(p_data, global_call, i, func);
                break;  

            default:
                set_error(SYNTACTIC_ERR);
                return false;
        }

    } else if (TT_RIGHT_PAR == token->type){

        if (func->fun_extension->cnt_param_type == *i){
            *i--;
            return true;
        } else
            return false;
        
    } else {
        set_error(SYNTACTIC_ERR);
        return false;
    }
        
}


/**
 *
 * <function_param_list> -> <function_param> <function_param_list_others>
 * <function_param_list_others> -> eps
 * <function_param_list_others> -> , <function_param> <function_param_list_others
 */

bool function_param_list(ParserData *p_data, bool global_call, TreeNode *func)
{
    // MAM TOKEN AJ KONTROLU PRAVEJ ZAVORKY
    Token *token = p_data->token;
    int i = 0;
    DataType type = find_data_type(token->type);

    switch (token->type)
    {   
        case TT_KW_NIL:
            CHECK_VARS(func->fun_extension->param_type[i], NIL, SEM_FUNC_PARAM_RET_ERR);
            break;

        case TT_STRING:
        case TT_NUMBER:
        case TT_INTEGER:
            
            //checks type
            CHECK_VARS(func->fun_extension->param_type[i], type, SEM_FUNC_PARAM_RET_ERR);
            // generate parameter
            return function_param_list_others(p_data, global_call, i, func);
            break;

        case TT_IDENTIFIER:
            char *id_name = copy_str(token->attribs.string);
            TreeNode *node = bst_search_in_stack(&(p_data->stack), id_name);

            if (node == NULL){
                //id is not in symbol table
                set_error(SEM_UNDEFINED_ERR);
                return;
            }
            //checks type
            CHECK_VARS(func->fun_extension->param_type[i], node->id, SEM_FUNC_PARAM_RET_ERR);

            // generate parameter
            return function_param_list_others(p_data, global_call, i, func);

            break;

        default:
            set_error(SYNTACTIC_ERR);
            return false;
    }

}

/**
 * <function_call> -> ID ( <function_param_list> )
 */
bool function_call(ParserData *p_data,char *fun_id ,bool global_call)
{
    //LEFT PARENTHISE WAS READ

    Token *token = p_data->token;

    // check if ID of function is in global frame

    TreeNode *func = bst_search(p_data->global_frame, token->attribs.string);

    // check if ID of function is in global frame
    if (func == NULL)
    {
        set_error(SEM_UNDEFINED_ERR);
        return false;
    }

    // check if function was defined
    CHECK_VARS(func->fun_extension->is_func_defined, true, SEM_UNDEFINED_ERR);


    GET_TOKEN(token, p_data->get_token);

    // <function_param_list> -> eps
    if (token->type == TT_RIGHT_PAR)
    {   
        // TODO CALL EMPTY function

        //generuj call pre funkciu pouzi GLOBAL_CALL

        return true;
    }
    else if (function_param_list(p_data, global_call, func))
    {
        
        //GENERUJ NAVESTI FUNKCIE A VYBER RETURNY

        return true;
    }
    else
    {
        return false;
    }
}

/**
 * <body> -> eps
 * <body> -> GLOBAL ID : FUNCTION ( <data_types_list> ) <return_types_list> <body>
 * <body> -> FUNCTION ID ( <param_list> ) <return_types_list> <block> END <body>
 * <body> -> <function_call> <body>
 */

bool body(ParserData *p_data)
{
    Token *token = p_data->token;

    GET_TOKEN(token, p_data->get_token);
    
    // eps
    switch (token->type){   
        case TT_EOF:
            return true;
            break;
        // global
        case TT_KW_GLOBAL:
            return global(p_data) && body(p_data);
            break;
        // function
        case TT_KW_FUNCTION:
            return function_def(p_data) && body(p_data);
            break;
        // call function
        case TT_IDENTIFIER:
            char *fun_id = copy_str(token->attribs.string);

            // check left parenthise
            GET_TOKEN(token, p_data->get_token);
            CHECK_VARS(token->type, TT_LEFT_PAR, SYNTACTIC_ERR);

            return function_call(p_data, fun_id, true) && body(p_data);
            break;
        default:
            set_error(SYNTACTIC_ERR);
            return false;        
    }
    
    
    
}

/**
 * <program> -> REQUIRE STRING <body>
 */
bool program(ParserData *p_data)
{
    Token *token = p_data->token;

    GET_TOKEN(token, p_data->get_token);

    // first word has to be "require"
    if (token->type == TT_KW_REQUIRE)
    {

        GET_TOKEN(token, p_data->get_token);

        // "ifj 21" after require
        if (token->type == TT_STRING && strcmp(token->attribs.string, "ifj 21"))
        {   

            return (true && body(p_data));
        }
    }
    
    // no require or string
    set_error(SYNTACTIC_ERR);
    return false;
}

// main function for parser
int syntactic_analyzator()
{

    ParserData *p_data;

    p_data = parser_init();

    if (p_data == NULL)
    {
        return INTERNAL_ERR;
    }

    if (program(p_data))
    {
        free_resources(p_data);
        return 0;
    }
    else
    {
        free_resources(p_data);
        return num_error;
    }
}
