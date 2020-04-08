#ifndef PARENT_H
#define PARENT_H

#pragma once
#include "MPRNG.h"

_Monitor Printer;										// forward declarations
_Monitor Bank;

_Task Parent {
    Printer & prt;
    Bank & bank;
    unsigned int numStudents;
    unsigned int parentalDelay;
    unsigned int maxTripCost;
    MPRNG mprng;

	void main();
  public:
	Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay, unsigned int maxTripCost );
	~Parent();
}; // Parent

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
#endif
