/**
 * @file psa.c
 * @brief Operator Precedence Syntax Analysis
 * @author Hugo Hežel
 *
 * Project: IFJ compiler
 * Date: 1.12.2021
 */

#include "psa.h"

char *PsaItemTypeStrings[] = {
    "#",
    "*",
    "/",
    "//",
    "+",
    "-",
    "..",
    "id",
    "int",
    "numb",
    "str",
    "(",
    ")",
    "$",
    "E",
    "<",
    "I_NULL"
};

void print_psa_stack(PsaStack *stack){

    if( psa_stack_is_empty( stack ) ){

        fprintf(stderr, "Stack is empty.\n");

    }else {

        for( int i = 0; i < stack->size; i++ ){
            fprintf(stderr, "stack[%i] = %s\n", i, PsaItemTypeStrings[ stack->content[i].type ] );
        }

        fprintf(stderr, "\n");

    }

}

// Create an empty token that will be given to items without token
Token empty_token;

PsaStackItem token_to_psa_stack_item(Token *token, SymStack *sym_stack){

    PsaStackItem item;

    //
    TreeNode *node;

    switch ( token->type ) {
        case TT_PLUS:
            item.data_type = NOTHING;
            item.type = I_PLUS;
            break;
        case TT_MINUS:
            item.data_type = NOTHING;
            item.type = I_MINUS;
            break;
        case TT_MUL:
            item.data_type = NOTHING;
            item.type = I_MUL;
            break;
        case TT_DIV:
            item.data_type = NOTHING;
            item.type = I_DIV;
            break;
        case TT_FLOOR_DIV:
            item.data_type = NOTHING;
            item.type = I_FLOOR_DIV;
            break;
        case TT_HASHTAG:
            item.data_type = NOTHING;
            item.type = I_HASHTAG;
            break;
        case TT_GREATER:
            item.data_type = NOTHING;
            item.type = I_DOLLAR;
            break;
        case TT_LESS:
            item.data_type = NOTHING;
            item.type = I_DOLLAR;
            break;
        case TT_GREATER_OR_EQ:
            item.data_type = NOTHING;
            item.type = I_DOLLAR;
            break;
        case TT_LESS_OR_EQ:
            item.data_type = NOTHING;
            item.type = I_DOLLAR;
            break;
        case TT_NOT_EQ:
            item.data_type = NOTHING;
            item.type = I_DOLLAR;
            break;
        case TT_EQ:
            item.data_type = NOTHING;
            item.type = I_DOLLAR;
            break;
        case TT_LEFT_PAR:
            item.data_type = NOTHING;
            item.type = I_LEFT_PAR;
            break;
        case TT_RIGHT_PAR:
            item.data_type = NOTHING;
            item.type = I_RIGHT_PAR;
            break;
        case TT_CONCAT:
            item.data_type = NOTHING;
            item.type = I_CONCAT;
            break;
        case TT_NUMBER:
            item.data_type = NUM;
            item.type = I_NUMBER;
            break;
        case TT_INTEGER:
            item.data_type = INT;
            item.type = I_INTEGER;
            break;
        case TT_STRING:
            item.data_type = STR;
            item.type = I_STRING;
            break;
        case TT_IDENTIFIER:

            // Search in symtable for id's type
            // node = bst_search_in_stack( sym_stack, token->attribs.string );

            // if( node == NULL ){
            //
            //     // TODO error
            //     // node sa nenašiel v symtable => neplatny identifikator
            //
            // }else{

                item.data_type = INT;
                item.type = I_ID;
                break;

            // }

        default:
            item.data_type = NOTHING;
            item.type = I_DOLLAR;
            break;
    }

    item.terminal = true;
    item.token_representation = *token; // QUESTION tokencopy

    return item;

}

