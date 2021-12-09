/**
 * @file generator.h
 * @brief Interface of generator.c
 * @author Denis Adásek Martin Kozák Adam Juliš
 *
 * Project: IFJ compiler
 * Date: 24.11.2021
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include "parser.h"

typedef enum {
    T_LF,
    T_GF,
    T_TF
} FrameType;

/**
* @brief Generate main and header
*
*/
void start();

/**
* @brief Print generated code
*
*/
void print_code();

/**
* @brief Generate datatype or frame
 *
 * @param token The token address
 * @param frame frame
*
*/
char *get_frame_term_str(Token *token, FrameType frame);

/**
* @brief Generate main
 *
 * @return True main was generated / false otherwise
*
*/
bool gen_main();

/**
* @brief Generate header
 *
 * @return True header was generated / false otherwise
*
*/
bool gen_header();

/**
 * @brief Read string
 *
 * @return True function was generated / false otherwise
 *
*/
bool gen_reads();

/**
 * @brief Read integer
 *
 * @return True function was generated / false otherwise
 *
*/
bool gen_readi();

/**
 * @brief Read number
 *
 * @return True function was generated / false otherwise
 *
*/
bool gen_readn();

/**
 * @brief Write
 *
 * @param p_data The token address
 * @param top_index Last identifier in order
 *
 * @return True function was generated / false otherwise
 *
*/
bool gen_write(ParserData *p_data, int topindex);

/**
 * @brief Retype float to integer
 *
 * @return True function was generated / false otherwise
*/
bool gen_tointeger();

/**
 * @brief ASCII value of character
 *
 * @return True function was generated / false otherwise
*/
bool gen_ord();

/**
 * @brief Character from ASCII value
 *
 * @return True function was generated / false otherwise
*/
bool gen_chr();

/**
 * @brief Push parameter on stack
 *
 * @param token The address of token
 * @param index Which one in order
 * @param str string 
 * @return True function was generated / false otherwise
*/
bool gen_push_E(Token token, int index, char **str);

/**
 * @brief Clear data stack
 *
 * @return True function was generated / false otherwise
*/
bool gen_clears();

/**
 * @brief Retype undefined operator
 *
 * @return True function was generated / false otherwise
*/
bool gen_op_retype(int i);

/**
 * @brief Zero check
 *
 * @return True function was generated / false otherwise
*/
bool gen_zero_check();

/**
 * @brief Addition of two values
 *
 * @return True function was generated / false otherwise
*/
bool gen_op_adds(int i);

/**
 * @brief Subtraction of two values
 *
 * @return True function was generated / false otherwise
*/
bool gen_op_subs(int i);

/**
 * @brief Multiplication of two values
 *
 * @return True function was generated / false otherwise
*/
bool gen_op_muls(int i);

/**
 * @brief Division of two values
 *
 * @return True function was generated / false otherwise
*/
bool gen_op_divs(int i);

/**
 * @brief Integer subtraction of two values
 *
 * @return True function was generated / false otherwise
*/
bool gen_op_idivs(int i);

/**
 * @brief Concatenation of two strings
 *
 * @return True function was generated / false otherwise
*/
bool gen_op_concat();


/**
 * @brief Number of characters
 *
 * @return True function was generated / false otherwise
*/
bool gen_op_hashtag();

/**
 * @brief Generate nil
 *
 * @return True function was generated / false otherwise
*/
bool gen_nil_nil();

/**
 * @brief Generate new frame
 *
 * @param global True if function is global
 * @return True function was generated / false otherwise
*/
bool gen_frame_global(bool global);

/**
 * @brief Insert value to return
 *
 * @param id Variable id
 * @param topindex How deep the variable is
 * @param index Which return in order
 * @return True function was generated / false otherwise
*/
bool gen_move_ret_to_val(char *id, int topindex, int index);


/**
 * @brief Generate label for function
 *
 * @param label_id Function name
 * @return True function was generated / false otherwise
*/
bool gen_fun_label(char *label_id);

/**
 * @brief Transfer local frame to temporary
 *
 * @param global True if function is global
 * @return True function was generated / false otherwise
*/
bool gen_popframe();

/**
 * @brief definition and declaration of parameters
 *
 * @param token The address of token
 * @param index Which param in order
 * @param global Is function global
 *
 * @return true function was generated / false otherwise
*/
bool gen_fun_par_to_be_sent(Token *token, int index, bool global);

/**
 * @brief definition and declaration of parameters
 *
 * @param fun_id The address of function name
 * @param global Is function global
 *
 * @return True function was generated / false otherwise
*/
bool gen_fun_call(char *fun_id, bool global);

/**
 * @brief generate function label and transfer temporary frame to local
 *
 * @param fun_id The address of function name
 * @param global Is function global
 *
 * @return True function was generated / false otherwise
*/
bool gen_fun_label(char *id);

/**
 * @brief generate function parameters
 *
 * @param id which parameter in order
 * @param index The address of function name
 * @param topindex Is function global
 *
 * @return True function was generated / false otherwise
*/
bool gen_fun_par(char *id, int index, int topindex);

/**
 * @brief Move temporary frame to local and return
 *
 * @return True function was generated / false otherwise
*/
bool gen_return();

/**
 * @brief Move variable value to return
 *
 * @return True function was generated / false otherwise
*/
bool gen_return_var(int index);

/**
 * @brief Move temporary frame to local and return
 *
 * @return True function was generated / false otherwise
*/
bool gen_move_to_return(int index);

/**
 * @brief Generate variable
 *
 * @param id Variable name
 * @param top_index Which variable in order
 *
 * @return True function was generated / false otherwise
*/
bool gen_var(char *id, int top_index);

/**
 * @brief move stack value to variable
 *
 * @param id Variable name
 * @param top_index Which variable in order
 *
 * @return True function was generated / false otherwise
*/
bool gen_pop_var(char *id, int top_index);

/**
 * @brief Check if label is else or end
 *
 * @param id Variable name
 * @param top_index Which variable in order
 *
 * @return True function was generated / false otherwise
*/
bool gen_if_label_name(bool else_end, int top_index);

/**
 * @brief Generate if label
 *
 * @param top_index Which variable in order
 * @param token The token address
 *
 * @return True function was generated / false otherwise
*/
bool gen_if(int top_index, Token token);
/**
 * @brief Generate else label
 *
 * @param top_index Which variable in order
 *
 * @return True function was generated / false otherwise
*/

bool gen_else(int top_index);

/**
 * @brief Generate end of if label
 *
 * @param top_index Which variable in order
 *
 * @return True function was generated / false otherwise
*/
bool gen_if_end(int top_index);

/**
 * @brief Generate end of if label
 *
 * @param start_end while start identifier
 * @param top_index deep controller
 *
 * @return True function was generated / false otherwise
*/
bool gen_while_label_name(bool start_end, int top_index);

/**
 * @brief Generate while label
 *
 * @param top_index Which variable in order
 * @param token The token address
 *
 * @return True function was generated / false otherwise
*/
bool gen_while(int top_index, Token token);

/**
 * @brief Generate end of while label
 *
 * @param top_index Which variable in order
 *
 * @return True function was generated / false otherwise
*/
bool gen_while_end(int top_index);

/**
 * @brief Jump on while end
 *
 * @param top_index Which variable in order
 *
 * @return True function was generated / false otherwise
*/
bool gen_if_jump_end(int top_index);

#endif
