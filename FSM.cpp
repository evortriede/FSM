#include "Arduino.h"
#include "FSM.h"

FSM::FSM(StateTransitionType **tt, char *rgState[], char *rgEvent[])
{
  fsm_tt=tt;
  rgStateTxt=rgState;
  rgEventTxt=rgEvent;
  fsm_state=0;
  fsm_queue.put=0;
  fsm_queue.take=0;
  fsm_queue.count=0;
}

EventType* FSM::dequeue()
{
  EventType *event = &fsm_queue.queue[fsm_queue.take++];
  fsm_queue.take &= 7;
  fsm_queue.count--;
  if (logMethod != 0)
  {
    char msg[64];
    sprintf(msg, "dequeueing %s\n",rgEventTxt[event->event]);
    (*logMethod)(msg);
  }
  return event;
}


void FSM::enqueue(int event, void *data)
{
  if (logMethod != 0)
  {
    char msg[64];
    sprintf(msg, "enqueueing %s\n",rgEventTxt[event]);
    (*logMethod)(msg);
  }
  fsm_queue.queue[fsm_queue.put].event=event;
  fsm_queue.queue[fsm_queue.put++].data=data;
  fsm_queue.put &= 7;
  fsm_queue.count++;
}

void FSM::setTickTimer(long interval, void *data)
{
  if (interval==0)
  {
    tickTimer=0;
  }
  else
  {
    tickTimer = millis() + interval;
    tickData = data;
  }
}

int FSM::processEvents()
{
  if (tickTimer && (tickTimer <= millis()))
  {
    tickTimer=0;
    enqueue(0,tickData);
  }
  while (fsm_queue.count)
  {
    if (logMethod != 0)
    {
      char msg[64];
      sprintf(msg, "in state %s\n",rgStateTxt[fsm_state]);
      (*logMethod)(msg);
    }
    EventType *pEvent=dequeue();
    StateTransitionType *transition = &fsm_tt[fsm_state][pEvent->event];
    fsm_state=transition->nextState;
    if (logMethod != 0)
    {
      char msg[64];
      sprintf(msg, "transitioning to %s\n",rgStateTxt[fsm_state]);
      (*logMethod)(msg);
    }
    if (transition->actionMethod) (*transition->actionMethod)(pEvent->data);
  }
  return fsm_state;
}

void FSM::setLogMethod(void (*method)(const char *))
{
  logMethod=method;
}
