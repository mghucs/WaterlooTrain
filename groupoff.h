#ifndef GROUPOFF_H
#define GROUPOFF_H

#pragma once

#include "watcard.h"
#include "printer.h"
#include "MPRNG.h"
_Task Groupoff {
    Printer & prt;
    unsigned int numStudents;
    unsigned int maxTripCost;
    unsigned int groupoffDelay;
    unsigned int nextDelivery;

    MPRNG mprng;

    WATCard::FWATCard *giftcards; // Giftcards to give to students
    WATCard **watcards;
	void main();
  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int maxTripCost, unsigned int groupoffDelay );
	~Groupoff();
	WATCard::FWATCard giftCard();
}; // Groupoff

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
#endif
