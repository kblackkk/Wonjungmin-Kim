//  -------------------------------------------------
//  Name:       Wonjunmin Kim
//  Email:      kick.kblack@gmail.com
//  Workshop:   MS04
//  Date:       2017-08-08
//  Section:    LL
//  -------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define LINEAR 1 // ms2
#define FORM 0 //ms2
#define STOCK 1 //ms3
#define CHECKOUT 0 //ms3
// #define MAX_ITEM_NO 21 //ms3
#define MAX_QTY 999 //ms3
#define SKU_MAX 999 //ms3
#define SKU_MIN 100 //ms3
#define MAX_ITEM_NO 500 // ms4
#define DATAFILE "144_fp_items.txt" //ms5

// ---------------------------------------
// place function PROTOTYPES below here...
// ---------------------------------------

// struct
struct Item {
	double price;
	int sku;
	int isTaxed;
	int quantity;
	int minQuantity;
	char name[21];
};

// user interface tools function PROTOTYPES:
//ms1
void welcome(void);
void printTitle(void);
void printFooter(double gTotal);
void flushKeyboard(void);
void pause(void);
int getInt(void);
int getIntLimited(int lowerLimit, int upperLimit);
double getDouble(void);
double getDoubleLimited(double lowerLimit, double upperLimit);
// void error(void); // delete because of new line


// ms2
struct Item itemEntry(int sku); // error? change the place?
double totalAfterTax(struct Item item);
int isLowQuantity(struct Item item);
void displayItem(struct Item item, int linear);
void listItems(const struct Item item[], int noOfItems);
int locateItem(const struct Item item[], int NoOfRecs, int sku, int* index);


// ms3
void search(const struct Item item[], int NoOfRecs);
void updateItem(struct Item* itemptr);
void addItem(struct Item item[], int *NoOfRecs, int sku);
void addOrUpdateItem(struct Item item[], int* NoOfRecs);
void adjustQuantity(struct Item item[], int NoOfRecs, int stock);


//ms4
void saveItem(struct Item item, FILE* dataFile);
int loadItem(struct Item* item, FILE* dataFile);
int saveItems(const struct Item item[], char fileName[], int NoOfRecs);
int loadItems(struct Item item[], char fileName[], int* NoOfRecsPtr);


// application user interface function PROTOTYPES:
int yes(void);
int menu(void);

//ms 5
void GroceryInventorySystem(void);

// global scope
const double TAX = 0.13; // only tax
 // if with the original price + 1

int main() { //add ms5
	GroceryInventorySystem();
	return 0;
}

void welcome(void) {
	printf("---=== Grocery Inventory System ===---\n\n");
}

void printTitle(void) {
	printf("Row |SKU| Name               | Price  |Taxed| Qty | Min |   Total    |Atn\n");
	printf("----+---+--------------------+--------+-----+-----+-----+------------|---\n");
}

void printFooter(double gTotal) {
	printf("--------------------------------------------------------+----------------\n");
	if (gTotal > 0) {
		printf("                                           Grand Total: |%12.2lf\n", gTotal);
	}
}

void flushKeyboard(void) { //inpu buffer	
	while (getchar() != '\n');
}

void pause(void) {
	printf("Press <ENTER> to continue...");
	flushKeyboard();
}

int getInt(void) {
	char NL = 'x';
	int value = 0;
	while (NL != '\n') {
		scanf("%d%c", &value, &NL);
		if (NL != '\n') {
			flushKeyboard();
			printf("Invalid integer, please try again: ");
		} //if
	} // while 
	return value;
} //func

int getIntLimited(int lowerLimit, int upperLimit) {

	int value = 0;
	do {
		value = getInt();
		if (value < lowerLimit || value > upperLimit) {
			printf("Invalid value, %d < value < %d: ", lowerLimit, upperLimit);
		}
	} while (value < lowerLimit || value > upperLimit);
	return value;
}

double getDouble(void) {
	char NL = 'x';
	double value = 0;

	while (NL != '\n') {
		scanf("%lf%c", &value, &NL);
		if (NL != '\n') {
			flushKeyboard();
			printf("Invalid number, please try again: ");
		} //if
	} // while 
	return value;
} //func

