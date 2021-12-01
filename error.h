/**
 * @file error.h
 * @brief definition of errors
 * @author 
 * 
 * Project: IFJ compiler
 * Date: 24.11.2021
 */ 

#ifndef ERROR_H_
#define ERROR_H_

extern int num_error;

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

void set_error(int err)

#endif 