#ifndef FSM_h
#define FSM_h

typedef struct
{
  unsigned char nextState;
  void (*actionMethod)(void *data);
} StateTransitionType;

typedef struct
{
  int event;
  void *data;
} EventType;

typedef struct
{
  int count;
  int put;
  int take;
  EventType queue[8];
} QueueType;

class FSM
{
  public:

    FSM(StateTransitionType **tt, char *rgState[], char *rgEvent[]);
    void enqueue(int event, void *data);
    int processEvents();
    void setLogMethod(void (*method)(const char*) );
    void setTickTimer(long interval, void *data);

  private:

    EventType* dequeue();

    QueueType fsm_queue;
    StateTransitionType **fsm_tt;
    int fsm_state=0;
    char **rgStateTxt;
    char **rgEventTxt;
    void (*logMethod)(const char *)=0;
    long tickTimer=0;
    void *tickData;
};
#endif
