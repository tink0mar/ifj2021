/**
 * @file psa.h
 * @brief Interface of psa.c and psa_stack.c
 * @author Hugo Hežel
 *
 * Project: IFJ compiler
 * Date: 24.11.2021
 */

#ifndef PSA_H
#define PSA_H

#include "scanner.h"
#include "symtable.h"
#include "parser.h"
#include "generator.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*! A starting capacity of PsaStack */
#define START_CAPACITY 150

/*! A number of token types, used to create psa table */
#define TABLE_SIZE 14

/**
 * @struct Structure represents the psa item's content enumerated
 *
 */
typedef enum {
    I_HASHTAG,
    I_MUL,
    I_DIV,
    I_FLOOR_DIV,
    I_PLUS,
    I_MINUS,
    I_CONCAT,
    I_ID,
    I_INTEGER,
    I_NUMBER,
    I_STRING,
    I_LEFT_PAR,
    I_RIGHT_PAR,
    I_DOLLAR,
    I_EXPR,
    I_HANDLE,
    I_NULL
} PsaItemType;

/**
 * @struct Structure represents an item in PsaStack
 */
typedef struct {
    bool terminal;                  // terminal or noterminal
    PsaItemType type;               // Type of psa item
    DataType data_type;             // Data type of psa item ( string, number, integer or nil )
    Token token_representation;    // Token that can be represented by this psa item if its terminal
} PsaStackItem;

/**
 * @struct Structure represents a stack used for PSA
*/
typedef struct {
    PsaStackItem *content;
    int top_index;
    int capacity;
    int size;
} PsaStack;

/**
 * @brief Create a stack used in PSA
 * @return
 */
PsaStack *psa_stack_create();

/**
 * @brief Realloc the capacity of the stack
 * @param stack The stack
 * @param new_capacity Integer representing the new capacity
 * @return
 */
void psa_stack_realloc_capacity(PsaStack *stack, int new_capacity);

/**
 * @brief Check if the size of the stack isn't too low according to capacity
 * @param stack The stack
 * @return
 */
void psa_stack_check_low_capacity(PsaStack *stack);

/**
 * @brief Check if the size of the stack isn't too high according to capacity
 * @param stack The stack
 * @return
 */
void psa_stack_check_high_capacity(PsaStack *stack);

/**
 * @brief A function to recognize whether a stack is empty or not
 * @param stack The stack that will be checked whether it's empty
 * @return true if the stack is empty, otherwise false
*/
bool psa_stack_is_empty(PsaStack *stack);

/**
 * @brief Push new item to stack
 * @param stack The stack
 * @return
 */
void psa_stack_push(PsaStack *stack, bool terminal, PsaItemType type, DataType data_type, Token token_representation);

/**
 * @brief Pop an item from stack
 * @param stack The stack
 * @return
 */
void psa_stack_pop(PsaStack *stack);

/**
 * @brief Function returns the top item of a stack
 * @param stack The stack
 * @return PsaStackItem *
 */
PsaStackItem *psa_stack_top(PsaStack *stack);

/**
 * @brief Function returns the top terminal item of a stack
 * @param stack The stack
 * @return PsaStackItem *
 */
PsaStackItem *psa_stack_top_terminal(PsaStack *stack);

/**
 * @brief Function that converts token to PsaStackItem
 * @param token The given token
 * @param parser_data ParserData for general info about parser
 * @return PsaStackItem 
 */
PsaStackItem token_to_psa_stack_item(Token *token, ParserData *parser_data);

/**
 * @brief A part function for the shift process
 * @param stack The stack
 * @param entry_item The entry item
 * @param entry_token The entry token
 * @param parser_data General info about parser
 * @return true Returns true on success
 * @return false Returns false when the function fails 
 */
bool psa_push_and_load(PsaStack *stack, PsaStackItem *entry_item, Token *entry_token, ParserData *parser_data);

/**
 * @brief A part function for the shift process
 * @param stack The stack
 */
void psa_modify_top_terminal(PsaStack *stack);

/**
 * @brief Auxiliary function of psa_reduce()
 * @param stack 
 * @param top_item_type 
 */
void psa_reduce_pop_and_update(PsaStack *stack, PsaItemType *top_item_type);

/**
 * @brief Reduce process of PSA
 * @param stack The stack
 * @param parser_data General info about parser
 * @return true Returns true on success
 * @return false Returns false on failure
 */
bool psa_reduce(PsaStack *stack, ParserData *parser_data);

/**
 * @brief Main PSA function
 * @param parser_data General info about parser
 * @return DataType Returns DataType of the final expression
 */
DataType psa(ParserData *parser_data);

/**
 * @brief Function is called to check 
 * 
 * @param parser_data General info about parser
 * @param its_if Bool indicates whether its if or while statement
 * @return true Returns true on success
 * @return false Returns false on failure
 */
bool psa_condition(ParserData *parser_data, bool its_if);

/**
 * @brief Function deallocates the stack
 * @param stack The stack to be deallocated
 */
void psa_stack_destroy(PsaStack *stack);

#endif