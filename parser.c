/**
 * @file parser.c
 * @brief Syntax analyzator
 * @author Martin Kozák
 *
 * Project: IFJ compiler
 * Date: 24.11.2021
 */

#include <stdlib.h>
#include <stdbool.h>
#include "error.h"
#include "parser.h"
#include "scanner.h"
#include "symtable.h"
#include "enum_list.h"

bool check_tokens(TokenType token_type)
{
    TokenType arr[5] = {TT_IDENTIFIER, TT_INTEGER, TT_STRING, TT_NUMBER, TT_KW_NIL};

    for (int i = 0; i < 5; i++)
    {
        if (token_type == arr[i])
            return true;
    }
    return false;
}

void free_resources(ParserData *p_data)
{

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
    bst_init(&(p_data->global_frame));
    p_data->get_token = true;
    return p_data;
}

bool if_clause(ParserData *p_data) {
    Token *token = p_data->token;

    GET_TOKEN(token, p_data->get_token);

    if (token->type != ) {

        psa(token);

    }


}

/**
 * <data_types_list_others> -> eps
 * <data_types_list_others> -> , <data_types> <data_types_list_others>
 */

bool data_types_list_others(ParserData *p_data, DataType *param_list, int *param_len)
{
    Token *token = p_data->token;

    GET_TOKEN(token, p_data->get_token);

    // eps
    if (token->type == TT_RIGHT_PAR)
    {
        return true;
    }
    else if (token->type == TT_COMMA)
    { // check comma after data_type

        GET_TOKEN(token, p_data->get_token);

        switch (token->type)
        {

        case TT_KW_STRING:
            enum_append(&param_list, STR, param_len);
            break;

        case TT_KW_NUMBER:
            enum_append(&param_list, NUM, param_len);
            break;

        case TT_KW_INTEGER:
            enum_append(&param_list, INT, param_len);
            break;

        default:
            set_error(SYNTACTIC_ERR);
            return false;
        }

        return data_types_list_others(p_data, param_list, param_len);
    }
    else
    {
        set_error(SYNTACTIC_ERR);
        return false;
    }
}

/**
 * GLOBAL RETURN TYPES
 * <return_types_list> -> eps
 * <return_types_list> -> : <data_type> <data_types_list_others>
 */

bool return_types_list_global(ParserData *p_data, DataType *return_list, int *return_len)
{
    Token *token = p_data->token;

    GET_TOKEN(token, p_data->get_token);

    switch (token->type)
    {
        case TT_EOF:
        case TT_KW_GLOBAL:
        case TT_KW_FUNCTION:
        case TT_IDENTIFIER:
            p_data->get_token = false;
            break;

        case TT_CO:

    }
}

/**
 * <data_types_list> -> eps
 * <data_types_list> -> <data_type> <data_types_list_others>
 */
bool data_types_list(ParserData *p_data, DataType **param_list, int *param_len)
{

    Token *token = p_data->token;

    GET_TOKEN(token, p_data->get_token);

    switch (token->type)
    {
    case TT_RIGHT_PAR:
        // eps
        return true;
        break;

    case TT_KW_STRING:
        enum_append(&param_list, STR, *param_len);
        break;

    case TT_KW_NUMBER:
        enum_append(&param_list, NUM, *param_len);
        break;

    case TT_KW_INTEGER:
        enum_append(&param_list, INT, *param_len);
        break;

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
    int return_len;
    int param_len;

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
        char *fun_id = token->attribs.string;

        // check colon
        GET_TOKEN(token, p_data->get_token);
        CHECK_VARS(token->type, TT_COLON, SYNTACTIC_ERR);

        // check "function"
        GET_TOKEN(token, p_data->get_token);
        CHECK_VARS(token->type, TT_KW_FUNCTION, SYNTACTIC_ERR);

        // check left parenthise
        GET_TOKEN(token, p_data->get_token);
        CHECK_VARS(token->type, TT_LEFT_PAR, SYNTACTIC_ERR);

        if ((data_types_list(p_data, &param_list, &param_len)))
        {

            if (return_types_list_global(p_data, &return_list, &return_len))
            {
                bool is_defined = false;

                return bst_insert_fun(p_data, fun_id, param_len, param_list, return_len, return_list, false);
            }
            else
            {
                return false
            }
        }
        else
        {
            return false;
        }

        // TODO return types
    }
}

bool function_param_list_others(ParserData *p_data, bool is_global_call)
{

    Token *token = p_data->token;
    DataType *params = NULL;
    // Bad parameters

    return true;
}

/**
 *
 * <function_param_list> -> <function_param> <function_param_list_others>
 * <function_param_list_others> -> eps
 * <function_param_list_others> -> , <function_param> <function_param_list_others
 */

bool function_param_list(ParserData *p_data, bool is_global_call, TreeNode *func)
{

    // MAM TOKEN
    Token *token = p_data->token;

    // Bad parameters
}

/**
 * <function_call> -> ID ( <function_param_list> )
 */
bool function_call(ParserData *p_data, bool is_global_call)
{

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
    CHECK_VARS(func->fun_extension->is_defined, true, SEM_UNDEFINED_ERR);

    // check if function has return types
    // TODO NEVIEM AKA CHYBA
    if (is_global_call)
    {
        CHECK_VARS(func->fun_extension->cnt_return_type, 0, );
    }

    // check left parenthise
    GET_TOKEN(token, p_data->get_token);
    CHECK_VARS(token->type, TT_LEFT_PAR, SYNTACTIC_ERR);

    GET_TOKEN(token, p_data->get_token);

    // <function_param_list> -> eps
    if (token->type == TT_RIGHT_PAR)
    {

        // TODO CALL EMPTY function

        return true;
    }
    else if (function_param_list(p_data, is_global_call, func))
    {

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
    if (token->type == TT_EOF)
    {
        return true;
    }
    else
    {

        switch (token->type)
        {

        // global
        case TT_KW_GLOBAL:
            return global(p_data) && body(p_data);
            break;

        // function
        case TT_KW_FUNCTION:
            return body(p_data);
            break;

        // call function
        case TT_IDENTIFIER:
            return function_call(p_data, true) && body(p_data);
            break;
        }
    }

    set_error(SYNTACTIC_ERR);
    return false;
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
            return true && body(p_data);
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
        error_mess(INTERNAL_ERR);
        return INTERNAL_ERR;
    }

    if (program(p_data))
    {
        free_resources(p_data);
        return 0;
    }
    else
    {
        error_mess(num_error);
        free_resources(p_data);
        return num_error;
    }
}
