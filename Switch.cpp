#include <iostream>
#include <cassert>
#include "StateMachine.hpp"

using namespace std;

class Switch: public StateMachine
{
    private:

        enum /* Event */ {
            EVENT_NONE = 0,
            EVENT_SWITCH_ON,
            EVENT_SWITCH_OFF,
            EVENT_SWITCH_TOGGLE,
            EVENT_SWITCH_MAX
        };

        enum /* State */ {
            STATE_OFF,
            STATE_ON,
            STATE_MAX
        };

        class InvalidTranstion : public Transition
        {
            public:
                pair<Event, EventData *> execute(Event evt, EventData *evtData) override
                {
                    assert(0 && "Invalid Transition");
                    return make_pair(EVENT_NONE, nullptr); 
                }
        };

        class NoopTranstion : public Transition
        {
            public:
                pair<Event, EventData *> execute(Event evt, EventData *evtData) override
                {
                    cout << "  No Operation" << endl; 
                    return make_pair(EVENT_NONE, nullptr); 
                }
        };

        class SwitchBrigthnessIntensity: public EventData
        {
            public:
                int intensity;

                SwitchBrigthnessIntensity(unsigned int value)
                {
                    intensity = value%100;
                }
        };

        class SwitchTurnOnTranstion : public Transition
        {
            public:
                pair<Event, EventData *> execute(Event evt, EventData *evtData) override
                {
                    cout << "  Switch turned on ";
                    if(evtData)
                    {
                        cout << "with brightness " << ((SwitchBrigthnessIntensity *)evtData)->intensity;
                    }
                    cout << endl; 
                    return make_pair(EVENT_NONE, nullptr);
                }
        };

        class SwitchTurnOffTranstion : public Transition
        {
            public:
                pair<Event, EventData *> execute(Event evt, EventData *evtData) override
                {
                    cout << "  Switch turned off " << endl; 
                    return make_pair(EVENT_NONE, nullptr);
                }
        };

        class InvalidTranstion       invalidTransition;
        class NoopTranstion          noopTransition;
        class SwitchTurnOnTranstion  switchTurnOnTranstion;
        class SwitchTurnOffTranstion switchTurnOffTranstion;

        bool continueFsm(Event evt) override
        {
            return evt != EVENT_NONE;
        }

        pair<State, Transition *> stateTransition(Event evt, State currentState) override
        {
            static pair<State, Transition *> transitionTbl[EVENT_SWITCH_MAX][STATE_MAX] =
            {      
                /* EVENT_NONE */
                {
                    /* STATE_OFF  */ make_pair(STATE_OFF,  &noopTransition),
                    /* STATE_ON   */ make_pair(STATE_ON,   &noopTransition),
                },
                /* EVENT_SWITCH_ON */
                {
                    /* STATE_OFF  */ make_pair(STATE_ON,   &switchTurnOnTranstion),
                    /* STATE_ON   */ make_pair(STATE_ON,   &noopTransition),
                },
                /* EVENT_SWITCH_OFF */
                {
                    /* STATE_OFF  */ make_pair(STATE_OFF,  &noopTransition),
                    /* STATE_ON   */ make_pair(STATE_OFF,  &switchTurnOffTranstion),
                },
                /* EVENT_SWITCH_TOGGLE */
                {
                    /* STATE_OFF  */ make_pair(STATE_ON,   &switchTurnOffTranstion),
                    /* STATE_ON   */ make_pair(STATE_OFF,  &switchTurnOnTranstion),
                },
            };
            return transitionTbl[evt][currentState];
        }

    public:

        Switch()
        {
            currentState = STATE_OFF;
        }

         void turnOn()
        {
            cout << " Turning the switch on" << endl;
            fsm(EVENT_SWITCH_ON, new SwitchBrigthnessIntensity(50));
        }

        void turnOff()
        {
            cout << " Turning the switch off" << endl;
            fsm(EVENT_SWITCH_OFF, nullptr);
        }

        void toggle()
        {
            cout << " Toggling the switch" << endl;
            fsm(EVENT_SWITCH_TOGGLE, nullptr);
        }
};

int main()
{
    Switch s;

    s.turnOn();
    s.turnOn();
    s.turnOff();
    s.toggle();

    return 0;
}

