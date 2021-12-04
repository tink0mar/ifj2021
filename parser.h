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

typedef struct {

    Token *token;
    bool get_token;
    SymStack stack;
    TreeNode *global_frame;

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

