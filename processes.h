#ifndef PROCESSES_H
#define PROCESSES_H
#include <stdlib.h>


typedef struct Process {
    unsigned short pid;
    char *process_name;
    unsigned int burst_time;
    unsigned int mem_alloc;
} Process;


Process *create_process(char *process_name,unsigned int burst_time,unsigned int mem_alloc){
    Process *ptr_process = (Process*) malloc(sizeof(Process));
    ptr_process->process_name = process_name;
    ptr_process->burst_time = burst_time;
    ptr_process->mem_alloc = mem_alloc;
    return ptr_process;
}


void terminate_process(Process *ptr_process) {
    ptr_process=NULL;
    free(ptr_process);
}

#endif