void psa_push_and_load(PsaStack *stack, PsaStackItem *entry_item, Token *entry_token, SymStack *sym_stack){

    // Push the item to psa stack
    psa_stack_push( stack, entry_item->terminal, entry_item->type, entry_item->data_type, entry_item->token_representation );

    // Get another token
    get_token( entry_token );

    // Convert new token to psa stack's item
    *entry_item = token_to_psa_stack_item( entry_token, sym_stack );

};

void psa_modify_top_terminal(PsaStack *stack){

    // Variable item is used for the while cycle that starts with top terminal
    PsaStackItem *item = psa_stack_top_terminal( stack );

    // Variables for temporary saving items from stack
    PsaStackItem tmp_1, tmp_2;

    // If top terminal is also top item on stack
    if( item == psa_stack_top( stack ) ){

        // Just push the handle item
        psa_stack_push( stack, false, I_HANDLE, NOTHING, empty_token);

    }else {

        // Save data of (top terminal + 1)
        tmp_1 = item[1];

        // Insert new "<" noterminal after top terminal.
        item[1].terminal = false;
        item[1].type = I_HANDLE;
        item[1].data_type = NOTHING;
        item[1].token_representation = empty_token;

        // If the top terminal is one item begind the top item on stack
        if( &( item[1] ) == psa_stack_top( stack ) ){

            // Push the last saved item back on stack
            psa_stack_push( stack, tmp_1.terminal, tmp_1.type, tmp_1.data_type, tmp_1.token_representation );

        // If there are 2 or more items between top terminal and top item
        }else{

            // Start the loop from second right neighbor of top terminal.
            int i = 2;

            while( &item[i] < &stack->content[ stack->top_index ] ){

                // Save data of the current item into tmp_2
                tmp_2 = item[i];

                // Rewrite current item's data with data saved from previous iteration
                item[i] = tmp_1;

                // "Empty" the tmp2 box for another iteration
                tmp_1 = tmp_2;

                i++;

            }

            // Save data of the top item of stack
            tmp_2 = item[i];

            // Rewrite the top item's data with data saved from previous iteration
            item[i] = tmp_1;

            // Push the leftover data to into the right neighbor of
            psa_stack_push( stack, tmp_2.terminal, tmp_2.type, tmp_2.data_type, tmp_2.token_representation );

        }

    }

}

void psa_reduce_pop_and_update(PsaStack *stack, PsaItemType *top_item_type){

    // Pop the item from stack
    psa_stack_pop( stack );

    // Update the top item's content
    *top_item_type = stack->content[ stack->top_index ].type;

}

