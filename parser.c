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
#include "generator.h"
#include "dll.h"
#include "psa.h"

char* tokens[] = {

    "TT_EOF",                 // End of file
    "TT_EOL",                 //  \n        // idk ci to potrebujeme
    
    "TT_PLUS",                //  +
    "TT_MINUS",               //  -
    "TT_MUL",                 //  *
    "TT_DIV",                 //  /
    "TT_FLOOR_DIV",           //  //
    "TT_HASHTAG",             //  #
    
    "TT_GREATER",             //  >
    "TT_LESS",                //  <
    "TT_GREATER_OR_EQ",       //  >=
    "TT_LESS_OR_EQ",          //  <=
    "TT_NOT_EQ",           //  ~=
    "TT_EQ",                  //  ==
    
    "TT_ASSIGN",              //  =
    "TT_LEFT_PAR",            //  () //???
    "TT_RIGHT_PAR",           //  )
    "TT_COLON",               //  :
    "TT_CONCAT",              //  ..
    "TT_COMMA",               //  ,

    "TT_NUMBER",   // uvidime ci to je potrebne zadanie 8 strana        
    "TT_INTEGER",
    "TT_STRING",

    "TT_IDENTIFIER",
    "TT_KW_DO",
    "TT_KW_ELSE",
    "TT_KW_END",
    "TT_KW_FUNCTION",
    "TT_KW_GLOBAL",
    "TT_KW_IF",
    "TT_KW_INTEGER",
    "TT_KW_LOCAL",
    "TT_KW_NIL",
    "TT_KW_NUMBER",
    "TT_KW_REQUIRE",
    "TT_KW_RETURN",
    "TT_KW_STRING",
    "TT_KW_THEN",
    "TT_KW_WHILE",

};

void print(Token *token){
    //fprintf(stderr, "%s\n", tokens[token->type]);

}

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
            return TT_KW_NIL;
    }
}

DataType kw_find_data_type(TokenType type){

    switch(type){
        case TT_KW_STRING:
            return STR;
        case TT_KW_NUMBER:
            return NUM;
        case TT_KW_INTEGER:
            return INT;
        
        case TT_KW_NIL:

        default:
            return TT_KW_NIL;
    }
}


void free_resources(ParserData *p_data){

    // token
    
    //free(p_data->token->attribs.string);
    free(p_data->token);

    // global frame dispose
    bst_dispose(&(p_data->global_frame));

    // stack dispose
    sym_stack_dispose(&(p_data->stack));
    dll_dispose(&p_data->dll_list);
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

    //token
    p_data->token = malloc(sizeof(Token));

    if (p_data->token == NULL)
    {
        free(p_data);
        return NULL;
    }
    p_data->token->attribs.string = NULL;
    
    //sym_stack
    sym_stack_init(&p_data->stack);
    
    //global frame
    bst_init(&(p_data->global_frame));
    
    //get token
    p_data->get_token = true;
    
    //dll list
    dll_init(&p_data->dll_list);

    return p_data;
}

char * copy_str(char *src){
    char *ptr = malloc(sizeof(char) * (strlen(src) + 1) ); \
    strcpy(ptr, src);
}

/**
 * @brief Function first checks if variable is in symtable,
 *        then insert variable to stack of variables, with 
 *        data_type
 */

bool insert_var_to_stack(ParserData *p_data, SymStack *pa_stack, char *id){
    
    TreeNode *var = bst_search_in_stack(&(p_data->stack), id);
    
    if (var == NULL){
        set_error(SEM_UNDEFINED_ERR);
        return false;
    } else {
        sym_stack_push(pa_stack, var);
        return true;
    }
}  

// assign return values to 
bool assign_return(ParserData *p_data, SymStack *pa_stack, char *func_id){
    TreeNode *var;
    TreeNode *func = bst_search(p_data->global_frame, func_id);
    int count = 0;

    while(pa_stack->topIndex != -1){
        var = sym_stack_top(pa_stack);
        int deep = bst_search_in_stack_gen(pa_stack, var->key);

        // check data types
        if (var->id != func->fun_extension->return_type[count]){
            set_error(5); 
        }

        gen_move_ret_to_val(var->key, p_data->stack.topIndex , (pa_stack->topIndex) );
        sym_stack_pop(pa_stack);
        count++;
    }

    if (count <= func->fun_extension->cnt_return_type){
        return true;
    } else {
        set_error(5);
        return false;
    }

}


