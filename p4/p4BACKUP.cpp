/*
Name: Luke R. Prescott
Unix login id: LP911211
Lab instructor: Ji Yong
Date and time of lab: Fridays at 10:25am
*/

/* Headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
Description: This C program inputs a MIPS Assembly Language (MAL) program and outputs a listing of the MAL program iwht line numbers
and/or a cross-reference table for the MAL program. The program will be executed via the command line by passing arguments to main in the form
?.out flag inputfile outputfile. The arguments inputfile and outputfile specify the names of the input file and the output file respectively. 
A flag argument is one of the following: -l, -c or -b. If the flag is -l, the program only produces a program listing of the MAL source program
in the specified output file. If the flag is -c, the program produces only the cross-reference table for the MAL source program in the specified 
output fule. If the flag is -b, the program produces both listing and table in the specified output file.

Assumptions: 
	(1) Every line of a MAL program is terminated by the newline (�\n�) character and has at most 80 characters (including the newline character). 
	(2) The maximum number of labels that can appear in any MAL program is 100.
	(3) The identifiers are case sensitive and the maximum length of an identifier is 10. 
	(4) All input files  satisfy the following conditions:
			The MAL program in the input file won�t contain any errors. 
			Every identifier will be defined somewhere in the MAL program. 
			There will be no multiply defined identifiers. 

*/

/* Constants */
#define SIZE_OF_IDENTIFIER 11 //The max size of identifiers
#define SIZE_OF_LINE 80 //The max size of one line
#define SIZE_OF_IDENTIFIER_USES 11 //The maximum number of uses for each identifier

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
	void referenceTable(char *inputFileString, char *outputFileString);

	/* Non-required functions */

/* End of prototypes*/

int main(int argc, char * argv[])
{
	/* Check the number of command line arguments. */
	if (argc != 4) {
		fprintf(stderr, "Usage: p4.out flag inputfile outputfile\n");
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
		referenceTable(argv[2], argv[3]);
	}
	else if (strcmp(argv[1], "-b") == 0) {
		/* Both */
		puts("\nThe program listing/cross reference table flag was entered.");
		programListing(argv[2], argv[3]);
		referenceTable(argv[2], argv[3]);
	}
	else {
		puts("\nAn unrecognized flag was entered.");
	}

	/* This stops the terminal from closing after the end of the program by waiting for user input via the getchar() function. */
	puts("\n...Type ENTER to continue... ");
	getchar();
    
	return 0;
}

/*
The programListing method takes a pointer to name of the inputFile and outputFile as parameters. It then creates a program listing, a source code
with line numbers for every not blank line via reading the inputFile and outputs the program listing to the outputFile. The method returns 
nothing.
*/
void programListing(char *inputFileString, char *outputFileString) {
	FILE *inputFile, *outputFile; /* Input and output files. */
		
	/* Open the input and output files. */
	if ((inputFile = fopen(inputFileString, "r")) == NULL) {
		fprintf(stderr, "Could not open file: %s\n", inputFileString);
		exit(1);
	}
	if ((outputFile = fopen(outputFileString, "w")) == NULL) {
		fprintf(stderr, "Could not open file: %s\n", outputFileString);
		exit(1);
	}

	char line[SIZE_OF_LINE]; /* String that contains the contents of one line. */
	int count = 1; /*The count of the line number. */

	while (fgets(line, SIZE_OF_LINE, inputFile)) {
		if (strcmp(line, "\n") != 0) {
			/* Line not empty */
			fprintf(outputFile, "%-3d    %s", count, line);
			count += 1;
		}
		else {
			fprintf(outputFile, "%s", line);
		}
	}

	fclose(inputFile);
	fclose(outputFile);
}

/*
The referenceTable method takes a pointer to the name of the inputFile and outputFile as parameters. It then creates a reference table, a list of 
identifiers that specifies the line number where the identifier is defined and the lines where it is used, amd outputs the reference table
to the outputFile. The method returns nothing.
*/
void referenceTable(char *inputFileString, char *outputFileString) {
	FILE *inputFile, *outputFile; /* Input and output files. */
	char line[SIZE_OF_LINE]; /* String that contains the contents of one line. */
	Node identifiers[100]; /* Array of size 100 that contains identifiers */
	
	int currentLine = 1; /* The counter for the current line being analyzed */
	int currentIdentifier = 0; /* The location/count in the array identifiers to edit in while loop */

	int w = 0;
	int x = 0;
	int y = 0;
	int z = 0;

	char firstChar;

	/* Open the input and output files. */
	if ((inputFile = fopen(inputFileString, "r")) == NULL) {
		fprintf(stderr, "Could not open file: %s\n", inputFileString);
		exit(1);
	}
	if ((outputFile = fopen(outputFileString, "a")) == NULL) {
		fprintf(stderr, "Could not open file: %s\n", outputFileString);
		exit(1);
	}

	fputs("Cross Reference Table\n", outputFile);
	fputs("Identifier  Definition  Use", outputFile);

	while (fgets(line, SIZE_OF_LINE, inputFile)) {
		if (strcmp(line, "\n") == 0) {
			/* Line is empty */
			currentLine += 1;
		}

		else if (strcmp(&line[0], "#") == 0) {
			/* Comment Line */
			currentLine += 1;
		}

		else {
			/* Line not empty */

			if (strstr(line, ":") != NULL) {
				/* Line contains a identifiers declaration */
				char *token;
				token = strtok(line, ":");
				
				strcpy(identifiers[currentIdentifier].identifier, token);
				identifiers[currentIdentifier].lineNumberDefined = currentLine;

				token = strtok(NULL, ":");

				for (w; w < currentIdentifier ; w++) {
					if (strstr(token, identifiers[w].identifier) != NULL) {
						identifiers[w].countUsed += 1;
						identifiers[w].lineNumberUsed[identifiers[w].countUsed] = currentLine;
					
				}

				currentLine += 1;
				currentIdentifier += 1;
				}
			}


			else {
				/* Line doesn't declare an identifier */
				for (x; x < currentIdentifier; x++) {
					if (strstr(line, identifiers[x].identifier) != NULL) {
						identifiers[x].countUsed += 1;
						identifiers[x].lineNumberUsed[identifiers[x].countUsed] = currentLine;
					}
				}
				currentLine += 1;
			}
		}

		fputs("Identifier  Definition  Use", outputFile);
		
		for (y; y < currentIdentifier; y++){
			fprintf(outputFile, "\n%12s %12d", identifiers[y].identifier, identifiers[y].lineNumberDefined);
			for (z; z < identifiers[y].countUsed; z++){
				fprintf(outputFile, "%12s", identifiers[y].lineNumberUsed[z]);
			}

			fprintf(outputFile, "\n");
		}
	}
}

