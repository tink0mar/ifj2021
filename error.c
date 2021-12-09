/**
 * @file error.c
 * @brief 
 * @author Martin Kozák
 * 
 * Project: IFJ compiler
 * Date: 24.11.2021
 */ 

#include "stdlib.h"
#include "stdio.h"

int num_error = 0;

void error_mess(int num_err){

    switch(num_err){
        
        case 1:
            fprintf(stderr, "ERROR 1: Lexical error");
            break;
        case 2:
            fprintf(stderr, "ERROR 2: Syntax error");
            break;
        case 3:
            fprintf(stderr, "ERROR 3: Semantic error - Undefined identifier or variable redefinition");
            break;
        case 4:
            fprintf(stderr, "ERROR 4: Semantic error - Incorrect types");
            break;
        case 5:
            fprintf(stderr, "ERROR 5: Semantic error - Incorrect number/types of parameters/return values");
            break;
        case 6:
            fprintf(stderr, "ERROR 6:Semantic error - Incorrect compatibility of types in expression");
            break;
        case 7:
            fprintf(stderr, "ERROR 7: Semantic error -  others");
            break;
        case 8:
            fprintf(stderr, "ERROR 8: Run error - work with not excepted nil");
            break;
        case 9:
            fprintf(stderr, "ERROR 9: Division by zero");
            break;
        case 99:
            fprintf(stderr, "ERROR 99: Internal error of compiler");
    }

}

void set_error(int err){

    if (num_error == 0){
        num_error = err;
    }

}