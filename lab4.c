#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Process struct
typedef struct {
    unsigned int pid;
    unsigned int arrive; // arrival time
    unsigned int burst;
    unsigned int remaining;  
    unsigned int w;      // waiting time
    unsigned int ta;     // turnaround
    bool active;
    unsigned int finish; // finish time
} Process_T;

// Queue struct
typedef struct Queue {
    struct Queue *next;
    Process_T process;
} Queue_T;

// Globally managed Queue
Queue_T *head = NULL;

Process_T dequeue() {
    if (head != NULL) {
        Process_T proc = head->process;
        Queue_T *tmp = head;
        head = head->next;
        free(tmp);
        return proc;
    }
    Process_T empty = {0};
    return empty;
}

void enqueue(Process_T p) {
    Queue_T *newNode = malloc(sizeof(Queue_T));
    newNode->process = p;
    newNode->next = NULL;
    if (head == NULL) {
        head = newNode;
    } else {
        Queue_T *current = head;
        while (current->next != NULL) current = current->next;
        current->next = newNode;
    }
}

// SJF (SRTF) sort by remaining burst time
void enqueue_sorted(Process_T p) {
    Queue_T *newNode = malloc(sizeof(Queue_T));
    newNode->process = p;
    newNode->next = NULL;

    if (head == NULL || head->process.remaining > p.remaining) {
        newNode->next = head;
        head = newNode;
    } else {
        Queue_T *current = head;
        while (current->next != NULL && current->next->process.remaining <= p.remaining) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

void print_process(Process_T p) {
    printf("[PID: %2d] Arrival: %2d | Burst: %2d | Wait: %2d | TA: %2d\n", 
           p.pid, p.arrive, p.burst, p.w, p.ta);
}

int main(void) {
    int n, choice, quantum = 0;
    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1) return 1;

    Process_T ps[n];
    for (int i = 0; i < n; i++) {
        printf("Process %d (PID Arrival Burst): ", i + 1);
        scanf("%u %u %u", &ps[i].pid, &ps[i].arrive, &ps[i].burst);
        ps[i].remaining = ps[i].burst;
        ps[i].active = false;
        ps[i].w = 0;
        ps[i].ta = 0;
    }

    printf("\nChoose Algorithm:\n1. Round Robin (RR)\n2. Shortest Job First (SJF - Preemptive)\nChoice: ");
    scanf("%d", &choice);
    if (choice == 1) {
        printf("Enter Time Quantum: ");
        scanf("%d", &quantum);
    }

    int time = 0, done = 0;
    Process_T running;
    bool has_running = false;
    int q_tick = 0;

    printf("\nStarting simulation...\nExecution Order: ");

    while (done < n) {
        // Enqueue arrivals
        for (int i = 0; i < n; i++) {
            if (ps[i].arrive == time && !ps[i].active) {
                if (choice == 1) enqueue(ps[i]);
                else enqueue_sorted(ps[i]);
                ps[i].active = true;
            }
        }

        if (choice == 1) { // Round Robin
            if (!has_running && head != NULL) {
                running = dequeue();
                has_running = true;
                q_tick = 0;
            }

            if (has_running) {
                printf("P%d ", running.pid);
                running.remaining--;
                time++;
                q_tick++;

                if (running.remaining == 0) {
                    for (int i = 0; i < n; i++) {
                        if (ps[i].pid == running.pid) {
                            ps[i].finish = time;
                            done++;
                            break;
                        }
                    }
                    has_running = false;
                } else if (q_tick == quantum) {
                    // Check arrivals at new time before re-enqueuing
                    for (int i = 0; i < n; i++) {
                        if (ps[i].arrive == time && !ps[i].active) {
                            enqueue(ps[i]);
                            ps[i].active = true;
                        }
                    }
                    enqueue(running);
                    has_running = false;
                }
            } else {
                time++;
            }
        } else { // SJF (Preemptive / SRTF)
            if (has_running && head != NULL && head->process.remaining < running.remaining) {
                enqueue_sorted(running);
                has_running = false;
            }

            if (!has_running && head != NULL) {
                running = dequeue();
                has_running = true;
            }

            if (has_running) {
                printf("P%d ", running.pid);
                running.remaining--;
                time++;

                if (running.remaining == 0) {
                    for (int i = 0; i < n; i++) {
                        if (ps[i].pid == running.pid) {
                            ps[i].finish = time;
                            done++;
                            break;
                        }
                    }
                    has_running = false;
                }
            } else {
                time++;
            }
        }
    }

    printf("\n\nSimulation Results:\n");
    float total_wait = 0, total_ta = 0;
    for (int i = 0; i < n; i++) {
        ps[i].ta = ps[i].finish - ps[i].arrive;
        ps[i].w = ps[i].ta - ps[i].burst;
        total_wait += ps[i].w;
        total_ta += ps[i].ta;
        print_process(ps[i]);
    }
    printf("\nAverage Waiting Time: %.2f\n", total_wait / n);
    printf("Average Turnaround Time: %.2f\n", total_ta / n);

    while (head != NULL) dequeue();

    return 0;
}
