#include "timer.h"

Timer::Timer(Printer & prt, NameServer & nameServer, unsigned int timerDelay):
    prt{prt}, nameServer{nameServer}, timerDelay{timerDelay} {
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
            for (int i = 0; i < nameServer.getNumStops(); i++) {
                yield(timerDelay);
                trainStopList[id]->tick();
            }
        }
    }
}