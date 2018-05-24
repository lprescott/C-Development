#Name: Luke R. Prescott
#Unix login id: LP911211
#Lab instructor: Ji Yong Park
#Date and time of lab: Fridays at 10:25am

#This MAL program (p5b.mal) prompts the user for a line of text and reads the line inputted by the user. If the line contains just white space characters the program outputs the message "Line contains only white space characters."
#If the input is not empty, the program computes and outputs the following:

		#1. The number of non-whitespace characters in the line. 
		#2. The number of words in the line.
		#3. The maximum length of a word in the line.
		#4. The minimum length of a word in the line.
		#5. The word of maximum length in the line. If there are two or more words of maximum length in the line, then the program prints the word of maximum length that appears first in the line.
		#6. The word of minimum length in the line. If there are two or more words of minimum length in the line, then the program prints the word of minimum length that appears first in the line.

		.data
		
input:	.space	81 													#This creates space for a 80 char string + \0 char
prompt:     .asciiz     	"Please enter a line of text: "
output1:    .asciiz 	"\nLine contains only white space characters." 
output2:	.asciiz 	"\nNo. of non-whitespace characters: "
output3:	.asciiz 	"\nNo. of words: "
output4:	.asciiz 	"\nMaximum length of a word: "
output5:	.asciiz 	"\nMinimum length of a word: "
output6:	.asciiz	"\nWord of maximum length: "
output7:	.asciiz 	"\nWord of minimum length: "

		#Unused strings:
		#output8: 	.asciiz 	"\nThe inputted line of text without white space: "
		#output9:	.asciiz	"\nThe elements in the array are of length: "
		#length:	.asciiz	"\nThe length of the input is (including the null char): "
		#wschars:	.asciiz 	"\nThe number of white space characters in the input: "
		#minpos:	.asciiz 	"\n\tThe first word of min. length is at pos (in the array): "
		#maxpos:	.asciiz 	"\n\tThe first word of max. length is at pos (in the array): "

		#bfmin:	.asciiz	"\n\tThe number of characters before the min word is: "
		#bfmax:	.asciiz	"\n\tThe number of characters before the max word is: "

defaultO:   .asciiz   	"N/A"
new_line:   .asciiz  	"\n"

onespace:  	.byte 	32												#This creates a identifier called onespace that is ' '
nospace:		.space	81												#This creates a space for the inputted string without whitespace characters
array:		.space	16 												#80 Element integer array (i.e. the maximum number [words] in the input

		#Registers and variables:
	    			#$s0 -- The inputted line of text.
	   			#$s1 -- The number of non-whitespace characters in the line.
	    			#$s2 -- The number of words in the line.
	    			#$s3 -- The maximum length of a word in the line.
	    			#$s4 -- The minimum length of a word in the line.
	    			#$s5 -- The word of maximum length in the line.
	    			#$s6 -- The word of minimum length in the line.
	    			#$s7 -- The number of white space characters in the line. (i.e. space: ' ' , tab: '\t', newline: '\n')
	    			#$t0 -- The address of the inputted string.
	    			#$t1 -- The counter, starting at 0.
	    			#$t2 -- The current byte in the address $t0 while looping.
	    			#$t3 -- The address of the current letter in the count_wrds function.
	    			
	 	.text
            	.globl     	main

main:
		#Prompt the user for the inputted line of text here.
            	la           	$a0, prompt     	#The first address of prompt is loaded into $a0
            	li            	$v0, 4          	#4 is the print_string command. This prints the prompt.
            	syscall
            	
            	#Input the line of text here.
            	li      		$v0, 8			#This loads the read_string system call code into $v0
  		la      	$a0, input			#Load the address of the string input into $a0
    		li      		$a1, 81			#Load the value of (80 chars) into $a1
   	 	syscall
   	 	
   	 	#Store address and initialize counter to zero here.
   	 	la 		$t0, input  			#Load the address of the string input into $t0
    		li 		$t1, 0    			#Initialize the counter to zero	
   	 	
