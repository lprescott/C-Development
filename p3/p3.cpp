/*
Name: Luke R. Prescott
Unix login id: LP911211
Lab instructor: Ji Yong
Date and time of lab: Fridays at 10:25am
*/

#include <stdio.h> //input/output header 
#include <stdlib.h>
#include <string.h>

/* 
Description: This C program prompts the user for commands, accepts commands from the keyboard (stdin) 
and executes those commands. When a command requires output, it is written to stdout. The program continues 
to accept and process commands until the user types the end command. 

The program deals with linked lists. Each node of such a list contains a string of length at most 10, a 
positive integer (i.e., an integer value ≥ 1) and a pointer to the next node of the list. For any node, the 
string and the integer stored in that node will be referred to as the symbol and count for that node 
respectively. At all times, the list must satisfy the following two important properties:

	1.	The symbols appearing in the list are all distinct; that is, no two nodes have the same symbol.
	2.	When the list is scanned from left to right, the counts must be in non-increasing order. An example 
	    of such a linked list is shown below.
*/

#define SIZE_OF_CMD 4
#define SIZE_OF_BOTH 14

/* Structure for each node in the linked list. */
struct node {
	char symbol[11];
	unsigned int count;
	struct node * next;
};

typedef struct node Node; // Synonym for the struct node
typedef Node * NodePtr; // Synonym for Node *

/* Prototypes for the functions used. */	
int isEmpty(NodePtr nPtr);
Node * searchString(NodePtr nPtr, char string[]);
Node * searchCount(NodePtr nPtr, int value);

void insert(NodePtr * nPtr, char *symbol);
void delete(NodePtr * nPtr, char string[]);
void forceDelete(NodePtr * nPtr, int value);

void printStatistics(NodePtr nPtr);
void printList(NodePtr nPtr);
void printRange(NodePtr nPtr, int value1, int value2);
void printPrefix(NodePtr nPtr, char string[]);
void printSuffix(NodePtr nPtr, char string[]);

void swap(struct node *a, struct node *b);



int main(){
	/* Preamble */
	char commandAndArgs[SIZE_OF_BOTH];
	char command[SIZE_OF_CMD];
	
	char argument1String[11], argument2String[11];
	int argument1, argument2;

	NodePtr headPtr = NULL;

	/* Initial request for input */
	puts("Please enter a command in the format: command (value) (value).");
	puts("The commands are: ins, del, fde, pst, prl, pcr, ppr and psu.");
	fgets(commandAndArgs, SIZE_OF_BOTH, stdin);

	/* Take first string of input*/
	sscanf(commandAndArgs, "%s %s %s", command, argument1String, argument2String);

	/* While the input isn't "end" execute correct methods.*/
	while (strcmp(command, "end") != 0){
		/* Insert command */
		if (strcmp(command, "ins") == 0){
			insert(&headPtr, argument1String);
			printf("\nThe insert command was entered. \"%s\" has been inserted or incremented.", argument1String);
			fflush(stdout);
		}

		/* Delete command */
		else if (strcmp(command, "del") == 0){
			delete(&headPtr, argument1String);
			printf("\nThe delete command was entered. \"%s\" has been be deleted or decremented.", argument1String);
			fflush(stdout);
			
		}

		/* Force-delete command */
		else if (strcmp(command, "fde") == 0){
			sscanf(argument1String, "%d", &argument1);
			forceDelete(&headPtr, argument1);
			printf("\nThe force delete command was entered. Nodes with a count of %d or less have been deleted.", argument1);
			fflush(stdout);

		}
	
		/* Print-stats command */
		else if (strcmp(command, "pst") == 0){
			puts("\nThe prints statistics command was entered.");
			printStatistics(headPtr);
		}
		
		/* Print-list command */
		else if (strcmp(command, "prl") == 0){
			puts("\nThe print list command was entered.");
			printList(headPtr);
		}
		
		/* Print-count-range command */
		else if (strcmp(command, "pcr") == 0){
			sscanf(argument1String, "%d", &argument1);
			sscanf(argument2String, "%d", &argument2);
			printf("\nThe print using count range command was entered. Nodes with a count between %d and %d will be printed.", argument1, argument2);
			fflush(stdout);

			printRange(headPtr, argument1, argument2);
		}
		
		/* Print-prefix command */
		else if (strcmp(command, "ppr") == 0){
			printf("\nThe print prefix command was entered. Nodes with the prefix \"%s\" will be printed.", argument1String);
			fflush(stdout);

			printPrefix(headPtr, argument1String);
		}
		
		/* Print-suffix command */
		else if (strcmp(command, "psu") == 0){
			printf("\nThe print suffix command was entered. Nodes with the suffix \"%s\" will be printed.", argument1String);
			fflush(stdout);

			printSuffix(headPtr, argument1String);
		}

		/* Unknown command */
		else {
			puts("\nAn unknown command was entered. Type \"end\" to stop.");
		}

		/* Request for input */
		puts("\nPlease enter another command in the format: command (value) (value) or end.");
		fgets(commandAndArgs, SIZE_OF_BOTH, stdin);

		/* Take the string of input */
		sscanf(commandAndArgs, "%s %s %s", command, argument1String, argument2String);
	}

	return 0;
} //end of main

