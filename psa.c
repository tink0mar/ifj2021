/**
 * @file psa.c
 * @brief Operator Precedence Syntax Analysis
 * @author Hugo Hežel
 *
 * Project: IFJ compiler
 * Date: 1.12.2021
 */

#include "psa.h"
#include "generator.h"
#include <stdio.h>
#include "dll.h"
#include <string.h>

#define UNGET_TOKEN_PSA \
    dll_set_active_previous(&parser_data->dll_list);\
    dll_return_token(&parser_data->dll_list, parser_data->token );\

char *PsaItemTypeStrings[] = {
        "I_HASHTAG",
        "I_MUL",
        "I_DIV",
        "I_FLOOR_DIV",
        "I_PLUS",
        "I_MINUS",
        "I_CONCAT",
        "I_ID",
        "I_INTEGER",
        "I_NUMBER",
        "I_STRING",
        "I_LEFT_PAR",
        "I_RIGHT_PAR",
        "I_DOLLAR",
        "I_EXPR",
        "I_HANDLE",
        "I_NULL"
    };

void print_psa_stack(PsaStack *stack){

    if( !psa_stack_is_empty( stack ) ){

        for( int i = 0; i < stack->size; i++ ){
            fprintf(stderr, "stack[%i] = %s\n", i, PsaItemTypeStrings[ stack->content[i].type] );
        }

        fprintf(stderr, "\n");

    }

}

// Create an empty token that will be given to items without token
Token empty_token;

Token *save_token_into_item(ParserData *parser_data){

    Token *token_representation = malloc( sizeof( Token ) );

    if( token_representation == NULL ){

        return NULL;        

    }else{

        char *ptr;

        if (token_representation->attribs.string == NULL) {

            fprintf(stderr, "Token string: %s\n", parser_data->token->attribs.string);

            ptr = copy_str(parser_data->token->attribs.string);
            token_representation->attribs.string = ptr;

            fprintf(stderr, "Item string: %s\n", token_representation->attribs.string);

        }
        
        token_representation->type = parser_data->token->type;       
        token_representation->attribs.number = parser_data->token->attribs.number;
        token_representation->attribs.integer = parser_data->token->attribs.integer;

        return token_representation;

    }

}

