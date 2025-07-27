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
    while(ptr!=NULL) {
        Process *proc = ptr->process;
        printf("\rProcess: %s\tStatus: %s\n", proc->process_name,status_name(proc));
        fflush(stdout);
        ptr=ptr->next;
    }
    printf("\n");
}