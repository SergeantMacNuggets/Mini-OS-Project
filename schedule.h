#include <time.h>
#include <signal.h>
#include "processes.h"
#include "memory_partition.h"
#include "queue.h"
#include "linkedlist.h"

#define TIME_QUANTUM 4

volatile sig_atomic_t stop;

typedef struct Running {
    Process *running_process;
}Running;

void inthand(int signum) {
    stop = 1;
}

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;
    while (time(0) < retTime);  
}

void *dispatcher();

// Where the dispatcher thread running

Queue *new_queue = NULL;
Queue *ready_queue = NULL;
Queue *waiting_queue = NULL;
Queue *terminated_queue = NULL;
Process *running_process = NULL;
LinkedList *processes_list = NULL;
volatile int interrupted = 0;


void new_process_queue(Process *process) {
    if(process != NULL){
        srand(time(NULL));
        int random_pid = rand() % 10000 + 1;
        process->pid = random_pid;
        enqueue(new_queue, process);
        add_element(processes_list, process);
    }
}


void terminate(Process *ptr_process) {
    ptr_process->process_status = TERMINATED;
    ptr_process->burst_time=0;
    enqueue(terminated_queue, ptr_process);
}

void ready_process_queue() {
    if(running_process!=NULL) {
        //if running process is not null check first if the running process was terminated or finished its burst time
        if(running_process->process_status==TERMINATED || running_process->burst_time <= 0) {

            enqueue(terminated_queue, running_process);

            running_process=NULL;
        }
        //if not running process will be queued back to the ready queue
        else {
            running_process->process_status = READY;

            enqueue(ready_queue,running_process);
                
            running_process=NULL;
        }
    }

    if(get_current_process(waiting_queue) != NULL) {

        Process *peak_waiting_process = get_current_process(waiting_queue);

        //Same procedure but check if its terminated from the kill function
        if(peak_waiting_process->process_status==TERMINATED || peak_waiting_process->burst_time <= 0) {

            enqueue(terminated_queue, peak_waiting_process);

            dequeue(waiting_queue);
                
        }
        else {
            //else, if there is space in the memory partition first in the waiting queue will be queued in ready queue.
            for(int i=0;i<NUM_PARTITION;i++) {

                if(memory_partitions[i]->process==NULL) {

                    peak_waiting_process->process_status = READY;

                    peak_waiting_process->mem_addr = i;

                    memory_partitions[i]->process = peak_waiting_process;

                    enqueue(ready_queue, peak_waiting_process);

                    dequeue(waiting_queue);
                }
            }   
        }
    }

    if(get_current_process(new_queue) != NULL) {

        Process *peak_new_process = get_current_process(new_queue);

        //lastly check if new process was created to be put in the ready queue or in the waiting queue if no memory space
        for(int i=0 ;i<=NUM_PARTITION;i++) {

            if(i==NUM_PARTITION) {

                peak_new_process->process_status=WAITING;

                enqueue(waiting_queue, peak_new_process);

                break;
            } 
            else if(memory_partitions[i]->process==NULL) {

                peak_new_process->process_status=READY;

                peak_new_process->mem_addr = i;

                memory_partitions[i]->process = peak_new_process;

                enqueue(ready_queue,peak_new_process);

                break;
            }
        }
        dequeue(new_queue);
    }

}
        

void running() {
    Process *ready = get_current_process(ready_queue);
    if(!ready) return;
    
    //here where it check if the proces was killed by the user
    else if(ready && ready->process_status == TERMINATED) {

        enqueue(terminated_queue, ready);

        dequeue(ready_queue);
    }
    else {
        running_process = get_current_process(ready_queue);

        dequeue(ready_queue);

        running_process->process_status = RUNNING;

        //if the burst time of a process is less than then process time will be equal to the procecss running
        int process_time = (running_process->burst_time >= TIME_QUANTUM) ?
            TIME_QUANTUM : running_process->burst_time;


        for(int i=0;i<process_time;i++) {

            waitFor(1);
            running_process->burst_time--;    
            

        }
        if(running_process->burst_time <= 0) {

            //If the prcess finished its job after running immidietly get thrown to terminated queue
            running_process->process_status = TERMINATED;

            enqueue(terminated_queue, running_process);

            memory_partitions[running_process->mem_addr]->process = NULL;

            running_process=NULL;
            
        }
    }

}


void *dispatcher() {

    processes_list = create_linked_list();
    new_queue = createQueue();
    ready_queue = createQueue();
    waiting_queue = createQueue();
    terminated_queue = createQueue();

    while(1){
        //Where the back end loop happens
        ready_process_queue();
        running();

        //Check if the user manipulate the loop such as killing a process mid iteration if so it will be interrupted
        if(interrupted) {
            interrupted=0;
            continue;
        }
        waitFor(1);
    }
}

void print_running() {
    printf("Running:\t\t\t");
    if(running_process!=NULL) {
        printf("[%s:%ds]", running_process->process_name,running_process->burst_time);
    }
    else {
        printf("No Process Running");
    }
    printf("\n");
}


void kill_process(unsigned short pid) {
    Process *p = find_process(processes_list, pid);
    if (p != NULL) {
        //kill command, where the proces will change its status to terminated and will be transfer to the termination queue upon its first queue
        p->process_status=TERMINATED;
        p->burst_time=0;

        //Its occupation in the memory will be remove
        memory_partitions[p->mem_addr]->process=NULL;
        interrupted = 1;
    }
}




void display_status() {

    signal(SIGINT, inthand);
    stop = 0;
    while(!stop){
        if(processes_list->process==NULL) {
            printf("No Process has been created\n");
            return 0;
        }
        display_list(processes_list);
        waitFor(1);
    }

    system("paise");
    return 0;
}

void display_mem() {
    signal(SIGINT, inthand);
    stop = 0;
    while(!stop) {
        display_memory(waiting_queue);
        waitFor(1);
    }
    system("praise");
    return 0;
}


void display_schedule() {
    signal(SIGINT, inthand);
    stop = 0;

    printf("\n");
    while(!stop) {
        printf("New Queue:\t\t\t");
        displayAllQ(new_queue);

        printf("Ready Queue:\t\t\t");
        displayAllQ(ready_queue);

        print_running();
        
        printf("Waiting Queue:\t\t\t");
        displayAllQ(waiting_queue);

        printf("Terminated Queue:\t\t");
        displayAllQ(terminated_queue);

        printf("Ctrl+C to Exit\n\n");
        waitFor(1);
    }

    system("praise");
    return 0;
}