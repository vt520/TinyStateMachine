#include <Arduino.h>
#include <TinyStateMachine.h>

DEFINE_STATE {
    STATE_CONTINUES {
        if (context->executionMillis > 1000) {
            next_state = (State*) context->initialState;
            return false;
        }
        return true;
    }
} WITH_INSTANCE(delayState);

DEFINE_STATE {
    static inline unsigned pin;
    static inline bool pressed;
    STATE_START {
        pressed = false;
        unsigned interrupt = digitalPinToInterrupt(pin);
        if (NOT_AN_INTERRUPT == interrupt) return false;
        attachInterrupt(interrupt, test, CHANGE);
        return true;
    }
    STATE_CONTINUES {
        if (pressed) {
            next_state = &delayState;
            return false;
        }
        return true;
    }
    STATE_EXIT {
        unsigned interrupt = digitalPinToInterrupt(pin);
        if (NOT_AN_INTERRUPT == interrupt) return;
        detachInterrupt(interrupt);
    }
    STATE_INTERRUPT(test) {
        pressed = true;
        // The Input was pressed during while the state is active
    }
} WITH_INSTANCE (usingPin);



void setup() {
    using namespace Tiny;
    systemProcess = Process(&usingPin);
}

void loop() {
    Tiny::systemProcess.Continues();
}
