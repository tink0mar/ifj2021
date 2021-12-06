/**
 * @file symtable.c
 * @brief Symtable for ifj compiler
 * @author Denis Adásek
 *
 * Project: IFJ compiler
 * Date: 24.11.2021
 */
#include "symtable.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "error.h"

void bst_init(TreeNode **tree){
    *tree = NULL;
}

TreeNode *bst_search(TreeNode  *tree, char *key)
{
    if (tree == NULL){
        return NULL;

    } else if (!strcmp(tree->key,key)) {
        return tree;

    } else if (strcmp(tree->key,key) > 0){
        return bst_search((TreeNode *) tree->left, key);

    } else {
        return bst_search((TreeNode *) tree->right, key);

    }
}

TreeNode *bst_search_in_stack(SymStack *stack, char *key){
    int a = stack->topIndex;
    TreeNode *k;
    while(stack->topIndex != 0)
    {
       TreeNode *tree = sym_stack_top(stack);
       k = bst_search(tree, key);
       if(k != NULL)
       {
           stack->topIndex = a;
           return k;
       }
       stack->topIndex--;
    }
    stack->topIndex = a;
    return NULL;

}

bool bst_search_var_in_stack(SymStack *stack, char *key, DataType id){
    int a = stack->topIndex;
    TreeNode *k;
    while(stack->topIndex != -1)
    {
        TreeNode *tree = sym_stack_top(stack);
        k = bst_search(tree, key);
        if(k != NULL)
        {
            stack->topIndex = a;
            return k->id == id;
        }
        stack->topIndex--;
    }
    stack->topIndex = a;
    return false;

}

bool bst_insert(TreeNode **tree, char *key, DataType id) {
    if ((*tree) == NULL) {

        (*tree) = malloc(sizeof(TreeNode));
        if ((*tree) == NULL){
            set_error(INTERNAL_ERR);
            return false;
        }


        (*tree)->key = key;
        (*tree)->id = id;
        (*tree)->left = NULL;
        (*tree)->right = NULL;
        (*tree)->fun_extension = NULL;
        return true;

    } else if (!strcmp((*tree)->key,key)) {
        set_error(SEM_UNDEFINED_ERR);
        return false;

    } else if (strcmp((*tree)->key,key) > 0) {
        return bst_insert((TreeNode **) &((*tree)->left), key, id);

    } else {
        return bst_insert((TreeNode **) &((*tree)->right), key, id);
    }
}

bool bst_insert_fun(TreeNode  **tree, char *key, int cnt_param_type, DataType *param_type, int cnt_return_type, DataType *return_type, bool is_defined) {
    if ((*tree) == NULL) {
        (*tree) = malloc(sizeof(TreeNode));
        if ((*tree) == NULL)
        {
            set_error(INTERNAL_ERR);
            return false;
        }
        (*tree)->key = key;
        (*tree)->left = NULL;
        (*tree)->right = NULL;
        FunNode *k = malloc(sizeof(FunNode));
        (*tree)->fun_extension = k;
        (*tree)->fun_extension->cnt_param_type = cnt_param_type;
        (*tree)->fun_extension->cnt_return_type = cnt_return_type;
        (*tree)->fun_extension->param_type = param_type;
        (*tree)->fun_extension->return_type = return_type;
        (*tree)->fun_extension->is_func_defined = is_defined;
        return true;

    } else if (!strcmp((*tree)->key,key)) {
        if(is_defined && !(*tree)->fun_extension->is_func_defined)
        {
            int tmp1 = sym_param_comp(cnt_param_type, param_type,(*tree)->fun_extension->cnt_param_type, (*tree)->fun_extension->param_type);
            int tmp2 = sym_param_comp(cnt_return_type, return_type,(*tree)->fun_extension->cnt_return_type, (*tree)->fun_extension->return_type);
            if(tmp1 && tmp2)
            {
                (*tree)->fun_extension->is_func_defined = true;
                return true ;
            }
            set_error(SEM_UNDEFINED_ERR);
            error_mess(num_error);
            return false;
        }
        else
        {
            set_error(SEM_UNDEFINED_ERR);
            error_mess(num_error);
            return false;
        }

    } else if (strcmp((*tree)->key,key) > 0) {
        return bst_insert_fun((TreeNode **) &((*tree)->left), key, cnt_param_type, param_type, cnt_return_type, return_type, is_defined);

    } else {
        return bst_insert_fun((TreeNode **) &((*tree)->right), key, cnt_param_type, param_type, cnt_return_type, return_type, is_defined);
    }

}

void bst_dispose(TreeNode  **tree){
    if ( (*tree) != NULL) {
        bst_dispose( (TreeNode **) &((*tree)->left) );
        bst_dispose( (TreeNode **) &((*tree)->right) );
        if((*tree)->fun_extension)
        {
            free((*tree)->fun_extension);
        }
        free((*tree)->key);
        free(*tree);
        (*tree) = NULL;
    }
}

bool sym_param_comp(int cnt1,  DataType *arr1, int cnt2, DataType *arr2){
    if(cnt1 == cnt2)
    {
        if(!cnt1)
        {
            return true;
        }
        for(int i = 0; i < cnt1; i++)
        {
            if(arr1[i] != arr2[i])
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
