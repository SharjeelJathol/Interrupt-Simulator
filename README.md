Opertaing Systems Concepts
Interrupt Handling  

Real Hardware	Our Simulation
A button press triggers a hardware interrupt.	A timer or keyboard signal triggers a software interrupt (signal).
The CPU jumps to the ISR (Interrupt Service Routine).	The program jumps to a signal handler function.
The ISR handles the event quickly and returns.	The signal handler prints a message and returns.
Shared variables must be volatile.	We use volatile to prevent compiler optimizations.


gcc -O2 -Wall interrupt_sim.c -o interrupt_sim
./interrupt_sim


Interrupts:
    SIGINT
        ctrl+c in the ame terminal
        It works for killing the process
    SIGUSR1
        kill -USR1 [PID] from another terminal
    SIGALRM
        Set inthe main code in the function setup_timer_interrupt