double getDoubleLimited(double lowerLimit, double upperLimit) {
	double value = 0;
	do {
		value = getDouble();
		if (value < lowerLimit || value > upperLimit) {
			printf("Invalid value, %lf< value < %lf: ", lowerLimit, upperLimit);
		}

	} while (value < lowerLimit || value > upperLimit);
	return value;
}


int yes(void) {

	char userInput;
	int result = 0;
	int flag = 1;
	do {
		// scanf("%c", &userInput);
		userInput = getchar();

		if (userInput == 'Y' || userInput == 'y') {
			flushKeyboard();
			flag = 0;
			result = 1;
		}

		else if (userInput == 'N' || userInput == 'n') {
			flushKeyboard();
			flag = 0;
			result = 0;
		}
		else {
			flag = 1;
			flushKeyboard();
			printf("Only (Y)es or (N)o are acceptable: ");
		}
	} while (flag == 1);
	return result;
}

int menu(void) {

	int value = 0;

	printf("1- List all items\n");
	printf("2- Search by SKU\n");
	printf("3- Checkout an item\n");
	printf("4- Stock an item\n");
	printf("5- Add new item or update item\n");
	printf("6- delete item\n");
	printf("7- Search by name\n");
	printf("0- Exit program\n");
	printf("> ");

	value = getIntLimited(0, 7); // compare the user option and return the values

	return value;

}

void GroceryInventorySystem(void) { //add more at ms5

	int userInput = 0;
	int result = 0;
	int records = 0; // ms5
	int totalResult = 0;
	int resultQ = 0;
	struct Item item[MAX_ITEM_NO]; //ms5

	welcome();
	records = loadItems(item, DATAFILE, &totalResult); //check t or f 
	// int loadItems(struct Item item[], char fileName[], int* NoOfRecsPtr);

	if (records != 0) { // T

		do {
			userInput = menu();
			switch (userInput)
			{

			case 1:
				// printf("List Items under construction!\n");
				listItems(item, totalResult);
				pause();
				break;

			case 2:
				// printf("Search Items under construction!\n");
				search(item, totalResult);
				pause();
				break;

			case 3:
				// printf("Checkout Item under construction!\n");
				adjustQuantity(item, totalResult, CHECKOUT);
				resultQ = saveItems(item, DATAFILE, totalResult);
				
				// too long............
				//if (resultQ == 0) {
				//	printf("Could not update data file %s\n", DATAFILE); //F
				//}
				//else {
				//	resultQ = 1; //T
				//}
				resultQ ? 1:printf("Could not update data file %s\n", DATAFILE);
				pause();
				break;

			case 4: //same with case 3
				// printf("Stock Item under construction!\n");
				// but pass STOCK to adjustQuantity instead of CHECKOUT.
				adjustQuantity(item, totalResult, STOCK); // only this change
				resultQ = saveItems(item, DATAFILE, totalResult);
				resultQ ? 1:printf("Could not update data file %s\n", DATAFILE);
				pause();
				break;

			case 5:
				// printf("Add/Update Item under construction!\n");
				addOrUpdateItem(item, &totalResult);
				resultQ = saveItems(item, DATAFILE, totalResult);
				resultQ ? 1 : printf("Could not update data file %s\n", DATAFILE);
				pause();
				break;

			case 6: // nothing
				printf("Not implemented!\n");
				pause();
				break;

			case 7: // nothing
				printf("Not implemented!\n");
				pause();
				break;

			case 0:
				printf("Exit the program? (Y)es/(N)o): ");
				result = yes(); // yes -> 1 return, no-> 0 return
				break;
			}
		} while (result == 0); // escape without return value
	}
    else {
		printf("Could not read from %s.\n", DATAFILE);
	}
}


// ms02
double totalAfterTax(struct Item item) {

	double totalAmount = 0;
	if (item.isTaxed == 1)
		totalAmount = (item.price * item.quantity) * (1 + TAX); // tax + cost = totla;
	// totalAmount = (item.price * item.quantity) * TAX; // only tax
	else
		totalAmount = (item.price * item.quantity);
	return totalAmount;

}


