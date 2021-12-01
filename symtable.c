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

bool bst_search(TreeNode  *tree, char *key)
{
    if (tree == NULL){

        return false;

    } else if (!strcmp(tree->key,key)) {
        fprintf(stderr, "search %s\n",tree->key);//TODO
        return true;

    } else if (strcmp(tree->key,key) < 0){
        fprintf(stderr, "idem left search %s\n", tree->key);//TODO
        return bst_search((TreeNode *) tree->left, key);

    } else {
        fprintf(stderr, " idem right search %s\n",tree->key);//TODO
        return bst_search((TreeNode *) tree->right, key);

    }
}

void bst_insert(TreeNode **tree, char *key, DataType id) {
    if ((*tree) == NULL) {

        (*tree) = malloc(sizeof(TreeNode));
        if ((*tree) == NULL){
            set_error(INTERNAL_ERR);
            return;
        }


        (*tree)->key = key;
        (*tree)->id = id;
        (*tree)->sym_type = VAR;
        (*tree)->left = NULL;
        (*tree)->right = NULL;
        (*tree)->fun_extension = NULL;
        fprintf(stderr, "inser %s\n",(*tree)->key);//TODO

    } else if (!strcmp((*tree)->key,key)) {
        set_error(SEM_UNDEFINED_ERR);
        return;

    } else if (strcmp((*tree)->key,key) < 0) {
        bst_insert((TreeNode **) &((*tree)->left), key, id);

    } else {
        bst_insert((TreeNode **) &((*tree)->right), key, id);
    }
}
void bst_insert_fun(TreeNode  **tree, char *key, DataType id, int cnt_param_type, DataType *param_type, int cnt_return_type, DataType *return_type) {
    if ((*tree) == NULL) {

        (*tree) = malloc(sizeof(TreeNode));
        if ((*tree) == NULL)
        {
            set_error(INTERNAL_ERR);
            return;
        }


        (*tree)->key = key;
        (*tree)->id = id;
        (*tree)->sym_type = FUN;
        (*tree)->left = NULL;
        (*tree)->right = NULL;
        (*tree)->fun_extension->cnt_param_type = cnt_param_type;
        (*tree)->fun_extension->cnt_return_type = cnt_return_type;
        (*tree)->fun_extension->param_type = param_type;
        (*tree)->fun_extension->return_type = return_type;

    } else if ((*tree)->key == key) {
        set_error(SEM_UNDEFINED_ERR);
        return;

    } else if ((*tree)->key > key) {
        bst_insert((TreeNode **) &((*tree)->left), key, id);

    } else {
        bst_insert((TreeNode **) &((*tree)->right), key, id);
    }
}

void bst_dispose(TreeNode  **tree){
    if ( (*tree) != NULL) {
        fprintf(stderr, "  dispose %s\n",(*tree)->key);//TODO
        bst_dispose( (TreeNode **) &((*tree)->left) );
        bst_dispose( (TreeNode **) &((*tree)->right) );
        free(*tree);
        (*tree) = NULL;
    }
}
