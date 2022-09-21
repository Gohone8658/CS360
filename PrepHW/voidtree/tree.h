#ifndef TREE_H
#define TREE_H

/*
 * Testing Objectives / Prerequisite Skills
 * 
 * This assignment tests the use of function and void pointers.
 * It also makes heavy use of dynamic memory
 */

/* 
 * This library uses void pointers and user-provided helper functions to 
 * implement a generic BST for storing key/value pairs.
 * 
 * The tree does not employ sentry nodes. Indicate that a child does not
 * exist by setting the relevant pointer to NULL.
 * 
 * The tree is /entirely recursive/ and may not make use of for or while loops.
 * 
 * The tree assumes responsibility for freeing the keys and values.
 * 
 * If at any time a memory allocation fails, exit(1) immediately.
 */

struct tree_node {
	void *key;
	void *value;
	struct tree_node *left;
	struct tree_node *right;
};

/*
 * tree_insert -- insert a given value, with given key, into the provided tree
 * 
 * If a provide key already exists in the tree, do nothing.
 * 
 * 		root   -- Pointer to root of the tree, or NULL
 * 		key    -- Key to use to find value in the future
 * 		value  -- Value to store, treated as an opaque value by the library
 * 		compar -- Pointer to a function which can compare two key objects
 * 		          Semantics should match strcmp(), i.e. 
 * 		          compar(a, b) should return:
 * 		          <0 if *a < *b,
 * 		          0  if *a == *b,
 * 		          >0 if *a > *b
 * 
 * Returns: pointer to (new) root node
 */
struct tree_node* tree_insert(struct tree_node* root, void* key, void* value, int (compar)(const void*, const void*));

/*
 * tree_search -- search for given key in tree, returning value if present
 * 
 * 		root   -- Pointer to root of the tree, or NULL
 * 		key    -- Key to use to find value in the future
 * 		compar -- Pointer to a function which can compare two key objects
 * 		          Semantics should match strcmp(), i.e. 
 * 		          compar(a, b) should return:
 * 		          <0 if *a < *b,
 * 		          0  if *a == *b,
 * 		          >0 if *a > *b
 * 
 * Returns: pointer to value if key is present, otherwise NULL
 */
void * tree_search(struct tree_node* root, void* key, int (compar)(const void*, const void*));

/*
 * tree_empty -- empty given tree, releasing all memory associated with it
 * 
 * Frees all keys and values stored in the tree, as well as the tree itself.
 * 
 * 		root    -- Pointer to root of the tree, or NULL
 * 		del_key -- Function to call to free the key (which is passed in)
 * 		del_val -- Function to call to free the value (which is passed in)
 */
void tree_empty(struct tree_node* root, void (del_key)(void *), void (del_val)(void*));

/*
 * tree_walk -- traverse the tree in sorted order, invoking a fcn for each key/value pair
 * 
 * 		root    -- Pointer to root of the tree, or NULL
 * 		pass_in -- This pointer will be passed to all calls of visitor
 * 		visitor -- Function invoked for each node in the tree
 * 		           Will be invoked as visitor(pass_in, key, value)
 * 		           DO NOT change the key in this function!
 * 		           Return 0 to continue walking tree, or anything else to stop
 * 
 * Returns: 0 on a completed tree walk, or non-zero if walk was interrupted by visitor
 */
int tree_walk(struct tree_node* root, void* pass_in, int (visitor)(void*, const void*, void*));

/* 
 * Node representing one step on a path through the tree.
 * Key and value pointers link to the versions in the tree.
 * DO NOT change the key!
 */
struct tree_path_node {
	void* key;
	void* value;
	struct tree_path_node* next;
};

/*
 * tree_path_search -- search for given key in tree, returning k/v pairs along the path
 * 
 * 		root   -- Pointer to root of the tree, or NULL
 * 		key    -- Key to use to find value in the future
 * 		compar -- Pointer to a function which can compare two key objects
 * 		          Semantics should match strcmp(), i.e. 
 * 		          compar(a, b) should return:
 * 		          <0 if *a < *b,
 * 		          0  if *a == *b,
 * 		          >0 if *a > *b
 * 
 * Returns: A linked list of tree path nodes starting at the root and leading
 *   to the search-for k/v pair, including the k/v pair.
 *   Or: NULL if key cannot be found.
 *   The last node in the list is indicated by a next value of NULL
 *   DO NOT modify the key values stored in the path nodes.
 *   Caller is responsible for freeing tree_path_nodes, but not keys/values
 */
struct tree_path_node* tree_path_search(struct tree_node* root, void* key, int (compar)(const void*, const void*));

#endif
