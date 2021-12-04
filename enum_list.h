/**
 * @file enum_list.h
 * @brief Interface of functions for dynamic enumerate array
 * @author Denis Adásek
 *
 * Project: IFJ compiler
 * Date: 24.11.2021
 */

#ifndef ENUM_LIST_H
#define ENUM_LIST_H


#include "symtable.h"
#include <stdlib.h>

/**
 * @brief 
 * 
 * @param list 
 * @param len 
 * @return true 
 * @return false 
 */
bool enum_append(DataType **list, DataType type, int *len);

/**
 * @brief 
 * 
 * @param type 
 */
void enum_delete(DataType *type);

/**
 * @brief 
 * 
 * @param list_one 
 * @param list_two 
 * @param len_one 
 * @param len_two 
 * @return true 
 * @return false 
 */
bool enums_check(DataType *list_one, DataType *list_two, int len_one, int len_two);

#endif