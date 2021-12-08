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

void start();
void print_code();
char *get_frame_term_str(Token *token, FrameType frame);
bool gen_main();
bool gen_header();
bool gen_reads();
bool gen_readi();
bool gen_readn();
bool gen_write(Token *token);
bool gen_tointeger();
bool gen_ord();
bool gen_chr();
bool gen_push_E(Token token, int index);
bool gen_clears();
bool gen_op_retype(int i);
bool gen_zero_check();
bool gen_op_adds(int i);
bool gen_op_subs(int i);
bool gen_op_muls(int i);
bool gen_op_divs(int i);
bool gen_op_idivs(int i);
bool gen_op_concat();
bool gen_op_hashtag();
bool gen_nil_nil();
bool gen_frame_global(bool global);
bool gen_move_ret_to_val(char *id, int topindex, int index);
bool gen_fun_label(char *label_id);
bool gen_popframe();
bool gen_fun_par_to_be_sent(Token *token, int index, bool global);
bool gen_fun_call(char *fun_id, bool global);
bool gen_fun_label(char *id);
bool gen_fun_par(char *id, int index);
bool gen_return();
bool gen_return_var(int index);
bool gen_move_to_return(int index);
bool gen_move_val_to_ret(int index);
bool gen_var(char *id, int top_index);
bool gen_pop_var(char *id, int top_index);
bool gen_if_label_name(bool else_end, int top_index);
bool gen_if(int top_index, Token token);
bool gen_else(int top_index);
bool gen_if_end(int top_index);
bool gen_while_label_name(bool start_end, int top_index);
bool gen_while(int top_index, Token token);
bool gen_while_end(int top_index);
bool gen_if_jump_end(int top_index);

#endif