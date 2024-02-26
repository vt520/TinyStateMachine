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
            Process();
            Process(State * idle_state);
            void Start(State * state);
            void StartNext(State * state);
            void Continues();
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

        struct Machine {
        public:
            using process_id = unsigned;
            static const process_id max_processes = 10;
            Process * processes[max_processes];
            Machine ( Process & initial_process ) {

            }
            process_id GetProcessId(const Process * process) {
                for ( unsigned i; i < max_processes; i++ ) {
                    if ( process == processes[i]  ) return i;
                }
            }
            void SetPriority(Process &processes) {

            }
            private:
            unsigned priorityCounters[max_processes];
            unsigned max_priority = 1;

        };
        //static Machine<> systemMachine = { &systemProcess, 1 };
    }
#endif