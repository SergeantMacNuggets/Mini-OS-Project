#include <time.h>
#include "processes.h"
#include "queue.h"
#include "linkedlist.h"

#define TIME_QUANTUM 4

// void delay(int milli_seconds)
// {

// 	clock_t start_time = clock();

// 	while (clock() < start_time + milli_seconds);
// }

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

void *dispatcher();



Queue *new_queue = NULL;
Queue *ready_queue = NULL;
Queue *waiting_queue = NULL;
Process *running_process = NULL;
LinkedList *processes_list = NULL;


void new_process_queue(Process *process) {
    if(process != NULL){
        enqueue(new_queue, process);
        add_element(processes_list, process);
    }
}

void ready_process_queue() {
    
    if(get_current_process(waiting_queue) != NULL) {
        Process *peak_waiting_process = get_current_process(waiting_queue);
        peak_waiting_process->process_status = READY;
        enqueue(ready_queue, peak_waiting_process);
        dequeue(waiting_queue);
    }

    if(get_current_process(new_queue) != NULL) {
        Process *peak_new_process = get_current_process(new_queue);
        peak_new_process->process_status = READY;
        enqueue(ready_queue, peak_new_process);
        dequeue(new_queue);
    }

}

void running() {
    if(get_current_process(ready_queue) != NULL) {
        running_process = get_current_process(ready_queue);
        running_process->process_status = RUNNING;
        dequeue(ready_queue);
        int process_time = (running_process->burst_time >= TIME_QUANTUM) ? TIME_QUANTUM : running_process->burst_time;
        running_process->burst_time-=process_time;
        waitFor(process_time);
        if(running_process->burst_time<=0) {
            running_process->process_status=TERMINATED;
        }
    }
}

void waiting_process_queue() {
    if(running_process!=NULL && running_process->burst_time > 0) {
        running_process->process_status = WAITING;
        enqueue(waiting_queue, running_process);
        running_process = NULL;
    } else {
        running_process = NULL;
    }
}

void *dispatcher() {
    // running_process = (Process*)malloc(sizeof(Process));

    processes_list = create_linked_list();
    new_queue = createQueue();
    ready_queue = createQueue();
    waiting_queue = createQueue();

    while(1){
        ready_process_queue();
        running();
        waiting_process_queue();
        waitFor(1);
    }
}

void print_running() {
    printf("Running: ");
    if(running_process!=NULL){
        printf("[%s]", running_process->process_name);
    }
    printf("\n");
}

void display_status() {
    while(1){

        printf("New Queue: ");
        displayAllQ(new_queue);

        printf("Ready Queue: ");
        displayAllQ(ready_queue);

        print_running();

        printf("Waiting Queue: ");
        displayAllQ(waiting_queue);
        display_list(processes_list);
        waitFor(1);
    }
}