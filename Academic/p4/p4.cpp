/*
Name: Luke R. Prescott
Unix login id: LP911211
Lab instructor: Ji Yong Park
Date and time of lab: Fridays at 10:25am
*/

/* Headers */
#include <stdio.h> // Needed for standard input output
#include <string.h> // Needed for string evaluations
#include <stdlib.h> // Needed for string searches


/*
Description: This C program inputs a MIPS Assembly Language program and outputs a listing of the MAL program with line numbers
and/or a cross-reference table. The program will be executed via the command line by passing arguments to main method in the form
?.out flag inputfile outputfile. The arguments inputfile and outputfile specify the names of the input file and the output file respectively. 
The flag argument is one of the following: -l, -c or -b. If the flag is -l, the program only produces a program listing of the MAL source program
in the specified output file. If the flag is -c, the program produces only the cross-reference table for the MAL source program in the specified 
output fule. If the flag is -b, the program produces both listing and table in the specified output file.

Assumptions: 
	(1) Every line of a MAL program is terminated by the newline ('\n') character and has at most 80 characters (including the newline character). 
	(2) The maximum number of labels that can appear in any MAL program is 100.
	(3) The identifiers are case sensitive and the maximum length of an identifier is 10. 
	(4) All input files  satisfy the following conditions:
			The MAL program in the input file won't contain any errors. 
			Every identifier will be defined somewhere in the MAL program. 
			There will be no multiply defined identifiers. 
*/

/* Constants */
#define SIZE_OF_IDENTIFIER 11 //The max size of identifiers
#define SIZE_OF_LINE 80 //The max size of one line
#define SIZE_OF_IDENTIFIER_USES 11 //The maximum number of uses for each identifier
#define SIZE_OF_LABELS 100 //The maximum number of labels in the MAL program

/* TypeDefs */
typedef struct node Node; // Synonym for the struct node
typedef Node * NodePtr; // Synonym for Node *

/* Structure for each node in the linked list. */
struct node {
	char identifier[SIZE_OF_IDENTIFIER]; //The identifier each node contains
	unsigned int lineNumberDefined; //The positive lineNumber each node contains 
	unsigned int lineNumberUsed[SIZE_OF_IDENTIFIER_USES]; //An int array that contains the line numbers where the identifiers were used
	unsigned int countUsed; //The amount of times the identifier was used (not declared)
};

/* Prototypes for the functions used. */

	/* Required functions */
	void programListing(char *inputFileString, char *outputFileString);
	void referenceTable(char *inputFileString, char *outputFileString, char *flag);

	/* Non-required functions */
	void searchLine(Node identifiers[SIZE_OF_LABELS], char line[SIZE_OF_LINE], int currentLineSearching, int numOfIdentifiers);

/* End of prototypes*/

/*
The main method takes arguments passed by command line. The number of arguments is 4; the program will be executed in the form ?.out flag inputfile outputfile.
The main method interprets the inputted arguments and selects the correct function(s) to execute in the program. The main method takes no input from stdin, but outputs
a reiterations of the command that was entered for clarity.
*/
int main(int argc, char * argv[])
{
	/* Check the number of command line arguments. */
	if (argc != 4) {
		fprintf(stderr, "Format: ?.out flag inputfile outputfile\n");
		exit(1);
	}

	/* Compare flags */
	if (strcmp(argv[1], "-l") == 0) {
		/* Program listing */
		puts("\nThe program listing flag was entered.");
		programListing(argv[2], argv[3]);
	}
	else if (strcmp(argv[1], "-c") == 0) {
		/* Cross reference table */
		puts("\nThe cross reference table flag was entered.");
		referenceTable(argv[2], argv[3], argv[1]);
	}
	else if (strcmp(argv[1], "-b") == 0) {
		/* Both */
		puts("\nThe program listing/cross reference table flag was entered.");
		programListing(argv[2], argv[3]);
		referenceTable(argv[2], argv[3], argv[1]);
	}
	else {
		/* Unknown flag */
		puts("\nAn unrecognized flag was entered.");
	}
    
	return 0;
}

/*
The programListing method takes a pointer to name of the inputFile and outputFile as parameters. It then creates a program listing, a source code
with line numbers for every not blank line via reading the inputFile and outputs the program listing to the outputFile. The method returns 
nothing.
*/
void programListing(char *inputFileString, char *outputFileString) {
	FILE *inputFile, *outputFile; /* Input and output files. */
	
	char line[SIZE_OF_LINE]; /* String that contains the contents of one line. */
	int count = 1; /*The count of the line number. */

	/* Open the input and output files. */
	if ((inputFile = fopen(inputFileString, "r")) == NULL) {
		fprintf(stderr, "Could not open file: %s\n", inputFileString);
		exit(1);
	}
	if ((outputFile = fopen(outputFileString, "w")) == NULL) {
		fprintf(stderr, "Could not open file: %s\n", outputFileString);
		exit(1);
	}

	/* While-loop to get lines of inputFile  */
	while (fgets(line, SIZE_OF_LINE, inputFile)) {
		if (strcmp(line, "\n") != 0) {
			/* Line not empty */
			fprintf(outputFile, "%-3d    %s", count, line);
			count += 1;
		}
		else {
			/* Line empty */
			fprintf(outputFile, "%s", line);
		}
	}

	/* Close the input and output files */
	fclose(inputFile);
	fclose(outputFile);
}

