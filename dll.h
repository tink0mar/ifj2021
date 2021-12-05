#ifndef DLL_H
#define DLL_H
#include "scnr.h"

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
 * @brief delete all elements
 *
 * @param list The list address
 */
void dll_dispose( DLList *list );
#endif //DLL_H
