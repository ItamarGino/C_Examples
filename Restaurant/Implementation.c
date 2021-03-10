#include "Header.h"
// Function for the restaurant :
Restaurant* CreateProduct()
{
	// createing Manager struct
	Restaurant *CafeCafe = OpenRestaurant(&CafeCafe);
	// declaration
	Product temp, *p;
	int flag;
	char name[99];
	// get the information from Manot.txt
	FILE *input = fopen("Manot.txt", "rt");
	if (input == NULL)
	{
		free_all_memory(CafeCafe);
		error_msg("\ninput file is wrong");
	}
	printf("Loading input file ... ");
	while (fscanf(input, "%s%d%d", name, &temp.Quantity, &temp.Price) == 3)
	{
		p = (Product *)malloc(sizeof(Product)); // creat product node
		if (p == NULL) // checking the dynamic memory
		{
			free_all_memory(CafeCafe);
			error_msg("\nallocation memory error.");
		}
		// copy the information that scaned to the product
		p->Product_name = (char *)malloc((strlen(name) + 1) * sizeof(char));
		if (p->Product_name == NULL)
		{
			free_all_memory(CafeCafe);
			error_msg("\nallocation memory error.");
		}
		strcpy(p->Product_name, name);
		if (CafeCafe->head != NULL)
			flag = Name_Correct(CafeCafe, p->Product_name); // checking if the name is correct
		if (!flag) // if flag == 0 => the name already exits
		{
			printf("error.\nThere are 2 products (or more) with the same name.\n");
			free_all_memory(CafeCafe);
			exit(1);
		}
		if (temp.Price <= 0) // checking as requeist
		{
			printf("error.\nWrong price error.\n");
			free_all_memory(CafeCafe);
			exit(1);
		}
		if (temp.Quantity <= 0) // checking as requeist
		{
			printf("error.\nWrong quantity error.\n");
			free_all_memory(CafeCafe);
			exit(1);
		}
		// insert the product to the list :
		p->next = CafeCafe->head;
		CafeCafe->head = p;
		p->Price = temp.Price;
		p->Quantity = temp.Quantity;
		p->Count = 0;
		CafeCafe->products_mount++;
		// oreder the list as requiest
		Order_Products(CafeCafe);
	}
	printf("Success.");
	fclose(input);
	GetTableAmount(CafeCafe);
	return CafeCafe;
}
Restaurant* OpenRestaurant(Restaurant *r)
{
	// createing Manager struct
	r = (Restaurant *)malloc(sizeof(Restaurant));
	if (r == NULL)
		error_msg("\nallocation memory error");
	// Initializing :
	r->head = NULL;
	r->tables_mount = 0;
	r->products_mount = 0;
	r->arr = NULL;
	return r;
}
void free_all_memory(Restaurant *r)
{
	// declaration :
	int i;
	Product *temp = r->head;
	ProductInTable *temp2;
	// free all the memory in the kitchen products list (one way) :
	while (temp != NULL)
	{
		r->head = temp->next;
		free(temp->Product_name);
		free(temp);
		temp = r->head;
	}
	// free all the products in the tables and the table array
	if (r->arr)
	{
		for (i = 0; i < r->tables_mount; i++)
		{
			temp2 = r->arr[i].head;
			while (temp2 != NULL)
			{
				r->arr[i].head = temp2->next;
				free(temp2->Product_name);
				free(temp2);
				if (r->arr[i].head)
					temp2 = r->arr[i].head;
				else
					temp2 = NULL;
			}
		}
		// free the rest :
		free(r->arr);
		free(r);
	}
}
void Report(Restaurant *r)
{
	// declaration :
	Product *p = r->head;
	ProductInTable *p2;
	char **s;
	int flag, index, i = 0, MaxCount, TempCounter = 0;
	// scaning the produts and return the most seller
	// return in => index - the number of equal ordered products
	// return in => s - product(s) name
	// return in => MaxCount - the mount of ordered
	s = ScanProductData(r, &MaxCount, &index);
	printf("\n** CafeCafe Reports **\n");
	printf("\nReport A :\n");
	if (s == NULL)
		printf("All products have not yet been sold !\n");
	else
	{
		printf("The most ordered product(s) :\n");
		for (i = 0; i < index; i++)
		{
			printf("%s	%d times\n", s[i], MaxCount);
			free(s[i]);
		}
		free(s);
	}
	printf("\nReport B :\nThe run out product(s) :\n");
	for (i = 0; i < r->products_mount; i++)
	{
		if (!(p->Quantity)) // checking if the product did not run out
		{
			printf("%s	", p->Product_name);
			TempCounter++;
		}
		p = p->next;
	}
	if (!TempCounter)
		printf("The kitchen is full.");
	printf("\n\nReport C :\n");
	if (CheckingTableMode(r)) // checking if the resaurant is not empty
	{
		printf("The unpaid products : \n");
		for (i = 0; i < r->tables_mount; i++)
		{
			flag = 0;
			p2 = r->arr[i].head;
			if (p2 != NULL) // if not empty
			{
				printf("\nTable[%d] :\n", i + 1);
				while (p2 != NULL) // print all the products in the table (that did not remove)
				{
					if (p2->HowMuchInTable)
					{
						printf("%s, (%d) times\n", p2->Product_name, p2->HowMuchInTable);
						flag = 1;
					}
					p2 = p2->next;
				}
				if (!flag)
					printf("Product(s) had been remove, but the table is still busy.\n");
			}
		}
	}
	else
		printf("\n**CafeCafe is Empty (all the tables were paid) !!**\n");
}
char ** ScanProductData(Restaurant *r, int *Max_Number, int *index)
{
	// scans all the products in the kitchen data and return the name(s) of the most added product(s)
	// return in *count the numbet of order of the product(s)
	// return in *index => counter
	// in case of more than 1 product with the same return => return **s as array
	// in case of all the products ordered 0 times (empty restaurant or after remove items) => return NULL
	int i, ReturnValue = 0, counter = 1;
	Product *p = r->head;
	char **s;
	// get ReturnValue and how much dynamic memory needed (counter) :
	for (i = 0; i < r->products_mount; i++)
	{
		if (ReturnValue < p->Count)
		{
			counter = 1;
			ReturnValue = p->Count;
		}
		else if (ReturnValue == p->Count)
			counter++;
		p = p->next;
	}
	if (ReturnValue)
	{
		s = (char **)malloc(counter * sizeof(char*));
		if (s == NULL) // checking the dynamic memory
		{
			free_all_memory(r);
			error_msg("Dynamic allocation error.");
		}
		// get the name(s) :
		p = r->head;
		i = 0;
		while (p != NULL)
		{
			if (ReturnValue == p->Count)
			{
				s[i] = (char *)malloc((strlen(p->Product_name) + 1) * sizeof(char));
				if (s[i] == NULL)// checking the dynamic memory
				{
					free_all_memory(r);
					error_msg("Dynamic allocation error.");
				}
				strcpy(s[i], p->Product_name);
				i++;
			}
			p = p->next;
		}
		*index = counter;
		*Max_Number = ReturnValue;
		return s;
	}
	else
		return NULL;
}
Boolean ScanKitchenQuantity(Restaurant *r)
{
	// Scanning all the products in the kitchen ; Checking if the kitchen does not empty
	Product *temp = r->head;
	int counter = 0;
	while (temp != NULL) // checking all the products
	{
		if (!(temp->Quantity)) // if the product run out => counter++
			counter++;
		temp = temp->next;
	}
	if (counter == r->products_mount) // if all the products run out => return Empty
		return Empty;
	else
		return Full;
}
// Functions for kitchen product (one way list struct) :
void AddTheItemToKichen(Restaurant *r, char *s, int Quantity)
{
	// Auxiliary function => Adding Quantity to the product with the the name *s
	Product *temp = r->head;
	// checking with Name_Correct if *s is exist in the kitchen :
	int flag = Name_Correct(r, s);
	if (flag)
		printf("\nThe product name doest not exits.");
	else
	{
		while ((strcmp(s, temp->Product_name))) // move temp to point the product
			temp = temp->next;
		temp->Quantity = temp->Quantity + Quantity; // add Quantity
	}
}
void Order_Products(Restaurant *r)
{
	// order the product in the kitchen by name
	// declaration :
	int flag;
	Product *temp1 = r->head->next;
	Product *temp2 = r->head;
	if (temp2 == NULL) // if the table is empty
		return;
	if (temp1 == NULL) // if there is only 1 product
		return;
	while (temp1 != NULL)
	{
		flag = 0;
		flag = strcmp(temp2->Product_name, temp1->Product_name); // checking who are biggest
		if (flag > 0)
			switch_products(r, temp2, temp1); // switch the products
		temp2 = temp2->next;
		temp1 = temp1->next;
	}
}
void switch_products(Restaurant *r, Product *H, Product *p2)
{
	// switch 2 product in the list. *H->next always pointing to *p2 
	Product *p = p2->next, temp = *H;
	*H = *p2;
	H->next = p2;
	*p2 = temp;
	p2->next = p;
}
int  Name_Correct(Restaurant *r, char *s)
{
	// checking if the requeist name is already exit
	// return 0 if the name exits in the products list
	// else => return 1 
	Product *temp = r->head;
	while (temp != NULL)
	{
		if (!strcmp(temp->Product_name, s))
			return 0;
		temp = temp->next;
	}
	return 1;
}
void PrintMenu(Restaurant *r)
{
	// print all the products in the kitchen :
	Product *temp = r->head;
	while (temp != NULL)
	{
		// print only the product with quantity (As requeist)
		if (temp->Quantity)
			printf("%s	%d$\n", temp->Product_name, temp->Price);
		temp = temp->next;
	}
}
void AddItems(Restaurant *r)
{
	// add quantity to product in the kitchen
	// declaration :
	char ch[99], *s, flag = 1;
	int  Quantity;
	printf("\nEnter the name of the product you wish to add to the kitchen :\n");
	// while the user input a correct product and quantity :
	while (flag)
	{
		// get the product name :
		scanf("%s", ch);
		s = (char*)malloc((strlen(ch) + 1) * sizeof(char));
		if (s == NULL)
		{
			free_all_memory(r);
			error_msg("Dynamic allocation error.");
		}
		strcpy(s, ch);
		if (Name_Correct(r, s)) // checking if the name exist in the kitchen
		{
			printf("Worng input. Please try again :\n");
			free(s);
		}
		else
		{
			// get the quantity :
			printf("How much %ss do you want to add ?\n", s);
			while (flag)
			{
				Quantity = GetQuantity();
				if (Quantity > 0) // if the quantity is correct :
				{
					AddTheItemToKichen(r, s, Quantity); // add the items to the kitchen
					flag = 0;
					free(s);
				}
				else
					printf("Worng number. Please try again :\n");
			}
		}
	}
}
// Functions for tables and product in each table :
void GetTableAmount(Restaurant *r)
{
	// get the mount of tables from the user
	// declaration :
	int flag = 0, flag2 = 1, i;
	char ch[99], *neto_ch;
	printf("\nBefore starting,\nPlease enter the number of tables in the restaurant :\n");
	// while untill the user input is correct :
	while (flag2)
	{
		// get the tables mount in string - checking if correct - change to int
		scanf("%s", ch);
		neto_ch = (char*)malloc((strlen(ch) + 1) * sizeof(char));
		if (neto_ch == NULL)
		{
			free_all_memory(r);
			error_msg("Dynamic allocation error.");
		}
		strcpy(neto_ch, ch);
		// checking the input :
		for (i = 0; i < strlen(neto_ch); i++)
			if (neto_ch[i] < '0' || neto_ch[i] > '9')
				flag = 1;
		// if worng input :
		if (flag || ch[0] == '0')
		{
			printf("\nWrong input. Please enter again :\n");
			flag = 0;
		}
		// if correct :
		else
		{
			r->tables_mount = atoi(neto_ch);
			flag2 = 0;
			free(neto_ch);
		}
	}
}
void AddTheItemToTable(Restaurant *r, Product *p, int TableIndex, int Quantity)
{
	// get name of product, number of table
	// create new product on the table
	ProductInTable *temp = (ProductInTable *)malloc(sizeof(ProductInTable)); // the new product
	if (temp == NULL) // checking the dynamic memory
	{
		free_all_memory(r);
		error_msg("Dynamic allocation error.");
	}
	// if the table is empty (initializing needed)
	if (r->arr[TableIndex - 1].Mode == Empty) // first meal
	{
		r->arr[TableIndex - 1].head = temp;
		r->arr[TableIndex - 1].tail = temp;
		temp->prev = NULL;
		temp->next = NULL;
		r->arr[TableIndex - 1].Mode = Full;
		r->arr[TableIndex - 1].sum_price = (p->Price)*Quantity;
	}
	// if the table is busy :
	else
	{
		temp->next = r->arr[TableIndex - 1].head;
		temp->next->prev = temp;
		temp->prev = NULL;
		r->arr[TableIndex - 1].head = temp;
		r->arr[TableIndex - 1].sum_price = r->arr[TableIndex - 1].sum_price + (p->Price)*Quantity;
	}
	// for the two cases :
	temp->Price = (p->Price);
	temp->HowMuchInTable = Quantity;
	temp->Product_name = (char *)malloc((strlen(p->Product_name) + 1) * sizeof(char));
	if (temp->Product_name == NULL) // checking the dynamic memory
	{
		free_all_memory(r);
		error_msg("Dynamic allocation error.");
	}
	strcpy(temp->Product_name, p->Product_name);
	temp->HowMuchInTable = Quantity;
	p->Count = p->Count + Quantity; // p->Count for the reports
}
Table* CreateTables(Restaurant *r)
{
	// Create all the table (after getting table amount)
	// declaration :
	int i;
	r->arr = (Table *)malloc((r->tables_mount) * sizeof(Table));
	if (r->arr == NULL)// checking dynamic memory
	{
		free_all_memory(r);
		error_msg("Dynamic allocation error.");
	}
	// Initializing the tables :
	for (i = 0; i < r->tables_mount; i++)
	{
		r->arr[i].Mode = Empty;
		r->arr[i].head = NULL;
		r->arr[i].tail = NULL;
		r->arr[i].Index = i + 1;
		r->arr[i].sum_price = 0;
	}
	return r->arr; // return the table array
}
void OrderItem(Restaurant *r)
{
	// get from the user the name and the quantity of the wanted product
	// make sure the input is correct and create a new node in the table[i] list (2 ways)
	// declaration :
	int flag = 1, flag2 = 0, TableIndex, Quantity;
	char char_index[99], ch, ProductName[99];
	Product *temp;
	ProductInTable *p;
	// get the table number :
	GetTableNumber(r, &TableIndex, 1);
	printf("\nThe menu of the kitchen products is :\n");
	PrintMenu(r);
	// get the product name :
	printf("\nWhat do you prefer to order ? \n");
	while (flag) // untill the input is correct
	{
		temp = r->head;
		scanf("%s", ProductName);
		flag2 = Name_Correct(r, ProductName); // checking if the name exist in the kitchen
		if (flag2) // if worng input :
			printf("\nIncorrect name. Please try again : ");
		else
		{
			while (strcmp(temp->Product_name, ProductName)) // till temp pointing to the product with the same name
				temp = temp->next;
			if (temp->Quantity) // checking if  product did not run out 
			{
				// get the mount of product to order
				printf("\nHow much %ss do you want ?\n", ProductName);
				while (flag) // till the input is correct
				{
					Quantity = GetQuantity();
					if (Quantity <= (temp->Quantity) && Quantity > 0) // if correct
					{
						temp->Quantity = temp->Quantity - Quantity;
						flag = 0;
					}
					else
						printf("\n**Worning - The mount of %ss in the kitchen is only %d.\nChoose again : ", ProductName, temp->Quantity);
				}
				// checkig if the product is already ordered to the table 
				flag2 = AreProductInTable(r, TableIndex, ProductName);
				if (flag2) // if exists => just add quantity
				{
					p = r->arr[TableIndex - 1].head;
					while (strcmp(p->Product_name, ProductName))
						p = p->next;
					(p->HowMuchInTable) = (p->HowMuchInTable) + Quantity;
					temp->Count = temp->Count + Quantity;
				}
				else // else => add the item to the table
					AddTheItemToTable(r, temp, TableIndex, Quantity);
			}
			else // if quantity == 0
				printf("\The product (%s) run out !!\n", temp->Product_name);
			// for more ordering :
			if (ScanKitchenQuantity(r))
			{
				printf("\nSomething else ? (Enter 'Y' for yes; 'N' for no)\n");
				ch = _getche();
				if (ch == 'Y' || ch == 'y')
				{
					printf("\nWhat do you prefer to eat ?\n");
					flag = 1;
				}
			}
			else
				flag = 0;
		}
	}
}
void RemoveItem(Restaurant *r)
{
	// Canceling item in table[i] ; Changing the price in the table[i]
	// declaration :
	int i, flag = 1, flag2 = 0, TableIndex, Quantity;
	char char_index[99], ch, ProductName[99];
	ProductInTable *temp;
	Product *p;
	// if the restuarant is not empty :
	if (CheckingTableMode(r))
	{
		GetTableNumber(r, &TableIndex, 0); // get the table index from the user
										   // get the product name :
		printf("\nWhat do you prefer to remove from table[%d] ? \n", TableIndex);
		while (flag) // till input is correct :
		{
			temp = r->arr[TableIndex - 1].head; // point to table[] head
			p = r->head; // point to the kitchen list head
			scanf("%s", ProductName);
			if (!(AreProductInTable(r, TableIndex, ProductName))) // checking if the product exists in the table
				printf("\nIncorrect name. Please try again : ");
			else // if correct
			{
				flag2 = 0;
				while (strcmp(temp->Product_name, ProductName)) //till temp pointing to the product with the same name
					temp = temp->next;
				while (strcmp(p->Product_name, ProductName)) //till p pointing to the product with the same name
					p = p->next;
				if (temp->HowMuchInTable) // Check if the product has not been returned
				{
					// get quantity 
					printf("\nHow much %ss do you want to remove ?\n", ProductName);
					while (flag) // till the user input is correct
					{
						Quantity = GetQuantity();
						if (Quantity <= (temp->HowMuchInTable) && Quantity > 0) // if correct
						{
							temp->HowMuchInTable = temp->HowMuchInTable - Quantity;
							p->Count = p->Count - Quantity;
							r->arr[TableIndex - 1].sum_price = r->arr[TableIndex - 1].sum_price - Quantity*(temp->Price);
							flag = 0;
						}
						else
							printf("Worng input. There are only %d %s(s) in the table.\n Please try again :  ", temp->HowMuchInTable, temp->Product_name);
					}
				}
				else
				{
					printf("The product has already been removed !!\n");
					flag = 0;
				}
			}
		}
	}
	else
		printf("\n**CafeCafe is still empty !!**\n");
}
int AreProductInTable(Restaurant *r, int index, char *s)
{
	// return 1 => if the product is in the table
	// else => return 0
	ProductInTable *temp = r->arr[index - 1].head;
	while (temp != NULL)
	{
		if (strcmp(temp->Product_name, s) == 0)
			return 1;
		else
			temp = temp->next;
	}
	return 0;
}
void RemoveTable(Restaurant *r)
{
	// free all the products in the table
	//declaration :
	int TableIndex, flag = 1, i = 1;
	ProductInTable *temp;
	if (CheckingTableMode(r))
	{
		while (flag) // till the user input is correct
		{
			GetTableNumber(r, &TableIndex, 0);// get the number of the table
			temp = r->arr[TableIndex - 1].head;
			flag = 0;
			// print the billing ; free the table :
			printf("\n** The billing is : **\n");
			while (r->arr[TableIndex - 1].head != NULL)
			{
				// print all the products
				printf("%d) %s,	Acquired : %d, Price per unit : %d$, Total = (%d$)\n", i++, temp->Product_name, temp->HowMuchInTable, temp->Price, (temp->Price)*(temp->HowMuchInTable));
				// free this product from the table
				r->arr[TableIndex - 1].head = temp->next;
				temp->next = NULL;
				free(temp->Product_name);
				free(temp);
				temp = r->arr[TableIndex - 1].head;
			}
			// initializing the table
			r->arr[TableIndex - 1].tail = NULL;
			printf("The total price is :                                          %d$", r->arr[TableIndex - 1].sum_price);
			r->arr[TableIndex - 1].Mode = Empty;
			r->arr[TableIndex - 1].sum_price = 0;
		}
	}
	else
		printf("\nThe restaurant is empty !!\n");
}
void GetTableNumber(Restaurant *r, int *Return_index, int Mode)
{
	// Auxiliary function => get from the user the table index
	// checking if the input is correct
	// return the index in *Return_index
	// Mode {0,1} => 0 for RemoveItem, RemoveTable ; 1 for OrderItem
	// declaration :
	int i, flag = 1, flag2 = 0, TableIndex;
	char char_index[99], *Index;
	printf("\nEnter the number of the table :\n");
	while (flag) // till the input is correct
	{
		scanf("%s", char_index);
		Index = (char*)malloc((strlen(char_index) + 1) * sizeof(char));
		if (Index == NULL) // checking the dynamic memory
		{
			free_all_memory(r);
			error_msg("Dynamic allocation error.");
		}
		strcpy(Index, char_index); // copy the input
		for (i = 0; i < strlen(Index); i++) // checking every bite
			if (Index[i]<'0' || Index[i]>'9')
				flag2 = 1; // not a number
		if (!flag2) // if a number
		{
			TableIndex = atoi(Index); // get the int number
			if (TableIndex <= r->tables_mount && TableIndex > 0)// checking if correct
			{
				if (Mode) // If the table mode does not improtant
				{
					flag = EmptyTable(r, TableIndex);
					if (flag)
						printf("\n**Worning - Table[%d] - Is busy**\n", TableIndex);
					flag = 0;
				}
				else // if the table must be busy (RemoveTable + RemoveItem)
				{
					printf("Checking if table number %d is busy .. ", TableIndex);
					flag = 1 - EmptyTable(r, TableIndex);
					if (flag)
						printf("No.\nThis table is empty. Please choose anther table : ");
				}
				*Return_index = TableIndex; // the return value
			}
			else
				printf("\nWorng number. Please try again :\n");
		}
		else
		{
			printf("\nWorng input. please try again :\n");
			flag2 = 0;
		}
		free(Index);
	}
}
Boolean EmptyTable(Restaurant *r, int index)
{
	// check if the table in the requeist index is empty
	// return 1 => full; 0 => empty
	if (r->arr[index - 1].Mode)
		return Full;
	return Empty;
}
Boolean CheckingTableMode(Restaurant *r)
{
	// checking all the tables in the restuarant
	// if all empty => return Empty ; else - Full
	int i;
	for (i = 0; i < r->tables_mount; i++)
	{
		if (r->arr[i].Mode)
			return Full;
	}
	return Empty;
}
// Auxiliary functions :
void error_msg(char* msg)
{
	// error for the user and exit the program
	printf("\n%s", msg);
	exit(1);
}
int GetQuantity()
{
	// Helping to get input from the user without error
	// return the input from the user ; else - return 0
	char char_quantity[99];
	int i;
	if (scanf("%s", char_quantity))
	{
		for (i = 0; i < strlen(char_quantity); i++)
		{
			if (char_quantity[i]<'0' || char_quantity[i]>'9')
				return 0;
		}
		return atoi(char_quantity);
	}
}