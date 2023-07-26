# CPU Usage Tracker

## COMPLETED PART

### Please run "setterm -linewrap off" command before running the code. Because I tried to implement a progression bar but it moves if linewrap is not set off.

Program has 5 threads except main threads. These are: 
* Executable files are located in cmake-build-debug file
* All log files located in cmake-build-debug file


1) Reader,which read "proc/stats",
2) Analyzer,which operates analytical calculations,
3) Printer,which prints the output,
4) Watchdog, which tries to detect if any deadlock is happening
5) Logger ,which records the CUT data in txt file.
6) Unit test for testing the correctness of array_comp function is added.
7) Producer(Reader), consumer(analyzer) solutions are implemented
8) Bounded and Circular buffer ideas are used to solve this problem
9) Logger File presents at the time of each read, if deadlock occurs also it shows date of these deadlocks.
10) Sigterm handler function now detects three type of signals.
* Also,this program contains a sigterm handler function. To close the program properly and release the heap memory.

