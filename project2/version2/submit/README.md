###Project 2
	team up: Bin Peng, Nhan Do
####Note
- alread talk to professor and he agree our team use the format we define
	- only need the processevent.log file
- The code are all tested fine in iupui bigred2 cluster, iupui pegasus

####reader writer problem
- written by bin peng
- the code is using c99, so look at the Makefile.
	- need to add -std=c99 when compile
	- and the code is tested in IU mason high performace cluster and mac
		- all run successfully
- the problem is solved using **writer priority**
	- and use lock function to implement the semaphores method
- input file
	- 2 in column 2 represent reader 
	- 1 in column 2 represent writer
	- **require_resource can only be 1**
	- **thread id sequence need to increase from 1**
	- input file format
		- already talk to professor and he agree I use this format
		- ```
			thread id | reader/writer |require_resource| arrive time | burst time
			1 | 1 |1| 1 | 2
			2 | 2 |1| 0 | 1
			3 | 2 |1| 0 | 2
			4 | 1 |1| 1 | 2
			5 | 1 |1| 1 | 1
		```

- the number of thread should be include in the first line
	- about the reader
		- only when the no writer is waiting, the reader can read
		- multi reader can read in the same time
	- about the writer
		- only one writer can write a resource in the same time
		- when a reader is reading, the writer will wait until it end, no preemptive
		- when a writer is writing, there are reader waiting, when a new writer come, the new writer will get the resource first
- output file
	- file name:processevent.log
	- ```
	processID Resource arrival-time start-time end-time
	2 1 0 0 1
	1 1 1 1 3
	4 1 1 3 5
	5 1 1 5 6
	3 1 0 6 8
	```
- use "make" command to compile
	- and use "./project2" to run the program

####bounded buffer problem
- written by Nhan Do
- input file
	- NUMBER_PRODUCERS
	- NUMBER_CONSUMERS
	- TOTAL_NUMBER_DATA: How many data point you want to producers to create

	- ```Example: input.1
		2 2 20
	```

	- which means: 2 producers, 2 consumers, and the data list contain 20 numbers (from 0 to 19)

- output file
	- Name: processLog_BoundedBuffer.log
	- List all events happenned in the timeline of the program:
	- List of data produced / consumed
	- When the producers/ consumers finished.

- use "make" to compile
	- use "./BoundedBuffer" to run the program