# Tiny State Machine
An Arduino library for easily creating event driven state machines.

## Quickstart
```cpp
#include <TinyStateMachine.h>

// Create a _very_ simple state
DEFINE_STATE {
    STATE_CONTINUES {
        // Periodic work goes here
    }
} WITH_INSTANCE(basic_state);

// defining a more complex state
DEFINE_STATE {
    static inline bool pressed = false;
    STATE_START {
        // Configure interrupts
    }
    STATE_CONTINUES {
        // return false to go to the exit this state
        next_state = &simple_state;
        return !pressed;
    }
    STATE_EXIT {
        // Reset interrupts
    }
    STATE_INTERRUPT(interrupt_name) {
        // The Input was pressed during while the state is active
        pressed = true;
    }
} WITH_INSTANCE (defined_state);



void setup() {
    using namespace Tiny;
    // Creating a process with an idle state
    systemProcess = Process(&usingPin);
}

void loop() {
    // Update the Process state machine, returns false if a process is stopped
    Tiny::systemProcess.Continues();
}

```