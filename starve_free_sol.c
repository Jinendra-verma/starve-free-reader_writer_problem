#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>


int read_count = 0;
int data = 1;
sem_t chance_queue,req,r_mutex;

void *reader(void *rid)
{  

    //  ENTRY SECTION
    sem_wait(&chance_queue);
    sem_wait(&r_mutex);

    read_count++;

    if(read_count == 1)
        sem_wait(&req);

    sem_post(&chance_queue);    
    sem_post(&r_mutex);
    
    //  CRITICAL SECTION
    printf("Reader %d: read data as %d\n",*((int *)rid),data);    


    //  EXIT SECTION
    sem_wait(&r_mutex);
    read_count--;

    if(read_count == 0) 
        sem_post(&req);
    
    sem_post(&r_mutex);
}

void *writer(void *wid)
{   

    //  ENTRY SECTION
    sem_wait(&chance_queue);
    sem_wait(&req);
    sem_post(&chance_queue);
    
    //  CRITICAL SECTION
    printf("Writer %d modified data from %d to %d\n",(*((int *)wid)),data,data+2);
    data += 2;
    
    //  EXIT SECTION
    sem_post(&req);
}




int main()
{   
    int readerCount,writerCount,mx;

    printf("Input Number of readers: ");
    scanf("%d", &readerCount);
    
    printf("Input Number of writers: ");
    scanf("%d", &writerCount);

    if(readerCount>=writerCount) mx=readerCount;
    else mx= writerCount;
        
    int arr[mx];
    for(int i = 0; i < mx; i++) 
        arr[i] = i+1;
    
    pthread_t t_read[readerCount],t_write[writerCount];    

    sem_init(&chance_queue,0,1);
    sem_init(&r_mutex,0,1);
    sem_init(&req,0,1);

    for(int i = 0; i < readerCount; i++)
        pthread_create(&t_read[i], NULL, (void *)reader, (void *)&arr[i]);
    
    for(int i = 0; i < writerCount; i++)
        pthread_create(&t_write[i], NULL, (void *)writer, (void *)&arr[i]);

    for(int i = 0; i < readerCount; i++)
        pthread_join(t_read[i], NULL);
    
    for(int i = 0; i < writerCount; i++)
        pthread_join(t_write[i], NULL);    

    sem_destroy(&chance_queue);
    sem_destroy(&r_mutex);
    sem_destroy(&req);

    return 0;    
}