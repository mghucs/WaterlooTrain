#ifndef TRAIN_H
#define TRAIN_H

#pragma once
#include "global.h"
#include "watcard.h"
#include <vector>
#include <tuple>

_Task Train {
  public:
	enum Direction { Clockwise, CounterClockwise };
	_Event Ejected {};					// Exception raised at student without ticket

  private:
    Printer &prt;
    NameServer & nameServer;
    unsigned int id;
    unsigned int maxNumStudents;
    unsigned int numStops;

    unsigned int currentlyCarrying = 0;
    unsigned int currentStop;
    unsigned int numberWaiting;
    TrainStop ** trainStopList;
    bool ejected = false;

    std::vector<std::tuple<unsigned int, bool>> travellingStudentPaid;
    uCondition * travellingStudentsCond;
    uCondition boardingCond;
    uCondition capacityCond;

    Direction direction;

	void main();

	void wakeUpStudents();
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