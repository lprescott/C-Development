/*
Name: Luke R. Prescott
Unix login id: LP911211
Lab instructor: Ji Yong
Date and time of lab: Fridays at 10:25am
*/

#include <stdio.h> //input/output header 
#include <stdlib.h>


#define SIZE 16

struct node {
	char symbol[10];
  unsigned count x;
  struct node *next;
};

int main()
{
	char commandAndArg[];
	char command[];
	char argument[];
	
	int position;
	int positionOfWhiteSpace;
	
	puts("Please enter a command in the format: command value.")
	fgets(commandAndArg, SIZE, stdin); 

	/*Here I split the inputted string via the space, assigning the command and argument respectively.*/ 
	for (position = 0; position < positionOfWhiteSpace; position++)
	
	printf("Your command is: %s; your argument is %s.", command, argument)
  return 0;
}

