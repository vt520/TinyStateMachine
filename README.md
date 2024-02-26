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

## Basics
### DEFINE_STATE Macro
Begins a State Definition; should be followed with a scope that contains the state

**`DEFINE_STATE`** `{ ... } WITH_INSTANCE(name);` 
**`class : public : Tiny::State`** `{ ... } WITH_INSTANCE(name);`

### WITH_INSTANCE Macro
Used to close and give a static name to the state.

`DEFINE_STATE { ... }` **`WITH_INSTANCE(name)`** `;`
`DEFINE STATE { ... }` **`static name`** `;`

### STATE_START Macro
Executed when a start is entered.
`DEFINE_STATE {` **`STATE_START`** `{return true;}} WITH_INSTANCE(name);`
`DEFINE_STATE {` **`bool Start(STATE_START_PARAMS)`** `{return true;}} WITH_INSTANCE(name);`

### STATE_CONTINUES Macro

Executed periodically to update the state and report status

`DEFINE_STATE {` **`STATE_CONTINUES`** `{return true;}} WITH_INSTANCE(name);`
`DEFINE_STATE {` **`bool Continues(STATE_CONTINUE_PARAMS)`** `{return true;}} WITH_INSTANCE(name);`

### STATE_EXIT
`void Exit(STATE_EXIT_PARAMS)`
Executed when a state hass finished, but before the next state is entered.

### STATE_INTERRUPT(name)
`public static void name()`
Allows for you to attach the given member to an interrupt
