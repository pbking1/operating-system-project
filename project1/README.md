###Project 1 Thread Scheduling
	bin peng

####project description
- The project is written in C++
- And tested in OSX.
- And implement the FCFS, SJF, RR, PRIO, PRIO_NON cpu schduling algorithm
- After that, I calculate the average waiting time, average response time, average turnaround time, overall cpu usage.

####brief description of the file
- project1.cpp
	- The file contain the implemment of FCFS, SJF, RR, PRIO, PRIO_NON cpu schduling algorithm
	- And using c++ STL.
- Makefile
	- used to compile the c++ file
	- simply type "make"
	- And then type "./proj1 input_file FCFS|SJF|RR|PRIO [quantum]" to run the program
		- for exmaple
			- "./proj1 input.1 SJF"
			- "./proj1 input.1 FCFS"
			- "./proj1 input.1 RR 2"
			- "./proj1 input.7 PRIO"
			- "./proj1 input.7 PRIO_NON"
- note that the priority scheduling need to run using input.7 file as input
	- because there will be a new column for the priority

- test file
	- input.1
		- origin file
	- input.2
		- use to test the CPU usage
		- because sometime the job ends but the new job have not arrived
	- input.3
		- all the jobs arrived in the same time
	- input.4
		- three jobs come at the same time
	- input.5
		- three jobs come at the same time and the burst time is sorted
		- after that, three project are come and the burst time is not sorted
	- input.6
		- the burst time is sorted
	- input.7
		- used for priority schduling
		- add a new priority column in the 4th column
