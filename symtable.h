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
}Data_type;


typedef enum
{
    VAR,
    FUN
}Symbol_Type;
typedef struct {
    /** counter of input parameters types*/
    int cnt_param_type;
    /** input parameters types*/
    Data_type *param_type;
    /** counter of return parameters types*/
    int cnt_return_type;
    /** Return types*/
    Data_type *return_type;
}FunNode;

typedef struct {
    /** Token identifier*/
    char *key;
    /**data type*/
    Data_type id;
    /** funkc or variable*/
    Symbol_Type sym_type;
    /** Left children*/
    struct TreeNode *left;
    /** Right children*/
    struct TreeNode *right;
    /** function extension */
    FunNode *fun_extension;

} TreeNode;

typedef struct {
    /** Pole pro uložení hodnot. */
    TreeNode *array[MAX_STACK];
    /** Index prvku na vrcholu zásobníku. */
    int topIndex;
} Stack;

void Stack_Error( int );
void Stack_Init( Stack *Stack);
int Stack_IsEmpty( const Stack *Stack );
int Stack_IsFull( const Stack *Stack );
TreeNode Stack_Top( const Stack *Stack);
void Stack_Pop( Stack *Stack );
void Stack_Push( Stack *Stack, TreeNode *root );
void bst_init(TreeNode **tree);
void bst_insert_fun(TreeNode  **tree, char *key, Data_type id, int cnt_param_type, Data_type *param_type, int cnt_return_type, Data_type *return_type);
void bst_insert(TreeNode **tree, char *key, Data_type id);
bool bst_search(TreeNode  *tree, char *key);
void bst_dispose(TreeNode  **tree);

#endif //IFJ_SYMTABLE_H
