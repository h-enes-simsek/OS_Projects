#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define SHARED_DATA_LEN 10
int shared_data[SHARED_DATA_LEN]; // producer writes, consumer reads
int consumers_final_data[SHARED_DATA_LEN]; // only consumer writes

pthread_mutex_t mutex;


void print_data(int data[])
{
    printf("[");
    int i;
    for(i = 0; i < SHARED_DATA_LEN; i++)
    {
        printf("%d ", data[i]);
    }
    printf("]\n");
}

// init array with zeros
void init_shared_data()
{
    int i;
    for(i = 0; i < SHARED_DATA_LEN; i++)
    {
        shared_data[i] = 0;
    }
    printf("shared data initialized: ");
    print_data(shared_data);
}

// producer writes the shared data
void* produce(void* arg)
{
    int i;
    for(i = 0; i < SHARED_DATA_LEN; i++)
    {
        shared_data[i] = 1;
        printf("_write[%d], data", i);
        print_data(shared_data);
        usleep(100); // 0.1 ms
    }
}

// consumer reads the shared data and writes to consumers_final_data
void* consume(void* arg)
{
    int i;
    for(i = SHARED_DATA_LEN - 1; i >= 0; i--)
    {
        consumers_final_data[i] = shared_data[i];
        printf("data[%d]=%d, data", i, shared_data[i]);
        print_data(shared_data);
        usleep(100); // 0.1 ms
    }
    printf("\n");
}

// producer writes the shared data
void* produce_with_mutex(void* arg)
{
    pthread_mutex_lock(&mutex);
    
    int i;
    for(i = 0; i < SHARED_DATA_LEN; i++)
    {
        shared_data[i] = 1;
        printf("_write[%d], data", i);
        print_data(shared_data);
        usleep(100); // 0.1 ms
    }
    
    pthread_mutex_unlock(&mutex);
}

// consumer reads the shared data and writes to consumers_final_data
void* consume_with_mutex(void* arg)
{
    pthread_mutex_lock(&mutex);
  
    int i;
    for(i = SHARED_DATA_LEN - 1; i >= 0; i--)
    { 
        consumers_final_data[i] = shared_data[i];
        printf("data[%d]=%d, data", i, shared_data[i]);
        print_data(shared_data);
        usleep(100); // 0.1 ms
    }
    printf("\n");
    
    pthread_mutex_unlock(&mutex);
}


int main(void)
{
    // threads without mutex
    printf("without mutex:\n");
    init_shared_data();
    pthread_t producer;
    pthread_t consumer;

    pthread_create(&producer, NULL, produce, NULL);
    usleep(100); // consumer should start second
    pthread_create(&consumer, NULL, consume, NULL);

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    
    printf("\nWhat consumer reads: ");
    print_data(consumers_final_data);
    
    // threads with mutex
    printf("\n\nwith mutex:\n");
    init_shared_data();
    pthread_t producer_m;
    pthread_t consumer_m;
    
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&producer_m, NULL, produce_with_mutex, NULL);
    usleep(100); // consumer should start second
    pthread_create(&consumer_m, NULL, consume_with_mutex, NULL);

    pthread_join(producer_m, NULL);
    pthread_join(consumer_m, NULL);
    
    printf("\nWhat consumer reads: ");
    print_data(consumers_final_data);
    
    pthread_mutex_destroy(&mutex); 
    return 0;
}
