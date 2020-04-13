#pragma once

#include <list>
#include <queue>
#include "watcard.h"
#include "printer.h"
#include "MPRNG.h"

_Monitor Printer;										// forward declarations
_Monitor Bank;

_Task WATCardOffice {
        struct Job { // marshalled arguments and return future
            unsigned int id;
            unsigned int amount;
            WATCard * watcard;

            WATCard::FWATCard result; // return future
            Job(unsigned int id, unsigned int amount, WATCard * watcard):
            id{id}, amount{amount}, watcard{watcard} {}

        };
        _Task Courier {
            Printer & prt;
            Bank & bank;
            WATCardOffice & cardOffice;
            unsigned int id;
            MPRNG mprng;

            void main();

            public:
            Courier(Printer & prt, Bank & bank, WATCardOffice & cardOffice, unsigned int id):
            prt{prt}, bank{bank}, cardOffice{cardOffice}, id{id} {
                prt.print(Printer::Kind::WATCardOfficeCourier, 'S');
            }
            ~Courier() {
                prt.print(Printer::Kind::WATCardOfficeCourier, 'F');
            }
        };
        Printer & prt;
        Bank & bank;
        unsigned int numCouriers;
        Courier ** couriers;
        std::queue <Job *> jobQueue;
        uCondition jobCond;
        MPRNG mprng;

        void main();
        public:
        _Event Lost {};										// lost WATCard
        WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
        ~WATCardOffice();
        WATCard::FWATCard create( unsigned int sid, unsigned int amount ); // called by student
        WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ); // called by student
        Job * requestWork();								// called by courier to request/return work
}; // WATCardOffice

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
