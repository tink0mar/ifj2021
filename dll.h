/**
 * @file dll.h
 * @brief Header file for DLL
 * @author Denis Adásek
 * 
 * Project: IFJ compiler
 * Date: 3.12.2021
 */ 

#ifndef DLL_H
#define DLL_H
#include "scanner.h"
#include <stdbool.h>

typedef struct DLLElement {
    /** Token. */
    Token *token;
    /** The address of previous element */
    struct DLLElement *previousElement;
    /** The address of next element */
    struct DLLElement *nextElement;
} *DLLElementPtr;

/** Double linked list. */
typedef struct {
    /** The address of first element */
    DLLElementPtr firstElement;
    /** The address of active element */
    DLLElementPtr activeElement;
    /** The address of last element */
    DLLElementPtr lastElement;
} DLList;
/**
 * @brief list initialization
 *
 * @param list The list address for initialization
 */
void dll_init( DLList *list );

/**
 * @brief Insert element
 *
 * @param list The list address for insert
 * @param token Data for inserted element
 *
 * @return 0 node was inserted / 1 malloc error
 */
int dll_insert( DLList *list, Token *token );

/**
 * @brief set next element active
 *
 * @param list The list address
 */
void dll_set_active_next( DLList *list );

/**
 * @brief set previous element active
 *
 * @param list The list address
 */
void dll_set_active_previous( DLList *list );

/**
 * @brief set last element active
 *
 * @param list The list address
 */
void dll_set_active_last( DLList *list );

/**
 * @brief find out if last element is active
 *
 * @param list The list address
 *
 * @return true if last element is active / false otherwise
 */
bool dll_is_active_last( DLList *list );

/**
 * @brief delete all elements
 *
 * @param list The list address
 */
void dll_dispose( DLList *list );

void dll_return_token( DLList *list, Token *token );
void  print_list(DLList *list);

#endif //DLL_H