void psa_reduce(PsaStack *stack){

    // Get the top item's type
    PsaItemType top_item_type = stack->content[ stack->top_index ].type;

    // <id
    if( top_item_type == I_ID ){

        // Save the id's ( top item's ) data_type and token
        DataType tmp_data_type = stack->content[ stack->top_index ].data_type;
        Token tmp_token = stack->content[ stack->top_index ].token_representation;

        // Pop and update top_item_type
        psa_reduce_pop_and_update( stack, &top_item_type );

        // If expresion is bordered by < ("HANDLE" mark)
        if( top_item_type == I_HANDLE ){

            // Change the < item to EXPR item
            stack->content[ stack->top_index ].terminal = false;
            stack->content[ stack->top_index ].type = I_EXPR;
            stack->content[ stack->top_index ].data_type = tmp_data_type;
            stack->content[ stack->top_index ].token_representation = tmp_token;

        }else{

            // TODO error

        }

    }else if( top_item_type == I_NUMBER || top_item_type == I_INTEGER ){

        // Save the id's ( top item's ) data_type and token
        DataType tmp_data_type = stack->content[ stack->top_index ].data_type;
        Token tmp_token = stack->content[ stack->top_index ].token_representation;

        // Pop and update top_item_type
        psa_reduce_pop_and_update( stack, &top_item_type );

        // If expresion is bordered by < ("HANDLE" mark)
        if( top_item_type == I_HANDLE ){

            // Change the < item to EXPR item
            stack->content[ stack->top_index ].terminal = false;
            stack->content[ stack->top_index ].type = I_EXPR;
            stack->content[ stack->top_index ].data_type = tmp_data_type;
            stack->content[ stack->top_index ].token_representation = tmp_token;

        }else{

            // TODO error

        }

    }else if( top_item_type == I_EXPR ){

        // Save the second expression's type
        // ( it's the second expression in the expression but first expression on stack )
        PsaItemType expr_2_data_type = stack->content[ stack->top_index ].data_type;

        psa_reduce_pop_and_update( stack, &top_item_type );

        if( top_item_type == I_PLUS || top_item_type == I_MINUS || top_item_type == I_MUL || top_item_type == I_DIV ){

            psa_reduce_pop_and_update( stack, &top_item_type );

            if( top_item_type == I_EXPR ){

                // Save the first expression's type
                // ( it's the first expression in the expression but second expression on stack )
                PsaItemType expr_1_data_type = stack->content[ stack->top_index ].data_type;

                if( ( expr_1_data_type == NUM || expr_1_data_type == INT ) && ( expr_2_data_type == NUM || expr_2_data_type == INT ) ){

                    psa_reduce_pop_and_update( stack, &top_item_type );

                    // If expresion is bordered by < ("HANDLE" mark)
                    if( top_item_type == I_HANDLE ){

                        // Change the < item to EXPR item
                        stack->content[ stack->top_index ].terminal = false;
                        stack->content[ stack->top_index ].type = I_EXPR;

                        if( expr_1_data_type == NUM || expr_2_data_type == NUM ){

                            stack->content[ stack->top_index ].data_type = NUM;

                        }else{

                            stack->content[ stack->top_index ].data_type = INT;

                        }

                        stack->content[ stack->top_index ].token_representation = empty_token;

                    }else{

                        // TODO error

                    }

                }else{

                    // TODO error
                    fprintf(stderr, "ERROR SEMANTIKA 1\n", );

                }

            }else{

                // TODO error
                fprintf(stderr, "ERROR SYNTAX 1\n");

            }

        }else if( top_item_type == I_FLOOR_DIV ){

            // Check the first operand

            psa_reduce_pop_and_update( stack, &top_item_type );

            if( top_item_type != I_EXPR ){

                // TODO error
                fprintf(stderr, "ERROR SYNTAX 2\n");

            }

        }else if( top_item_type == I_CONCAT ){

            // TODO
            // Operandy musia byt string

        }else{

            // TODO error
            fprintf(stderr, "ERROR SYNTAX 3\n");

        }

    }else if( top_item_type == I_RIGHT_PAR ){

        psa_reduce_pop_and_update( stack, &top_item_type );

        if( top_item_type == I_EXPR ){

            psa_reduce_pop_and_update( stack, &top_item_type );

            if( top_item_type != I_LEFT_PAR ){

                // TODO error
                fprintf(stderr, "ERROR SYNTAX 4\n");

            }

        }else{

            // TODO error
            fprintf(stderr, "ERROR SYNTAX 5\n");

        }

    }else if( top_item_type == I_HASHTAG ){



    }else{

        fprintf(stderr, "ERROR - %s\n", PsaItemTypeStrings[top_item_type]);

        // TODO error
        fprintf(stderr, "ERROR SYNTAX 6\n");

    }

    psa_reduce_pop_and_update( stack, &top_item_type );

    // If expresion is bordered by < ("HANDLE" mark)
    if( top_item_type == I_HANDLE ){

        // Change the < item with E
        stack->content[ stack->top_index ].terminal = false;
        stack->content[ stack->top_index ].type = I_EXPR;
        stack->content[ stack->top_index ].data_type = NOTHING;
        // stack->content[ stack->top_index ].token_representation = "";

    }else{

        // TODO error

    }

}

