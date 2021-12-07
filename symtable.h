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

typedef struct {
    /** counter of input parameters types*/
    int cnt_param_type;
    /** input parameters types*/
    DataType *param_type;
    /** counter of return parameters types*/
    int cnt_return_type;
    /** Return types*/
    DataType *return_type;
    /** function definition*/
    bool is_func_defined;

}FunNode;

typedef struct {
    /** Token identifier*/
    char *key;
    /**data type*/
    DataType id;
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

/**
 * @brief stack initialization, set topindex on -1
 *
 * @param stack The address to which stack will be initialized
 */
void sym_stack_init(SymStack *stack);   // topindex = -1

/**
 * @brief find emptiness of stack
 *
 * @param stack The address to which stack will be controlled
 *
 * @return true if stack is empty / false otherwise
 */
bool sym_stack_is_empty( const SymStack *stack ); // ak je prazdny vrati true

/**
 * @brief find fullness of stack
 *
 * @param stack The address to which stack will be controlled
 *
 * @return true if stack is full / false otherwise
 */
bool sym_stack_is_full( const SymStack *stack ); // ak je plny vrati true

/**
 * @brief controling emptiness emptiness of stack
 *
 * @param stack The address to which stack will be controlled
 *
 * @return The address of root node on top of the stack
 */
TreeNode *sym_stack_top( const SymStack *stack); // vrati ukazatel na koren

/**
 * @brief Removing framework from stack
 *
 * @param stack The address to which framework of stack will be removed
 */
void sym_stack_pop( SymStack *stack );

/**
 * @brief Pushing new framework on stack
 *
 * @param stack The address to which stack will be pushed
 */
void sym_stack_push( SymStack *stack, TreeNode *root );

/**
 * @brief Delete stack
 *
 * @param stack The address to which stack will be removed
 */
void sym_stack_dispose( SymStack *stack);

/**
 * @brief Tree initialization
 *
 * @param tree The address for initialization
 */
void bst_init(TreeNode **tree);

/**
 * @brief Search for node
 *
 * @param tree The address of root node
 * @param key wanted node name
 *
 * @return found element address if element was found / NULL otherwise
 */
TreeNode *bst_search(TreeNode *tree, char *key);

/**
 * @brief Search for node in stack
 *
 * @param stack  The address of root node
 * @param key wanted node name
 *
 * @return true node was found / false otherwise
 */
TreeNode * bst_search_in_stack(SymStack *stack, char *key);

/**
 * @brief Search for node in stack
 *
 * @param stack  The address of root node
 * @param key wanted node name
 *
 * @return true node was found with correct data type / false otherwise
 */
bool bst_search_var_in_stack(SymStack *stack, char *key, DataType id);  //vrati true ked najde

/**
 * @brief Search node in stack for generator
 *
 * @param stack  The address of root node
 * @param key wanted node name
 *
 * @return stack topindex when node was found / -2 otherwise
 */
int bst_search_in_stack_gen(SymStack *stack, char *key);

/**
 * @brief Insert a node
 *
 * @param tree The address of root node
 * @param key node name
 * @param id node data type
 *
 * @return true node was inserted/ false otherwise
 *
 * @error 99 not compilation fault
 * @error 3 redefinition of variable / not definited variable
 */
bool bst_insert(TreeNode **tree, char *key, DataType id); //bude insertne alebo vrati chybu

/**
 * @brief Insert a node with function symbol type
 *
 * @param tree The address of root node
 * @param key node name
 * @param cnt_param_type number of node input parameters
 * @param param_type node input parameters
 * @param return_type node return types
 * @param cnt_return_type number of node return types
 * @param is_defined node definitions
 *
 * @return true node was inserted / false otherwise
 *
 * @error 99 not compilation fault
 * @error 3 redefinition of variable / not definited variable / not matching number of params/
 * not matching number of returns / not matching param types / not matching return types
 */
bool bst_insert_fun(TreeNode  **tree, char *key, int cnt_param_type, DataType *param_type, int cnt_return_type, DataType *return_type, bool is_defined); //bude insertne alebo vrati chybu

/**
 * @brief Delete whole tree
 *
 * @param tree root node address
 *
 */
void bst_dispose(TreeNode  **tree); // vymaze strom

/**
 * @brief compare two DataType arrays
 *
 * @param cnt1 number of first array elements
 * @param arr1 the address of first array element
 * @param cnt2 number of second array elements
 * @param arr2 the address of second array element
 *
 * @return true number of array elements is same and all array elements are same / false otherwise
 */
bool sym_param_comp(int cnt1,  DataType *arr1, int cnt2, DataType *arr2);

#endif //IFJ_SYMTABLE
