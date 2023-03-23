Description of the project is given in the report.pdf file. Memory traces are read by and a page replacement algorithm is used to simulate the paging of virtual addresses.
***********************************************************************************************************************************************************************

File(s) to compile the program:
-memsim.cpp

Additional files:
-bzip.trace
-sixpack.trace
***********************************************************************************************************************************************************************

To compile the program:
1. In the same directory as the main source code, use command "make" to compile all files in the makefile under the "memsim" target. 
2. Makefile contains the command: g++ -o memsim memsim.cpp
3. If not using the makefile, just run the command: g++ -o memsim memsim.cpp

To run the program:
General format: ./memsim <tracefile> <nframes> <lru|fifo|vms> <p> <debug|quiet>
<tracefile> --> name of the tracefile to be read in 
<nframes> --> number of frames 
<lru|fifo|vms> --> type of replacement algorithm
<p> --> percentage of secondary buffer if algorithm is "vms" otherwise don't include <p> in the command
<debug|quiet> --> quiet will print the statistics in the end, debug will print information on the screen for every virtual address read.

NOTE: ON "DEBUG" MODE, THE PROGRAM TAKES A LONG TIME TO RUN
 
1. For FIFO: 
./memsim bzip.trace 64 fifo quiet
./memsim bzip.trace 64 fifo debug

2. For LRU:
./memsim bzip.trace 64 lru quiet
./memsim bzip.trace 64 lru debug

3. For SEGMENTED-FIFO
./memsim bzip.trace 64 vms 25 quiet
./memsim bzip.trace 64 vms 75 debug


********RUNNING EXAMPLES**********************************************************************************************************************************************
Example 1: ./memsim bzip.trace 64 fifo quiet
total memory frames: 64
events in trace: 1000000
total disk reads: 1467
total disk writes: 514

Example 2: ./memsim bzip.trace 64 lru quiet
total memory frames: 64
events in trace: 1000000
total disk reads: 1264
total disk writes: 420

Example 3: ./memsim bzip.trace 64 vms 0 quiet
vms with parameter: 0
total memory frames: 64
events in trace: 1000000
total disk reads: 1467
total disk writes: 514

Example 4: ./memsim bzip.trace 64 vms 25 quiet
vms with parameter: 25
total memory frames: 64
events in trace: 1000000
total disk reads: 1269
total disk writes: 422

Example 5: ./memsim bzip.trace 64 vms 50 quiet
vms with parameter: 50
total memory frames: 64
events in trace: 1000000
total disk reads: 1265
total disk writes: 420

Example 6: ./memsim bzip.trace 64 vms 75 quiet
vms with parameter: 75
total memory frames: 64
events in trace: 1000000
total disk reads: 1265
total disk writes: 420

Example 7: ./memsim bzip.trace 64 vms 100 quiet
vms with parameter: 100
total memory frames: 64
events in trace: 1000000
total disk reads: 1264
total disk writes: 420