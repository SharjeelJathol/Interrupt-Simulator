#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>

// ==========================================
// Shared data between main loop and ISR
// ==========================================
static volatile bool interrupt_flag = false;
static volatile bool timer_interrupt_flag = false;
static volatile uint32_t interrupt_count = 0;
static volatile uint32_t timer_interrupt_count = 0;

// ==========================================
// Interrupt Service Routine (ISR)
// This function is called when the signal arrives.
// ==========================================
void interrupt_handler(int sig) {
    // Signal handler - this is our "ISR"
    interrupt_flag = true;
    interrupt_count++;
    
    // Print from ISR (unsafe in real ISRs, but safe in simulation)
    printf("\n[ISR] Interrupt %u triggered!\n", interrupt_count);
}

void timer_interrupt_handler(int sig) {
    // Signal handler - this is our "ISR"
    timer_interrupt_flag = true;
    timer_interrupt_count++;
    
    // Print from ISR (unsafe in real ISRs, but safe in simulation)
    printf("\n[TIMER ISR] Interrupt %u triggered!\n", timer_interrupt_count);
}

void setup_timer_interrupt(void) {
    struct itimerval timer;
    
    // Trigger after 2 seconds
    timer.it_value.tv_sec = 2;
    timer.it_value.tv_usec = 0;
    
    // Repeat every 3 seconds
    timer.it_interval.tv_sec = 3;
    timer.it_interval.tv_usec = 0;
    
    setitimer(ITIMER_REAL, &timer, NULL);
}

// ==========================================
// Main program
// ==========================================
int main(void) {
    printf("=== Interrupt Simulator ===\n");
    printf("PID: %d\n", getpid());
    printf("Press Ctrl+C to send SIGINT (simulated interrupt).\n");
    printf("Or send SIGUSR1 from another terminal: kill -USR1 %d\n\n", getpid());
    
    // ==========================================
    // Step 1: Register the interrupt handler
    // ==========================================
    signal(SIGINT, interrupt_handler);   // Ctrl+C triggers this
    signal(SIGUSR1, interrupt_handler);  // Can be sent via kill command
    signal(SIGALRM, timer_interrupt_handler);
    setup_timer_interrupt();

    
    // ==========================================
    // Step 2: Main loop - polls the interrupt flag
    // ==========================================
    while (1) {
        // Check if an interrupt occurred
        if (interrupt_flag) {
            // Clear the flag
            interrupt_flag = false;
            
            // ==========================================
            // Step 3: Process the event (Main loop context)
            // ==========================================
            printf("[MAIN] Processing interrupt %u...\n", interrupt_count);
            
            // Simulate some work being done
            printf("[MAIN] Event processed.\n");
            printf("[MAIN] Waiting for next interrupt...\n\n");
        }

        if (timer_interrupt_flag) {
            // Clear the flag
            timer_interrupt_flag = false;
            
            // ==========================================
            // Step 3: Process the event (Main loop context)
            // ==========================================
            printf("[MAIN] Processing timer interrupt %u...\n", timer_interrupt_count);
            
            // Simulate some work being done
            printf("[MAIN] Event processed.\n");
            printf("[MAIN] Waiting for next interrupt...\n\n");
        }
        
        // Simulate other background work
        usleep(100000); // Sleep for 100ms to reduce CPU usage
    }
    
    return 0;
}