#Check if the inputted string is empty and branch if so. Also determine the length of the input.
count_char:
		lb		$t2, 0($t0)			#Loads the current byte from the address in the input
		beqz 		$t2, end_count_char	#If the current byte is null end the loop
		
		beq 		$t2, 9, incr_ws		#Increment white space if TAB
		beq		$t2, 32, incr_ws		#Increment white space if SPACE
		beq		$t2, 10, incr_ws		#Increment white space if NEW LINE
		
		add 		$t0, $t0, 1			#Increment the address
		add		$t1, $t1, 1			#Increment the count	
		j 		count_char			#Jump to the beginning of the loop
	
#This code increments the number of white space character used to calculate the number of non-white space character.	
incr_ws:
		add 		$s7, $s7, 1			#Increment $s7 by 1
		
		add 		$t0, $t0, 1			#Increment the address
		add		$t1, $t1, 1			#Increment the count	
		j 		count_char			#Jump to the beginning of the loop
		
end_count_char:

   	 	#Initialize all outputted integer variables to 0 and strings to "N/A" here.
   	 	move $s1, $0
   	 	move $s2, $0
   	 	move $s3, $0
   	 	addi $s4, $0, 80
   	 	la $s5, defaultO
   	 	la $s6, defaultO

		#Calculate the number of white space characters here.
		sub 		$s1, $t1, $s7			#$s1 = length - #ofwschars
		
		#Reset counter and address used in loop above here.
		la 		$t0, input  			#Load the address of the string input into $t0
    		li 		$t1, 0    			#Initialize the counter to zero
		
#This code loops through the user input byte by byte similarly to above, and calculates the number of words.	
count_wrds:
		lb		$t2, 0($t0)			#Loads the current byte from the address in the input
		beqz 		$t2, end_count_wrds	#If the current byte is null end the loop
		
		bge		$t2, 33, curr_word	#Branch if the current byte is a character (start of a word)
		
		add 		$t0, $t0, 1			#Increment the address
		add		$t1, $t1, 1			#Increment the count	
		j 		count_wrds			#Jump to the beginning of the loop

#This code signifies the start of a word.	
curr_word:
		lb		$t3, 0($t0)			#Store the current byte from the current address in the input
		beqz 		$t3, end_count_wrds	#If the current byte is null end the loop
		
		beq 		$t3, 9, incr_wrds			#jump if TAB
		beq		$t3, 32, incr_wrds		#jump if SPACE
		beq		$t3, 10, incr_wrds		#jump if NEW LINE 
	    	
		add 		$t0, $t0, 1			#Increment the address in input
		add		$t1, $t1, 1			#Increment the count in input
		j 		curr_word			#Jump to the beginning of the loop

#This code signifies the end of a word		
incr_wrds:
	    	add 		$t0, $t0, 1			#Increment the address in input
		add		$t1, $t1, 1			#Increment the count in input
		
		add		$s2, $s2, 1			#Increment the count of words
		j count_wrds

end_count_wrds:

		#Branch if empty
		beqz 		$s2, empty_input

#Unused print statements here:
		
		#Print the lengths of the input
		#la 		$a0, length
	    	#li 		$v0, 4
	    	#syscall
	    
	    	#la 		$a0, ($t1)	
 	    	#li 		$v0, 1		
	    	#syscall
	    	
	    	#Print the number of white space chars
	    	#la 		$a0, wschars
	    	#li 		$v0, 4
	    	#syscall
	    
	    	#la 		$a0, ($s7)	
 	    	#li 		$v0, 1		
	    	#syscall
	    	
	    	


		#Unused print statement:
		#The letters of non-whitespace characters in the line.
    		#la 		$a0, output8
	    	#li 		$v0, 4 	
	    	#syscall
	    	
		#Reset counter and address used in loop above here.
		la 		$t0, input  			#Load the address of the string input into $t0
		la 		$t2, array			#Load the address of the array into $t2
    		li 		$t1, 0    			#Initialize the counter to zero
    		la 		$t3, nospace			#Load the address of the array into $t2

		
