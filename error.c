/**
 * @file error.c
 * @brief 
 * @author 
 * 
 * Project: IFJ compiler
 * Date: 24.11.2021
 */ 

#include "stdlib.h"
#include "stdio.h"


int error_handle(int num_err){

    if (num_err == 1) {
        fprintf(stderr, "Lexical error");
    }

 }
