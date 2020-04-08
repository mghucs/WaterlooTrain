#ifndef NAMESERVER_H
#define NAMESERVER_H

#pragma once

_Monitor Printer;
_Task TrainStop;

_Task NameServer {
    Printer & prt;
    unsigned int numStops;
    unsigned int numStudents;
	TrainStop ** trainStopList;

	void main();
  public:
	NameServer( Printer & prt, unsigned int numStops, unsigned int numStudents );
	~NameServer();
	void registerStop( unsigned int trainStopId );
	TrainStop * getStop( unsigned int studentId, unsigned int trainStopId );
	TrainStop ** getStopList();  // called by Timer
	TrainStop ** getStopList( unsigned int trainId );	// called by Train
	unsigned int getNumStops();
};

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
#endif