/*
The referenceTable method takes a pointer to the name of the inputFile and outputFile as parameters. It also takes the flag passed to the main method via the command line. 
It then creates a reference table, a list of identifiers that specifies the line number where the identifier is defined and the lines where it is used, amd outputs the reference table
to the outputFile. The method returns nothing.
*/
void referenceTable(char *inputFileString, char *outputFileString, char *flag) {
	FILE *inputFile, *outputFile; /* Input and output files. */
	Node identifiers[SIZE_OF_LABELS]; /* Array of size 100 that contains identifiers */

	char line[SIZE_OF_LINE]; /* String that contains the contents of one line. */
	
	int currentLine = 1; /* The counter for the current line being analyzed */
	int currentIdentifier = 0; /* The location/count in the array identifiers to edit in while loop */

	int x = 0; /* The outer for-loop that prints each identifier and declaration locations */
	int z = 0; /* The inner for-loop that prints each identifiers use location(s) */

	/* Open the input file for reading. */
	if ((inputFile = fopen(inputFileString, "r")) == NULL) {
		fprintf(stderr, "Could not open file: %s\n", inputFileString);
		exit(1);
	}

	/* Open the output file for writing only when the flag is -c */
	if (strcmp(flag, "-c") == 0){
		if ((outputFile = fopen(outputFileString, "w")) == NULL) {
			fprintf(stderr, "Could not open file: %s\n", outputFileString);
			exit(1);
		}
	}


	/* Open the output file for appending only when the flag is -b */
	if (strcmp(flag, "-b") == 0){
		if ((outputFile = fopen(outputFileString, "a")) == NULL) {
			fprintf(stderr, "Could not open file: %s\n", outputFileString);
			exit(1);
		}

		/* Add a newline before the reference table */
		fputs("\n", outputFile);
	}

	/* Print head of reference table to output file */ 
	fputs("Cross Reference Table:\n", outputFile);
	fputs("Identifier  Definition  Use", outputFile);

	/* While-loop to get lines of inputFile  */
	while (fgets(line, SIZE_OF_LINE, inputFile)) {
		/* If the line is empty do nothing */
		if (strcmp(line, "\n") == 0) {
			/* Line is empty */
		}

		/* If the line is a comment just increment currentLine. */
		else if (strncmp(line, "#", 1) == 0) {
			/* Comment Line */
			currentLine += 1;
		}

		/* Else the line is code */
		else {

			/* If the line contains a ':' */
			if (strstr(line, ":") != NULL) {
				/* Line contains a identifiers declaration */
				char *token; /* Char array called token that contains text before ':' */
				token = strtok(line, ":"); /* Assign token */
				
				/* Assign identifier name, defined line number and count of use. */
				strcpy(identifiers[currentIdentifier].identifier, token);
				identifiers[currentIdentifier].lineNumberDefined = currentLine;
				identifiers[currentIdentifier].countUsed = 0;

				/* Assign token to string after ':' */
				token = strtok(NULL, ":");
				currentIdentifier += 1;

				/* Search line for known identifiers w/ token as key */
				searchLine(identifiers, token, currentLine, currentIdentifier);
			}
			/* There is no identifier declared on the line*/
			else{
				/* Search line for known identifiers w/ line as key */
				searchLine(identifiers, line, currentLine, currentIdentifier);
			}
			/* Increment line */
			currentLine += 1;
		}

	}

	/* Loop through count of identifiers printing declaration locations and loop location(s). */
	for (x; x < currentIdentifier; x++){
		fprintf(outputFile, "\n%-10s  %-10d  ", identifiers[x].identifier, identifiers[x].lineNumberDefined);	
		
		/* Loop through each identifier and print use locations. */
		for (z; z < identifiers[x].countUsed; z++){
			fprintf(outputFile, "%d ", identifiers[x].lineNumberUsed[z]);
		}
	}

	/* Close the input and output files */
	fclose(inputFile);
	fclose(outputFile);
}

/* 
The searchLine returns nothing. The method takes four arguments as parameters. The first argument is the array of nodes to edit; the second is the line to search through using identifiers. 
The third parameter is the current line # that the method is searching and the fourth parameter is the number of identifiers contained in the array of nodes. The method loops through the identifiers and 
compares the names to strings contained withing the current line. The method then assigns the corresponding used identifier line numbers to the array in each identifier node for printing later.
*/
void searchLine(Node identifiers[SIZE_OF_LABELS], char line[], int currentLineSearching, int numOfIdentifiers)
{
	/* The integer y loops through the length of the list of identifiers. */
	int y = 0;

	/* The looping of identifiers. */
	for (y; y < numOfIdentifiers; y++)
	{
		/* The method should ignore text behind comments. */
		if (strstr(line, "#") != NULL) 
		{
			char *token2;
			token2 = strtok(line, "#");
			/* The method only searches the line before the comment and after a declaration. */
			if (strstr(token2, identifiers[y].identifier) != NULL) 
			{
				/* Assign corresponding counts and lines used. */
				identifiers[y].countUsed += 1;
				identifiers[y].lineNumberUsed[identifiers[y].countUsed-1]=currentLineSearching;
			}
		}

		/* The line does not contain a comment. */
		else
		{
			/* The method searches the entirety of the line. */
			if (strstr(line, identifiers[y].identifier) != NULL) {
				/* Assign corresponding counts and lines used. */
				identifiers[y].countUsed += 1;
				identifiers[y].lineNumberUsed[identifiers[y].countUsed-1]=currentLineSearching;
			}
		}
	}
}