#include "dll.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "parser.h"

void dll_init( DLList *list ) {

    list->firstElement = NULL;
    list->activeElement = NULL;
    list->lastElement = NULL;
}

char * copy_str_dll(char *src){
    if (src == NULL){
        return NULL;
    }
    char *ptr = malloc(sizeof(char) * (strlen(src) + 1) ); \
    strcpy(ptr, src);
}

int dll_insert( DLList *list, Token *token1 ) {

    DLLElementPtr newElemPtr = malloc(sizeof(struct DLLElement));
    newElemPtr->token = (Token *)malloc(sizeof( Token));

    if (newElemPtr != NULL) {
        
        newElemPtr->token->type = token1->type;
        newElemPtr->token->attribs.number = token1->attribs.number;
        newElemPtr->token->attribs.integer = token1->attribs.integer;
        newElemPtr->token->attribs.string = copy_str_dll(token1->attribs.string);
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

Token *dll_return_token( DLList *list ){

    return list->activeElement->token;

}

