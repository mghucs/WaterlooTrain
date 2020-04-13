#ifndef STUDENT_H
#define STUDENT_H
#pragma once

#include "MPRNG.h"
_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;
_Task Groupoff;

_Task Student {
    Printer &prt;
    NameServer &nameServer;
    WATCardOffice &cardOffice;
    Groupoff &groupoff;
    unsigned int id;
    unsigned int numStops;
    unsigned int stopCost;
    unsigned int maxStudentDelay;
    unsigned int maxStudentTrips;
    MPRNG mprng;
    bool paid = false;

    void main();
  public:
        Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
        unsigned int id, unsigned int numStops, unsigned int stopCost, unsigned int maxStudentDelay, unsigned int maxStudentTrips );
        ~Student();
};

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
#endif