#Name: Luke R. Prescott
#Unix login id: LP911211
#Lab instructor: Ji Yong Park
#Date and time of lab: Fridays at 10:25am

#This MAL program (p5a.mal) prompts the user for a positive decimal integer, reads a positive decimal integer typed by the user and outputs the following: 
		
		#1. The total number of 1's in the right half of the binary representation of the integer.
		#2. The total number of 0's in the left half of the binary representation of the integer.
		#3. The highest power of 2 that evenly divides the integer.
		#4. The value of the largest digit in the decimal representation of the integer.

		.data

prompt:     .asciiz     	"Please enter a positive decimal integer: "
input:     	.asciiz 	"\nThe inputted integer is: "
inputBIN:   .asciiz	"\nThe inputted integer in binary is: "
output1:    .asciiz    	"\nThe total number of 1's in the right half of the binary representation of the integer = "
output2:    .asciiz   	"\nThe total number of 0's in the left half of the binary representation of the integer = "
output3:    .asciiz     	"\nThe highest power of 2 that evenly divides the integer = "
output4:    .asciiz     	"\nThe value of the largest digit in the decimal representation of the integer = "
newLine:    .asciiz 	"\n"
int: 	    	.word 	0 	#This creates a location in memory of 32 bits (4 bytes) that defaults to the binary value representing 0.

   	    	#Registers and variables:
   	 
	    			#int -- Location in memory of inputted integer.
	    			#$s0 -- Location in register of inputted integer.
	   			#$s1 -- Location in register of the total number of 1's in the right half of the binary representation of the integer.
	    			#$s2 -- Location in register of the total number of 0's in the left half of the binary representation of the integer
	    			#$s3 -- Location in register of the highest power of 2 that evenly divides the integer
	    			#$s4 -- Location in register of the value of the largest digit in the decimal representation of the integer
	    			#$t0 -- The mask to compare to in loop and loop2
	    			#$t1 -- The count of bits (starts at 32) in loop and loop2
	    			#$t2 -- The value and operator adds to when comparing user input and mask in loop and loop2
	    			#$t3 -- The not of the users input in binary (used for calculating $s3)
	    			#$t4 -- The remainder used in loop3 for successive division by 10 to determine $s4
	    			#$t5	-- The integer value used in loop3 for successive division by 10 to determine $s4
        
            	.text
            	.globl     	main