int isLowQuantity(struct Item item) {

	int result = 0;
	if (item.quantity <= item.minQuantity) { // currently < min
		result = 1;
	}

	return result;
}

struct Item itemEntry(int sku) {

	struct Item enterItem;

	printf("        SKU: %d\n", sku);
	enterItem.sku = sku;
	printf("       Name: ");
	scanf("%20[^\n]", enterItem.name);
	flushKeyboard();
	printf("      Price: ");
	enterItem.price = getDouble();
	printf("   Quantity: ");
	enterItem.quantity = getInt();
	printf("Minimum Qty: ");
	enterItem.minQuantity = getInt();
	printf("   Is Taxed: ");
	enterItem.isTaxed = yes();
	return enterItem;

}


void displayItem(struct Item item, int linear) {

	double totalPrice;
	totalPrice = totalAfterTax(item);

	if (linear == LINEAR) {
		if (item.isTaxed == LINEAR)
			printf("|%3d|%-20s|%8.2lf|  Yes| %3d | %3d |%12.2lf|",
				item.sku, item.name, item.price, item.quantity, 
				item.minQuantity, totalPrice);
		else
			printf("|%3d|%-20s|%8.2lf|   No| %3d | %3d |%12.2lf|",  // without tax
				item.sku, item.name, item.price, item.quantity, 
				item.minQuantity, totalPrice);

		if (isLowQuantity(item) == LINEAR)
			printf("***\n"); // pdf check
		else
			printf("\n");
	}
	else {
		printf("        SKU: %d\n", item.sku);
		printf("       Name: %s\n", item.name);
		printf("      Price: %.2lf\n", item.price);
		printf("   Quantity: %d\n", item.quantity);
		printf("Minimum Qty: %d\n", item.minQuantity);

		if (item.isTaxed == LINEAR) {
			printf("   Is Taxed: Yes\n");
		}
		else {
			printf("   Is Taxed: No\n");
		}
		if (isLowQuantity(item) == LINEAR) {
			printf("WARNING: Quantity low, please order ASAP!!!\n");
		}
	}
}

void listItems(const struct Item item[], int noOfItems) {
	// 144_ms2.c:40:34: error: array type has incomplete element type
	int i = 0;
	double gTotal = 0;
	printTitle();

	for (i = 0; i < noOfItems; i++) {
		printf("%-4d", i + 1);
		displayItem(item[i], 1);
		gTotal += totalAfterTax(item[i]); // total price
	}
	printFooter(gTotal);
}

int locateItem(const struct Item item[], int NoOfRecs, int sku, int* index) {

	int i = 0;
	int value = 0;
	int flag = 1;
	for (i = 0; i < NoOfRecs && flag == 1; i++) {
		if (sku == item[i].sku) { // when it found point to the index
			value = 1;
			// 144_ms2.c:40:34: error: array type has incomplete element type
			*index = i;
			flag = 0;
		}
	}
	return value;
}


void search(const struct Item item[], int NoOfRecs) {

	int result = 0;
	int index = 0;
	printf("Please enter the SKU: ");
	result = getIntLimited(SKU_MIN, SKU_MAX); //compare 

	if (locateItem(item, NoOfRecs, result, &index) != 0) {
		// int locateItem(const struct Item item[], int NoOfRecs, int sku, int* index);
		displayItem(item[index], FORM);
	}
	else {
		printf("Item not found!\n");
	}
}


void updateItem(struct Item* itemptr) {
	struct Item item;
	printf("Enter new data:");
	printf("\n");
	//printf("\n\n");
	item = itemEntry(itemptr->sku); // if the same sku
	printf("Overwrite old data? (Y)es/(N)o: "); // update

	if (yes()) { // if user yes,
		*(itemptr) = item;
		printf("--== Updated! ==--");
		printf("\n");
		// printf("\n");
	}
	else {
		printf("--== Aborted! ==--\n");
	}
}



