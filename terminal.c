#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "processes.h"
#include "commands.h"
#include "queue.h"
#include "schedule.h"
#include "memory_partition.h"


typedef enum {
    RUN,
    KILL,
    STATUS,
    MEM,
    SCHEDULE,
    EXIT,
    INVALID

}Command;

void *loop_start();

Command ret_command(char *str) {
    if(strcmp(str, "run")==0) return RUN;
    else if(strcmp(str, "kill")==0) return KILL;
    else if(strcmp(str, "status")==0) return STATUS;
    else if(strcmp(str, "mem")==0) return MEM;
    else if(strcmp(str, "schedule")==0) return SCHEDULE;
    else if(strcmp(str, "exit")==0) return EXIT;
    else return INVALID;
} 

void os_start() {
    create_memory();
    pthread_t dispatcher_thread;
    pthread_t command_thread;
    pthread_create(&dispatcher_thread, NULL, dispatcher,NULL);
    pthread_create(&command_thread, NULL, loop_start, NULL);
    pthread_join(dispatcher_thread, NULL);
    pthread_join(command_thread, NULL);
}

void *loop_start() {
    char input[256];
    printf("\nCMD:/> ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    char *string_register = strtok(input, " ");
    if (!string_register) return loop_start();

    Command cmd = ret_command(string_register);

    switch (cmd) {
        case RUN: {
            int flag = 0;
            char *input_registers[3] = {NULL};
            for (int i = 0; i < 3; i++) {

                string_register = strtok(NULL, " ");

                if (string_register) {

                    input_registers[i] = string_register;

                }
                else {

                    printf("run <Process-Name> <Burst-Time-In-Seconds> <Memory-Request>\n");

                    flag = 1;

                    break;
                }
            }
            
            if(!flag) {

                unsigned int burst_time = (unsigned int) atoi(input_registers[1]);

                unsigned int memory_alloc =(unsigned int) atoi(input_registers[2]);

                Process *p = create_process(input_registers[0],NEW,burst_time, memory_alloc);

                new_process_queue(p);
                
            }
            break;
        }
        case KILL:{
            int flag = 0;
            char *input_registers[1] = {NULL};
            for (int i = 0; i < 1; i++) {

                string_register = strtok(NULL, " ");

                if (string_register) {

                    input_registers[i] = string_register;

                }
                else {

                    printf("kill <Process-ID>\n");

                    flag = 1;

                    break;
                }
            }
            
            if(!flag) {

                unsigned short pid = (unsigned int) atoi(input_registers[0]);

                kill_proc(pid);
                
            }
            break;}
        case STATUS:
            status();
            break;
        case MEM:
            printf("Memory\n");
            break;
        case SCHEDULE:
            printf("Schedule\n");
            schedule();
            break;
        case EXIT:
            printf("Exit\n");
            exit_cmd();
        default:
            printf("Invalid command.\n");
            break;
    }

    loop_start();
}

int main() {
    os_start();
    return 0;
}
