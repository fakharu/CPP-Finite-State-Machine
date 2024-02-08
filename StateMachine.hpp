#include <iostream>
#include <cassert>

using namespace std;

class StateMachine
{
    public:

        using Event = int;
        using State = int;

        State currentState;

        /* Deriving Class should extend this class for each transtion data*/
        class EventData { };

        /* Deriving Class should extend this class for each transtion in stateTransition and override execute() */
        class Transition
        {
            public:
                virtual pair<Event, EventData *> execute(Event evt, EventData *evtData) = 0;
        };

        /* Must be redefined by the derivied class */
        virtual pair<State, Transition *> stateTransition(Event evt, State currentState) = 0;
 
        virtual bool continueFsm(Event evt) = 0;

        virtual void fsm(Event evt, EventData *evtData)
        {
            pair<Event, EventData *> next = make_pair(evt, evtData);
            do
            {
                pair<State, Transition *> entry = stateTransition(next.first, currentState);
                currentState = entry.first;
                next = entry.second->execute(next.first, next.second);
            } while(continueFsm(next.first));
        }
};
