#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define SHARED_DATA_LEN 10
int shared_data[SHARED_DATA_LEN];

pthread_mutex_t mutex;

void init_shared_data()
{
    int i;
    for(i = 0; i < SHARED_DATA_LEN; i++)
    {
        shared_data[i] = 0;
    }
}

void* produce(void* arg)
{
    int i;
    for(i = 0; i < SHARED_DATA_LEN; i++)
    {
        shared_data[i] = 1;
        usleep(100); // 0.1 ms
    }
}

void* consume(void* arg)
{
    int i;
    printf("\nConsumer(without mutex):\n");
    for(i = SHARED_DATA_LEN - 1; i >= 0; i--)
    {
        printf("%d ",shared_data[i]);
        usleep(100); // 0.1 ms
    }
    printf("\n");
}

void* produce_with_mutex(void* arg)
{
    pthread_mutex_lock(&mutex);
    
    int i;
    for(i = 0; i < SHARED_DATA_LEN; i++)
    {
        shared_data[i] = 1;
        usleep(100); // 0.1 ms
    }
    
    pthread_mutex_unlock(&mutex);
}

void* consume_with_mutex(void* arg)
{
    pthread_mutex_lock(&mutex);
  
    int i;
    printf("\nConsumer(with mutex):\n");
    for(i = SHARED_DATA_LEN - 1; i >= 0; i--)
    {
        printf("%d ",shared_data[i]);
        usleep(100); // 0.1 ms
    }
    printf("\n");
    
    pthread_mutex_unlock(&mutex);
}


int main(void)
{
    // threads without mutex
    init_shared_data();
    pthread_t producer;
    pthread_t consumer;

    pthread_create(&producer, NULL, produce, NULL);
    pthread_create(&consumer, NULL, consume, NULL);

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    
    // threads with mutex
    init_shared_data();
    pthread_t producer_m;
    pthread_t consumer_m;
    
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&producer_m, NULL, produce_with_mutex, NULL);
    pthread_create(&consumer_m, NULL, consume_with_mutex, NULL);

    pthread_join(producer_m, NULL);
    pthread_join(consumer_m, NULL);
    
    pthread_mutex_destroy(&mutex); 
    return 0;
}