/**
 * <expression_list_others> -> eps
 * <expression_list_others> -> , <expression> <expression_list_others> 
 */

bool expression_list_others(ParserData *p_data, SymStack *pa_stack){
    Token *token = p_data->token;
    int index;
    DataType type;
    
    switch (token->type){
        case TT_IDENTIFIER:
        case TT_KW_LOCAL:
        case TT_KW_RETURN:
        case TT_KW_IF:
        case TT_KW_WHILE:
        case TT_KW_ELSE:
        case TT_KW_END:
            p_data->get_token = false;
            return true;
        
        case TT_COMMA:
            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list))
            switch(token->type){
                case TT_STRING:
                case TT_INTEGER:
                case TT_NUMBER:
                case TT_HASHTAG:
                case TT_IDENTIFIER:
                case TT_KW_NIL: 
                    type = psa(p_data);
                    
                    if (expression_list_others(p_data, pa_stack) == true){
                        // EXPRESSION //semantic control
                        
                        TreeNode *var = sym_stack_top(pa_stack);
                        sym_stack_pop(pa_stack);
                        
                        if (type != var->id){
                            set_error(5);
                            return false;
                        }
                        //GENERATE POP
                        index = bst_search_in_stack_gen(&(p_data->stack), var->key);
                        gen_pop_var(var->key, index);
                        return true;
                    }else {
                        return false;
                    }
                default:
                    
                    set_error(SYNTACTIC_ERR);
                    return false;
            }
        default:
            set_error(2);
            return false;
    }
}


/**
 * <expression_list_and_func>
 */

bool expression_list_and_func(ParserData *p_data, SymStack *pa_stack){
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list))
    // "=" was loaded
    char *id_name;
    DataType type;
    
    switch(token->type){
        case TT_IDENTIFIER:
            id_name = copy_str(token->attribs.string);
            
            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
            
            if (token->type == TT_LEFT_PAR){
                    bool is_global = false; 

                    //FUNCTION CALL /
                    if (function_call(p_data, id_name, false) == true){                
                        
                        return assign_return(p_data, pa_stack, id_name);

                    } else {
                        return false;
                    }

            } else { 
                UNGET_TOKEN(token)
                break;
            }

        case TT_STRING:
        case TT_INTEGER:
        case TT_NUMBER:
        case TT_HASHTAG:
        case TT_KW_NIL: 
            
            break;

        default:
            
            set_error(SYNTACTIC_ERR);
            return false;
    
    }
    
    type = psa(p_data);
    
    //recursion 
    if (expression_list_others(p_data, pa_stack) == true){
        TreeNode *var = sym_stack_top(pa_stack);
        sym_stack_pop(pa_stack);
        //
        if (type != var->id){
            set_error(5);
            
            return false;
        }
        
        //GENERATE POP
        int index = bst_search_in_stack_gen(&(p_data->stack), var->key);
        gen_pop_var(var->key, index);
        return true;
    } else {
        return false;
    }

}


/**
 * <id_list> = <expr_list_and_func> <block>
 *  
 */
bool id_list(ParserData *p_data, char *id){
    Token *token = p_data->token;
    // stack for variables, every item of stack is tree with one node
    
        
    SymStack pa_stack;
    sym_stack_init(&pa_stack);
    
    //insert variable and checks in symtable
    if (insert_var_to_stack(p_data,&pa_stack, id) == false){
        return false;
    }
    
    //loading parameters
    while (1){
        
        GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));

        if (token->type == TT_COMMA){
            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));

            if (token->type == TT_IDENTIFIER){
                
                //save name of identifier
                id = copy_str(token->attribs.string);
                if (insert_var_to_stack(p_data,&pa_stack, id) == false){
                    return false;
                }
            } else {
                    
                set_error(SYNTACTIC_ERR);
                return false;
            }

        } else if (token->type == TT_ASSIGN){
            
            break;
        } else {
            
            set_error(SYNTACTIC_ERR);
            return false;
        }
    }
    //IDS WAS LOADED
    return expression_list_and_func(p_data, &pa_stack);


}

