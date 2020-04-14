#include "timer.h"
#include "printer.h"
#include "nameserver.h"
#include "trainstop.h"

Timer::Timer(Printer & prt, NameServer & nameServer, unsigned int timerDelay):
    prt{prt}, nameServer{nameServer}, timerDelay{timerDelay}, tickNum{0} {
}

Timer::~Timer() {
}

void Timer::main() {
    prt.print(Printer::Kind::Timer, 'S');
    trainStopList = nameServer.getStopList();
    numStops = nameServer.getNumStops();
    unsigned int id = 0;
    for (;;) {
        id = (id + 1) % numStops;
        _Accept(~Timer)
        {
            break;
        }
        _Else{
                yield(timerDelay);
                trainStopList[id]->tick();
                prt.print(Printer::Kind::Timer, 't', tickNum++);
                break;
        }
    }
    prt.print(Printer::Kind::Timer, 'F');
}
