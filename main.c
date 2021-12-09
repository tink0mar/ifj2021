/**
 * @file main.c
 * @brief Main code for ifj compiler
 * @author Martin Kozák
 *
 * Project: IFJ compiler
 * Date: 24.11.2021
 */


#include <stdio.h>
#include <stdbool.h>
#include "parser.h"
#include "error.h"
#include "generator.h"
#include "dll.h"

int main(){

    int error = syntactic_analyzator();
    print_code();
    if (error == 0){
        
        return 0;
    } else {
        error_mess(error);
        return error;
    }


}
