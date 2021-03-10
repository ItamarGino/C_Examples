#include "Header.h"

void main()
{
	// declaration :
	int flag = 1, ShowMenu = 1;
	char c;
	Restaurant *CafeCafe = CreateProduct(); // get the products from Manot.txt + table mount from user
	Table *array = CreateTables(CafeCafe); // Initializing and get memory for the table array
	printf("\n**Wellcome to CafeCafe !! enjoy your meal !!\n");
	while (flag) // till the user press 'X','x' for exit
	{
		if (ScanKitchenQuantity(CafeCafe))
		{
			if (ShowMenu) // this menu shows only in the start. Showing again by pressing => 'R','r'
			{
				printf("\n\nFor order items for a table => press '1'\n");
				printf("For removing an item in a tabel => press '2'\n");
				printf("For removing table (closing a table) => press '3'\n");
				printf("For adding products in the kitchen => press '4'\n");
				printf("Reports (A,B,C) => press '9'\n");
				printf("For closing CafeCafe => press 'X'\n");
				printf("For repeating this menu => press 'R'\n\n");
				ShowMenu = 0;
			}
			c = _getche(); // get the digit from user
			switch (c)
			{
				// Order Item to table :
			case '1':
				OrderItem(CafeCafe);
				printf("\n*Finish.\n");
				break;
				// Remove item :
			case '2':
				RemoveItem(CafeCafe);
				printf("\n*Finish.\n");
				break;
				// Table billing + Total price + free all the product
			case '3':
				RemoveTable(CafeCafe);
				printf("\n*Finish.\n");
				break;
			case '4': // adding item to CafeCafe's kitchen (AddItems)
				AddItems(CafeCafe);
				printf("\n*Finish.\n");
				break;
			case '9':
				Report(CafeCafe);
				printf("\n*Finish.\n");
				break;
			case 'x':case 'X':
				free_all_memory(CafeCafe);
				flag = 0;
				break;
			case 'r':case 'R':
				ShowMenu = 1;
				break;
			default:
				printf("\nWorng input. Try again =>");
				break;
			}
		}
		else
		{
			printf("\n**Worning**\nAll the products in the kitchen run out !!\n");
			printf("\nFor adding products in the kitchen => press '1'\n");
			printf("\nFor closing CafeCafe => press the rest keys\n");
			c = _getche();
			if (c == '1')
			{
				AddItems(CafeCafe);
				ShowMenu = 1;
			}
			else
			{
				free_all_memory(CafeCafe);
				flag = 0;
			}
		}
	}
	return 0;
}