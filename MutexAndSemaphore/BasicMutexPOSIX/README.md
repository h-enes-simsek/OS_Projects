# Basic Mutex Usage With Pthread
Two threads access the same shared source simultaneously. One thread tries to write and the other tries to read.

Firstly the program runs the threads without a sync mechanism, then runs with mutex. 

# What Do This Program Do?   

Shared array is initialized with zeros. Producer thread writes ones to shared array starting from the beginning. Consumer thread reads the data from the end and writes to another array(but not shared) to see results.


# Possible Outcomes
```
gcc main.c -pthread
./a.out
without mutex:
shared data initialized: [0 0 0 0 0 0 0 0 0 0 ]
_write[0], data[1 0 0 0 0 0 0 0 0 0 ]
data[9]=0, data[1 0 0 0 0 0 0 0 0 0 ]
data[8]=0, data[1 0 0 0 0 0 0 0 0 0 ]
_write[1], data[1 1 0 0 0 0 0 0 0 0 ]
data[7]=0, data[1 1 0 0 0 0 0 0 0 0 ]
_write[2], data[1 1 1 0 0 0 0 0 0 0 ]
data[6]=0, data[1 1 1 0 0 0 0 0 0 0 ]
_write[3], data[1 1 1 1 0 0 0 0 0 0 ]
data[5]=0, data[1 1 1 1 0 0 0 0 0 0 ]
_write[4], data[1 1 1 1 1 0 0 0 0 0 ]
data[4]=1, data[1 1 1 1 1 0 0 0 0 0 ]
_write[5], data[1 1 1 1 1 1 0 0 0 0 ]
data[3]=1, data[1 1 1 1 1 1 0 0 0 0 ]
_write[6], data[1 1 1 1 1 1 1 0 0 0 ]
data[2]=1, data[1 1 1 1 1 1 1 0 0 0 ]
_write[7], data[1 1 1 1 1 1 1 1 0 0 ]
data[1]=1, data[1 1 1 1 1 1 1 1 0 0 ]
_write[8], data[1 1 1 1 1 1 1 1 1 0 ]
data[0]=1, data[1 1 1 1 1 1 1 1 1 0 ]
_write[9], data[1 1 1 1 1 1 1 1 1 1 ]


What consumer reads: [1 1 1 1 1 0 0 0 0 0 ]


with mutex:
shared data initialized: [0 0 0 0 0 0 0 0 0 0 ]
_write[0], data[1 0 0 0 0 0 0 0 0 0 ]
_write[1], data[1 1 0 0 0 0 0 0 0 0 ]
_write[2], data[1 1 1 0 0 0 0 0 0 0 ]
_write[3], data[1 1 1 1 0 0 0 0 0 0 ]
_write[4], data[1 1 1 1 1 0 0 0 0 0 ]
_write[5], data[1 1 1 1 1 1 0 0 0 0 ]
_write[6], data[1 1 1 1 1 1 1 0 0 0 ]
_write[7], data[1 1 1 1 1 1 1 1 0 0 ]
_write[8], data[1 1 1 1 1 1 1 1 1 0 ]
_write[9], data[1 1 1 1 1 1 1 1 1 1 ]
data[9]=1, data[1 1 1 1 1 1 1 1 1 1 ]
data[8]=1, data[1 1 1 1 1 1 1 1 1 1 ]
data[7]=1, data[1 1 1 1 1 1 1 1 1 1 ]
data[6]=1, data[1 1 1 1 1 1 1 1 1 1 ]
data[5]=1, data[1 1 1 1 1 1 1 1 1 1 ]
data[4]=1, data[1 1 1 1 1 1 1 1 1 1 ]
data[3]=1, data[1 1 1 1 1 1 1 1 1 1 ]
data[2]=1, data[1 1 1 1 1 1 1 1 1 1 ]
data[1]=1, data[1 1 1 1 1 1 1 1 1 1 ]
data[0]=1, data[1 1 1 1 1 1 1 1 1 1 ]


What consumer reads: [1 1 1 1 1 1 1 1 1 1 ]
```

As you can see, without mutex consumer reads wrong data that includes zeros. However, with mutex producer runs first and consumer reads expected data.