#include "Header.h"

/***********************************************/
/*        Public struct TreeNode               */
typedef struct TreeNode
{
	struct TreeNode *left;	 //left child
	struct TreeNode *right; //right child

	void * TreeNode_info;	 //info data in node
} TreeNode;
/***********************************************/

/***********************************************/
/*       Public functions:                     */
PNode create_tree_node(void * data)
{
	// get data (void*)
	// return new TreeNode with the data
	TreeNode *new_node = (TreeNode *)malloc(sizeof(TreeNode));
	if (new_node == NULL) // check the dynamic memory
	{
		printf("\n ALLOCATION MEMORY ERROR\n");
		return NULL;
	}
	else // else - creating the new tree node
	{
		new_node->TreeNode_info = data;
		new_node->left = NULL;
		new_node->right = NULL;
		return new_node;	//returns initialized node
	}
}
void insert_info_by_key(PNode * root, PNode * root_original, void * data, compare_func f_com, free_func f_free)
{
	// get data from the user
	// create & order the new TreeNode in the binary tree
	int temp;
	if (!(*root))
	{
		*root = create_tree_node(data);			//get new node for tree
		if (*root == NULL)
		{
			printf("Allocation tree node failed");
			empty_tree(root_original, f_free); /*deleting the whole tree,since root_original is a pointer to the whole tree*/
			exit(1);
		}
	}
	else
	{
		temp = f_com(data, (*root)->TreeNode_info);
		if (!temp)	// if the new node's key is bigger			
			insert_info_by_key(&((*root)->right), root_original, data, f_com, f_free);	//call insert_node with the right child
		else if(temp==1) // if smaller
			insert_info_by_key(&((*root)->left), root_original, data, f_com, f_free);
		else // equal
		{
			printf("\n** There is a Tree node with the same value !! **\n");
			f_free(data);
			return;
		}
	}
}
void empty_tree(PNode *root, free_func f_free)
{
	// frees all the allocated memory and sets the root to NULL
	if (*root == NULL)
		return;
	empty_tree(&((*root)->right),f_free);
	empty_tree(&((*root)->left), f_free);
	f_free((*root)->TreeNode_info);
	free(*root);
	*root = NULL;
}
void PrintPreOrder(PNode *root, print_func f_print)
{
	// Print all the node in preorder
	if (!(*root))
		return;
	f_print((*root)->TreeNode_info);
	PrintPreOrder(&((*root)->left), f_print);
	PrintPreOrder(&((*root)->right), f_print);
}
void PrintKInOrder(PNode *root, print_func f_print, int * k)
{
	// Print the 'K' biggest keys in the tree
	if ((*root) == NULL || *k == 0) // Stop condition
		return;
	PrintKInOrder(&((*root)->right), f_print, k); // goes to the right tree were the biggest keys are
	if (*k > 0) // just if k>0 - print
		f_print((*root)->TreeNode_info);
	(*k)--;
	PrintKInOrder(&((*root)->left), f_print, k);
}
int BFvalue(PNode *root)
{
	// Returns the height difference between the children
	int sum_left, sum_right;
	sum_left = tree_high((*root)->left); // get the high of the left child
	sum_right = tree_high((*root)->right); // get the high of the right child
	return sum_left - sum_right;
}
int tree_high(PNode root)
{
	// return the high of the node in the tree
	// empty tree => -1
	int sum_left, sum_right;
	if (root == NULL)
		return -1;
	sum_left = 1 + tree_high(root->left);
	sum_right = 1 + tree_high(root->right);
	return Max(sum_left, sum_right);
}
int CountTree(PNode root)
{
	// Count how much nodes are in the tree
	if (root == NULL)
		return 0;
	return 1 + CountTree(root->left) + CountTree(root->right);
}
PNode * CheckData(PNode *root, void * data, compare_func f_com)
{
	PNode *ReturnNodeLeft = NULL, *ReturnNodeRight = NULL;
	if (*root == NULL)
		return NULL;
	if (f_com(data, (*root)->TreeNode_info) == 2)
		return root;
	ReturnNodeLeft = CheckData(&((*root)->left), data, f_com);
	ReturnNodeRight = CheckData(&((*root)->right), data, f_com);
	if (ReturnNodeLeft)
		return ReturnNodeLeft;
	if (ReturnNodeRight)
		return ReturnNodeRight;
	return NULL;
}
/***********************************************/