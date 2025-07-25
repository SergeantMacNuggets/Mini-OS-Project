#include <stdio.h>
#include <string.h>
#include <time.h>
#include "commands.h"
#include "processes.h"

typedef enum {
    RUN,
    STATUS,
    MEM,
    SCHEDULE,
    EXIT,
    INVALID

}Command;

void delay(int milli_seconds)
{
	clock_t start_time = clock();

	while (clock() < start_time + milli_seconds);
}

Command ret_command(char *str) {
    if(strcmp(str, "run")==0) return RUN;
    else if(strcmp(str, "status")==0) return STATUS;
    else if(strcmp(str, "mem")==0) return MEM;
    else if(strcmp(str, "schedule")==0) return SCHEDULE;
    else if(strcmp(str, "exit")==0) return EXIT;
    else return INVALID;
} 

void loop_start() {
    char input[256];
    printf("C:/> ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    char *string_register = strtok(input, " ");
    if (!string_register) return loop_start();

    Command cmd = ret_command(string_register);

    switch (cmd) {
        case RUN: {
            printf("Running\n");
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
                run(create_process(input_registers[0], stroi(input_registers[1]), stroi(input_registers[2])));
                printf("%s %s %s\n", input_registers[0], input_registers[1], input_registers[2]);
                
            }
            break;
        }
        case STATUS:
            printf("Status\n");
            break;
        case MEM:
            printf("Memory\n");
            break;
        case SCHEDULE:
            printf("Schedule\n");
            break;
        case EXIT:
            printf("Exit\n");
            return;
        default:
            printf("Invalid command.\n");
            break;
    }

    loop_start();
}

int main() {
    loop_start();
    return 0;
}