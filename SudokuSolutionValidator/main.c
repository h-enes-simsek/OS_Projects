/* (c) 2022 Hasan Enes Şimşek
 * This code is licensed under MIT license (see LICENSE.txt for details)
 *
 * I've designed a sudoku validator with using pthreads while self-studying as described in
 * Operating System Concepts (Silberschatz, Galvin, and Gagne; 2009, 2011, or 2012)
 */


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>

void *validate_one_line(void *); /* one column or row checker */
void generate_solved_sudoku(int **s, int size); /* generate solved sudoku to check is valid */
void print_matrix(int** matrix, int numberOfLines, int numberColumns); /* print a amtrix to terminal */
void randomize(int arr[], int n); /* shuffle an array */

int** sudoku; /* sudoku matrix */
int isValid = 1; /* is sudoku valid */

struct timespec start, stop;

enum validation_direction 
{
	ROW = 0,
	COLUMN = 1
};

/* args structure to pass thread function */
struct arg_struct {
    int direction;
    int index;
		int size;
};

int main(int argc, char* argv[])
{
	int i, j;
	int size_of_sudoku = 5; /* default size of sudoku */
	if(argc > 1)
	{
		size_of_sudoku = atoi(argv[1]);
	}
	int number_of_threads = 2*size_of_sudoku; /* validator algo needs it */
	
	sudoku = malloc(size_of_sudoku*sizeof(int*)); /* allocate mem for rows */
	for(i=0; i < size_of_sudoku; i++)
	{
		sudoku[i] = malloc(size_of_sudoku*sizeof(int)); /* allocate mem for columns */
	} 
	
	printf("Solved sudoku:\n");
	generate_solved_sudoku(sudoku, size_of_sudoku);
	if(size_of_sudoku < 21)
		print_matrix(sudoku, size_of_sudoku, size_of_sudoku);
	
	printf("Validation process will be started with new threads.\n");
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	
	/* array to hold thread variables */
	//pthread_t** thread_id = malloc(number_of_threads*sizeof(pthread_t*));
	pthread_t thread_arr[number_of_threads];
	
	/* create threads */
	struct arg_struct args[number_of_threads];
	for(i=0; i < size_of_sudoku; i++)
	{
		args[i].direction = ROW; 
		args[i].index = i; 
		args[i].size = size_of_sudoku; 
		pthread_create(&thread_arr[i], NULL, validate_one_line, (void *)&args[i] );
	}
	for(i=0; i < size_of_sudoku; i++)
	{
		args[i+size_of_sudoku].direction = COLUMN; 
		args[i+size_of_sudoku].index = i; 
		args[i+size_of_sudoku].size = size_of_sudoku; 
		pthread_create(&thread_arr[i+size_of_sudoku], NULL, validate_one_line, (void *)&args[i+size_of_sudoku] );
	}
	
	for(j=0; j < number_of_threads; j++)
	{
		pthread_join(thread_arr[j], NULL); 
	}
	
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
	double time_elaps = (stop.tv_sec - start.tv_sec) * 1e6 + (stop.tv_nsec - start.tv_nsec) / 1e3;  // in microseconds
	printf("Validation has been finished, was sudoku solved validly: %d\n", isValid);
	printf("Elapsed time[usec]: %lf\n", time_elaps);
	
	
	printf("\nValidation process will be started on single thread.\n");
	isValid = 1; // reset value
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	
	for(i=0; i < size_of_sudoku; i++)
	{
		args[i].direction = ROW; 
		args[i].index = i; 
		args[i].size = size_of_sudoku; 
		validate_one_line(&args[i]);
	}
	for(i=0; i < size_of_sudoku; i++)
	{
		args[i+size_of_sudoku].direction = COLUMN; 
		args[i+size_of_sudoku].index = i; 
		args[i+size_of_sudoku].size = size_of_sudoku; 
		validate_one_line(&args[i+size_of_sudoku]);
	}
	
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
	time_elaps = (stop.tv_sec - start.tv_sec) * 1e6 + (stop.tv_nsec - start.tv_nsec) / 1e3;  // in microseconds
	
	printf("Validation has been finished, was sudoku solved validly: %d\n", isValid);
	printf("Elapsed time[usec]: %lf\n", time_elaps);
	
	
	
	for(i=0; i < size_of_sudoku; i++)
	{
		free(sudoku[i]); 
	}
	free(sudoku);
	
	
	return 0;
}

/* fill the sudoku matrix with randomized values (only rows will be valid) */
void generate_solved_sudoku(int **s, int size)
{
	/* create array from 1 to size */
	int numbers[size];
	int i, j;
	for(i=0; i < size; i++)
	{
		numbers[i] = i+1;
	}
	
	/* for each row, shuffle array and set */
	for(i=0; i < size; i++)
	{
		randomize(numbers, size);
		for(j=0; j < size; j++)
		{
			s[i][j] = numbers[j];
		}
	}
	
}

// A function to generate a random permutation of arr[]
void randomize(int arr[], int n )
{
	// Use a different seed value so that we don't get same
	// result each time we run this program
	srand ( time(NULL) );

	// Start from the last element and swap one by one. We don't
	// need to run for the first element that's why i > 0
	for (int i = n-1; i > 0; i--)
	{
		// Pick a random index from 0 to i
		int j = rand() % (i+1);

		// Swap arr[i] with the element at random index
		int temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}
}


void print_matrix(int** matrix, int numberOfLines, int numberColumns)
{
	int row, columns;
	for (row=0; row<numberOfLines; row++)
	{
		for(columns=0; columns<numberColumns; columns++)
		{
			printf("%d\t", matrix[row][columns]);
		}
		printf("\n");
	}
}

void *validate_one_line(void *arguments)
{
	struct arg_struct *args = (struct arg_struct *)arguments;
	int index = args->index;
	int arr[args->size];
	int arrIsExist[args->size];/* is ith element exist in arr */
	memset(arrIsExist, 0, args->size*sizeof(int) );
	int j;
	int isValidLocal = 1;
	
	/* load row or column to arrau to check */
	if(args->direction == ROW)
	{
		for(j=0; j < args->size; j++)
		{
			arr[j] = sudoku[index][j];
		}
	}
	else if(args->direction == COLUMN)
	{
		for(j=0; j < args->size; j++)
		{
			arr[j] = sudoku[j][index];
		}
	}
	
	/* validate array time O(n), space O(n) */
	for(j=0; j < args->size; j++)
	{
		if(arr[j] > args->size || arr[j] < 1)
		{
			isValidLocal = 0;
			isValid = 0;
		}
			
		else
		{
			arrIsExist[arr[j]-1] = 1;
		}
	}
	for(j=0; j < args->size; j++)
	{
		if(arrIsExist[j] == 0)
		{
			isValidLocal = 0;
			isValid = 0;
		}
			
	}
	
	/* print results
	printf("direction:%d i:%d ", args->direction, index);
	for(j=0; j < args->size; j++)
	{
		printf("%d-", arr[j]);
	}
	printf(" isValid%d\n", isValidLocal);
	*/
	
}