#ifndef _vt520_tinystatemachine_h
    #define _vt520_tinystatemachine_h

    //#include <chrono>

    #define STATE_START_PARAMS const Tiny::Context * context, Tiny::State * substitute
    #define STATE_START bool Start(STATE_START_PARAMS) override
    #define STATE_CONTINUES_PARAMS const Tiny::Context * context, Tiny::State * next_state

    #define STATE_CONTINUES bool Continues(STATE_CONTINUES_PARAMS)

    #define STATE_EXIT_PARAMS const Tiny::Context * context
    #define STATE_EXIT void Exit(STATE_EXIT_PARAMS) override 

    #define STATE_INTERRUPT(name) public: static void name() 
    //#define 
    #define DEFINE_STATE class : public Tiny::State
    #define WITH_INSTANCE(name) static name;

    namespace Tiny {
        using millis_t = unsigned long;
        class Context {
            public:
                unsigned priority = 0;
                unsigned count = 0;
                millis_t executionMillis = (millis_t)0;
                void Reset();
                virtual void Continues();
                void * initialState = nullptr;
                void * previousState = nullptr;
            private:
                millis_t startMillis = (millis_t) 0;
        };
        struct State {
            virtual bool Start(STATE_START_PARAMS);
            virtual bool Continues(STATE_CONTINUES_PARAMS);
            virtual void Exit(STATE_EXIT_PARAMS);
        } ;
        
        class Process : public Context {
            public:
                Process();
                Process(State * idle_state);
                void Start(State * state);
                void StartNext(State * state);
                void Continues() override;
                bool Executing();
                bool ExecutingIdle();
                bool Active();
                bool Idle();
                
            private:
                State * currentState = nullptr;
                State * idleState = nullptr;

        } static systemProcess;
    }
#endif