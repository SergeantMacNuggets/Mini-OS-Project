#include <time.h>
#include <signal.h>
#include "processes.h"
#include "queue.h"
#include "linkedlist.h"

#define TIME_QUANTUM 4

volatile sig_atomic_t stop;

void inthand(int signum) {
    stop = 1;
}

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

void *dispatcher();



Queue *new_queue = NULL;
Queue *ready_queue = NULL;
Queue *waiting_queue = NULL;
Queue *terminated_queue = NULL;
Process *running_process = NULL;
LinkedList *processes_list = NULL;
int interrupted = 0;


void new_process_queue(Process *process) {
    if(process != NULL){
        srand(time(NULL));
        int random_pid = rand() % 1000 + 1;
        process->pid = random_pid;
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
        int process_time = (running_process->burst_time >= TIME_QUANTUM) ?
         TIME_QUANTUM : running_process->burst_time;

        waitFor(process_time);
        running_process->burst_time-=process_time; 
        
        if(running_process->burst_time<=0) {
            running_process->process_status=TERMINATED;
            enqueue(terminated_queue, running_process);
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

    processes_list = create_linked_list();
    new_queue = createQueue();
    ready_queue = createQueue();
    waiting_queue = createQueue();
    terminated_queue = createQueue();

    while(1){
        if(interrupted) {
            interrupted = 0;
        }
        else {
            ready_process_queue();
            running();
            waiting_process_queue();
            waitFor(1);
        }
    }
}

void print_running() {
    printf("Running:\t\t\t");
    if(running_process!=NULL) {
        printf("[%s]", running_process->process_name);
    }
    else {
        printf("No Process Running");
    }
    printf("\n");
}

void kill_process(unsigned short pid) {
    Process *p = find_process(processes_list, pid);
    if (p != NULL) {
        interrupted = 1;
        waitFor(1);

        switch (p->process_status) {
            case READY:
                if (ready_queue && find_n_delete(ready_queue, p->pid)) {
                    p->process_status = TERMINATED;
                    enqueue(terminated_queue, p);
                }
                break;

            case WAITING:
                if (waiting_queue && find_n_delete(waiting_queue, p->pid)) {
                    p->process_status = TERMINATED;
                    enqueue(terminated_queue, p);
                }
                break;

            case RUNNING:
                if (running_process && running_process->pid == p->pid) {
                    running_process->process_status = TERMINATED;
                    enqueue(terminated_queue, running_process);
                    running_process = NULL; // clear it to avoid reuse
                }
                break;

            default:
                break;
        }
    }
    
}


void display_status() {

    signal(SIGINT, inthand);
    stop = 0;
    while(!stop){

        display_list(processes_list);
        waitFor(1);
    }

    system("paise");
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