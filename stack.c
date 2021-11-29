/**
 * @file symtable.c
 * @brief Symtable for ifj compiler
 * @author Denis Adásek
 *
 * Project: IFJ compiler
 * Date: 24.11.2021
 */

#include "symtable.h"
#include <stdlib.h>

int STACK_SIZE = MAX_STACK;

void Stack_Init( Stack *Stack) {

    if (Stack == NULL) {
//todo
        return;
    } else {
        Stack->topIndex = -1;

    }
}
int Stack_IsEmpty( const Stack *Stack ) {
    return Stack->topIndex == -1;
}

int Stack_IsFull( const Stack *Stack ) {


    return Stack->topIndex == STACK_SIZE-1;
}

TreeNode Stack_Top( const Stack *Stack) {
    if(Stack_IsEmpty(Stack))
    {
        return;
    }
    return Stack->array[Stack->topIndex];
}
void Stack_Pop( Stack *Stack ){
    if(!Stack_IsEmpty(stack))
    {
        stack->topIndex -= 1;
    }
}
void Stack_Push( Stack *Stack, TreeNode *root ) {
    if (Stack_IsFull(Stack)) {
        return;
    } else {
        Stack->topIndex++;
        Stack->array[Stack->topIndex] = root;
    }
}
