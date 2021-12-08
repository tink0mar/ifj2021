/**
 * @file parser.h
 * @brief Interface of syntactic analyser
 * @author Martin Kozák
 * 
 * Project: IFJ compiler
 * Date: 24.11.2021
 */ 

#ifndef PARSER_H_
#define PARSER_H_

#include <stdbool.h>
#include "symtable.h"
#include "scanner.h"
#include "error.h"
#include "enum_list.h"
#include "dll.h"


/**
#define GET_TOKEN(TOKEN, READ) \
    if (READ == true) {\
        get_token(TOKEN); \
        if (num_error != 0) { \
            return false;\
        } \
    } else {READ = true;} \
*/
//fprintf(stderr, "%d_\n", TOKEN->type);
#define GET_TOKEN(TOKEN, READ, DLL)\
    \
    if (dll_is_active_last(DLL) == true){\
        if (READ == true) {\
            get_token(TOKEN); \
            if (dll_insert(DLL, TOKEN) == 1){\
                return false;\
            }\
            \
            if (num_error != 0) { \
                return false;\
            }\
        } else {READ = true;}\
    } else {\
        dll_set_active_next(DLL);\
    }\

#define CHECK_VARS(A,B,ERR)\
    if (A != B) { \
        set_error(ERR); \
        return false; \
    }\

#define ENUM_APPEND(LIST, TYPE, LEN) \
    if (enum_append(LIST, TYPE, LEN) == false){ \
        set_error(INTERNAL_ERROR); \
        return false; \
    }\

#define COPY_ID(DST, SRC) \
    DST = malloc(sizeof(char) * (strlen(SRC) + 1) ); \
    strcpy(DST, SRC); \


#define NEW_SYMTABLE_FRAME \
    TreeNode *frame;\
    bst_init(&frame);\
    sym_stack_push(&(p_data->stack), frame);\

#define INSERT_VAR_TO_SYM(var_id, type) \
    node = sym_stack_top(&(p_data->stack));\
    sym_stack_pop(&(p_data->stack));\
    if (bst_insert(&node, var_id, type) == false) {\
        return false;\
    }\
    sym_stack_push(&(p_data->stack), node)\


#define VYPIS \
     fprintf(stderr, "pipik\n");\

#define TOKEN \
    fprintf(stderr, "%s\n", tokens[token->type]);\

typedef struct {

    Token *token;
    bool get_token;
    SymStack stack;
    TreeNode *global_frame;
    DLList dll_list;

} ParserData;


/**
 * @brief Function frees all allocated resources
 * 
 */
void free_resources(ParserData *p_data);

/**
 * @brief Function allocates data for parser
 * 
 * @return ParserData * 
 */
ParserData *parser_init();
char * copy_str(char *src);
bool insert_var_to_stack(ParserData *p_data, SymStack *pa_stack, char *id);
bool expression_list_others(ParserData *p_data, SymStack *pa_stack);
bool assign_return(ParserData *p_data, SymStack *pa_stack, char *func_id);
bool expression_list_and_func(ParserData *p_data, SymStack *pa_stack);
bool id_list(ParserData *p_data, char *id);
bool return_expr_list_others(ParserData *p_data, TreeNode *func, int *return_len, char *fun_id);
bool return_expr_list(ParserData *p_data, char *fun_id);
bool local_expr_2(ParserData *p_data, char *var_id, DataType type);
bool local_expr(ParserData *p_data, char *var_id, DataType type);
bool local(ParserData *p_data);
bool while_clause(ParserData *p_data, char* fun_id);
bool if_clause(ParserData *p_data, char *fun_id);
bool block_fwe(ParserData *p_data, bool is_if_block, char *fun_id);
bool param_list_others(ParserData *p_data, DataType **param_list, int *param_len, int *i);
bool param_list_func(ParserData *p_data, DataType **param_list, int *param_len);
bool return_types_list_local_others(ParserData *p_data, DataType **return_list, int *return_len, int *i);
bool return_types_list_local(ParserData *p_data, DataType **return_list, int *return_len);
bool function_def(ParserData *p_data);
bool return_types_list_others_global(ParserData *p_data, DataType **return_list, int *return_len);
bool return_types_list_global(ParserData *p_data, DataType **return_list, int *return_len);
bool data_types_list_others(ParserData *p_data, DataType **param_list, int *param_len);
bool data_types_list(ParserData *p_data, DataType **param_list, int *param_len);
bool global(ParserData *p_data);
bool function_param_list_others(ParserData *p_data, bool global_call, int *i, TreeNode *func);
bool function_param_list(ParserData *p_data, bool global_call, TreeNode *func);
bool function_call(ParserData *p_data,char *fun_id ,bool global_call);
bool body(ParserData *p_data);
bool program(ParserData *p_data);
/**
 * @brief Main program for parser
 * 
 * @return int 
 */
int syntactic_analyzator(); 

#endif