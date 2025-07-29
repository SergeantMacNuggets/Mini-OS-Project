#include <stdio.h>
#include <stdlib.h>
#include "processes.h"


typedef struct LinkedList {
    Process *process;
    struct LinkedList *next;
}LinkedList;


LinkedList *create_linked_list() {
    LinkedList *head = (LinkedList*)malloc(sizeof(LinkedList));
    head->process = NULL;
    head->next = NULL;
    return head;
}

void add_element(LinkedList *head, Process *p) {
    if(head->process==NULL) {
        head->process = p;
        return;
    }
    LinkedList *ptr = head;
    while(ptr->next!=NULL){
        ptr=ptr->next;
    }
    ptr->next = (LinkedList*)malloc(sizeof(LinkedList));
    ptr->next->process=p;
}

char *status_name(Process *p) {
    switch(p->process_status) {
        case NEW: return "NEW";
        case READY: return "READY";
        case RUNNING: return "RUNNING";
        case WAITING: return "WAITING";
        case TERMINATED: return "TERMINATED";
    }
    return NULL;
}

void display_list(LinkedList *head) {

    LinkedList *ptr = head;
    printf("\033[2J\033[H");
    while (ptr != NULL) {
        Process *proc = ptr->process;
        printf("Process: %s (%d)                    \tStatus: %-10s            \tBurst-Time: %d\n", 
               proc->process_name, proc->pid, status_name(proc), proc->burst_time);
        ptr = ptr->next;
    }
    printf("Ctrl + C to exit\n");
    fflush(stdout);
}

Process *find_process(LinkedList *head, unsigned short pid) {
    LinkedList *ptr = head;

    while(ptr!=NULL) {
        if(ptr->process->pid == pid) {
            return ptr->process;
        }
        ptr = ptr->next;
    }
    return NULL;
}