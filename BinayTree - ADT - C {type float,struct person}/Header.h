#ifndef _ HEADER_H
#define _ HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <math.h>

#define Max(a,b) ((a) > (b)? (a): (b)) 

typedef enum { FALSE, TRUE, EQUAL } BOOL;

typedef BOOL(*compare_func)(void*, void*);
typedef void(*print_func)(void*);
typedef void(*free_func)(void*);

typedef  struct TreeNode* PNode;

/***********************************************/
/*       Public functions:                     */
PNode create_tree_node(void * data); // return new TreeNode with the (void*) data
void insert_info_by_key(PNode * root, PNode * root_original, void * data, compare_func f_com, free_func f_free);
void empty_tree(PNode *root, free_func f_free); // frees all the allocated memory and sets the root to NULL
void PrintPreOrder(PNode * root, print_func f_print); // Print all the node in preorder
int BFvalue(PNode *root); // Returns the height difference between the children
int tree_high(PNode root); // return the high of the node in the tree
PNode * CheckData(PNode *root, void * data, compare_func f_com);
void PrintKInOrder(PNode *root, print_func f_print,int * k); // Print the 'K' biggest keys in the tree
int CountTree(PNode root); // Count how much nodes are in the tree
/***********************************************/
#endif