#This code loops through the user input byte by byte similarly to above, and calculates the input without spaces.		
loop:
  		lb 		$a0, 0($t0)				#Load the byte

    		addi 		$t0, $t0, 1
    		beq 		$a0, 9, loop_contd		#branch if the char is whitespace
    		beq 		$a0, 10, loop_contd
    		beq 		$a0, 32, loop_contd
   	 	beq 		$a0, $0, end_loop   		#end if char is 0
   	 	
   	 	addi 		$t1, $t1, 1

		sb	 	$a0, 0($t3)
    		addi 		$t3, $t3, 1
    		
    		#li $v0, 11
    		#syscall

    		j loop
    		
#If the character is not a space
loop_contd:
    		sb	 	$t1, 0($t2)				#Store the byte in nospace
    		addi 		$t2, $t2, 4
    		
		li 		$t1, 0 
		
		j loop
		
#End the loop
end_loop:
		
		#Add a \zero to the last position in the string nospace
		li		$t1, 0
		sb		$t1, 0($t3)

		#Unused print statement here:
		
    		#Print a new line
	    	#la 		$a0, new_line
	    	#li 		$v0, 4
	    	#syscall	
	    	
	    	    	
    		#Print output prompts and values here.
    		#The number of non-whitespace characters in the line.
    		la 		$a0, output2
	    	li 		$v0, 4 	
	    	syscall
	    	
	    	la 		$a0, ($s1)	
 	    	li 		$v0, 1		
	    	syscall
	    			
		#The number of words in the line.
		la 		$a0, output3
	    	li 		$v0, 4
	    	syscall

	    	la 		$a0, ($s2)	
 	    	li 		$v0, 1		
	    	syscall

#Code to calculate the word lengths and add them to an array:

	    	#Reset counter and address
	    	li 		$t1, 0
	    	la 		$t2, array
	    	
	    	li		$t6, 0			#char before min
	    	li		$t7, 0			#char before max
	    	
array_loop:
		lb 		$t3, 0($t2)					#Load the byte
		beq 		$t3, $0, end_array_loop 		#Branch if 0
		
		bgt		 $t3, $s3, set_max			#Branch if gt
		blt		$t3, $s4, set_min			#Branch if lt
		
		addi 		$t1, $t1, 1					#Increment address and count
		addi 		$t2, $t2, 4
		
		j array_loop
		
#This sets the max length if needed:	
set_max:
		lb		$s3, 0($t2)					#Load the byte
		blt		$t3, $s4, set_min			#Call set_min in case
		
		addi		$t8, $t1, 1					#Increment
		addi 		$t1, $t1, 1
		addi 		$t2, $t2, 4
		
		j array_loop
		
#This sets the min length if needed:
set_min:
		lb		$s4, 0($t2)					#Load the byte
		bgt		 $t3, $s3, set_max			#Call set_max in case
		
		addi		$t9, $t1, 1					#Increment
		addi 		$t1, $t1, 1
		addi 		$t2, $t2, 4
			
		j array_loop
		
#This ends the loop:
end_array_loop:

		#Unused print statements here:
		
		#la 		$a0, maxpos
	    	#li 		$v0, 4
	    	#syscall

	    	#la 		$a0, ($t8)	
 	    	#li 		$v0, 1		
	    	#syscall
	    	

		#la 		$a0, minpos
	    	#li 		$v0, 4
	    	#syscall

	    	#la 		$a0, ($t9)	
 	    	#li 		$v0, 1		
	    	#syscall

		#The elements in the array
    		#la 		$a0, output9
	    	#li 		$v0, 4 	
	    	#syscall
	    	
	    	#Reset counter and address
	    	li 		$t1, 0
	    	la 		$t2, array
	    	li		$t3, 0
	    	
#This can print the array elements if needed; and stores the largest and smallest length values:
print_array_elements:
		lb 		$a0, 0($t2)							#Load the byte
		bge		$t1, $s2, end_print_array_elements		#Branch if end
		
		add		$t3, $t3, $a0						#Assign and increment
    		addi 		$t1, $t1, 1
		addi 		$t2, $t2, 4
		
		beq		$t1, $t9, set_char_min					#Branch if needed
		beq		$t1, $t8, set_char_max
		
		#li $v0, 1
    		#syscall
    		
    		#lb $a0, onespace
    		#li $v0, 11
    		#syscall
    		
    		j print_array_elements
    		