bool psa(ParserData *parser_data ){

    // NOTE dynamicka kontrola delenia nulou

    // INITIALIZE ENVIRONMENT

    // Create a stack for PSA
    PsaStack *stack = psa_stack_create();

    // Vlož "$" na zásobník
    psa_stack_push( stack, true, I_DOLLAR, NIL, empty_token );

    // Create table for the psa
    char table[TABLE_SIZE][TABLE_SIZE] = {

        {'?','>','>','>','>','>','>','<','E','E','<','<','>','>'},
        {'<','>','>','>','>','>','>','<','<','<','E','<','>','>'},
        {'<','>','>','>','>','>','>','<','<','<','E','<','>','>'},
        {'<','>','>','>','>','>','>','<','<','E','E','<','>','>'},
        {'<','<','<','<','>','>','>','<','<','<','E','<','>','>'},
        {'<','<','<','<','>','>','>','<','<','<','E','<','>','>'},
        {'<','<','<','<','<','<','<','<','E','E','<','<','>','>'},
        {'E','>','>','>','>','>','>','E','E','E','E','E','>','>'},
        {'E','>','>','>','>','>','E','E','E','E','E','E','>','>'},
        {'E','>','>','E','>','>','E','E','E','E','E','E','>','>'},
        {'E','E','E','E','E','E','>','E','E','E','E','E','>','>'},
        {'<','<','<','<','<','<','<','<','<','<','<','<','=','E'},
        {'E','>','>','>','>','>','>','E','E','E','E','E','>','>'},
        {'<','<','<','<','<','<','<','<','<','<','<','<','E','E'}

    };

    // Get the highest terminal on stack
    PsaStackItem *top_terminal = psa_stack_top_terminal( stack );

    // Convert entry token to psa stack's item
    PsaStackItem entry_item = token_to_psa_stack_item( parser_data->token, &parser_data->stack );

    do {

        // REMOVE
        // fprintf(stderr, "TOP_TERMINAL = %s ENTRY_TOKEN = %s\n", top_terminal->item_content, entry_item.item_content);
        // fprintf(stderr, "TOP_TERMINAL_TABLE_INDEX = %i ENTRY_TOKEN_TABLE_INDEX = %i\n", top_terminal->table_index, entry_item.table_index);

        print_psa_stack( stack );


        if( table[top_terminal->type][entry_item.type] == '>' ){

            // REDUCTION
            psa_reduce( stack );

        }else if( table[top_terminal->type][entry_item.type] == '<' ){

            // SHIFT REDUCTION
            psa_modify_top_terminal( stack );
            psa_push_and_load( stack, &entry_item, parser_data->token, &parser_data->stack );

        }else if( table[top_terminal->type][entry_item.type] == '=' ){

            // PUSH and NEW TOKEN
            psa_push_and_load( stack, &entry_item, parser_data->token, &parser_data->stack );

        }else if( table[top_terminal->type][entry_item.type] == 'E' ){

            // TODO error
            // psa_error();
            fprintf(stderr, "ERROR 1\n");
            return false;

        }else{

            // TODO psa program error
            // psa_program_error();
            fprintf(stderr, "ERROR 2\n");
            return false;

        }

        // Update top terminal
        top_terminal = psa_stack_top_terminal( stack );

    } while ( entry_item.type != I_DOLLAR || top_terminal->type != I_DOLLAR );

    print_psa_stack( stack );

    return true;

}

// REMOVE
int main(){

    // Malloc place for ParserData
    ParserData *parser_data = malloc( sizeof( ParserData ) );

    // Malloc place for token
    Token *token = malloc( sizeof( Token ) );

    // Check malloc return
    if( token == NULL || parser_data == NULL ){
        printf("Error token malloc.\n");
        return 0;
    }

    parser_data->token = token;

    // Load token
    get_token( token );

    if( psa(parser_data) == true ){
        printf("WORKS!\n");
    }else{
        printf("MISTAKE!\n");
    }

    // printf("%s\n", TokenTypeStrings[token->type]);

}