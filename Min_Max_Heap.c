#include <stdio.h>
#include <stdlib.h>
/* Element in the Heaps */
typedef struct Node
{
	int data;
	int TwinIndex; // Twin index in the second Heap
} Node;
/* Manager Structure */
typedef struct TwoHeaps
{
	struct Node *MinArr;
	struct Node *MaxArr;
	int HeapsSize;
	int MaxHeapSize;
} TwoHeaps;
/* Required Functions - declaration */
TwoHeaps * Init(int arr[], int arrSize);
void Insert(TwoHeaps *S, int x);
int FindMin(TwoHeaps S);
int FindMax(TwoHeaps S);
void DelMin(TwoHeaps *S);
void DelMax(TwoHeaps *S);
/* Auxiliary Functions - declaration */
void MinHeapifyDown(TwoHeaps *S, int i);
void MaxHeapifyDown(TwoHeaps *S, int i);
void MinHeapifyUp(TwoHeaps *S, int i);
void MaxHeapifyUp(TwoHeaps *S, int i);
void FreeAll(int arr[], TwoHeaps *S);
void ErrorMes(char * ErrorStr);

int main()
{
	/* Declaration */
	int i, x, *arr, M, flag = 1;
	char ch;
	TwoHeaps *S;
	/* Getting the int type element ; and size M */
	printf("**	Before Start	**\n");
	printf("Enter the size of the array: ");
	scanf("%d", &M);
	if (M <= 0) // if M negative or 0
	{
		while (flag) // till M is correct
		{
			printf("Wrong input. Try again :");
			scanf("%d", &M);
			if (M)
				flag = 0;
		}
	}
	arr = (int *)malloc(M * sizeof(int));
	if (arr == NULL) // checking dynamic memory
		ErrorMes("\n** Error **\n");
	// Getting the input :
	printf("\nEnter the values (int type) : ");
	for (i = 0; i < M; i++)
		scanf("%d", &arr[i]);
	S = Init(arr, M); // Initiallizing the 2 heaps
	flag = 1;
	/* The menu => */
	while (flag)
	{
		printf("\n\nAdding a new element ( Insert(x) ) => press '1'");
		printf("\nPrinting the minimum ( Find-Min() ) => press '2'");
		printf("\nPrinting the maximum ( Find-Max() ) => press '3'");
		printf("\nDeleting the minimum ( Del-Min() ) => press '4'");
		printf("\nDeleting the maximum ( Del-Max() ) => press '5'");
		printf("\nClosing the program => press 'x'\n");
		ch = _getche();
		switch (ch)
		{
		case '1': // ( insert(x) )
			printf("\nEnter the new element (int type) :");
			scanf("%d", &x);
			Insert(S, x);
			break;
		case '2': // ( Find-Min() )
			if (S->HeapsSize < 1)
				printf("\nHeap underflow !! \n");
			else
				printf("\n**	The minimum value is : %d	**", FindMin(*S));
			break;
		case '3': // ( Find-Max() )
			if (S->HeapsSize < 1)
				printf("\nHeap underflow !! \n");
			else
				printf("\n**	The maximum value is : %d	**", FindMax(*S));
			break;
		case '4': // ( Del-Min() )
			DelMin(S);
			break;
		case '5': // ( Del-Max() )
			DelMax(S);
			break;
		case 'x': case 'X': // Closing - Free all memory
			FreeAll(arr, S);
			flag = 0;
			break;
		default:
			printf("\n**	Wrong input. Try again :	**");
			break;
		}
	}
	return 0;
}