void addItem(struct Item item[], int *NoOfRecs, int sku) {
	
	struct Item itemAdd;

	if (*NoOfRecs == MAX_ITEM_NO) {
		printf("Can not add new item; Storage Full!\n");
	}

	else {
		itemAdd = itemEntry(sku); // more info need
		printf("Add Item? (Y)es/(N)o: ");

		if (yes()) {
			item[*NoOfRecs] = itemAdd; // records add 
			(*NoOfRecs)++;
			printf("--== Added! ==--");
			// printf("\n\n");
		}
		else {
			printf("--== Aborted! ==--");
		}
		printf("\n");
	}
}

void addOrUpdateItem(struct Item item[], int* NoOfRecs) {

	int result = 0;
	int index = 0;

	printf("Please enter the SKU: ");
	result = getIntLimited(SKU_MIN, SKU_MAX);

	if (locateItem(item, *NoOfRecs, result, &index) == 1) {
		displayItem(item[index], FORM);
		printf("Item already exists, Update? (Y)es/(N)o: ");

		if (yes()) {
			updateItem(&item[index]);
		}
		else {
			printf("--== Aborted! ==--");
			printf("\n");
		}
	}
	else
		addItem(item, &(*NoOfRecs), result); 
		 //Call the addItem() function to add the item with the entered SKU at the end of the data in the array.
}



void adjustQuantity(struct Item item[], int NoOfRecs, int stock) {

	int result = 0;
	int index = 0;
	int amount = 0;
	int max = 0;

	printf("Please enter the SKU: ");
	result = getIntLimited(SKU_MIN, SKU_MAX);

	if (locateItem(item, NoOfRecs, result, &index) == 1) {
		displayItem(item[index], FORM); // the same

		if (stock == STOCK) {
			max = MAX_QTY - item[index].quantity;
			printf("Please enter the quantity to stock; Maximum of %d or 0 to abort: ", max);
		}
		else {
			max = item[index].quantity;
			printf("Please enter the quantity to checkout; Maximum of %d or 0 to abort: ", max);
		}

		amount = getIntLimited(0, max);


		if (amount == 0) {
			printf("--== Aborted! ==--");
			printf("\n");
		}

		if (amount != 0) {

			if (stock == STOCK) {
				item[index].quantity += amount;
				printf("--== Stocked! ==--\n");
			}
			// %s should be replaced by >to stock< and %d
			else {
				item[index].quantity -= amount;
				printf("--== Checked out! ==--\n");
			}

			if (isLowQuantity(item[index]) == 1) {
				printf("Quantity is low, please reorder ASAP!!!\n");
			} 
		}
	}
	else {
		printf("SKU not found in storage!");
		printf("\n");
	}

}


void saveItem(struct Item item, FILE* dataFile) {

	if (dataFile != NULL) {
		fprintf(dataFile, "%d,%d,%d,%.2lf,%d,%s\n", 
			               item.sku,item.quantity,item.minQuantity,
			               item.price, item.isTaxed, item.name); // comma separated
	 // order sku,quantity,minQuantity,price,isTaxed,name<NEWLINE>
	}
}

int loadItem(struct Item* item, FILE* dataFile) {

	int result = 0; //f
	if (dataFile != NULL) {
		if (fscanf(dataFile, "%d,%d,%d,%lf,%d,%20[^\n]",
			&item->sku, &item->quantity, &item->minQuantity,
			&item->price, &item->isTaxed, item->name) == 6) {
			// six ','
			result = 1; //t
		}
}
	return result;
}

int saveItems(const struct Item item[], char fileName[], int NoOfRecs) {

	FILE * fp = fopen(fileName, "w"); // overwrite, I'm not sure
	int result, i = 0;

	if (fp != NULL) {
		for (i = 0; i < NoOfRecs; i++) {
			saveItem(item[i], fp);
		}
		fclose(fp);
		result = 1;
	}
	return result;
}

int loadItems(struct Item item[], char fileName[], int* NoOfRecsPtr) {

	FILE *fp = fopen(fileName, "r");

	int i, result = 0;
	int NoOfRecs = 0;
	if (fp != NULL) {
		while (loadItem(item, fp)) {
			NoOfRecs++;
		}
		
		rewind(fp);

		for (i = 0; i < NoOfRecs; i++) {
			loadItem( &item[i], fp);
		}
		*NoOfRecsPtr = NoOfRecs;
		fclose(fp);
		result = 1;
	}
	return result;
}
