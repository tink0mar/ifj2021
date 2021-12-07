/**
 * @file symtable.c
 * @brief Symtable for ifj compiler
 * @author Denis Adásek
 *
 * Project: IFJ compiler
 * Date: 24.11.2021
 */

#include "symtable.h"
#include "error.h"
#include <stdbool.h>
#include <stdio.h>


int STACK_SIZE = MAX_STACK;

void sym_stack_init( SymStack *stack) {

    if (stack == NULL) {
        set_error(INTERNAL_ERR);
        return;
    } else {
        stack->topIndex = -1;

    }
}
bool sym_stack_is_empty( const SymStack *stack ) {
    return stack->topIndex == -1;
}

bool sym_stack_is_full( const SymStack *stack ) {


    return stack->topIndex == STACK_SIZE-1;
}

TreeNode *sym_stack_top( const SymStack *stack){
    if(sym_stack_is_empty(stack))
    {
        return NULL;
    }
    return stack->array[stack->topIndex];
}
void sym_stack_pop( SymStack *stack ){
    if(!sym_stack_is_empty(stack))
    {
        stack->topIndex -= 1;
    }
}
void sym_stack_push( SymStack *stack, TreeNode *root ) {
    if (sym_stack_is_full(stack)) {
        return;
    } else {
        stack->topIndex++;
        stack->array[stack->topIndex] = root;


    }
}
void sym_stack_dispose( SymStack *stack){
    while (stack->topIndex != -1){
        TreeNode *ptr = sym_stack_top(stack);
        bst_dispose(&ptr);
        sym_stack_pop(stack);
    }
}
