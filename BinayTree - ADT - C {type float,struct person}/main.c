#include "Header.h"

/***********************************************/
/*       Specific struct:                      */
typedef struct person
{
	int id;
	char * name;
}Person;
/***********************************************/

/***********************************************/
/*            Specific functions               */
/* Struct Person :	    	        		   */
void print_Person(void *p)
{
	// print p's id to the user
	Person *temp = (Person*)p;
	printf("%s	%d\n", (temp)->name, (temp)->id);
}
BOOL compare_Person(void *p1, void *p2)
{
	// compare the id number and return =>
	// if p2 smaller than p1 - false (0)
	// if p1 smaller than p2 - true (1)
	// if p1 and p2 equal - equal (2)
	Person *temp1, *temp2;
	temp1 = (Person*)p1;
	temp2 = (Person*)p2;
	if (temp1->id < temp2->id)
		return TRUE;
	else if (temp1->id == temp2->id)
		return EQUAL;
	else
		return FALSE;
}
void free_Person(void *p)
{
	// free (char*)name and struct Person
	Person *temp = (Person *)p;
	free(temp->name);
	free(temp);
}
/***********************************************/
/* Float type :								   */
void print_float(void *a)
{
	// print float to the user
	printf("%.1f	", *(float*)a);
}
BOOL compare_float(void *a1, void*a2)
{
	// compare the id number and return =>
	// if a2 smaller than a1 - false (0)
	// if a1 smaller than a2 - true (1)
	// if a1 and a2 equal - equal (2)
	if (*(float*)a1 < *(float*)a2)
		return TRUE;
	else if (*(float*)a1 == *(float*)a2)
		return EQUAL;
	else
		return FALSE;
}
void free_float(void *a)
{
	free((float*)a);
}
/***********************************************/
/* Help function :							   */
void GetNodeValue(PNode * root, int mode, void ** data)
{
	// Help function for the main
	// Get from the user the requiest data, return it in void ** data
	// mode = 0 (float); mode = 1 (struct Person)
	char ch, temp[99];
	float *float_memory, float_temp;
	Person *p;
	if (mode) // preson
	{
		p = (Person *)malloc(sizeof(Person)); // create new data
		if (p == NULL)
		{
			printf("Allocation memory error");
			empty_tree(root, free_Person);
		}
		// get the name
		printf("\nEnter the Name :\n");
		scanf("%s", temp);
		p->name = (char *)malloc((strlen(temp) + 1) * sizeof(char));
		if (p->name == NULL)
		{
			printf("Allocation memory error");
			empty_tree(root, free_Person);
		}
		strcpy(p->name, temp);
		// get the id
		printf("\nEnter the id :\n");
		scanf("%d", &(p->id));
		// return in data
		*data = p;
	}
	else // float
	{
		// get the float value
		printf("\nEnter the float value :\n");
		float_memory = (float*)malloc(sizeof(float));
		if (float_memory == NULL)
		{
			printf("Allocation memory error");
			empty_tree(root, free_float);
		}
		scanf("%f", &float_temp);
		*float_memory = float_temp;
		// return in data
		*data = float_memory;
	}
}
void CreateTreeSearch(PNode * root, int mode)
{
	// Help function for the main
	// get input from the user and create tree search
	// GetNodeValue - get the data from the user
	// mode = 0 (float); mode = 1 (struct Person)
	void * data;
	char ch;
	int flag = 1;
	while (flag)
	{
		GetNodeValue(root, mode, &data);
		if (mode) // person
			insert_info_by_key(root, root, data, compare_Person, free_Person);
		else
			insert_info_by_key(root, root, data, compare_float, free_float);
		printf("\nMore ? [Y/N]	");
		ch = _getche();
		if (!(ch == 'Y' || ch == 'y'))
			flag = 0;
	}
}
/***********************************************/

