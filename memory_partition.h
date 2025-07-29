#ifndef PARTITION_H
#define PARTITION_H
#include <stdio.h>
#include <stdlib.h>


#define NUM_PARTITION 4

typedef struct Partition {
    Process *process;
    int free_mem;
    int consumed_mem;
    int max_mem;
}Partition;

int total_memory;
Partition *memory_partitions[NUM_PARTITION]={NULL};

Partition *create_partition(int mem_size) {
    Partition *ptr = (Partition*)malloc(sizeof(Partition));
    ptr->max_mem=mem_size;
    ptr->free_mem = mem_size;
    ptr->consumed_mem = 0;
    return ptr;
} 

void create_memory() {
    printf("Enter total memory size (in bytes): ");
    scanf("%d", &total_memory);
    divide_memory();
}

int partitioned_size() {
    return (int) total_memory / NUM_PARTITION;
}

void divide_memory() {
    for(int i=0; i<NUM_PARTITION;i++) {
        memory_partitions[i] = create_partition(partitioned_size());
    }
}

void display_memory() {
    int total_usage = 0;
    int total_free = total_memory;
    printf("\033[2J\033[H");
    for(int i = 0; i < NUM_PARTITION; i++) {
        if(memory_partitions[i]->process != NULL) {
            Process *temp = memory_partitions[i]->process;
            total_usage += temp->mem_alloc;
            total_free -= temp->mem_alloc;
            printf("Mem %d: %s (%d)      \tUsage: %-5d bytes       \tFree: %d bytes     \tMax: %d bytes\n",
                   i, temp->process_name, temp->pid, temp->mem_alloc, (memory_partitions[i]->max_mem - temp->mem_alloc), partitioned_size());
        }
        else {
            printf("Mem %d: %s (%d)      \tUsage: %-5d bytes       \tFree: %d bytes     \tMax: %d bytes\n",
                   i, "NULL", 0, 0, partitioned_size(), partitioned_size());
        }
    }
    printf("\nTotal Usage: %d bytes     \tTotal Free: %d bytes\n", total_usage, total_free);
    printf("Ctrl + C to exit\n");
    fflush(stdout);
}

#endif