/**
 * @file psa_stack.c
 * @brief Stack for Operator Precedence Syntax Analysis
 * @author Hugo Hežel
 *
 * Project: IFJ compiler
 * Date: 1.12.2021
 */

#include "psa.h"

PsaStack *psa_stack_create(){

    PsaStack *stack = malloc( sizeof( PsaStack ) );

    if( stack == NULL ){

        // TODO

    }else {

        stack->top_index = -1;
        stack->capacity = START_CAPACITY;
        stack->size = 0;
        stack->content = malloc( START_CAPACITY * sizeof( PsaStackItem ) );

        if( stack->content == NULL ){

            // TODO

        }else{

            return stack;

        }

    }

}

void psa_stack_realloc_capacity(PsaStack *stack, int new_capacity){

    // Realloc memory allocated for stack's content.
    stack->content = realloc( stack->content, new_capacity * sizeof( PsaStackItem ) );

    if( stack->content == NULL ){

        // TODO

    }else {

        // Set the new capacity value.
        stack->capacity = new_capacity;

    }

}

void psa_stack_check_low_capacity(PsaStack *stack){

    // If stack's size is more than 4/5 of its capacity.
    if( stack->size > ( stack->capacity / 5 * 4 ) ){

        // Calculate new value for stack's capacity (double its size).
        int new_capacity = 2 * stack->capacity;

        // Cal the realloc function.
        psa_stack_realloc_capacity( stack, new_capacity );

    }

}

void psa_stack_check_high_capacity(PsaStack *stack){

    // If stack's size is less than 1/4 of its capacity.
    if( stack->size < ( stack->capacity / 4 ) ){

        // Calculate new value for stack's capacity (divide its size by 2).
        int new_capacity = stack->capacity / 2;

        // Call the realloc function.
        psa_stack_realloc_capacity( stack, new_capacity );

    }

}

bool psa_stack_is_empty(PsaStack *stack){

    if( stack->top_index == -1 ){

        return true;

    }else{

        return false;

    }

}

void psa_stack_push(PsaStack *stack, bool terminal, PsaItemType type, DataType data_type, Token token_representation){

    stack->top_index++;
    stack->size++;
    stack->content[ stack->top_index ].terminal = terminal;
    stack->content[ stack->top_index ].type = type;
    stack->content[ stack->top_index ].data_type = data_type;
    stack->content[ stack->top_index ].token_representation = token_representation;

    psa_stack_check_low_capacity( stack );

}

void psa_stack_pop(PsaStack *stack){

    stack->top_index--;
    stack->size--;

    psa_stack_check_high_capacity( stack );

}

PsaStackItem *psa_stack_top(PsaStack *stack){

    return &( stack->content[ stack->top_index ] );

}

PsaStackItem *psa_stack_top_terminal(PsaStack *stack){

    if( !psa_stack_is_empty( stack ) ){

        for( int i = stack->top_index; i >= 0; i-- ){

            if( stack->content[i].terminal ){

                return &( stack->content[i] );

            }

        }

    }

}

// REMOVE THIS FUNCTION
// void print_psa_stack(PsaStack *stack){
//
//     if( psa_stack_is_empty( stack ) ){
//
//         fprintf(stderr, "Stack is empty.\n");
//
//     }else {
//
//         for( int i = 0; i < stack->size; i++ ){
//             fprintf(stderr, "stack[%i] = %i\n", i, stack->content[i].type );
//         }
//
//         fprintf(stderr, "\n");
//
//     }
//
// }

// REMOVE THIS FUNCTION
void print_psa_stack_top(PsaStack *stack){

    if( psa_stack_is_empty( stack ) ){

        printf("Stack is empty.\n");

    }else {

        printf( "Stack's top item: %i - %i\n", stack->content[ stack->top_index ].type, stack->content[ stack->top_index ].type );

    }

}

// REMOVE THIS FUNCTION
void print_psa_stack_configuration(PsaStack *stack){

    if( stack != NULL ){

        printf("STACK top_index = %i, size = %i, capacity = %i\n", stack->top_index, stack->size, stack->capacity);

    }

}