/**
 * RETURN
 * <ret_expr_list_others> -> eps
 * <ret_expr_list_others> -> , <expression> <ret_expr_list_others>
 */
bool return_expr_list_others(ParserData *p_data, TreeNode *func, int *return_len, char *fun_id){
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
    DataType type;
    ++*return_len;
    TreeNode *node;

    switch(token->type){
        case TT_KW_LOCAL:
        case TT_KW_IF:
        case TT_KW_WHILE:
        case TT_KW_ELSE:
        case TT_KW_END:
            //eps
            // check if there is same return types count
            if ( (*return_len - 1) == func->fun_extension->cnt_return_type){
                set_error(SEM_FUNC_PARAM_RET_ERR);
                return false;
            }
            p_data->get_token = false;
            return true;

        case TT_COMMA:
            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
            switch(token->type){
                case TT_STRING: 
                case TT_INTEGER:
                case TT_NUMBER:
                case TT_HASHTAG:            
                case TT_IDENTIFIER:
                    
                    type = psa(p_data);
                    
                    if ( type != func->fun_extension->return_type[*return_len] ){
                        
                        set_error(5); 
                        return false;
                    }
                    
                    //GENERATE
                    gen_move_val_to_ret(*return_len);
                    
                    if ( return_expr_list_others(p_data, func, return_len, fun_id) == true){
                        return true;
                    } else {
                        return false;
                    }

                case TT_KW_NIL:
                    if ( (*return_len) == func->fun_extension->cnt_return_type){
                        set_error(SEM_FUNC_PARAM_RET_ERR);
                        return false;
                    }
                    return true;

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
 * RETURN
 * MUSIM SI PAMATAT MENO FUNKCIE
 * <ret_expr_list> -> eps
 * <ret_expr_list> -> <expression> <ret_expr_list_others>
 */
bool return_expr_list(ParserData *p_data, char *fun_id){
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
    
    int return_len = 0;
    TreeNode *func = bst_search(p_data->global_frame, fun_id);
    if (func == NULL){
        set_error(SEM_UNDEFINED_ERR);
        return false;
    }

    switch(token->type){
        case TT_KW_LOCAL:
        case TT_KW_IF:
        case TT_KW_WHILE:
        case TT_KW_ELSE:
        case TT_KW_END:
            //GENERATE EMPTY RETURN
            if (func->fun_extension->cnt_return_type != 0){
                set_error(SEM_OTHERS);
                return false;
            }
            gen_return();
            // next token cant be read
            p_data->get_token = false;
            return true;
            
        case TT_STRING:
        case TT_INTEGER:
        case TT_NUMBER:
        case TT_HASHTAG:
        case TT_IDENTIFIER:
            
            if (psa(p_data) != func->fun_extension->return_type[return_len] ){
                set_error(5); 
                
                return false;
            }
            
            //GENERATE
            gen_move_val_to_ret(return_len);
            break;

        case TT_KW_NIL:
            if ( (return_len) == func->fun_extension->cnt_return_type){
                set_error(SEM_FUNC_PARAM_RET_ERR);
                return false;
            }
            return true;

        default:
            set_error(SYNTACTIC_ERR);
            return false;
    }
    
    if ( return_expr_list_others(p_data, func ,&return_len, fun_id) == true){
        
        //GENERATE return label
        
        gen_return();

        return true;
    } else {
        
        return false;
    }

}


bool local_expr_2(ParserData *p_data, char *var_id, DataType type){
    // "=" was loaded
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
    char *id_name;
    DataType type1;
    

    switch (token->type){
        case TT_STRING:
        case TT_INTEGER:
        case TT_NUMBER:
        case TT_HASHTAG:
        case TT_LEFT_PAR:
            //expression
            
            type1 = psa(p_data);
            if (type != type1) {
                
                set_error(5); 
                return false; 
            }
            
            //GENERATE POP
            gen_pop_var(var_id, p_data->stack.topIndex);
            
            return true;

        case TT_KW_NIL:
            return true;

        case TT_IDENTIFIER:
            // remember identifier
            id_name = copy_str(token->attribs.string);
            
            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
            if ( token->type == TT_LEFT_PAR ){
                //CALL FUNCTION
                bool is_global = false; 
                //FUNCTION CALL
                if ( function_call(p_data, id_name, is_global) == true){
                    //assign value
                    int index = bst_search_in_stack_gen(&p_data->stack, var_id);
                    gen_move_ret_to_val(var_id, index ,0);
                    return true;
                } else {
                    
                    return false;
                }
            } else {
                //expression
                
                UNGET_TOKEN(token)
                
                if (type != psa(p_data)) {
                    
                    set_error(5); 
                    return false; 
                }
                
                //GENERATE POP
                gen_pop_var(var_id, p_data->stack.topIndex);
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
bool local_expr(ParserData *p_data, char *var_id, DataType type){
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
    
    switch(token->type){
        // eps
        case TT_IDENTIFIER:
        case TT_KW_LOCAL:
        case TT_KW_RETURN:
        case TT_KW_IF:
        case TT_KW_WHILE:
        case TT_KW_ELSE:
        case TT_KW_END:
            
            //GENERATE VARIABLE
            gen_var(var_id, p_data->stack.topIndex);

            // next token cant be read
            p_data->get_token = false;
            return true;
        
        case TT_ASSIGN:
            gen_var(var_id, p_data->stack.topIndex);
            return local_expr_2(p_data, var_id, type);

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
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
    DataType type;
    TreeNode *node;
    // LOCAL was loaded

    if (token->type == TT_IDENTIFIER){
        //remember identifier
        char *var_id = copy_str(token->attribs.string);

        // check colon
        GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
        CHECK_VARS(token->type, TT_COLON, SYNTACTIC_ERR);

        GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
        type = kw_find_data_type(token->type);
        
        switch (token->type){

            //insert var to sym
            case TT_KW_STRING:
            case TT_KW_NUMBER:
            case TT_KW_INTEGER:
                
                INSERT_VAR_TO_SYM(var_id, type); 
                
                return local_expr(p_data, var_id, type);
                break;

            case TT_KW_NIL:
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
 * WHILE <expression> DO <block> END <block>
 */
bool while_clause(ParserData *p_data, char* fun_id){
    Token *token = p_data->token;
    
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
    
    NEW_SYMTABLE_FRAME
    //GENERATE LABEL
    // false = while
    if (psa_condition(p_data, false) == false){
        return false;
    }

    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list))
    CHECK_VARS(token->type, TT_KW_DO, SYNTACTIC_ERR);

    bool is_if_block = false;
    
    if (block_fwe(p_data, is_if_block, fun_id)) {
        
        //end of block
        //GENERATE WHILE END
        
        gen_while_end(p_data->stack.topIndex + 1);
        
        return true;

    } else {
        return false;
    }

       
}


/**
 * IF <expression> THEN <block_if> ELSE <block> <END> 
 */
bool if_clause(ParserData *p_data, char *fun_id) {
    
    Token *token = p_data->token;
    // IF was read
    // HUGO to potrebuje mat nacitane
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
    
    // GENEROVANIE PODMIENKY A PRVEHO LABELU
    // true = IF
    NEW_SYMTABLE_FRAME
    
    if (psa_condition(p_data, true) == false){
            
        return false;
    }
    
    
    
    // check THEN
    
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list))
    CHECK_VARS(token->type, TT_KW_THEN, SYNTACTIC_ERR);

    //generate new symtable
    

    bool is_if_block = true; // we are in if block
    // BLOCK_IF
    
    if ( block_fwe(p_data, is_if_block, fun_id) == false){
        
        return false;
    } else {
        // == true, ELSE was read
        gen_if_jump_end(p_data->stack.topIndex + 1);

        NEW_SYMTABLE_FRAME
        //GENEROVANIE DRUHEHO LABELU
        gen_else(p_data->stack.topIndex);

        

        bool is_if_block = false; // we are in else block which ends with end
        if (block_fwe(p_data, is_if_block, fun_id) == true){
            
            //GENEROVANIE ENDU
            gen_if_end(p_data->stack.topIndex + 1);
            return true;
        } else {
            return false;
        }

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
    
    Token *token = p_data->token;
    
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
    

    char *id_name;
    bool value, value1;
    
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
            
            return local(p_data) && block_fwe(p_data, is_if_block, fun_id);

        case TT_KW_IF:
            return if_clause(p_data, fun_id) && block_fwe(p_data, is_if_block, fun_id);

        case TT_KW_WHILE:
            
            return while_clause(p_data, fun_id) && block_fwe(p_data, is_if_block, fun_id);

        case TT_IDENTIFIER:
            
            id_name = copy_str(token->attribs.string);
            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
            if (token->type == TT_LEFT_PAR){
                bool is_global = false; 
                
                //FUNCTION CALL // TODO RETURNOVANIE
                return (function_call(p_data, id_name, false)) && block_fwe(p_data, is_if_block, fun_id);
            } else {
                
                UNGET_TOKEN(token)
                
                // TODO DOUBLE LINKED LIST
                

                return id_list(p_data, id_name) && block_fwe(p_data, is_if_block, fun_id); 

            }

        case TT_KW_RETURN:
            return return_expr_list(p_data, fun_id) && block_fwe(p_data, is_if_block, fun_id);

        default:
                   
            set_error(SYNTACTIC_ERR);
            return false;

    }
}



/**
 * <param_list_others> -> eps
 * <param_list_others> -> , ID : <data_type> <param_list_others>
 */
bool param_list_others(ParserData *p_data, DataType **param_list, int *param_len, int *i) {
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
    char *id;
    DataType type = find_data_type(token->type); 
    int index = ++*i;
    TreeNode *node;
    TreeNode *uzol;

    switch(token->type) {
        //eps
        case TT_RIGHT_PAR:
            
            return true;

        case TT_COMMA:
            
            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
            //check if token is identifier
            CHECK_VARS(token->type, TT_IDENTIFIER ,SYNTACTIC_ERR);
            
            id = copy_str(token->attribs.string);

            //check if token is colon
            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
            CHECK_VARS(token->type, TT_COLON ,SYNTACTIC_ERR);
            
            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
            type = kw_find_data_type(token->type);            

            switch (token->type){
                
                case TT_KW_STRING:
                case TT_KW_NUMBER:
                case TT_KW_INTEGER:
                    //add variable to symtable
                    uzol = sym_stack_top(&p_data->stack);
                    
                    INSERT_VAR_TO_SYM(id, type);
                    
                    enum_append(param_list, type, param_len);
                    //GENERATE PARAMETER MAM CISLO PARAMETRA
                    gen_fun_par(id, index, 0);
                    return param_list_others(p_data, param_list, param_len, i);
                case TT_KW_NIL:
                default:
                    set_error(SYNTACTIC_ERR);
                    return false;
            }

            return param_list_others(p_data, param_list, param_len, i);
        
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
    DataType type = find_data_type(token->type);
    int i = 0;
    TreeNode *node;

    switch(token->type){
        //eps
        case TT_RIGHT_PAR:
            return true;

        case TT_IDENTIFIER:
            
            id = copy_str(token->attribs.string);

            //check if token is colon
            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
            CHECK_VARS(token->type, TT_COLON ,SYNTACTIC_ERR);
            

            //data type of id
            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
            type = kw_find_data_type(token->type);
            
            switch(token->type){
                
                case TT_KW_STRING:
                case TT_KW_NUMBER:
                case TT_KW_INTEGER:
                    //add variable to symtable        
                    INSERT_VAR_TO_SYM(id, type);
                    
                    enum_append(param_list, type, param_len);
                    
                    //GENERATE PARAMETER
                    gen_fun_par(id, i, 0);

                    return param_list_others(p_data, param_list, param_len, &i);

                case TT_KW_NIL:
                default:
                    
                    set_error(SYNTACTIC_ERR);
                    return false;
            }
   
        }
}


bool return_types_list_local_others(ParserData *p_data, DataType **return_list, int *return_len, int *i){
    Token *token = p_data->token;
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
    DataType type = find_data_type(token->type);
    int index = ++*i;

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
            
            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
            type = kw_find_data_type(token->type);

            switch(token->type) 
            {
                case TT_KW_STRING:
                case TT_KW_NUMBER:
                case TT_KW_INTEGER:
                    //GENERATE RETURN NILS
                    enum_append(return_list, type, return_len);
                    gen_return_var(index);
                    return return_types_list_local_others(p_data,return_list, return_len, i);
                
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
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
    DataType type;  
    int i = 0;

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

            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
            type = kw_find_data_type(token->type);
            
            switch(token->type) 
            {
                case TT_KW_STRING:
                case TT_KW_NUMBER:
                case TT_KW_INTEGER:
                    // return_len++
                    enum_append(return_list, type, return_len);
                    //GENERATE RETURN NILS
                    gen_return_var(i);
                    return return_types_list_local_others(p_data,return_list,return_len, &i);
                
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
    // get id
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
    
    int return_len = 0;
    int param_len = 0;

    DataType *param_list = NULL;
    DataType *return_list = NULL;

    if (token->type == TT_IDENTIFIER){

        char *fun_id = copy_str(token->attribs.string);
            
        //new sytable frame
        NEW_SYMTABLE_FRAME
        // generovanie LABELU
        
        gen_fun_label(fun_id);
        
        //check variable if LEFT PARENTHISE
        GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
        CHECK_VARS(token->type, TT_LEFT_PAR, SYNTACTIC_ERR);

        GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
        
        if ( param_list_func(p_data, &param_list, &param_len) ){
            
            if ( return_types_list_local(p_data, &return_list, &return_len) ){
                
                // TODO - MAYBE DONE porovnat datove type ak je uz deklarovana
                
                bool flag =  bst_insert_fun( &(p_data->global_frame), fun_id, param_len, param_list, return_len, return_list, true);
                if (flag == false)
                    return false;

                /* <BLOCK> */
                bool is_if_block = false;
                
                if (block_fwe(p_data, is_if_block, fun_id) == true) {
                    // END was read GENERATE
                    gen_return();
                    
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

    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list)); 

    switch(token->type) {
        case TT_COMMA:
        
            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));

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
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));

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

            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));

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
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));

    // eps
    if (token->type == TT_RIGHT_PAR){
        return true;
    } else if (token->type == TT_COMMA) { 
        // check comma after data_type

        GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));

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
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));

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

/**ntf(stderr, "%d", num_error);
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

    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));

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
        GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
        CHECK_VARS(token->type, TT_COLON, SYNTACTIC_ERR);

        // check "function"
        GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
        CHECK_VARS(token->type, TT_KW_FUNCTION, SYNTACTIC_ERR);

        // check left parenthise
        GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
        CHECK_VARS(token->type, TT_LEFT_PAR, SYNTACTIC_ERR);

        if ((data_types_list(p_data, &param_list, &param_len))){
            
            if (return_types_list_global(p_data, &return_list, &return_len)) {
                bool is_defined = false;
                
                bool flag = bst_insert_fun(&(p_data->global_frame), fun_id, param_len, param_list, return_len, return_list, false);
                
                return flag;
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
    ++*i;
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
    DataType type; 
    int index = *i;
    char *id_name;
    TreeNode *node;
    
    if (token->type == TT_COMMA){
        GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
        switch (token->type)
        {
            case TT_STRING:
            case TT_NUMBER:
            case TT_INTEGER:
                type = find_data_type(token->type);
                //checks type
                if (!strcmp(func->key, "write")){
                    gen_write(p_data, p_data->stack.topIndex);
                    return function_param_list_others(p_data, global_call, i, func);
                } else {
                    CHECK_VARS(func->fun_extension->param_type[*i], type, SEM_FUNC_PARAM_RET_ERR);
                }
                
             case TT_KW_NIL:

                // generate parameter
                gen_fun_par_to_be_sent(token, index , global_call);
                return function_param_list_others(p_data, global_call, i, func);
                break;

            case TT_IDENTIFIER:
                id_name = copy_str(token->attribs.string);
                node = bst_search_in_stack(&(p_data->stack), id_name);

                if (node == NULL){
                    set_error(SEM_UNDEFINED_ERR);
                    return false;
                }

                //checks type
                if (!strcmp(func->key, "write")){
                    gen_write(p_data , p_data->stack.topIndex);
                    return function_param_list_others(p_data, global_call, i, func);
                } else {
                    CHECK_VARS(func->fun_extension->param_type[*i], node->id, SEM_FUNC_PARAM_RET_ERR);
                }

                // generate parameter
                gen_fun_par_to_be_sent(token, index , global_call);

                return function_param_list_others(p_data, global_call, i, func);
                break;  

            default:
                set_error(SYNTACTIC_ERR);
                return false;
        }

    } else if (TT_RIGHT_PAR == token->type){
        //write
        if ( !strcmp(func->key, "write") ){
            return true;
        }

        if (func->fun_extension->cnt_param_type != *i){
            *i--;
            set_error(5);
            return false;
        } else
            return true;
        
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
    TreeNode *node;
    char *id_name;

    switch (token->type)
    {   

        case TT_STRING:
        case TT_NUMBER:
        case TT_INTEGER:
            //checks type
            
            if (!strcmp(func->key, "write")){
                gen_write(p_data, p_data->stack.topIndex);
                return function_param_list_others(p_data, global_call, &i, func);
            } else {
                CHECK_VARS(func->fun_extension->param_type[i], type, SEM_FUNC_PARAM_RET_ERR);
            }
        
        case TT_KW_NIL:
            //GENERATE PARAMETER
            gen_fun_par_to_be_sent(token, i, global_call);
            
            return function_param_list_others(p_data, global_call, &i, func);
            break;

        case TT_IDENTIFIER:
            id_name = copy_str(token->attribs.string);
            node = bst_search_in_stack(&(p_data->stack), id_name);

            if (node == NULL){
                //id is not in symbol table
                set_error(SEM_UNDEFINED_ERR);
                return false;
            }
            //checks type
            if (!strcmp(func->key, "write")){
                gen_write(p_data, p_data->stack.topIndex);
                return function_param_list_others(p_data, global_call, &i, func);
                
            } else {
                CHECK_VARS(func->fun_extension->param_type[i], node->id, SEM_FUNC_PARAM_RET_ERR);
            }

            // generate parameter
            gen_fun_par_to_be_sent(token, i, global_call);

            return function_param_list_others(p_data, global_call, &i, func);

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
    TreeNode *func = bst_search(p_data->global_frame, fun_id);
    
    // check if ID of function is in global frame
    if (func == NULL)
    {
        
        set_error(SEM_UNDEFINED_ERR);
        return false;
    }
    // check if function was defined
    CHECK_VARS(func->fun_extension->is_func_defined, true, SEM_UNDEFINED_ERR);

    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));

    // generate new frame
    gen_frame_global(global_call);

    // <function_param_list> -> eps
    if (token->type == TT_RIGHT_PAR)
    {   
        // CALL EMPTY function
        if (func->fun_extension->cnt_param_type != 0){
            set_error(5);
            return false;
        }

        gen_fun_call(fun_id, global_call);

        return true;
    }
    else if (function_param_list(p_data, global_call, func))
    {
        if (!strcmp(func->key, "write")){
            //
        } else {
            gen_fun_call(fun_id, global_call);
        }
        //GENERATE CALL
        
        return true;
    }
    else
    {
        return false;
    }
}


bool predefined_func(ParserData *p_data){
    DataType *param_list = NULL;
    int i = 0;
    char *id_name;

    //write
    id_name = "write";
    if ( bst_insert_fun(&p_data->global_frame, id_name, 0, NULL, 0, NULL, true) == false){
        return false;
    } 

    //reads
    
    enum_append(&param_list, STR, &i);
    id_name = "reads";
    if ( bst_insert_fun(&p_data->global_frame, id_name, 0, NULL,  1, param_list, true) == false){
        return false;
    } 

    //readi
    DataType *param_list_i = NULL;
    i = 0;
    enum_append(&param_list_i, INT, &i);
    id_name = "readi";
    if ( bst_insert_fun(&p_data->global_frame, id_name, 0, NULL,  1, param_list_i, true) == false){
        return false;
    } 


    // readn
    DataType *param_list_n = NULL;
    i = 0;
    enum_append(&param_list_n, NUM, &i);
    id_name = "readn";
    if ( bst_insert_fun(&p_data->global_frame, id_name, 0, NULL,  1, param_list_n ,true) == false){
        return false;
    } 

    return true;
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
    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
    char *fun_id;
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
            fun_id = copy_str(token->attribs.string);

            // check left parenthise
            GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
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

    GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));
    // first word has to be "require"
    if (token->type == TT_KW_REQUIRE)
    {

        GET_TOKEN(token, p_data->get_token, &(p_data->dll_list));

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
    start();

    ParserData *p_data;

    p_data = parser_init();
    
    if (p_data == NULL)
    {
        return INTERNAL_ERR;
    }

    if (predefined_func(p_data) == false){
        return false;
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
