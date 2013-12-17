CPS633
======

This is a working project for Lab2 of CPS633 course from Ryerson University

CPS 633 - LAB #2 - README



This readme contains brief descriptions with regards to the functionality of all programs written in C for this project, i.e. questions 1, 2. There are also instructions provided for compiling each solution.

***IMPORTANT NOTE*** The applications were written on a Linux system (Ubuntu) using a different version of the GCC compiler than that used on the moon.scs.ryerson.ca server. As a result, question 1 and 2 solution works as intended on my Linux VM, but may not function correctly on the Ryerson moon server.


TABLE OF CONTENTS
(I) 	REQUIREMENTS
(II) 	DESCRIPTIONS
(III) 	INSTRUCTIONS





(I)			REQUIREMENTS

- Pentium CPU or better
- At least 1GB of RAM
- At least 10MB of storage space
- Linux operating system

(II)			DESCRIPTIONS

1. Discretionary Access Control (DAC)

The source code containing the solution to this problem can be found in:
	DAC.c

2. Role-Based Access Control (RBAC)

The source code containing the solution to this problem can be found in:
	RBAC.c

(III)			INSTRUCTIONS

COMPILING:

To compile the source code provided for questions 1, 2, and 3, enter the following in a linux terminal window:

	gcc -o DAC DAC.c
	gcc -o RBAC RBAC.c

	EXAMPLE:

	/home/dmoscuzz/cps633> gcc -o DAC DAC.c
	/home/dmoscuzz/cps633> gcc -o RBAC RBAC.c
	/home/dmoscuzz/cps633> 




USING THE APPLICATIONS:

The applications will prompt the user for input.
