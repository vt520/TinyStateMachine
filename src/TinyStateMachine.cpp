#include "TinyStateMachine.h"
//#include <chrono>

namespace Tiny {
    millis_t millis() {
        //using namespace std::chrono;
        return millis();
    }

    void Context::Reset() {
        count = 0;
        startMillis = millis();
        executionMillis = (millis_t)0;
    }

    void Context::Continue() {
        millis_t current_millis = millis();
        executionMillis = current_millis - startMillis;
    }

    bool State::Start(STATE_START_PARAMS) {
        return false;
    }

    bool State::Continue(STATE_CONTINUE_PARAMS) {
        return false;
    }

    void State::Exit(STATE_EXIT_PARAMS) {
        // nothing
    }

    Process::Process() {
        Process(nullptr);
    }

    Process::Process(State *default_state) {
        idleState = default_state;
        StartNext(idleState);
    }

    void Process::Start(State *state) {
        this->initialState = state;
        StartNext(state);
    }
    void Process::StartNext(State *state) {
        previousState = currentState;
        if (currentState != nullptr) {
            currentState->Exit(this);
        }        
        currentState = state;
        Reset();
        if (nullptr == state) return;
        State * fail_state = idleState;
        if (!state->Start(this, fail_state)) StartNext(fail_state);
    }

    void Process::Continue() {
        if(nullptr == currentState) return;
        State * next_state;
        bool busy = currentState->Continue(this, next_state);
        Context::Continue();
        if (!busy) {
            StartNext(next_state);
        }
    }

    bool Process::Executing() {
        return nullptr != currentState;
    }

    bool Process::ExecutingIdle() {
        return idleState == currentState;
    }

    bool Process::Active() {
        Continue();
        return Executing();
    }

    bool Process::Idle() {
        Continue();
        return ExecutingIdle();
    }
}