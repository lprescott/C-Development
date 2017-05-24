/*
Name: Luke R. Prescott
Unix login id: LP911211
Lab instructor: Ji Yong
Date and time of lab: Fridays at 10:25am
*/

#include <stdio.h> //input/output header (needed for fgets and fputs)
#include <string.h> //string header (needed for strlen() function)

/* Description: This C program carries out a left-to-right evaluation of an arithmetic expression. 
The arithmetic expression consists of non-negative integer constants and the operators: +, -, *,
and /. The operator / denotes integer division. There is no notion of precedence; the expression 
is evaluated strictly from left to right. The inputted expression is assumed to be valid (ie): It 
satisfies all of the previous conditions; It wont contain parentheses; Each integer constant in 
the expression consists of only one decimal digit; The expression begins with an integer constant 
without any preceding sign and in the expression: integer constants and operators alternate. The 
function evaluateExpression() inputs from stdin via fgets() and outputs to stdout via printf(). */

// The defined constant SIZE is the maximum size of inputted characters from the arithmetic expression. 
#define SIZE 32

// Prototype(s) for the function(s)
int evaluateExpression();
void removeWhiteSpace(char stringToFormat[]);

void main() // Beginning of main.
{
	// Calling evaluateExpression()
	evaluateExpression();

	/* This stops the terminal from closing after the end of the program by waiting for user input
	via the getchar() function. */
	puts("\n...Type ENTER to continue... ");
	getchar();

} // End of main. 

/* The function evaluateExpression() inputs an arithmetic expression consisting of non-negative integer 
constants and the operators: +, -, *, and / via stdin (inputted from the user) and outputs the evaluated 
int via stout (outputted to the user). Please see above for assumptions. */
int evaluateExpression()
{	
	/* A character array called expression that stores all characters inputted via stdin. This is the 
	expression. */
	char expression[SIZE]; 
	
	/* lengthOfExpression is needed to keep track of the number of characters in the expression. */
	int lengthOfExpression = 0; 

	int answer; //This is the answer outputted via stdout after the calculations are completed. 
	
	int position; //This variable position is needed for the functions main for-loop.

	puts("Please enter an arithmetic expression consisting of non-negative integer constants and the "
		 "operators +, -, *, and /: ");
	
	/* This reads a line from standard input and stores it into the string pointed to by expression. It
	stops when either (SIZE-1) characters are read, the newline character is read, or the end-of-file
	is reached, whichever comes first. */
	fgets(expression, SIZE, stdin); 

	//This is a call to the function removeWhiteSpace(); it removes all spaces from the passed string.
	removeWhiteSpace(expression);

	/* This initializes answer to be the first integer in the arithmetic expression (the character in 
	the 0 position of the char array expression). To convert the char to an integer, one subtracts the 
	'0' character which is represented by the value of 48. Because a chars are represented by a number and '0'
	is the first of them all, this accurately changes the char to the correct int. */
	answer = expression[0] - '0';

	//This assigns the int lengthOfExpression to be the length of the expression. 
	lengthOfExpression = strlen(expression) - 1;

	/* This for-loop loops through the entirety of the expression. Using a switch statement, the loop 
	looks at each position position in expression for four cases: +, -, *, and /. For each of the cases, the 
	switch statement uses the correct operation on answer and the int in the 
	next position in the expression. Again, to change the char that is read to the correct integer, subtracting 
	the character '0' is used. See above explanation. */
	for (position = 0; position < lengthOfExpression && expression[position] != '\n'; position++) 
	{
		switch (expression[position]) 
		{
		case '+': //Case for addition
			answer += (expression[position + 1] - '0');
			break;

		case '-': //Case for subtraction
			answer -= (expression[position + 1] - '0');
			break;

		case '*': //Case for multiplication
			answer *= (expression[position + 1] - '0');
			break;

		case '/': //Case for division
			answer /= (expression[position + 1] - '0');
			break;

		default: //Default case
			break;
		}

	}

	//The inputted arithmetic expression is then reprinted for clarity, along with the evaluated answer.
	printf("\nThe inputted arithmetic expression: %s\nThe evaluated answer: %d\n", expression, answer); 
	fflush(stdout);

	return answer; //This would return the answer if called differently though the main method.

} //End of evaluateExpression().

/* This function returns no value. It accepts a character array as a paramter and removes the white 
space characters throughout by looping through the entire length: While skipping all blank characters (ie ' '), the function assignes 
them to a new array. It then uses strncpy() to replace the values in 
stringToFormat with the values in formattedString. */
void removeWhiteSpace(char stringToFormat[])
{
	int positionInString; //This variable positionInString is needed for the functions main for-loop.
	int characterPosition = 0; //This variable characterPosition is needed for the functions while-loop.
	
	// This assigns the length of the unformatted string to a integer variable lengthOfString 
	int lengthOfString = strlen(stringToFormat) - 1; 

	char formattedString[SIZE]; //This will be the expression without white-space.

	/* This for-loop traverses the lengthOfString with the variable positionInString. While the string to format 
	contains chars that are ' ' (white-space) it increments a second variable i that keeps track of the 
	non-whiteSpaceCharacters. If the character are not white-space the if control-structure adds to the 
	formattedString char array. It terminates with an else statement that adds the break character to the 
	formatted char array. */ 
	for (positionInString = 0; positionInString < lengthOfString; positionInString++)
	{
		//If the char is white-space and i is less than lengthOfString...
		while (stringToFormat[characterPosition] == ' ' && characterPosition < lengthOfString)
		{
			characterPosition++; //increment i
		}

		//If i is less than lengthOfString...
		if (characterPosition < lengthOfString)
		{
			//Add values to formatted String.
			formattedString[positionInString] = stringToFormat[characterPosition++];
		}
		
		else
		{
			formattedString[positionInString] = 0; //This ends the formatted String.
			break; //This breaks out of the function.
		}
	}

	/* This copies the new formattedString back to the stringToFormat without any white-space, completing the
	function. */
	strncpy(stringToFormat, formattedString, lengthOfString);
}

 