int main()
{
	// declaration
	char ch;
	int ShowMe = 1, flag = 1, Must = 0, mode, BF;
	int *BF_IntValue, BF_IntTemp, k;
	float *BF_FloatValue, BF_FloatTemp;
	PNode * temp, root = NULL;
	// Get the requeist type of data
	printf("** Before building the tree **\n\n");
	printf("What kind of data type do you prefer ?\n");
	printf("Float type	            	press => 'f'\n");
	printf("Struct Person type [key:(int) id] press => 'i'\n");
	while (flag)
	{
		// mode = 0 (float) [Default] ; mode = 1 (struct Person)
		ch = _getche();
		if (ch == 'f' || ch == 'F')
			flag = mode = 0;
		else if (ch == 'i' || ch == 'I')
		{
			flag = 0;
			mode = 1;
		}
		else
			printf("\nWorng input ..... Please try again : ");
	}
	// loop while the user close the program (and free the memory)
	printf("\n\n** Don't forget to build the tree first **");
	while (!flag)
	{
		// Show ones. By presing 'r', the user can gets the menu again.
		if (ShowMe)
		{
			printf("\n\nFor adding a new node => press '1'\n");
			printf("For printing the tree (Preorder) => press '2'\n");
			printf("For BF value => press '3'\n");
			printf("For printing the most biggest keys in the tree => press '4'\n");
			printf("For repting this menu => press 'r'\n");
			printf("For closing the program => press 'x'\n");
			ShowMe = 0;
		}
		ch = _getche();
		switch (ch)
		{
			// Creating node in the tree
		case '1':
			CreateTreeSearch(&root, mode);
			Must = 1; // Open the rest option for the user
			printf("\n** Finish **\n");
			break;
		case '2':
			if (Must)
			{
				printf("\nThe search tree (Preorder) :\n");
				if (mode) // person
					PrintPreOrder(&root, print_Person);
				else // float
					PrintPreOrder(&root, print_float);
				printf("\n** Finish **\n");
			}
			else
				printf("\n** Please insert data to the tree first ! **\n");
			break;
		case '3':
			// Get node data from the user (By key only)
			// Return the BF value of the node in the tree
			if (Must)
			{
				if (mode) // person
				{
					// get the id
					printf("\nEnter the id : ");
					BF_IntValue = (int *)malloc(sizeof(int));
					if (BF_IntValue == NULL)
					{
						printf("Allocation memory error");
						empty_tree(root, free_Person);
					}
					scanf("%d", &BF_IntTemp);
					*BF_IntValue = BF_IntTemp;
					// Check if the node data is correct
					// if correct => get the root with the data in the tree in temp
					// else => NULL
					temp = CheckData(&root, BF_IntValue, compare_Person);
					if (temp == NULL)
						printf("\nThe value does not exist !!\n");
					else
					{
						BF = BFvalue(temp); // Get the BF value of temp (the node with the data)
						printf("\nThe BF value is : %d\n", BF);
					}
					free(BF_IntValue);
				}
				else
				{
					// Get the float value
					printf("\nEnter the float value : ");
					BF_FloatValue = (float *)malloc(sizeof(float));
					if (BF_FloatValue == NULL)
					{
						printf("Allocation memory error");
						empty_tree(root, free_Person);
					}
					scanf("%f", &BF_FloatTemp);
					*BF_FloatValue = BF_FloatTemp;
					// Check if the node data is correct
					// if correct => get the root with the data in the tree in temp
					// else => NULL
					temp = CheckData(&root, BF_FloatValue, compare_float);
					if (temp == NULL)
						printf("\nThe value does not exist !!\n");
					else
					{
						BF = BFvalue(temp); // Get the BF value of temp (the node with the data)
						printf("The BF value is : %d\n", BF);
					}
					free(BF_FloatValue);
				}
				printf("\n** Finish **\n");
			}
			else
				printf("\n** Please insert data to the tree first ! **\n");
			break;
		case '4':
			if (Must)
			{
				// Get the requeist number
				printf("\nEnter 'k' number : ");
				scanf("%d", &k);
				// Checking if the k is correct
				// CountTree return the number of nodes in the tree
				if (k <= 0 || CountTree(root) < k)
					printf("\n**Worng number !!**\n");
				else
				{
					if (mode) // Person
						PrintKInOrder(&root, print_Person, &k);
					else
						PrintKInOrder(&root, print_float, &k);
				}
				printf("\n** Finish **\n");
			}
			else
				printf("\n** Please insert data to the tree first ! **\n");
			break;
		case 'r': case 'R':
			// Show the user the menu again
			ShowMe = 1;
			break;
		case 'x': case 'X':
			// Close and free the program
			if (mode) // person.id
				empty_tree(&root, free_Person);
			else // float
				empty_tree(&root, free_float);
			printf("\n** Finish **\n");
			flag = 1;
			break;
		default:
			// Else :
			printf("Worng input ! try again :	");
			break;
		}
	}
	return 0;
}
