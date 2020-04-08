#ifndef TRAIN_H
#define TRAIN_H

#pragma once
#include "global.h"
#include "watcard.h"

_Task Train {
  public:
	enum Direction { Clockwise, CounterClockwise };
	_Event Ejected {};						// Exception raised at student without ticket

  private:
    Printer &prt;
    NameServer & nameServer;
    unsigned int id;
    unsigned int maxNumStudents;
    unsigned int numStops;
    unsigned int currentlyCarrying = 0;

	void main();
  public:
	Train( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int maxNumStudents, unsigned int numStops );
	~Train();
	_Nomutex unsigned int getId() const;
	TrainStop * embark( unsigned int studentId, unsigned int destStop, WATCard& card ); // Student waits until train reaches destination stop.
	void scanPassengers(); // called by conductor
};

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
#endif