/* This method returns 1 if the linked-list is empty, 0 if not. */
int isEmpty(NodePtr nPtr){
	if (nPtr == NULL) 
		return 1;
	else return 0;
} //end of isEmpty

/* This method returns the location of a searched node (key=symbol) in the linked-list, null if none. */
Node * searchString(NodePtr nPtr, char string[]) {
	Node * temp;
	
	if (nPtr == NULL) 
		return NULL;

	if (strcmp(nPtr->symbol, string) == 0)
		return nPtr;
	
	return searchString(nPtr->next, string);
} //end of search

/* This method returns the location of a searched node (key=count) in the linked-list, null if none. */
Node * searchCount(NodePtr nPtr, int value) {
	Node * temp;

	if (nPtr == NULL)
		return NULL;

	if (nPtr->count == value)
		return nPtr;

	return searchCount(nPtr->next, value);
} //end of search

/* 
This insert method takes a pointer to the head node of a linked-list and the pointer to a string to insert as parameter.
The method then either increments the count of a preexisiting node with the same string or creates a new node
at the end of the list. If the method is incrementing the count of a node, that node will be sorted in non-
increasing order.
*/

void insert(NodePtr * nPtr, char *symbol) {
	struct node *temp = *nPtr; //Temp node is needed to traverse the list for incrementing existing nodes

	// While the temp node is not NULL comapare symbols and increment count if needed, return, after sort
	while (temp) {
		if (strcmp(temp->symbol, symbol) == 0) {
			temp->count += 1;

			// Sort the list here. 
			int swapped, i;
			struct node *ptr1;
			struct node *lptr = NULL;

			if (ptr1 == NULL)
				return;

			do
			{
				swapped = 0;
				ptr1 = nPtr;

				while (ptr1->next != lptr)
				{
					if (ptr1->count < ptr1->next->count)
					{
						struct node *a = ptr1;
						struct node *b = ptr1->next;

						NodePtr swapNode;
						strCpy(swapNode->symbol, a->symbol);
						swapNode->count = a->count;

						strCpy(a->symbol, b->symbol);
						a->count = b->count;

						strCpy(b->symbol, swapNode->symbol);
						b->count = swapNode->count;
						swapped = 1;
					}
					ptr1 = ptr1->next;
				}
				lptr = ptr1;
			} while (swapped);
			
		}
		temp = temp->next;
	}

	NodePtr newNode; //pointer for the node to be inserted
	NodePtr previousNode; //pointer for the previous node
	NodePtr currentNode; //pointer for the current node

	//allocate memory for the new node
	newNode = malloc(sizeof(Node));

	//if there is mem available
	if (newNode != NULL) { 
		//add data to new node
		strcpy(newNode->symbol, symbol);
		newNode->count = 1; 
		newNode->next = NULL; 

		//set current node to location in linked list
		previousNode = NULL;
		currentNode = *nPtr;

		//while the list continues find the correct location for the new node
		while (currentNode != NULL && currentNode->count > newNode->count - 1) {
			previousNode = currentNode; 
			currentNode = currentNode->next; 
		} 
		 
		//assign pointers
		if (previousNode == NULL) {
			newNode->next = *nPtr;
			*nPtr = newNode;
		} 

		//assign pointers cont'd
		else { 
			previousNode->next = newNode;
			newNode->next = currentNode;
		} 
	} 
	//If there is no mem available
	else puts("No memory available.\n");

} //end of insert method

void delete(NodePtr * nPtr, char string[]) {
	if (isEmpty(nPtr) == 1) {
		/* The list is empty */
	}
	else {
		/* The list is not empty */

	}
}

void forceDelete(NodePtr * nPtr, int value) {
	if (isEmpty(nPtr) == 1) {
		/* The list is empty */
	}
	else {
		/* The list is not empty */
	}
}

void printStatistics(NodePtr nPtr) {
	if (isEmpty(nPtr) == 1) {
		puts("The list is empty.");
	}
	else {
		//TODO
	}
}

void printList(NodePtr nPtr) {
	
	if (isEmpty(nPtr) == 1) {
		puts("The list is empty.");
	}
	else {
		NodePtr currentNode = nPtr;
		puts("The linked-list is: ");
		while (currentNode != NULL) {
			printf("\n%s:%d", currentNode->symbol, currentNode->count);
			fflush(stdout);

			currentNode = currentNode->next;
		}
		puts("\nNULL\n");
	}
}

void printRange(NodePtr nPtr, int value1, int value2) {
	if (isEmpty(nPtr) == 1) {
		puts("The list is empty.");
	}
	else {
		//TODO
	}
}

void printPrefix(NodePtr nPtr, char string[]) {
	if (isEmpty(nPtr) == 1) {
		puts("The list is empty.");
	}
	else {
		//TODO
	}
}

void printSuffix(NodePtr nPtr, char string[]) {
	if (isEmpty(nPtr) == 1) {
		puts("The list is empty.");
	}
	else {
		//TODO
	}
}

/* function to swap data of two nodes a and b*/
void swap(struct node *a, struct node *b)
{
	NodePtr swapNode;
	strCpy(swapNode->symbol, a->symbol);
	swapNode->count = a->count;

	strCpy(a->symbol, b->symbol);
	a->count = b->count;

	strCpy(b->symbol, swapNode->symbol);
	b->count = swapNode->count;
}
