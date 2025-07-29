

void run(Process *ptr_process) {
    new_process_queue(ptr_process);
}

void status() {
    display_status();
}

void schedule() {
    display_schedule();
}

void mem() {
    display_mem();
}

void kill_proc(unsigned short pid) {
    kill_process(pid);
}

void help() {
    printf("run <Process-Name> <Burst-Time-In-Seconds> <Memory-Request>\tCreate a new process.\n\n");
    printf("status                                                  \tCheck the process status and its remaining burst time.\n\n");
    printf("schedule                                                \tLive tracking of processes showing its queue\n\n");
    printf("kill <Process-ID>                                       \tTerminate a process using its id\n\n");
    printf("exit                                                    \tExit the OS simulation\n\n");
}

void exit_cmd() {
    exit(EXIT_SUCCESS);
}