#This sets the min char:
set_char_min:
	
		add 		$t6, $t3, $0						#Calculate min
		sub		$t6, $t6, $a0
		#li $v0, 1
    		#syscall
    		
    		#lb $a0, onespace
    		#li $v0, 11
    		#syscall
    		
    		j print_array_elements

#This sets the max char:
set_char_max: 
		
		add 		$t7, $t3, $0						#Calculate max
		sub		$t7, $t7, $a0
		#li $v0, 1
    		#syscall
    		
    		#lb $a0, onespace
    		#li $v0, 11
    		#syscall
    		
    		j print_array_elements

#This ends the loop:
end_print_array_elements:

		#Unused print statements here:
		
		#la 		$a0, bfmin
	    	#li 		$v0, 4
	    	#syscall

	    	#la 		$a0, ($t6)
 	    	#li 		$v0, 1		
	    	#syscall
	    	
	    	#la 		$a0, bfmax
	    	#li 		$v0, 4
	    	#syscall

	    	#la 		$a0, ($t7)	
 	    	#li 		$v0, 1		
	    	#syscall

#Code to print the bytes of the maximum and minimum words in registers $s5 and $s6:

	#Reference for registers:
	
	#$t8 -- The position in the array of lengths that is the max word
	#$t9 -- The position in the array of lengths that is the min word
	#$s5 -- The word of maximum length in the input
	#$s6 -- The word of min length in the input
	#0($t2) -- The first address of the array of lengths
	#$s3 -- The length of the max word
	#$s3 -- The length of the min word
	#nospace -- the identifier in memory of the array without whitespace
		
		#The maximum length of a word in the line.
		la 		$a0, output4
	    	li 		$v0, 4
	    	syscall

	    	la 		$a0, ($s3)	
 	    	li 		$v0, 1		
	    	syscall
		
		#The minimum length of a word in the line.
		la 		$a0, output5
	    	li 		$v0, 4
	    	syscall

	    	la 		$a0, ($s4)	
 	    	li 		$v0, 1		
	    	syscall
	    	
		#The word of maximum length in the line. 
		la 		$a0, output6
	    	li 		$v0, 4
	    	syscall
	    	
	    	#Loop to print word of maximum length
	    	la		$t0, nospace
	    	li		$t1, 0
	    	li		$t2, 0
	    	
#This prints the maximum word via a loop through nospace
max_loop:

    		beq		$t1, $t7, print_max_word			#Branch if greater than charbfmax
    		
    		addi		$t1, $t1, 1						#Increment count
    		addi		$t0, $t0, 1
    		
    		j max_loop
 
#The correct place was found   		
print_max_word:
 		
 		beq		$t2, $s3, end_max_loop			#Branch if greater than count of max word
 		
 		lb 		$a0, 0($t0)						#Load the byte
		li $v0, 11
    		syscall
    		
    		addi		$t0, $t0, 1						#Increment count						
    		addi		$t2, $t2, 1
    		
    		j print_max_word
    	
#End the loop	
end_max_loop:

		
		#The word of minimum length in the line. 
 		la 		$a0, output7
	    	li 		$v0, 4
	    	syscall
	    	
	    	#Loop to print word of minimum length
	    	la		$t0, nospace
	    	li		$t1, 0
	    	li		$t2, 0
	
#This prints the minumum word via a loop through nospace    	
min_loop:

    		beq		$t1, $t6, print_min_word			#Branch if greater than charbfmin
    		
    		addi		$t1, $t1, 1						#Increment
    		addi		$t0, $t0, 1
    		
    		j min_loop

#The correct place was found      		
print_min_word:
 		
 		beq		$t2, $s4, end_min_loop			#Branch if greater than count of min word
 		
 		lb 		$a0, 0($t0)						#Load the byte
		li $v0, 11
    		syscall
    			
    		addi		$t0, $t0, 1						#Increment
    		addi		$t2, $t2, 1
    		
    		j print_min_word

#End the loop
end_min_loop:

#Skip the empty output 
j end
	    	
#Code to print the input is empty and then end the program.
empty_input:
		la 		$a0, output1
	    	li 		$v0, 4
	    	syscall
	    	
#Code to end the program.	    		  	    	
end:	    	          	          	
            	
            	li $v0, 10
            	syscall

	    	
