#ifndef QUEUE_H
#define QUEUE_H
#include "processes.h"


typedef struct Node_Queue {
    Process *process;
    struct Node_Queue *next;
}Node_Queue;


typedef struct Queue {
    Node_Queue *first;
    Node_Queue *last;
}Queue;


Node_Queue *create_node_queue(Process *ptr_process) {
    Node_Queue *n_queue = (Node_Queue*) malloc(sizeof(Node_Queue));
    n_queue->process = ptr_process;
    n_queue->next = NULL;
    return n_queue;
}


Queue *createQueue() {
    Queue *new_queue = (Queue*)malloc(sizeof(Queue));
    if(!new_queue) {
        perror("Failed to Allocate Queue");
        exit(EXIT_FAILURE);
    }
    new_queue->last = NULL;
    new_queue->first = new_queue->last;
    return new_queue;
}

void enqueue(Queue *queue, Process *process) {
    Node_Queue *new_node_queue = create_node_queue(process);
    if(queue->last == NULL) {
        queue->first = queue->last = new_node_queue;
        return;
    }
    queue->last->next = new_node_queue;
    queue->last = new_node_queue;
}

void dequeue(Queue *queue) {

    if(queue->first==NULL && queue->last==NULL) {
        return;
    }

    Node_Queue *temp = queue->first;
    queue->first = queue->first->next;

    if(queue->first==NULL) {
        queue->last = NULL;
    }

    free(temp);

}


Process *get_current_process(Queue *queue) {
    return (queue->first == NULL && queue->last == NULL) ? NULL : queue->first->process;
}

void displayAllQ(Queue *q) {
     if (q == NULL || q->first == NULL) {
        printf("Queue is empty.\n");
        return;
    }

    Node_Queue *ptr = q->first;
    while (ptr != NULL) {
        if (ptr->process != NULL) {
            printf("[%s:%ds] -> ", ptr->process->process_name, ptr->process->burst_time);
        } else {
            printf("[NULL process] -> ");
        }
        ptr = ptr->next;
    }
    printf("NULL\n");
}

#endif
