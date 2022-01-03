/**
 * @file error.h
 * @brief Definition of errors
 * @author Martin Kozák
 * 
 * Project: IFJ compiler
 * Date: 24.11.2021
 */ 

#ifndef ERROR_H_
#define ERROR_H_

extern int num_error;

#define LEXICAL_ERR                 1    // Lexikálna chyba
#define SYNTACTIC_ERR               2    // Syntaktická chyba
#define SEM_UNDEFINED_ERR           3    // Sémantická chyba - nedefinovaná funkcia/premenná, pokus o redefinovanie
#define SEM_T_UNCOM_ERR             4    // Sémantická chyba - priradenie - typová nekompabilita
#define SEM_FUNC_PARAM_RET_ERR      5    // Sémantická chyba - zlý počet/ typ parametrov, pokus o redefinovanie premennej
#define SEM_T_UNCOM_EXPRESS_ERR     6    // Sémantická chyba - typová kompabilita vo výrazoch
#define SEM_OTHERS                  7    // Ostatné semantické chyby
#define NIL_ERR                     8    // Behová chyba pri práci s nil hodnotou
#define DIV_ZERO_ERR                9    // Behová chyba pri delení nulou  
#define INTERNAL_ERR                99   // Interná chyba prekladača


/**
 * @brief Prints error message
 * 
 * @param num_err 
 */

void error_mess(int num_err);

/**
 * @brief Set global variable num_error to err
 *  
 * @param err
 */

void set_error(int err);

#endif 