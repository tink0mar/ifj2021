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
        if (READ == true){\
            dll_set_active_next(DLL);\
            dll_return_token(&p_data->dll_list, p_data->token);\
            TOKEN = p_data->TOKEN;\
        }\
        else {\
            READ = true;\
        }\
    }\

#define UNGET_TOKEN(TOKEN) \
    dll_set_active_previous(&p_data->dll_list);\
    dll_return_token(&p_data->dll_list, p_data->token);\
    TOKEN = p_data->token;\

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
 * @return ParserData *p_data
 */
ParserData *parser_init();

/**
 * @brief copy src to return
 * 
 * @param src source file
 * @return char* array
 */
char * copy_str(char *src);

/**
 * @brief Inserts variable to stack to be evaluated postorded
 * 
 * @param p_data parser data
 * @param pa_stack stack of variable
 * @param id id of item
 * @return true 
 * @return false 
 */
bool insert_var_to_stack(ParserData *p_data, SymStack *pa_stack, char *id);

/**
 * @brief Rules for expression list
 * 
 * @param p_data Data for parser
 * @param pa_stack Stack of variables
 */
bool expression_list_others(ParserData *p_data, SymStack *pa_stack);

/**
 * @brief assigns values to return variables
 * 
 * @param p_data Data parser
 * @param pa_stack Stack of variables
 * @param func_id Name of function
 */

bool assign_return(ParserData *p_data, SymStack *pa_stack, char *func_id);

/**
 * @brief Function for expressions
 * 
 * @param p_data Data of Parser
 * @param pa_stack Stack of variables
 */
bool expression_list_and_func(ParserData *p_data, SymStack *pa_stack);

/**
 * @brief Function for list of ids
 * 
 * @param p_data Data for parser
 * @param id name of first identifier
 */
bool id_list(ParserData *p_data, char *id);

/**
 * @brief Function for more return values
 * 
 * @param p_data 
 * @param func 
 * @param return_len 
 * @param fun_id 
 */
bool return_expr_list_others(ParserData *p_data, TreeNode *func, int *return_len, char *fun_id);

/**
 * @brief Function for return values
 * 
 * @param p_data Data for parser
 * @param fun_id Name of function
 */
bool return_expr_list(ParserData *p_data, char *fun_id);

/**
 * @brief Function to choose which expression will be executed
 * 
 * @param p_data Data for parser
 * @param var_id Name of identifier
 * @param type Type of variable 
 */
bool local_expr_2(ParserData *p_data, char *var_id, DataType type);

/**
 * @brief Function checks if there is assign 
 * 
 * @param p_data Data for parser
 * @param var_id Name of identifier
 * @param type Data type of identifier
 */
bool local_expr(ParserData *p_data, char *var_id, DataType type);

/**
 * @brief Local command
 * 
 * @param p_data Data for parser
 */
bool local(ParserData *p_data);

/**
 * @brief Block for while clause
 * 
 * @param p_data Data for parser
 * @param fun_id Name of function
 */

bool while_clause(ParserData *p_data, char* fun_id);

/**
 * @brief Block of if clause
 * 
 * @param p_data Data for parser
 * @param fun_id Name of function
 */
bool if_clause(ParserData *p_data, char *fun_id);

/**
 * @brief Function chooses blocks 
 * 
 * @param p_data Data for parser
 * @param is_if_block Bool for if and while block
 * @param fun_id Name of function
 */
bool block_fwe(ParserData *p_data, bool is_if_block, char *fun_id);


/**
 * @brief 
 * 
 * @param p_data @brief Function stores parameters types of function
 * @param param_list List of parameters
 * @param param_len Length of list of parameters
 * @param i index of parameter
 */
bool param_list_others(ParserData *p_data, DataType **param_list, int *param_len, int *i);

/**
 * @brief Function stores parameters types of function
 * 
 * @param p_data Data for parser
 * @param param_list List of parameters
 * @param param_len Length of list of parameters 
 */

bool param_list_func(ParserData *p_data, DataType **param_list, int *param_len);

/**
 * @brief Function stores return types of function
 * 
 * @param p_data Data of parser
 * @param return_list Return list of parameters
 * @param return_len Length of return list
 * @param i Index of return types 
 */
bool return_types_list_local_others(ParserData *p_data, DataType **return_list, int *return_len, int *i);

/**
 * @brief Function stores return types of function
 * 
 * @param p_data Data of parser
 * @param return_list List of return parameters
 * @param return_len  Length of return parameters
 */
bool return_types_list_local(ParserData *p_data, DataType **return_list, int *return_len);

/**
 * @brief Function creates and checks declaration of function
 * 
 * @param p_data Data of parser
 */
bool function_def(ParserData *p_data);

/**
 * @brief Function stores data types of others returns parameters
 * 
 * @param p_data Data of parser
 * @param return_list List of return parameters
 * @param return_len Length of return parameters
 */

bool return_types_list_others_global(ParserData *p_data, DataType **return_list, int *return_len);

/**
 * @brief Function stores data types of returns parameters
 * 
 * @param p_data Data of parser
 * @param return_list List of return parameters
 * @param return_len Length of return parameters
 */
bool return_types_list_global(ParserData *p_data, DataType **return_list, int *return_len);

/**
 * @brief Function store others data types to param_list
 * 
 * @param p_data Data of parser
 * @param param_list List of parameters
 * @param param_len Length of parameters
 */
bool data_types_list_others(ParserData *p_data, DataType **param_list, int *param_len);

/**
 * @brief Function store data types to param_list
 * 
 * @param p_data Data of parser
 * @param param_list list of data types of parameters
 * @param param_len length of parameters
 */
bool data_types_list(ParserData *p_data, DataType **param_list, int *param_len);

/**
 * @brief Function generates declaration of function
 * 
 * @param p_data Data of Parser
 */
bool global(ParserData *p_data);

/**
 * @brief Function check others parameters 
 * 
 * @param p_data Data of Parser
 * @param global_call bool for global call
 * @param i index of parameter
 * @param func node of function 
 */
bool function_param_list_others(ParserData *p_data, bool global_call, int *i, TreeNode *func);

/**
 * @brief Function checks parameters
 * 
 * @param p_data Data of parser
 * @param global_call bool for global call
 * @param func btree of function to be called
 */
bool function_param_list(ParserData *p_data, bool global_call, TreeNode *func);

/**
 * @brief Function to call function in assembly
 * 
 * @param p_data Data of parser
 * @param fun_id id of function
 * @param global_call bool for global call
 */

bool function_call(ParserData *p_data,char *fun_id ,bool global_call);

/**
 * @brief Rule for body global, function, function call
 * 
 * @param p_data data for parser
 */

bool body(ParserData *p_data);

/**
 * @brief Main rule for ll gramatics
 * 
 * @param p_data Data of parser
 */
bool program(ParserData *p_data);


/**
 * @brief Main program for parser
 * 
 * @return int 
 */
int syntactic_analyzator(); 

#endif