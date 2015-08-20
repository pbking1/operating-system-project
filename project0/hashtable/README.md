###Project 0
	bin peng

####project description
- The project is written in C 
- And implement the hash table
- After that, use the hash table to allocate resource to the processes

####brief description of the file
- hash.c
	- The file contain the implemment the data structure of the hash table
	- And use the hash table to allocate resource
	- the hash function is (key*4)%10
	- int the main function
		- I first create a table that have 10 processes
		- And than print the table out
		-  after that I allocate the resource to the processes
		- then print the table out to see the result
		- the search for a resource
		- then delete a resource from the process
		- then print the table out to see the delete result
- Makefile
	- used to compile the c file
	- simply type "make"
	- And then type "./project0" to run the program

