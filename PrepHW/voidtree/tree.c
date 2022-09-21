//Name: John Gregerson --- Class: CS360 --- Assignment: Prep-Voidtree --- Date: 08/27/2022

#include "tree.h"
//#include "stdio.h"
#include "stdlib.h"

struct tree_node* tree_insert(struct tree_node* root, void* key, void* value, int (compar)(const void*, const void*)){
    if(root == NULL){
        struct tree_node* new = malloc(sizeof(struct tree_node));
        void *newKey = malloc(sizeof(*key));
        new->key = key;

        void *newValue = malloc(sizeof(*value));
        newValue = &value;
    }
}

void * tree_search(struct tree_node* root, void* key, int (compar)(const void*, const void*)){

}

void tree_empty(struct tree_node* root, void (del_key)(void *), void (del_val)(void*)){

}

int tree_walk(struct tree_node* root, void* pass_in, int (visitor)(void*, const void*, void *)){

}

struct tree_path_node* tree_path_search(struct tree_node* root, void* key, int (compar)(const void*, const void*)){

}
