# Basic Mutex Usage With Pthread
Two threads access the same shared source simultaneously. One thread tries to write and the other tries to read.

Firstly the program runs the threads without a sync mechanism, then runs with mutex.   

# Possible Outcomes
```
sudo gcc main.c -pthread
./a.out
Consumer(without mutex):
0 0 0 0 0 1 1 1 1 1

Consumer(with mutex):
1 1 1 1 1 1 1 1 1 1
```