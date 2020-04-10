#include "nameserver.h"
#include "printer.h"
#include "trainstop.h"

NameServer::NameServer(Printer & prt, unsigned int numStops, unsigned int numStudents):
    prt{prt}, numStops{numStops}, numStudents{numStudents} {
    trainStopList = new TrainStop *[numStops];
}

NameServer::~NameServer() {
    delete [] trainStopList;
}
void NameServer::registerStop(unsigned int trainStopId) {
    trainStopList[trainStopId] = static_cast<TrainStop*>(&uThisTask());
}

TrainStop * NameServer::getStop(unsigned int studentId, unsigned int trainStopId) {
    return trainStopList[trainStopId];
}

TrainStop ** NameServer::getStopList() {
    return trainStopList;
}

TrainStop ** NameServer::getStopList(unsigned int trainId) {
    return trainStopList;
}

unsigned int NameServer::getNumStops() {
    return numStops;
}

void NameServer::main() {}

