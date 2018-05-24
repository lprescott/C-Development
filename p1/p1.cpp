/* 
	Name: Luke R. Prescott
	Unix login id: LP911211
	Lab instructor: Ji Yong
	Date and time of lab: Fridays at 10:25am
*/

#include <stdio.h> //Standard input/output header

/*
	Description: This C program converts the decimal integer d into its equivalent number in radix r.  
	The first decimal integer, d, is assumed to be nonnegative and the second integer, r, is assumed 
	to be an integer such that: 2 <= r <= 16.  The decimal integer, d, is inputted by the user via 
	the function scanf(), as is the integer r. The program outputs the equivalent number in base 
	r via the function printf(). 
*/

#define SIZE 64 //Symbolic constant for the maximum size of the outputted character array.

/* Prototype(s) for the function(s) called by main. */
void convertDecimalToAnyBase();

void main() /* Beginning of main.*/
{
	/* Calling convertDecimalToAnyBase()*/
	convertDecimalToAnyBase();

	/* 
		This stops the terminal from closing after the end of the program by waiting for user input 
		via the getchar() function.
	*/
	puts("\n\n...Type ENTER to continue... ");
	getchar();

} /* End of main. */

/*
	The convertDecimalToAnyBase function neither returns a value, nor takes one as a parameter. It prompts 
	the user for a base 10 number and a radix to convert it to. The function then prints the inputted values
	(for clarity) and the converted base-r number. 
*/
void convertDecimalToAnyBase() /* Beginning of convertDecimalToAnyBase.*/
{
	int d; //The decimal integer to convert, inputted by the user.
	int r; //The radix, inputted by the user.
	int remainder; //The remainder when dividing the inputted decimal integer by the radix.

	int count = 0; /*The count of characters in the character array temp (recorded by incrementing in
				   the while loop.*/

	int position; /*Used in the for-loop that prints the characters in temp backwards by assigning it the
				  length of tempand then decrememting its value. */

	char temp[SIZE]; /*A character array called temp used to store the characterCodes in (64 is the size
				     of the array. */


	char characterCodes[] = "0123456789ABCDEF"; /*The characterCodes needed for outputting characters
												greater than nine.*/

	/*Asks and scans for d and r. */
	puts("Please enter the base 10 integer to be converted (d) and then the base to convert to (r): \n");
	scanf("%d %d", &d, &r);
	
	/*Prints the inputted items*/
	printf("\n%d base-10 in base-%d is: ", d, r); fflush(stdout); 

	/*
		This while-loop appends the correct character into the temp character array. It does this by using %
		to determine the remainder between d and r and referencing the characterCodes array. The temp array will
		need to be reversed later. 
	*/
	while (d != 0) //While d does not equal zero (d will evaluate to zero when there is no remainder).
	{
		remainder = d % r; //Determine the remainder of the decimal int and radix. 
		d = d / r; //Set the decimal int to be itself divided by the radix.
		temp[count] = characterCodes[remainder]; /*Set the temp array's character at location count
												   to the characterCodes value at index remainder. */
		
		count++; //Increment count for next execution of the while loop.
	}
	 
	/*
		The character array temp is in backwards order. This for-loop prints the char. array in reverse-order. 
		For position being equal to the length of the character array temp, while greater than zero,
		decremented.
	*/
	for (position = count - 1; position >= 0; --position) 
	{
		printf("%c", temp[position]); fflush(stdout); /*Prints the character that is indexed in the temp
													  character arrays position. */
	}

	getchar(); //This clears the input buffer. Needed to pause the program at end. 
} /*End of convertDecimalToAnyBase.*/