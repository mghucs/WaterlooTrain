#include "train.h"
#include <cmath>

Train::Train(Printer & prt, NameServer & nameServer, unsigned int id, unsigned int maxNumStudents, unsigned int numStops):
    prt{prt}, nameServer{nameServer}, id{id}, maxNumStudents{maxNumStudents}, numStops{numStops} {
        prt.print(Printer::Kind::Train, 'S');

        if (id == 0) {
            direction = Clockwise;
            currentStop = 0;
        }
        else {
            direction = CounterClockwise;
            currentStop = ceil(numStops / 2);
        }
        trainStopList = nameServer.getStopList(id);

        travellingStudents = new uCondition[maxNumStudents];
}
Train::~Train() {
    prt.print(Printer::Kind::Train, 'F');

    delete [] travellingStudents;
}


_Nomutex unsigned int Train::getId() const{
    return id;
}

TrainStop * Train::embark(unsigned int studentId, unsigned int destStop, WATCard& card) {
    prt.print(Printer::Kind::Train, 'E', studentId, destStop);

    currentlyCarrying++;

    while (currentStop != destStop) travellingStudents[studentId].wait();

    else {
        boardingCond.signal()
    }

    return trainStopList[destStop];
}

void Train::scanPassengers() {

}

void Train::wakeUpStudents() {
    for (int id = 0; i < maxNumStudents; i++) travellingStudents[id].signal();
}

void Train::main() {
    if (direction == Clockwise) {
        currentStop = (currentStop + 1) % numStops;
        while (currentStop != 0) { // Go to each stop
            _Accept(embark) {

            }
            trainStopList[currentStop]->arrive(id, Clockwise, maxNumStudents - currentlyCarrying);
            prt.print(Printer::Kind::Train, 'A', maxNumStudents - currentlyCarrying, currentlyCarrying);
            wakeUpStudents();
            boardingCond.wait();
            currentStop = (currentStop + 1) % numStops;
        }

    }
    else if (direction == CounterClockwise) {
        currentStop = (currentStop - 1) % numStops;
        while (currentStop != ceil(numStops / 2)) { // Go to each stop
            _When (currentlyCarrying < maxNumStudents)_Accept(embark) {

            }
            trainStopList[currentStop]->arrive(id, CounterClockwise, maxNumStudents - currentlyCarrying);
            prt.print(Printer::Kind::Train, 'A', maxNumStudents - currentlyCarrying, currentlyCarrying);
            wakeUpStudents();
            boardingCond.wait();
            currentStop = (currentStop - 1) % numStops;
        }
    }
}
