
void run(Process *ptr_process) {
    new_process_queue(ptr_process);
}

void status() {
    display_status();
}

void schedule() {
    display_schedule();
}

void exit_cmd() {
    exit(EXIT_SUCCESS);
}