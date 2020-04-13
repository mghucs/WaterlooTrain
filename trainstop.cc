#include "trainstop.h"
#include "nameserver.h"
#include "printer.h"
TrainStop::TrainStop(Printer & prt, NameServer & nameServer, unsigned int id, unsigned int stopCost):
    prt{prt}, nameServer{nameServer}, id{id}, stopCost{stopCost}, clockwiseWaitingStudents{0}, counterClockwiseWaitingStudents{0} {
    prt.print(Printer::Kind::TrainStop, 'S');
}

TrainStop::~TrainStop() {
    prt.print(Printer::Kind::TrainStop, 'F');
    while (!clockwiseCond.empty()) clockwiseCond.signal();
    while (!counterClockwiseCond.empty()) counterClockwiseCond.signal();
}

void TrainStop::buy(unsigned int numStops, WATCard & card) {
    unsigned int tripCost = stopCost * numStops;
    if (tripCost > card.getBalance()) {
        unsigned int amountNeeded = tripCost - card.getBalance();
        throw Funds(amountNeeded);
    }
    else {
        card.withdraw(stopCost * numStops);
    }
    card.resetPOP();
    prt.print(Printer::Kind::TrainStop, 'B', tripCost);
}

Train * TrainStop::wait(unsigned int studentId, Train::Direction direction) {
    prt.print(Printer::Kind::Student, 'W', studentId, direction);

    if (direction == Train::Direction::Clockwise) {
        clockwiseWaitingStudents++;
        clockwiseCond.wait();
    }
    else if (direction == Train::Direction::CounterClockwise) {
        counterClockwiseWaitingStudents++;
        counterClockwiseCond.wait();
    }

    if (direction == Train::Direction::Clockwise) clockwiseWaitingStudents--;
    else if (direction == Train::Direction::CounterClockwise) counterClockwiseWaitingStudents--;

    return arrivedTrain;
}

void TrainStop::tick(){
    prt.print(Printer::Kind::TrainStop, 't');
    boardingCond.signal();
}

void TrainStop::disembark(unsigned int studentId) {
    prt.print(Printer::Kind::Student, 'D', studentId);

}

//The train calls arrive, specifying its direction of travel and the maximum number of students it can take from
//this stop after taking into account the number it is currently transporting. Within arrive, the TrainStop unblocks
//the appropriate number of waiting students and these students call Train::embark (see point 5) to get on the train.
unsigned int TrainStop::arrive(unsigned int trainId, Train::Direction direction, unsigned int maxNumStudents) {
    if (trainId == Train::Direction::Clockwise) prt.print(Printer::Kind::TrainStop, 'A',  maxNumStudents, clockwiseWaitingStudents);
    else if (trainId == Train::Direction::CounterClockwise) prt.print(Printer::Kind::TrainStop, 'A',  maxNumStudents, counterClockwiseWaitingStudents);

    trainArrived = true;
    arrivedTrain = static_cast<Train *>(&uThisTask());

    boardingCond.wait();

    if (trainId == Train::Direction::Clockwise) {
        while (!clockwiseCond.empty()) clockwiseCond.signal(); // Only unblocks the students waiting for clockwise train
        return clockwiseWaitingStudents;
    }
    if (trainId == Train::Direction::CounterClockwise) {
        while (!counterClockwiseCond.empty()) counterClockwiseCond.signal(); // Only unblocks the students waiting for counter clockwise train
        return counterClockwiseWaitingStudents;
    }


}

void TrainStop::main() {
    nameServer.registerStop(id);
}


