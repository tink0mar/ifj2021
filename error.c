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

int num_error = 0;

void error_mess(int num_err){

    switch(num_err){
        
        case 1:
            fprintf(stderr, "ERROR: 1");
            break;
        case 2:
            fprintf(stderr, "ERROR: 2");
            break;
        case 3:
            fprintf(stderr, "ERROR: 3");
            break;
        case 4:
            fprintf(stderr, "ERROR: 4");
            break;
        case 5:
            fprintf(stderr, "ERROR: 5");
            break;
        case 6:
            fprintf(stderr, "ERROR: 6");
            break;
        case 7:
            fprintf(stderr, "ERROR: 7");
            break;
        case 8:
            fprintf(stderr, "ERROR: 8");
            break;
        case 9:
            fprintf(stderr, "ERROR: 9");
            break;
        case 99:
            fprintf(stderr, "ERROR: Internal error of compiler");
    }

}

void set_error(int err){

    if (num_error == 0){
        num_error = err;
    }

}