main:
            	#Prompt the user for the positive decimal integer here. 
            	la           	$a0, prompt     	#The first address of prompt is loaded into $a0
            	li            	$v0, 4          	#4 is the print_string command. This prints the prompt.
            	syscall

	    	#Get the integer from the user
	    	li 	      	$v0, 5 		#Loads 5 into $v0 for getting an integer from the user.
	    	syscall
 	    
 	    	#Save the integer to a register that won't be changed and a location in memory (int).
 	    	move		$s0, $v0	#Adds the value in $v0 to $s0 for later usage.
 	    	sw		$s0, int	#Stores the contents of register $s0; int1 = $s0 (in RAM).
 	    
 	    	#Print the user's input prompt
 	    	la 		$a0, input		#Loads the first address of the input string into $a0.
 	    	li 		$v0, 4		#4 is the print_string command. This prints the input prompt.
 	    	syscall
 	    
 	    	#Print the user's input in integer form
 	    	lw 		$a0, int		#Adds the word in int to $a0 for printing.
 	    	li 		$v0, 1		#1 is the print_int command. This prints the int.
	    	syscall	
	    
	    	#Print the user's input in binary prompt
 	    	la 		$a0, inputBIN	#Loads the first address of the inputBIN string into $a0.
 	    	li 		$v0, 4		#4 is the print_string command. This prints the inputBIN when syscalled.
	    
	    	#Calculate the user's input in binary form and counts $s1 and $s2
	    	 
	    	#Create a mask and count, $t0 and $t1 respectively
 	    	add		$t0, $0, 1		#Sets $t0 register to 1 + 0 = 1
	    	sll		$t0, $t0, 31	#The shift left logical command here shifts $t0 31 places to the left leaving the value to fill the register with 0's			
	    	addi 		$t1, $0, 32  	#Adds 32 + 0 = $t1
	    	syscall
	    
	    	#This loop checks to see if the inputted int in binary form is a 1 or zero by using the and, branch and add operators,
	    	#The loop continues while the register $t1 is greater than zero, decrementing at each call of print_and_incr,
	    	#And this loop also increments values $s1 and $s2 when appropriate by referring to the current count and current digit..
	    	loop: 
  				and $t2, $s0, $t0					#This and operator checks if $t0 and $s0 are both 1, then moving it to $t2 
  				beq $t2, $0, print_and_incr			#This branches to print if $t2 remains 0
  				addi $t2, $0, 1					#This adds $t2 to 0 + 1 (if not zero)
  				j print_and_incr					#This jumps to print regardless
  	    
  	    	#Depending on what is contained in $t2 either 0 or 1 is printed from left to right in run I/O
  	    	print_and_incr: 
      			
  				li $v0, 1					#1 is the print_int command. This prints the int in $a0.
  				move $a0, $t2				#Moves either 0 or 1 to $a0
  				syscall
     			
     				bgt $t1, 16, incr_zeros			#If the count is greater than 16 jump to incr_zeros
     				ble $t1, 16, incr_ones			#If the count is less than or equal to 16 jump to incr_ones
  		
  		#Checks if the current value is 0 and increments
    	     	incr_zeros:
    	     			beq $t2, 1, decrement_and_jump	#If the value in $t2 is 1 skip this
             			addi $s2, $s2, 1				#Else increment $s2
             			j decrement_and_jump			#Jump to decrement_and_jump
  		
  		#Checks if the current value is 1 and increments
             	incr_ones:
             			beq $t2, 0, decrement_and_jump	#If the value in $t2 is 0 skip this
             			addi $s1, $s1, 1				#Else increment $s1
             			j decrement_and_jump			#Jump to decrement_and_jump
             		
  		#This function shifts the comparable in $t0 to the right one place, decrements count and jumps to the location appropriate.	
  	     	decrement_and_jump:
  	    			srl $t0, $t0, 1			#Shift $t0 to the right 1
  				addi $t1, $t1, -1			#Decrements count $t1 
			
  				bne $t1, $0, loop	  			#If the count is not 0 go to beginning of loop
  				beq $t1, $0, end_print_and_incr 	#If the count is 0 end the loop
             
             	#A place to jump when appropriate. The loop has completed.
  	     	end_print_and_incr:
  	     	
  	     			#Print the correct prompts and values for $s1 and $s2 here:
           		
				#Code to print a new line
	   			la 		$a0, newLine
	   			li 		$v0, 4
	    			syscall				
				           		           		
           			#Code to print the prompt and total number of 1's in the right half of the binary representation of the integer.
	   			la 		$a0, output1
	   			li 		$v0, 4
	    			syscall
	    				
	    			#Print the value calculated in loop above
	    			la		$a0, ($s1)	
 	    			li 		$v0, 1		
	    			syscall			

         			#Code to print the prompt and total number of 0's in the left half of the binary representation of the integer.
	  			la 		$a0, output2
	   	  		li 		$v0, 4
	    			syscall
	    				
	    			#Print the value calculated	    		
	    			la 		$a0, ($s2)	
 	    			li 		$v0, 1		
	    			syscall	
	
            	#Code to calculate and print the highest power of 2 that evenly divides the integer.
            	
	    			#Print the prompt
	    			la 		$a0, output3
	  			li 		$v0, 4
	    				
	    			move		$s3, $0		#Initialize the saved temp reg s3 to 0	
	    		
	    			# Calculate (find the rightmost 1-bit set) below	
	    			# ($s0) & -($s0) = the rightmost 1-bit set 
	    			# In two's complement form, -($s0) = not($s0 + 1)		  
	    			    		      		      		  
	    		    	not $t3, $s0			#This takes the opposite binary representation of the inputted int and assigns it to the reg. $t3
	    		    	addi $t3, $t3, 1			#This adds 1 to the value in $t3
	    		    	and $s3, $s0, $t3		#This takes the and of the original inputted integer and its opposite + 1, assigning it to $s3
				
				#Reset mask and count
		 	   	add		$t0, $0, 1	#Sets $t0 register to 1 + 0 = 1
	    			sll		$t0, $t0, 31	#The shift left logical command here shifts $t0 31 places to the left leaving the value to 
	    								#fill the register with 0's
	    			addi 		$t1, $0, 32  	#Adds 32 + 0 = $t1	
	    			syscall   
	    
	    	#This second loops looks for the first location of a 1 in the binary representation of the rightmost 1-bit set (i.e. counts the number of zeros to the right)
	    	loop2: 
  				and $t2, $s3, $t0							#This and operator checks if $t0 and $s3 are both 1, then moving it to $t2 
  				beq $t2, $0, determine_correct_location		#This branches to print if $t2 remains 0
  				addi $t2, $0, 1						#This adds $t2 to 0 + 1 (if not zero)
  				j determine_correct_location					#This jumps to print 
  	    
  	    	#Depending on what is contained in $t2 (either 0 or 1) the location will be assigned
  	    	determine_correct_location: 	
  				srl $t0, $t0, 1		#Shift to the right 1
  				addi $t1, $t1, -1		#Decrements count $t1 
  			
  				beq $t2, 1, assign_power_of_two		#The location to set $s3
			
  				bne $t1, $0, loop2	  			#If the count is not 0 go to beginning of loop
  				beq $t1, $0, end_print_and_assign 	#If the count is 0 end the loop
  	     
  	     	#Assigns the power of two to the current location of the 1 bit left over from operations above
  	    	assign_power_of_two:
  	     			move $s3, $t1
  	     		
              	#A place to jump when appropriate. The loop has completed.
  	     	end_print_and_assign:
			    				    			
	    			#Print the value calculated
	    			la 		$a0, ($s3)	
 	    			li 		$v0, 1		
	    			syscall	

	    	#Code to extract the decimal digits and compute the largest digit, using successive divisions by 10.
            
            			#Code to calculate and print output prompt
	    			la 		$a0, output4
	    			li 		$v0, 4
	    				
	    			move		$s4, $0		#Initialize the saved temp reg s4 to 0	
	    		
	    			#Set s4 to 0	
	    			move		$t4, $0
	    			syscall   
	    		    		
	    	#This third loop uses successive division by 10 to determine the largest digit in the decimal representation of the integer.
	    	loop3: 
	    			
	    			div $s0, $s0, 10 			#Divide $s3 by 10 and set itself to the resulting integer value 
	    			mfhi $t4				#Set remainder to $t4 using move from hi
	    			mflo $t5				#Set integer result to $t5 using move from lo

	    			blt $s4, $t4, assign_digit	#If the value in $s4 < $t4, jump to assign digit (set $s4 to $t4)
	    			
				blez $t5, end_divide		#Branch to the end of the loop if the integer result is less than or equal to 0
				bgtz $t5, loop3			#Branch to the beginning of the loop if the integer result is still greater than 0
  		
  		#This assigns the correct current digit to $s4 and then jumps accordingly
  		assign_digit:
  				move $s4, $t4			#Set the contents of $s4 to $t4
  				blez $t5, end_divide		#Branch to the end of the loop if the integer result is less than or equal to 0
				bgtz $t5, loop3			#Branch to the beginning of the loop if the integer result is still greater than 0
  		
  		#A place to jump when appropriate. The loop has completed.		
  		end_divide:	
  		
	    			#Print the value calculated
	    			la 		$a0, ($s4)	
 	    			li 		$v0, 1		
	    			syscall	
	    			
            	#Code to end the program.
            	li $v0, 10
            	syscall