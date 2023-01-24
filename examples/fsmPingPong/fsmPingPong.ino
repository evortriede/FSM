#include <FSM.h>

void tick(void *data);
void tock(void *data);
void ping(void *data);
void pong(void *data);

void logMethod(const char *msg)
{
  Serial.print(msg);
}

typedef enum
{
   tick_ev
  ,tock_ev
  ,ping_ev
  ,pong_ev
} events;

char *rgEventTxt[]=
{
   "tick_ev"
  ,"tock_ev"
  ,"ping_ev"
  ,"pong_ev"
};

typedef enum
{
   tick_st
  ,tock_st
  ,ping_st
  ,pong_st
}states;

char *rgStateTxt[]
{
   "tick_st"
  ,"tock_st"
  ,"ping_st"
  ,"pong_st"
};

#define TT StateTransitionType
//              tick             tock             ping             pong                   
TT tick_std[]={{tock_st,&tick}, {tick_st,&tock}, {tick_st,&ping}, {tick_st,&pong}};
TT tock_std[]={{ping_st,&tick}, {tock_st,&tick}, {tock_st,&ping}, {tock_st,&pong}};
TT ping_std[]={{pong_st,&tick}, {ping_st,&tock}, {ping_st,&tick}, {ping_st,&pong}};
TT pong_std[]={{tick_st,0},     {pong_st,&tock}, {pong_st,&ping}, {pong_st,&tick}};

StateTransitionType *transTable[]=
{
   tick_std
  ,tock_std
  ,ping_std
  ,pong_std
};

FSM fsm(transTable, rgStateTxt, rgEventTxt);

void tick(void *data)
{
  Serial.println("tick");
  fsm.setTickTimer(1000, 0);
}

void tock(void *data)
{
  Serial.println("tock");
}

void ping(void *data)
{
  Serial.println("ping");
}

void pong(void *data)
{
  Serial.println("pong");
}

void setup()
{
  Serial.begin(115200);
  fsm.setLogMethod(&logMethod);
  fsm.setTickTimer(1000,0);
}

char buf[100];

void loop()
{
  if (Serial.available())
  {
    buf[0]=0;
    delay(100);
    for (int i=0;Serial.available();i++)
    {
      buf[i]=Serial.read();
      buf[i+1]=0;
    }
    if (strcmp(buf,"tock")==0)
    {
      fsm.enqueue(tock_ev,0);
    }
    else if (strcmp(buf,"ping")==0)
    {
      fsm.enqueue(ping_ev,0);
    }
    else if (strcmp(buf,"pong")==0)
    {
      fsm.enqueue(pong_ev,0);
    }
    else if (strcmp(buf,"tick")==0)
    {
      fsm.enqueue(tick_ev,0);
    }
  }
  fsm.processEvents();
}