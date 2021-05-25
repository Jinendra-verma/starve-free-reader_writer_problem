#include <iostream>
using namespace std;

struct Node{    
    Node* next;
    pid_t value;
};


class Queue{
    Node* Front, *Rear;
    
    public:
    
   	void push(pid_t val){        
        Node* new_process = new Node();             //new node(process)
        new_process->value = val;       
        
        if(Rear != NULL){
            Rear->next = new_process;               //if queue is empty
        }else{
            Front = new_process;
        }
        
        Rear = new_process;                         //pointing rear to new node
    }    
    
    pid_t pop(){ 
        
        if(Front == NULL){
            return -1;
        }
        else{
            pid_t process_id = Front->value;
            Front = Front->next;
            if(Front == NULL) Rear = NULL;                
                
            return process_id;
        }
    }
};

//Semaphore implementation
class Semaphore{    
    int value = 1;    
    Queue processes = Queue();          //processes that are to be executed
    
    public:

    void wait(pid_t process_id){
        value--;
        if(value < 0){
            processes.push(process_id);
            wait(process_id); 
        }
    }
    
    void signal()
    {
        value++;
        if(value <= 0){
            pid_t pid = processes.pop();
            wakeup(pid); 
        }
    }
};

//Declare Semaphores
struct Semaphore chance_queue;
struct Semaphore req;
struct Semaphore r_mutex;

int read_count = 0;
int data = 1;

void *reader(pid_t process_id){
    
    //  ENTRY SECTION
    chance_queue.wait(process_id);
    r_mutex.wait(process_id);

    read_count++;

    if(read_count == 1){
        req.wait(process_id);
    }

    chance_queue.signal();
    r_mutex.signal();
    
    //  CRITICAL SECTION
    printf("Readed data is: %d",data);
    
    //  EXIT SECTION
    r_mutex.wait(process_id);
    read_count--;

    if(read_count == 0) 
        req.signal();

    r_mutex.signal();

}

void *writer(pid_t process_id){
    //  ENTRY SECTION
    chance_queue.wait(process_id);
    req.wait(process_id);
    chance_queue.signal();    

    //  CRITICAL SECTION
    data += 2;

    //  EXIT SECTION
    req.signal();
}







