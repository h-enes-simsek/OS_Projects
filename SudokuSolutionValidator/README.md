# SudokuSolutionValidator
I've designed a sudoku validator with using pthreads while self-studying as described in Operating System Concepts (Silberschatz, Galvin, and Gagne; 2009, 2011, or 2012).

I wrote a function to create solved sudokus. (but only rows are guaranteed to be correct) 
Then, I assigned a thread for every row and every column. So total number of threads will be 2*N. N is fixed in the question but I changed it to dynamic. Program takes the value N as an input from terminal. (did not check 3x3 grids because N is dynamic)

Also, I validated whole matrix on single thread to compare elapsed times.

# Textbook Wants
A Sudoku puzzle uses a 9 × 9 grid in which each column and row, as well as
each of the nine 3 × 3 subgrids, must contain all of the digits 1 ⋅⋅⋅ 9. Figure
4.26 presents an example of a valid Sudoku puzzle. This project consists of
designing a multithreaded application that determines whether the solution
to a Sudoku puzzle is valid.

# Example Output
```
bash>./a.out 8
Solved sudoku:
3       1       7       5       4       2       6       8
7       3       6       4       5       1       2       8
6       7       2       5       4       3       1       8
2       6       1       4       5       7       3       8
1       2       3       5       4       6       7       8
3       1       7       4       5       2       6       8
7       3       6       5       4       1       2       8
6       7       2       4       5       3       1       8
Validation process will be started with new threads.
Validation has been finished, was sudoku solved validly: 0
Elapsed time[usec]: 587.300000

Validation process will be started on single thread.
Validation has been finished, was sudoku solved validly: 0
Elapsed time[usec]: 2.600000
```

# Building
```
git clone git@github.com:h-enes-simsek/SudokuSolutionValidator.git
cd \SudokuSolutionValidator 
gcc -pthread main.c
./a.out 7 (if you want to create 7x7 sudoku)
```