/* Required Functions => */
TwoHeaps* Init(int arr[], int arrSize)
{
	/* Initiallizing 2 heaps */
	int i;
	TwoHeaps *S = (TwoHeaps*)malloc(sizeof(TwoHeaps));
	if (S == NULL) // checking dynamic memory
	{
		free(arr);
		ErrorMes("** Error **\n");
	}
	S->MaxArr = (Node *)malloc(arrSize * sizeof(Node));
	if (S->MaxArr == NULL) // checking dynamic memory
	{
		free(arr);
		ErrorMes("** Error **\n");
	}
	S->MinArr = (Node *)malloc(arrSize * sizeof(Node));
	if (S->MinArr == NULL) // checking dynamic memory
	{
		free(arr);
		ErrorMes("** Error **\n");
	}
	S->HeapsSize = arrSize;
	S->MaxHeapSize = arrSize;
	// Build the Heaps :
	for (i = 0; i < arrSize; i++)
	{
		S->MaxArr[i].data = arr[i];
		S->MaxArr[i].TwinIndex = i;
		S->MinArr[i].data = arr[i];
		S->MinArr[i].TwinIndex = i;
	}
	// Fix the Heaps :
	for (i = ((S->HeapsSize) / 2) - 1; i >= 0; i--)
	{
		MinHeapifyDown(S, i);
		MaxHeapifyDown(S, i);
	}
	return S;
}
void Insert(TwoHeaps *S, int x)
{
	/* Insert new element */
	if (S->MaxHeapSize == S->HeapsSize) // if overflow
		printf("\nHeap overflow !! \n");
	else
	{
		S->HeapsSize++;
		S->MaxArr[S->HeapsSize - 1].data = x;
		S->MinArr[S->HeapsSize - 1].data = x;
		// Fix the Heaps :
		MinHeapifyUp(S, S->HeapsSize - 1);
		MaxHeapifyUp(S, S->HeapsSize - 1);
		printf("\nSuccess.");
	}
}
int FindMin(TwoHeaps S)
{
	return S.MinArr[0].data;
}
int FindMax(TwoHeaps S)
{
	return S.MaxArr[0].data;
}
void DelMin(TwoHeaps *S)
{
	/* Delete the minimum value in the Heaps */
	int min, TwinIndex;
	if (S->HeapsSize < 1) // if underflow
		printf("\nHeap underflow !! \n");
	else
	{
		// Min Heap - delete the Min
		min = S->MinArr[0].data;
		TwinIndex = S->MinArr[0].TwinIndex;
		S->MinArr[0].data = S->MinArr[S->HeapsSize - 1].data;
		S->MinArr[0].TwinIndex = S->MinArr[S->HeapsSize - 1].TwinIndex;
		S->HeapsSize--;
		MinHeapifyDown(S, 0);
		S->HeapsSize++;
		// Max Heap - delete the Min (twin)
		S->MaxArr[TwinIndex].data = S->MaxArr[S->HeapsSize - 1].data;
		S->MaxArr[TwinIndex].TwinIndex = S->MaxArr[S->HeapsSize - 1].TwinIndex;
		MaxHeapifyDown(S, TwinIndex);
		MaxHeapifyUp(S, TwinIndex);
		S->HeapsSize--;
		printf("**	The min value (%d) was deleted.	**\n", min);
	}
}
void DelMax(TwoHeaps *S)
{
	/* Delete the maximum value in the Heaps */
	int max, TwinIndex;
	if (S->HeapsSize < 1)
		printf("\nHeap underflow !! \n");
	else
	{
		// Max Heap - delete the Max
		max = S->MaxArr[0].data;
		TwinIndex = S->MaxArr[0].TwinIndex;
		S->MaxArr[0].data = S->MaxArr[S->HeapsSize - 1].data;
		S->MaxArr[0].TwinIndex = S->MaxArr[S->HeapsSize - 1].TwinIndex;
		S->HeapsSize--;
		MaxHeapifyDown(S, 0);
		S->HeapsSize++;
		// Min Heap - delete the Max (twin)
		S->MinArr[TwinIndex].data = S->MinArr[S->HeapsSize - 1].data;
		S->MinArr[TwinIndex].TwinIndex = S->MinArr[S->HeapsSize - 1].TwinIndex;
		MinHeapifyDown(S, TwinIndex);
		MinHeapifyUp(S, TwinIndex);
		S->HeapsSize--;
		printf("**	The max value (%d) was deleted.	**\n", max);
	}
}
/* Auxiliary Functions => */
void MinHeapifyDown(TwoHeaps *S, int i)
{
	/* Heapify-Down for Min Heap */
	int flag = 1, largest, n, left, right, temp, temp2;
	while (flag)
	{
		largest = i;
		n = S->HeapsSize;
		left = (2 * (i)) + 1;
		right = left + 1;
		if (left < n && S->MinArr[left].data < S->MinArr[i].data)
			largest = left;
		if (right < n && S->MinArr[right].data < S->MinArr[largest].data)
			largest = right;
		if (largest != i) // exchange :
		{
			// data <=> data
			temp = S->MinArr[i].data;
			S->MinArr[i].data = S->MinArr[largest].data;
			S->MinArr[largest].data = temp;
			// TwinIndex <=> TwinIndex
			temp = S->MinArr[i].TwinIndex;
			temp2 = S->MinArr[largest].TwinIndex;
			S->MinArr[i].TwinIndex = temp2;
			S->MinArr[largest].TwinIndex = temp;

			/* Fix Max Heap twins index of i and largest */
			S->MaxArr[temp].TwinIndex = largest;
			S->MaxArr[temp2].TwinIndex = i;

			i = largest;
		}
		else // get out :
			flag = 0;
	}
}
void MaxHeapifyDown(TwoHeaps *S, int i)
{
	/* Heapify-Down for Max Heap */
	int flag = 1, largest, n, left, right, temp, temp2;
	while (flag)
	{
		largest = i;
		n = S->HeapsSize;
		left = (2 * i) + 1;
		right = left + 1;
		if (left < n && S->MaxArr[left].data > S->MaxArr[i].data)
			largest = left;
		if (right < n && S->MaxArr[right].data > S->MaxArr[largest].data)
			largest = right;
		if (largest != i) // exchange :
		{
			// data <=> data
			temp = S->MaxArr[i].data;
			S->MaxArr[i].data = S->MaxArr[largest].data;
			S->MaxArr[largest].data = temp;
			// TwinIndex <=> TwinIndex
			temp = S->MaxArr[i].TwinIndex;
			temp2 = S->MaxArr[largest].TwinIndex;
			S->MaxArr[i].TwinIndex = temp2;
			S->MaxArr[largest].TwinIndex = temp;

			/* Fix Min Heap twins index of i and largest */
			S->MinArr[temp].TwinIndex = largest;
			S->MinArr[temp2].TwinIndex = i;

			i = largest;
		}
		else // get out :
			flag = 0;
	}
}
void MaxHeapifyUp(TwoHeaps *S, int i)
{
	/* Heapify-Up for Max Heap */
	int flag = 1, smaller, n, father, temp, temp2;
	while (flag)
	{
		smaller = i;
		n = S->HeapsSize;
		father = i / 2;
		if (father < n && S->MaxArr[father].data < S->MaxArr[i].data)
			smaller = father;
		if (smaller != i) // exchange :
		{
			// data <=> data
			temp = S->MaxArr[i].data;
			S->MaxArr[i].data = S->MaxArr[smaller].data;
			S->MaxArr[smaller].data = temp;
			// TwinIndex <=> TwinIndex
			temp = S->MaxArr[i].TwinIndex;
			temp2 = S->MaxArr[smaller].TwinIndex;
			S->MaxArr[i].TwinIndex = temp2;
			S->MaxArr[smaller].TwinIndex = temp;

			/* Fix Min Heap twins index of i and smaller */
			S->MinArr[temp].TwinIndex = smaller;
			S->MinArr[temp2].TwinIndex = i;

			i = smaller;
		}
		else // getout :
			flag = 0;
	}
}
void MinHeapifyUp(TwoHeaps *S, int i)
{
	/* Heapify-Up for Min Heap */
	int flag = 1, largest, n, father, temp, temp2;
	while (flag)
	{
		largest = i;
		n = S->HeapsSize;
		father = i / 2;
		if (father < n && S->MinArr[father].data > S->MinArr[i].data)
			largest = father;
		if (largest != i) // exchange :
		{
			// data <=> data
			temp = S->MinArr[i].data;
			S->MinArr[i].data = S->MinArr[largest].data;
			S->MinArr[largest].data = temp;
			// TwinIndex <=> TwinIndex
			temp = S->MinArr[i].TwinIndex;
			temp2 = S->MinArr[largest].TwinIndex;
			S->MinArr[i].TwinIndex = temp2;
			S->MinArr[largest].TwinIndex = temp;

			/* Fix Max Heap twins index of i and largest */
			S->MaxArr[temp].TwinIndex = largest;
			S->MaxArr[temp2].TwinIndex = i;

			i = largest;
		}
		else // getout :
			flag = 0;
	}
}
void FreeAll(int arr[], TwoHeaps *S)
{
	/* Free all the dynamic memory */
	free(S->MaxArr);
	free(S->MinArr);
	free(S);
	free(arr);
}
void ErrorMes(char * ErrorStr)
{
	// error for the user and exit the program
	printf("\n%s", ErrorMes);
	exit(1);
}
