                                                  starve free reader writer problem solution
 
A reader writer problem is a situation when multiple processes are trying to access(read) and edit(write) the same data structure or shared file simultaneously. In the classical solution of this problem, there is starvation for either reader or writer. So in a starve free solution of the problem to avoid this starvation, only one writer is allowed to access the critical section at any point of time and when there is no writer active then any number of readers can access the critical section.
In our solution we have used three semaphores namely chance_queue,req and r_mutex. chance_queue represents the chance of the next process to enter the critical section, req is the semaphore required to access the critical section and r_mutex is required to change the read_count variable. 

**Code explanation:**

**Initialization :**


int read_count = 0;					// set read count variable=0 which represents the number of active readers	
int data = 1;			
sem_t chance_queue,req,r_mutex;	    // declaring semaphores		

**Reader’s code :**

//  ENTRY SECTION
    sem_wait(&chance_queue);	//wait for its chance
    sem_wait(&r_mutex);	    //requesting access to change read_count
    read_count++;	         //increase read_count by 1 


if(read_count == 1)	     // if the current reader is the first reader wait till Other writers release the resource semaphore 
        sem_wait(&req);	 

sem_post(&chance_queue);    //Release the chance_queue semaphore for other process
sem_post(&r_mutex);         Release access to he read count
    
//  CRITICAL SECTION
    printf("Reader %d: read data as %d\n",*((int *)rid),data);    


//  EXIT SECTION

sem_wait(&r_mutex);     //Requesting access to change read_count

read_count--;           //Decreasing read count after reading is done

if(read_count == 0)      //If no other reader is remaining then release req semaphore 
    sem_post(&req);

sem_post(&r_mutex);     //Allow other readers to edit read_count 


**Writer’s code :**

//  ENTRY SECTION


sem_wait(&chance_queue);    //wait for its chance

sem_wait(&req);           //requesting access to change read_count

sem_post(&chance_queue);    //Release the chance_queue semaphore for other process
    
//  CRITICAL SECTION
printf("Writer %d modified data from %d to %d\n",(*((int*)wid)),data,data+2);
    	data += 2;


//  EXIT SECTION

sem_post(&req);     //Release req semaphore for next process
