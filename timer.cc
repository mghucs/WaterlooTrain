#include "timer.h"
#include "printer.h"
#include "nameserver.h"
#include "trainstop.h"

Timer::Timer(Printer & prt, NameServer & nameServer, unsigned int timerDelay):
    prt{prt}, nameServer{nameServer}, timerDelay{timerDelay}, tickNum{0} {
    prt.print(Printer::Kind::Timer, 'S');
    trainStopList = nameServer.getStopList();
    numStops = nameServer.getNumStops();
}

Timer::~Timer() {
    prt.print(Printer::Kind::Timer, 'F');
}

void Timer::main() {
    for (;;) {
        _Accept(~Timer) {
            break;
        }
        _Else {
            for (int id = 0; id < nameServer.getNumStops(); id++) {
                yield(timerDelay);
                trainStopList[id]->tick();
                prt.print(Printer::Kind::Timer, 'T', tickNum++);
            }
        }
    }
}