#include "trainstop.h"
#include "nameserver.h"
#include "printer.h"
TrainStop::TrainStop(Printer & prt, NameServer & nameServer, unsigned int id, unsigned int stopCost):
    prt{prt}, nameServer{nameServer}, id{id}, stopCost{stopCost} {
    prt.print(Printer::Kind::TrainStop, 'S');
    nameServer.registerStop(id);
}

TrainStop::~TrainStop() {
    prt.print(Printer::Kind::TrainStop, 'F');
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

Train * wait(unsigned int studentId, Train::Direction direction) {
    prt.print(Printer::Kind::TrainStop, 'W', studentId, direction);

    if (trainId == Direction::Clockwise) {
        clockwiseWaitingStudents++;
        clockwiseCond.wait();
    }
    else if (trainId == Direction::CounterClockwise) {
        counterClockwiseWaitingStudents++;
        counterClockwiseCond.wait();
    }

    if (trainId == Direction::Clockwise) clockwiseWaitingStudents--;
    else if (trainId == Direction::CounterClockwise) counterClockwiseWaitingStudents--;

    return arrivedTrain;
}

void TrainStop::tick(){
    prt.print(Printer::Kind::TrainStop, 'T');
    waiting.signal();
}

void TrainStop::disembark(unsigned int studentId) {
    prt.print(Printer::Kind::TrainStop, 'D', studentId);

}

void TrainStop::arrive(unsigned int trainId, Train::Direction direction, unsigned int maxNumStudents) {
    if (trainId == Direction::Clockwise) prt.print(Printer::Kind::TrainStop, 'A',  maxNumStudents, clockwiseWaiting);
    else if (trainId == Direction::CounterClockwise) prt.print(Printer::Kind::TrainStop, 'A',  maxNumStudents, counterClockwiseWaiting);

    trainArrived = true;
    arrivedTrain = static_cast<Train *>(&uThisTask());

    if (trainId == Direction::Clockwise) while (!clockwiseCond.empty()) clockwiseCond.signal(); // Only unblocks the students waiting for clockwise train
    if (trainId == Direction::CounterClockwise) while (!counterClockwiseCond.empty()) counterClockwiseCond.signal(); // Only unblocks the students waiting for counter clockwise train
}

void TrainStop::main() {}


