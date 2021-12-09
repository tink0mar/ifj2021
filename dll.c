/**
 * @file dll.c
 * @brief Implementation of DLL
 * @author Denis Adásek
 * 
 * Project: IFJ compiler
 * Date: 3.12.2021
 */ 

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

    char *ptr = copy_str_dll(token1->attribs.string);
    
    if (newElemPtr != NULL) {
        
        newElemPtr->token->type = token1->type;
        newElemPtr->token->attribs.number = token1->attribs.number;
        newElemPtr->token->attribs.integer = token1->attribs.integer;
        newElemPtr->token->attribs.string = ptr;
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

        free(elemPtr->token);
        free(elemPtr->token->attribs.string);
        free(elemPtr);
        elemPtr = list->firstElement;
    }
    list->firstElement = NULL;
    list->activeElement = NULL;
    list->lastElement = NULL;
}

void dll_return_token( DLList *list, Token *token ){
    char *ptr;
    if (list->activeElement->token->attribs.string != NULL){
         ptr = copy_str(list->activeElement->token->attribs.string);
    }
    token->type = list->activeElement->token->type;
    token->attribs.string = ptr;
    token->attribs.number = list->activeElement->token->attribs.number;
    token->attribs.integer = list->activeElement->token->attribs.integer;


}

void print_list(DLList *list){
    DLLElementPtr elemPtr;
    fprintf(stderr, "START \n");
    elemPtr = list->firstElement;
    while(elemPtr != list->lastElement){
        
        fprintf(stderr, "DLL LIST ELEMENT TOKEN TYPE = %s\n", elemPtr->token->attribs.string);
        elemPtr = elemPtr->nextElement;
    }
    fprintf(stderr, "DLL LIST ELEMENT TOKEN TYPE = %s\n", elemPtr->token->attribs.string);

}

