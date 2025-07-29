#include <stdio.h>
#include <stdlib.h>

#define NUM_PARTITION 20

int total_memory;

void create_memory() {
    printf("Enter total memory size (in bytes): ");
    scanf("%d", &total_memory);
}

int partitioned_size() {
    return (int) total_memory / NUM_PARTITION;
}