PsaStackItem token_to_psa_stack_item(Token *token, ParserData *parser_data){
    PsaStackItem item;

    // Declare node value that will be used in switch's case - TT_IDENTIFIER
    TreeNode *node;

    switch ( parser_data->token->type ) {
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
            
            //fprintf(stderr, "ID ENTRY TOKEN %d\n", parser_data->token->type);

            // Load the next token
            // Copied GET_TOKEN macro because of the return inside it
            if( dll_is_active_last( &parser_data->dll_list ) == true ){
                
                if( parser_data->get_token == true ){
                    
                    get_token( parser_data->token );
                    dll_insert( &parser_data->dll_list, parser_data->token);
                    dll_return_token( &parser_data->dll_list, parser_data->token );
                     parser_data->get_token = false;
                    if( num_error != 0 ){

                        // return undefined item
                        // MUST CHECK AFTER token_to_psa_stack_item() CALL WHETHER num_error != 0
                        return item;

                    }

                }else{
                    parser_data->get_token = true;
                }

            }else{
                dll_set_active_next( &parser_data->dll_list );
                dll_return_token( &parser_data->dll_list, parser_data->token );
            }

            if( dll_is_active_last( &parser_data->dll_list ) == true ){
                
                if( parser_data->get_token == true ){
                    
                    get_token( parser_data->token );
                    dll_insert( &parser_data->dll_list, parser_data->token);
                    dll_return_token( &parser_data->dll_list, parser_data->token );
                     parser_data->get_token = false;
                    if( num_error != 0 ){

                        // return undefined item
                        // MUST CHECK AFTER token_to_psa_stack_item() CALL WHETHER num_error != 0
                        return item;

                    }

                }else{
                    parser_data->get_token = true;
                }

            }else{
                dll_set_active_next( &parser_data->dll_list );
                dll_return_token( &parser_data->dll_list, parser_data->token );
            }           

            //fprintf(stderr, "ID ENTRY->NEXT TOKEN %d\n", parser_data->token->type);

            //if( dll_is_active_last( &parser_data->dll_list) ){fprintf(stderr,"LAST = ACTIVE\n");}
            //fprintf(stderr, "LAST ITEM - %d\n", parser_data->dll_list.lastElement->token->type);
            //fprintf(stderr, "ACTIVE ITEM - %d\n", parser_data->dll_list.activeElement->token->type);
            //print_list(&parser_data->dll_list);

            // Check whether the next token is "=" or ","
            if( parser_data->token->type == TT_ASSIGN || parser_data->token->type == TT_COMMA || parser_data->token->type == TT_LEFT_PAR ){

                // Unget the next token
                UNGET_TOKEN_PSA
                UNGET_TOKEN_PSA
                
                //fprintf(stderr, "ID ENTRY->UNGETN TOKEN %d\n", parser_data->token->type);

                // Set the current ID token as dollar - expression ending item
                item.data_type = NOTHING;
                item.type = I_DOLLAR;
                

                break;

            }else{

                // Unget the next token
                UNGET_TOKEN_PSA

                // Search in symtable for id's type
                node = bst_search_in_stack( &parser_data->stack, parser_data->token->attribs.string );
                fprintf(stderr, "@_ %d @_", token->type);
                print_list(&parser_data->dll_list);
                if( node == NULL ){
                    fprintf(stderr, "@ kokot @"); //parser_data->token->type)
                    // Node wasn't found in symtable => undefined identifier
                    set_error( SEM_UNDEFINED_ERR );
                    return item;

                }else{
                    // why there was INT
                    item.data_type = node->id;
                    item.type = I_ID;
                    break;

                }

            }

        default:
            
            item.data_type = NOTHING;
            item.type = I_DOLLAR;
            parser_data->get_token = false;
            break;
    }

    item.terminal = true;
    item.token_representation = save_token_into_item( parser_data );

    fprintf(stderr, "Item string: %s\n", item.token_representation->attribs.string);
    
    
    /**
    item.token_representation.type = parser_data->token->type;
    item.token_representation.attribs.integer = parser_data->token->attribs.integer;
    item.token_representation.attribs.number = parser_data->token->attribs.number;
    ptr = copy_str(parser_data->token->attribs.string);
    fprintf( stderr, "_%s_", ptr);
    item.token_representation.attribs.string = ptr;
    */
    // fprintf(stderr, "RETURNING ITEM %d\n", item.type);

    return item;

}

bool psa_push_and_load(PsaStack *stack, PsaStackItem *entry_item, Token *entry_token, ParserData *parser_data){

    // Push the item to psa stack
    psa_stack_push( stack, entry_item->terminal, entry_item->type, entry_item->data_type, entry_item->token_representation );
    
    // Get another token
    // Load the next token
    // Copied GET_TOKEN macro because of the return inside it
    if( dll_is_active_last( &parser_data->dll_list ) == true ){

        if( parser_data->get_token == true ){

            get_token( parser_data->token );
            dll_insert( &parser_data->dll_list,  parser_data->token);

            if( num_error != 0 ){

                // return undefined item
                // MUST CHECK AFTER token_to_psa_stack_item() CALL WHETHER num_error != 0
                return false;

            }

        }else{
            parser_data->get_token = true;
        }

    }else{
        dll_set_active_next( &parser_data->dll_list );
        dll_return_token( &parser_data->dll_list, parser_data->token );
    }

   // fprintf(stderr, "\n ENTERING CONVERT FUNCTION WITH TokenType = %i\n", parser_data->token->type);

    // Convert new token to psa stack's item
   
    
    *entry_item = token_to_psa_stack_item( parser_data->token, parser_data );
     
    
    // If there is an error
    if( num_error != 0 ){
        return false;
    }

    return true;

};

