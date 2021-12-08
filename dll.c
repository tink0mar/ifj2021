#include "dll.h"
#include <stdio.h>
#include <malloc.h>

void dll_init( DLList *list ) {

    list->firstElement = NULL;
    list->activeElement = NULL;
    list->lastElement = NULL;
}
int dll_insert( DLList *list, Token *token ) {

    DLLElementPtr newElemPtr = malloc(sizeof(struct DLLElement));
    if (newElemPtr != NULL) {
        newElemPtr->token = token;
        newElemPtr->previousElement = list->lastElement;
        newElemPtr->nextElement = NULL;

        if (list->firstElement != NULL) {
            list->lastElement->nextElement = newElemPtr;
        }
        else
        {

            list->firstElement = newElemPtr;
        }
        list->lastElement = newElemPtr;
        list->activeElement = newElemPtr;
        return 0;
    }
    return 1;

}

void dll_set_active_next( DLList *list ) {

    if(list->activeElement != NULL)
    {
        if(list->activeElement != list->lastElement)
        {
            DLLElementPtr elemPtr;
            elemPtr = list->activeElement->nextElement;
            list->activeElement = elemPtr;
        }
    }
}

void dll_set_active_previous( DLList *list ) {

    if(list->activeElement != NULL)
    {
        if(list->activeElement != list->firstElement)
        {
            DLLElementPtr elemPtr;
            elemPtr = list->activeElement->previousElement;
            list->activeElement = elemPtr;
        }
    }
}

void dll_set_active_last( DLList *list ) {
    if(list->activeElement != NULL)
    {
        list->activeElement = list->lastElement;
    }
}
bool dll_is_active_last( DLList *list ) {
    if(list->firstElement == NULL) { 
        return true;
    }

    if(list->activeElement == list->lastElement)
        {
            return true;
        }
    
    return false;
}
void dll_dispose( DLList *list ) {
    DLLElementPtr elemPtr;
    elemPtr = list->firstElement;
    while( elemPtr != NULL)
    {
        list->firstElement = list->firstElement->nextElement;
        free(elemPtr);
        elemPtr = list->firstElement;
    }
    list->firstElement = NULL;
    list->activeElement = NULL;
    list->lastElement = NULL;
}
