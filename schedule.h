#include <time.h>
#include "processes.h"
#include "queue.h"

#define TIME_QUANTUM 4

void *dispatcher();

void delay(int milli_seconds)
{
	clock_t start_time = clock();

	while (clock() < start_time + milli_seconds);
}


Queue *new_queue = NULL;
Queue *ready_queue = NULL;
Queue *waiting_queue = NULL;
Process *running_process = NULL;


void new_process_queue(Process *process) {
    if(process != NULL){
        enqueue(new_queue, process);
    }
}

void ready_process_queue() {
    
    if(get_current_process(waiting_queue) != NULL) {
        Process *peak_waiting_process = get_current_process(waiting_queue);
        enqueue(ready_queue, peak_waiting_process);
        dequeue(waiting_queue);
    }

    if(get_current_process(new_queue) != NULL) {
        Process *peak_new_process = get_current_process(new_queue);
        enqueue(ready_queue, peak_new_process);
        dequeue(new_queue);
    }

}

void running() {
    if(get_current_process(ready_queue) != NULL) {
        running_process = get_current_process(ready_queue);
        dequeue(ready_queue);
    }
}

void waiting_process_queue() {
    if(running_process!=NULL) {
        enqueue(waiting_queue, running_process);
        running_process = NULL;
        // free(running_process);
    }
}

void *dispatcher() {
    new_queue = createQueue();
    ready_queue = createQueue();
    waiting_queue = createQueue();

    while(1){
        ready_process_queue();
        running();
        waiting_process_queue();
        delay(1000);
    }
}