void psa_modify_top_terminal(PsaStack *stack){

    // Variable item is used for the while cycle that starts with top terminal
    PsaStackItem *item = psa_stack_top_terminal( stack );

    // Variables for temporary saving items from stack
    PsaStackItem tmp_1, tmp_2;

    // If top terminal is also top item on stack
    if( item == psa_stack_top( stack ) ){
        
        // Just push the handle item
        psa_stack_push( stack, false, I_HANDLE, NOTHING, NULL);
        
    }else {

        // Save data of (top terminal + 1)
        tmp_1 = item[1];
    
        // Insert new "<" noterminal after top terminal.
        item[1].terminal = false;
        item[1].type = I_HANDLE;
        item[1].data_type = NOTHING;
        item[1].token_representation = NULL;

        

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

bool psa_reduce(PsaStack *stack, ParserData *parser_data){
    
    // Get the top item's type
    PsaItemType top_item_type = stack->content[ stack->top_index ].type;

    // <id
    if( top_item_type == I_ID ){
        
        // Save the id's ( top item's ) data_type and token
        DataType tmp_data_type = stack->content[ stack->top_index ].data_type;
        Token *tmp_token = stack->content[ stack->top_index ].token_representation;
        
        // Pop and update top_item_type
        psa_reduce_pop_and_update( stack, &top_item_type );

        // If expresion is bordered by < ("HANDLE" mark)
        if( top_item_type == I_HANDLE ){

            // Change the < item to EXPR item
            stack->content[ stack->top_index ].terminal = false;
            stack->content[ stack->top_index ].type = I_EXPR;
            stack->content[ stack->top_index ].data_type = tmp_data_type;
            stack->content[ stack->top_index ].token_representation = tmp_token;

            int index = bst_search_in_stack_gen( &(parser_data->stack), tmp_token->attribs.string );

            // Generate output code
            
            gen_push_E( *tmp_token, index, &tmp_token->attribs.string);

            return true;

        }else{

            set_error( INTERNAL_ERR );
            return false;

        }

    }else if( top_item_type == I_NUMBER || top_item_type == I_INTEGER || top_item_type == I_STRING ){

        // Save the id's ( top item's ) data_type and token
        DataType tmp_data_type = stack->content[ stack->top_index ].data_type;
        Token *tmp_token = stack->content[ stack->top_index ].token_representation;

        // Pop and update top_item_type
        psa_reduce_pop_and_update( stack, &top_item_type );

        // If expresion is bordered by < ("HANDLE" mark)
        if( top_item_type == I_HANDLE ){

            // Change the < item to EXPR item
            stack->content[ stack->top_index ].terminal = false;
            stack->content[ stack->top_index ].type = I_EXPR;
            stack->content[ stack->top_index ].data_type = tmp_data_type;
            stack->content[ stack->top_index ].token_representation = tmp_token;

            // Generate output code

            fprintf(stderr, "STRING: %s\n",tmp_token->attribs.string);

            gen_push_E( *tmp_token, 0, &tmp_token->attribs.string);

            return true;

        }else{

            set_error( INTERNAL_ERR );
            return false;

        }

    }else if( top_item_type == I_EXPR ){

        // Save the second expression's type
        // ( it's the second expression in the expression but first expression on stack )
        PsaItemType expr_2_data_type = stack->content[ stack->top_index ].data_type;

        psa_reduce_pop_and_update( stack, &top_item_type );

        if( top_item_type == I_PLUS || top_item_type == I_MINUS || top_item_type == I_MUL ){

            // Save the operand type
            PsaItemType operand_type = top_item_type;

            psa_reduce_pop_and_update( stack, &top_item_type );

            if( top_item_type == I_EXPR ){

                // Save the first expression's type
                // ( it's the first expression in the expression but second expression on stack )
                PsaItemType expr_1_data_type = stack->content[ stack->top_index ].data_type;

                if( ( expr_1_data_type == NUM || expr_1_data_type == INT ) && ( expr_2_data_type == NUM || expr_2_data_type == INT ) ){

                    psa_reduce_pop_and_update( stack, &top_item_type );

                    // If expresion is bordered by < ("HANDLE" mark)
                    if( top_item_type == I_HANDLE ){

                        // Change the < item to EXPR item
                        stack->content[ stack->top_index ].terminal = false;
                        stack->content[ stack->top_index ].type = I_EXPR;

                        if( expr_1_data_type == INT && expr_2_data_type == INT ){

                            stack->content[ stack->top_index ].data_type = INT;

                            if( operand_type == I_PLUS ){

                               

                                // Generate output code - don't change type of the expressions
                                gen_op_adds( 0 );

                            }else if( operand_type == I_MINUS ){

                                // Generate output code - don't change type of the expressions
                                gen_op_subs( 0 );

                            }else if( operand_type == I_MUL ){

                                // Generate output code - don't change type of the expressions
                                gen_op_muls( 0 );

                            }else{

                                // If SOMEHOW is the type of the operant different
                                set_error( INTERNAL_ERR );
                                return false;

                            }

                        }else{

                            stack->content[ stack->top_index ].data_type = NUM;

                            if( expr_1_data_type == INT && expr_2_data_type == NUM ){
                                
                                 
                                if( operand_type == I_PLUS ){

                                   
                                    // Generate output code - change type of the first expression
                                    gen_op_adds( 1 );

                                }else if( operand_type == I_MINUS ){

                                    // Generate output code - change type of the first expression
                                    gen_op_subs( 1 );

                                }else if( operand_type == I_MUL ){

                                    // Generate output code - change type of the first expression
                                    gen_op_muls( 1 );

                                }else{

                                    // If SOMEHOW is the type of the operant different
                                    set_error( INTERNAL_ERR );
                                    return false;

                                }

                            }

                            if( expr_1_data_type == NUM && expr_2_data_type == INT ){

                                if( operand_type == I_PLUS ){

                                    // Generate output code - change type of the second expression
                                    gen_op_adds( 2 );

                                }else if( operand_type == I_MINUS ){

                                    // Generate output code - change type of the second expression
                                    gen_op_subs( 2 );

                                }else if( operand_type == I_MUL ){

                                    // Generate output code - change type of the second expression
                                    gen_op_muls( 2 );

                                }else{

                                    // If SOMEHOW is the type of the operant different
                                    set_error( INTERNAL_ERR );
                                    return false;

                                }

                            }
                            
                            // If both expressions types are NUM
                            if( expr_1_data_type == NUM && expr_2_data_type == NUM ){

                                if( operand_type == I_PLUS ){

                                     
                                    // Generate output code - don't change type of the expressions
                                    gen_op_adds( 0 );

                                }else if( operand_type == I_MINUS ){

                                    // Generate output code - don't change type of the expressions
                                    gen_op_subs( 0 );

                                }else if( operand_type == I_MUL ){

                                    // Generate output code - don't change type of the expressions
                                    gen_op_muls( 0 );

                                }else{

                                    // If SOMEHOW is the type of the operant different
                                    set_error( INTERNAL_ERR );
                                    return false;

                                }

                            }


                        }

                        stack->content[ stack->top_index ].token_representation = NULL;

                        return true;

                    }else{

                        set_error( INTERNAL_ERR );
                        return false;

                    }

                }else{

                    set_error( SEM_T_UNCOM_EXPRESS_ERR );
                    return false;

                }

            }else{

                set_error( SYNTACTIC_ERR );
                return false;

            }

        }else if( top_item_type == I_DIV ){

            psa_reduce_pop_and_update( stack, &top_item_type );

            if( top_item_type == I_EXPR ){

                DataType expr_1_data_type = stack->content[ stack->top_index ].data_type;

                psa_reduce_pop_and_update( stack, &top_item_type );

                if( top_item_type == I_HANDLE ){

                    // Change the < item to EXPR item
                    stack->content[ stack->top_index ].terminal = false;
                    stack->content[ stack->top_index ].type = I_EXPR;
                    stack->content[ stack->top_index ].data_type = NUM;
                    stack->content[ stack->top_index ].token_representation = NULL;

                    if( expr_1_data_type == INT && expr_2_data_type == INT ){

                        // Generate output code
                        gen_op_divs( 3 );

                    }else if( expr_1_data_type == INT ){

                        // Generate output code
                        gen_op_divs( 1 );

                    }else if( expr_2_data_type == INT ){

                        // Generate output code
                        gen_op_divs( 2 );

                    }else {

                        // Generate output code
                        gen_op_divs( 0 );

                    }

                    return true;

                }else{

                    set_error( SYNTACTIC_ERR );
                    return false;

                }

            }else{

                set_error( SYNTACTIC_ERR );
                return false;

            }

        }else if( top_item_type == I_FLOOR_DIV ){

            if( expr_2_data_type == INT ){

                psa_reduce_pop_and_update( stack, &top_item_type );

                if( top_item_type == I_EXPR ){

                    DataType expr_1_data_type = stack->content[ stack->top_index ].data_type;

                    if( expr_1_data_type == INT ){

                        psa_reduce_pop_and_update( stack, &top_item_type );

                        if( top_item_type == I_HANDLE ){

                            // Change the < item to EXPR item
                            stack->content[ stack->top_index ].terminal = false;
                            stack->content[ stack->top_index ].type = I_EXPR;
                            stack->content[ stack->top_index ].data_type = NUM;
                            stack->content[ stack->top_index ].token_representation = NULL;

                            // Generate output code
                            gen_op_idivs( 3 );

                            return true;

                        }else{

                            set_error( SYNTACTIC_ERR );
                            return false;

                        }

                    }else{

                        // e // E
                        // E is not integer
                        set_error( SEM_T_UNCOM_EXPRESS_ERR );
                        return false;

                    }

                }else{

                    set_error( SYNTACTIC_ERR );
                    return false;

                }

            }else{

                // E // e
                // E is not integer
                set_error( SEM_T_UNCOM_EXPRESS_ERR );
                return false;

            }

        }else if( top_item_type == I_CONCAT ){

            if( expr_2_data_type == STR ){

                psa_reduce_pop_and_update( stack, &top_item_type);

                if( top_item_type == I_EXPR ){

                    // Save the first expression's type
                    // ( it's the first expression in the expression but second expression on stack )
                    PsaItemType expr_1_data_type = stack->content[ stack->top_index ].data_type;

                    // Check the first expression's data type ( second expression on stack )
                    if( expr_1_data_type == STR ){

                        psa_reduce_pop_and_update( stack, &top_item_type );

                        if( top_item_type == I_HANDLE ){

                            // Change the < item to EXPR item
                            stack->content[ stack->top_index ].terminal = false;
                            stack->content[ stack->top_index ].type = I_EXPR;
                            stack->content[ stack->top_index ].data_type = STR;
                            stack->content[ stack->top_index ].token_representation = NULL;

                            // Generate output code
                            gen_op_concat();

                            return true;

                        }else{

                            set_error( SYNTACTIC_ERR );
                            return false;

                        }

                    }else{

                        // E .. e
                        // E is not a string
                        set_error( SEM_T_UNCOM_EXPRESS_ERR );
                        return false;

                    }

                }else{

                    set_error( SYNTACTIC_ERR );
                    return false;

                }

            }else{

                // e .. E
                // E is not a string
                set_error( SEM_T_UNCOM_EXPRESS_ERR );
                return false;

            }

        }else if( top_item_type == I_HASHTAG ){

            if( expr_2_data_type == STR ){

                psa_reduce_pop_and_update( stack, &top_item_type );

                if( top_item_type == I_HANDLE ){

                    // Change the < item to EXPR item
                    stack->content[ stack->top_index ].terminal = false;
                    stack->content[ stack->top_index ].type = I_EXPR;
                    stack->content[ stack->top_index ].data_type = INT;
                    stack->content[ stack->top_index ].token_representation = NULL;

                    // Generate output code
                    gen_op_hashtag();

                    return true;

                }else{

                    set_error( SYNTACTIC_ERR );
                    return false;

                }

            }else{

                // # E
                // E is not a string
                set_error( SEM_T_UNCOM_EXPRESS_ERR );
                return false;

            }

        }else{

            set_error( SYNTACTIC_ERR );
            return false;

        }

    }else if( top_item_type == I_RIGHT_PAR ){

        psa_reduce_pop_and_update( stack, &top_item_type );

        if( top_item_type == I_EXPR ){

            // Save data_type of the expression
            DataType tmp_data_type = stack->content[ stack->top_index ].data_type;

            psa_reduce_pop_and_update( stack, &top_item_type );

            if( top_item_type == I_LEFT_PAR ){

                psa_reduce_pop_and_update( stack, &top_item_type );

                // If expresion is bordered by < ("HANDLE" mark)
                if( top_item_type == I_HANDLE ){

                    // Change the < item with E
                    stack->content[ stack->top_index ].terminal = false;
                    stack->content[ stack->top_index ].type = I_EXPR;
                    stack->content[ stack->top_index ].data_type = tmp_data_type;
                    stack->content[ stack->top_index ].token_representation = NULL;

                    return true;

                }else{

                    set_error( INTERNAL_ERR );
                    return false;

                }

            }else{

                set_error( SYNTACTIC_ERR );
                return false;

            }

        }else{

            set_error( SYNTACTIC_ERR );
            return false;

        }

    }else{

        set_error( INTERNAL_ERR );
        return false;

    }

}

DataType psa(ParserData *parser_data){

    // INITIALIZE ENVIRONMENT

    // Create a stack for PSA
    PsaStack *stack = psa_stack_create();

    // Vlož "$" na zásobník
    psa_stack_push( stack, true, I_DOLLAR, NIL, NULL );

    // Create table for the psa
    char table[TABLE_SIZE][TABLE_SIZE] = {

        {'?','>','>','>','>','>','>','<','<','<','<','<','>','>'},
        {'<','>','>','>','>','>','>','<','<','<','<','<','>','>'},
        {'<','>','>','>','>','>','>','<','<','<','<','<','>','>'},
        {'<','>','>','>','>','>','>','<','<','<','<','<','>','>'},
        {'<','<','<','<','>','>','>','<','<','<','<','<','>','>'},
        {'<','<','<','<','>','>','>','<','<','<','<','<','>','>'},
        {'<','<','<','<','<','<','<','<','<','<','<','<','>','>'},
        {'E','>','>','>','>','>','>','E','E','E','E','E','>','>'},
        {'E','>','>','>','>','>','>','E','E','E','E','E','>','>'},
        {'E','>','>','>','>','>','>','E','E','E','E','E','>','>'},
        {'E','>','>','>','>','>','>','E','E','E','E','E','>','>'},
        {'<','<','<','<','<','<','<','<','<','<','<','<','=','E'},
        {'E','>','>','>','>','>','>','E','E','E','E','E','>','>'},
        {'<','<','<','<','<','<','<','<','<','<','<','<','E','E'}

    };

    // Get the highest terminal on stack
    PsaStackItem *top_terminal = psa_stack_top_terminal( stack );

    // Convert entry token to psa stack's item
    PsaStackItem entry_item = token_to_psa_stack_item( parser_data->token, parser_data );

    if( num_error != 0 ){

        return NOTHING;

    }

    do {

        //fprintf(stderr, "ITEM NA VSTUPU: %s\n", PsaItemTypeStrings[entry_item.type]);
        print_psa_stack( stack );

        if( table[top_terminal->type][entry_item.type] == '>' ){

            //fprintf(stderr, "REDUCE entry_item = %i\n", entry_item.type);

            // REDUCTION
            if( psa_reduce( stack, parser_data ) == false ){
                break;
            }

        }else if( table[top_terminal->type][entry_item.type] == '<' ){

            // SHIFT
            psa_modify_top_terminal( stack );

            if( psa_push_and_load( stack, &entry_item, parser_data->token, parser_data ) == false ){

                
                return NOTHING;

            }

        }else if( table[top_terminal->type][entry_item.type] == '=' ){

            // PUSH and NEW TOKEN
            if( psa_push_and_load( stack, &entry_item, parser_data->token, parser_data ) == false ){

                return NOTHING;

            }

        }else if( table[top_terminal->type][entry_item.type] == 'E' ){

            //fprintf(stderr, "TOP_TERMINAL %d VSTUP %d", top_terminal->type, entry_item.type);

            set_error( SYNTACTIC_ERR );
            break;

        }else{

            set_error( INTERNAL_ERR );
            break;

        }

        // Update top terminal
        top_terminal = psa_stack_top_terminal( stack );

    } while ( entry_item.type != I_DOLLAR || top_terminal->type != I_DOLLAR );

    print_psa_stack( stack );

    // If for some reason the stack is NULL
    if( stack == NULL ){

        set_error( INTERNAL_ERR );
        return NOTHING;

    }else{

        // If fore some reason the stack->content is NULL
        if( stack->content == NULL ){

            set_error( INTERNAL_ERR );
            return NOTHING;

        }

        // Return DataType of the final expression to parser.c
        return stack->content[ stack->top_index ].data_type;

    }

}

bool psa_condition(ParserData *parser_data, bool its_if){

    // E == e
    // Check the E expression
    DataType expr_1_data_type = psa(parser_data);

    // Check if there was any error in psa
    if( num_error != 0 ){

        return false;

    }

    
    
    
    Token condition_operator_token = *parser_data->token;

    // If token type is in range of condition operators
    // token.type >= TT_GREATER
    // token.type <= TT_EQ

    if( condition_operator_token.type >= TT_GREATER && condition_operator_token.type <= TT_EQ  ){

        // e == E
        // Check the E expression

        
        
        parser_data->get_token = true;

        if( dll_is_active_last( &parser_data->dll_list ) == true ){
            
            if( parser_data->get_token == true ){
                
                get_token( parser_data->token );

                if( num_error != 0 ){

                    // return undefined item
                    // MUST CHECK AFTER token_to_psa_stack_item() CALL WHETHER num_error != 0
                    return false;

                }

            }else{
                parser_data->get_token = true;
            }

        }else{
            dll_set_active_next( &parser_data->dll_list );
            dll_return_token( &parser_data->dll_list, parser_data->token );
        }

        
        
        DataType expr_2_data_type = psa(parser_data);
        

        // Check if there was any error in psa
        if( num_error != 0 ){

            return false;

        }

        // Semantic check
        if( ( expr_1_data_type == INT || expr_2_data_type == NUM ) && expr_2_data_type == STR ){

            set_error( SEM_T_UNCOM_EXPRESS_ERR );
            return false;

        }else if( ( expr_2_data_type == INT || expr_2_data_type == NUM ) && expr_1_data_type == STR ){

            set_error( SEM_T_UNCOM_EXPRESS_ERR );
            return false;

        }

        // If its condition for an IF statement
        if( its_if ){

            gen_if( parser_data->stack.topIndex, condition_operator_token );

        // If its condition for a while statement
        }else{

            gen_while( parser_data->stack.topIndex, condition_operator_token );

        }

        return true;

    }else{

        set_error( SYNTACTIC_ERR );
        return false;

    }

}

