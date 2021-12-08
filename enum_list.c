/**
 * @file enum_list.c
 * @brief Functions for dynamic enumerate array
 * @author Denis Adásek
 *
 * Project: IFJ compiler
 * Date: 24.11.2021
 */

#include <stdbool.h>
#include <stdio.h>
#include "enum_list.h"

bool enum_append(DataType **list, DataType type, int *len){

    DataType *ptr = (DataType *)realloc(*list, sizeof(DataType) * (*len + 1)); 
    
    if (ptr == NULL){
        return false;
    } else {
        
        ptr[*len] = type;
        
        *list = ptr;
        
        ++*len;
        return true;
    }
}

void enum_delete(DataType *type){
    free(type);
}

bool enums_check(DataType *list_one, DataType *list_two, int len_one, int len_two){
    
    if (len_one == len_two) {
        for (int i = 0; i < len_one; i++) {
            if (list_one[i] != list_two[i]){
                return false;
            };
        }
        return true;
    } else {
        return false;
    }
}


