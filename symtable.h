/**
 * @file symtable.h
 * @brief Interface of symtable for ifj project
 * @author Denis Adásek
 *
 * Project: IFJ compiler
 * Date: 24.11.2021
 */

#include <stdbool.h>

#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

#define MAX_STACK 20

typedef enum
{
    NUM,
    INT,
    STR,
    NIL
}DataType;


typedef enum
{
    VAR,
    FUN
}SymbolType;
typedef struct {
    /** counter of input parameters types*/
    int cnt_param_type;
    /** input parameters types*/
    DataType *param_type;
    /** counter of return parameters types*/
    int cnt_return_type;
    /** Return types*/
    DataType *return_type;
}FunNode;

typedef struct {
    /** Token identifier*/
    char *key;
    /**data type*/
    DataType id;
    /** funkc or variable*/
    SymbolType sym_type;
    /** Left children*/
    struct TreeNode *left;
    /** Right children*/
    struct TreeNode *right;
    /** function extension */
    FunNode *fun_extension;

}TreeNode;

typedef struct {
    /** Array of roots */
    TreeNode *array[MAX_STACK];
    /** Top index */
    int topIndex;
} SymStack;

void sym_stack_init(SymStack *stack);   // topindex = -1
bool sym_stack_is_empty( const SymStack *stack ); // ak je prazdny vrati true
bool sym_stack_is_full( const SymStack *stack ); // ak je plny vrati true
TreeNode *sym_stack_top( const SymStack *stack); // vrati ukazatel na koren
void sym_stack_pop( SymStack *stack );
void sym_stack_push( SymStack *stack, TreeNode *root );
void sym_stack_dispose( SymStack *stack, TreeNode *ptr );
void bst_init(TreeNode **tree);  // koren = NULL
bool bst_search(TreeNode *tree, char *key);  //vrati true ked najde
void bst_insert(TreeNode **tree, char *key, DataType id); //bude insertne alebo vrati chybu
void bst_insert_fun(TreeNode  **tree, char *key, DataType id, int cnt_param_type, DataType *param_type, int cnt_return_type, DataType *return_type); //bude insertne alebo vrati chybu
void bst_dispose(TreeNode  **tree); // vymaze strom

#endif //IFJ_SYMTABLE_H
