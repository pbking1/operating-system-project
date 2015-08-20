###Project 2
####Note
- alread talk to professor and he agree our team use the format we define
	- and do not need to output the flag.log file
	- only need the processevent.log file

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
			1 | 1 |1| 0 | 2
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

####bounded buffer problem
- written by Nhan Do