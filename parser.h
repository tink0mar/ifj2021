/**
 * @file parser.h
 * @brief Interface of syntactic analyser
 * @author Martin Kozák
 * 
 * Project: IFJ compiler
 * Date: 24.11.2021
 */ 

#include <stdbool.h>
#include "symtable.h"
#include "scanner.h"
#include "error.h"
#include "enum_list.h"

#define GET_TOKEN(TOKEN, READ) \
    if (READ == true) {\
        get_token(TOKEN); \
        if (num_error != 0) { \
            return false;\
        } \
    } else {READ = true;} \
/**
#define GET_TOKEN(TOKEN, READ, DLL)\
    if (dll_last_active(DLL) == true){\
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
    } \
*/

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
    TreeNode *node = sym_stack_top(&(p_data->stack));\
    if (bst_insert(&node, var_id, type) == false) {\
        return false;\
    }\

typedef struct {

    Token *token;
    bool get_token;
    SymStack stack;
    TreeNode *global_frame;
    

    //char *code;
    //char *main;

} ParserData;


bool program();

/**
 * @brief Main program for parser
 * 
 * @return int 
 */
int syntactic_analyzator();

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

/**
 * @brief Function for checking tokens
 * 
 * @param token_type Type of token 
 * @param arr Array of token types
 * @param len Length of arr
 * @return true  
 * @return false 
 */
bool check_tokens(TokenType token_type);

