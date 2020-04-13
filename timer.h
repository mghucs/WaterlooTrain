#pragma once

#include "global.h"

_Task Timer {
    Printer & prt;
    NameServer & nameServer;
    unsigned int timerDelay;

    TrainStop ** trainStopList;
    unsigned int numStops;
    unsigned int tickNum;
    void main();
  public:
    Timer( Printer & prt, NameServer & nameServer, unsigned int timerDelay );
    ~Timer();
};

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
