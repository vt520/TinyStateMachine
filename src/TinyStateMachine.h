#ifndef _vt520_tinystatemachine_h
    #define _vt520_tinystatemachine_h

//    #include <TinyBuffer.h>

    //#include <chrono>

    #define STATE_CONTEXT_PARAM const Tiny::Context * context
    #define STATE_SUBSTITUTE_PARAM Tiny::State * substitute
    #define STATE_NEXT_PARAM Tiny::State * next_state
    #define STATE_START_PARAMS STATE_CONTEXT_PARAM, STATE_SUBSTITUTE_PARAM

    #define STATE_START bool Start(STATE_START_PARAMS) override
    #define STATE_CONTINUES_PARAMS STATE_CONTEXT_PARAM, STATE_NEXT_PARAM

    #define STATE_CONTINUES bool Continues(STATE_CONTINUES_PARAMS)

    #define STATE_EXIT_PARAMS STATE_CONTEXT_PARAM
    #define STATE_EXIT void Exit(STATE_EXIT_PARAMS) override 

    #define STATE_INTERRUPT(name) public: static void name() 
    //#define 
    #define DEFINE_STATE class  : public Tiny::State
    #define WITH_INSTANCE(name) name

    namespace Tiny {
        class State;
        class Process;
        class Machine;

        using millis_t = unsigned long;
        
        class Context {
        public:
            unsigned priority = 0;
            unsigned count = 0;
            millis_t executionMillis = (millis_t)0;
            Process * process;
            void Reset();
            virtual void Update();
        private:
            millis_t startMillis = (millis_t) 0;
        };

        struct State {
            virtual bool Start(STATE_START_PARAMS);
            virtual bool Continues(STATE_CONTINUES_PARAMS);
            virtual void Exit(STATE_EXIT_PARAMS);
        } ;

        class Process {
        public:
            using priority_t = unsigned;
            static const priority_t disabled = 0;
            static const priority_t defaultPriority = 1;
            priority_t priority = disabled;

            Process();
            Process(State * idle_state);
            void Start(State * state);
            void StartNext(State * state);
            void Execute();
            bool Executing();
            bool ExecutingIdle();
            bool Active();
            bool Idle();
            State * GetPreviousState();
            State * GetIdleState();
            State * GetCurrentState();
            State * GetInitialState();
        protected:
            Context context;
        private:
            State * initialState = nullptr;
            State * previousState = nullptr;
            State * currentState = nullptr;
            State * idleState = nullptr;
        } static systemProcess;

        #define PROCESSES_PER_STATE 10
        class Machine {
        public:
            using process_id = unsigned;
            static const process_id max_processes = PROCESSES_PER_STATE;
            Process * processes[max_processes];
            Machine(Process * process) {
                Start(process);
            }
            Machine(Process * process[], unsigned count) {
                for (process_id index = 0; index < count; index ++) {
                    Start(process[index]);
                }
            }
            unsigned GetHighestPriority() {
                unsigned result = 1;
                for ( process_id process_id; process_id < max_processes; process_id++) {
                    if ( processes[process_id]->Executing() ) {
                        if ( processes[process_id]->priority > result ) {
                            result = processes[process_id]->priority;
                        }
                    }
                }
                return result;
            }
            void Execute () {
                unsigned execute_threshold = GetHighestPriority();
                for ( process_id process_id; process_id < max_processes; process_id++ ) {
                    processAccumulator[process_id] += processes[process_id]->priority;
                    if(processAccumulator[process_id] > execute_threshold) {
                        processAccumulator[process_id] -= execute_threshold;
                        processes[process_id]->Execute();
                    }
                }
            }
            
            bool Start (Process * process) {
                for (process_id pid = 0; pid < max_processes; pid++) {
                    if (!processes[pid]->Executing()) {
                        processes[pid] = process;
                        return true;
                    }
                }
                return false;
            }
            private:
            unsigned processAccumulator[max_processes];
        };
        //static Machine<> systemMachine = { &systemProcess, 1 };
    }
#endif