#include "nameserver.h"
#include "printer.h"
#include "trainstop.h"

NameServer::NameServer(Printer & prt, unsigned int numStops, unsigned int numStudents):
    prt{prt}, numStops{numStops}, numStudents{numStudents} {
    prt.print(Printer::Kind::NameServer, 'S');
    trainStopList = new TrainStop *[numStops];
}

NameServer::~NameServer() {
    prt.print(Printer::Kind::NameServer, 'F');
    delete [] trainStopList;
}
void NameServer::registerStop(unsigned int trainStopId) {
    trainStopList[trainStopId] = static_cast<TrainStop*>(&uThisTask());
    prt.print(Printer::Kind::NameServer, 'R', trainStopId);
}

TrainStop * NameServer::getStop(unsigned int studentId, unsigned int trainStopId) {
    prt.print(Printer::Kind::NameServer, 'T', studentId, trainStopId);
    return trainStopList[trainStopId];
}

TrainStop ** NameServer::getStopList() {
    prt.print(Printer::Kind::NameServer, 'L');
    return trainStopList;
}

TrainStop ** NameServer::getStopList(unsigned int trainId) {
    prt.print(Printer::Kind::NameServer, 'L', trainId);
    return trainStopList;
}

unsigned int NameServer::getNumStops() {
    return numStops;
}

void NameServer::main() {}

