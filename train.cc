#include "train.h"
#include <cmath>
#include "printer.h"
#include "nameserver.h"
#include "trainstop.h"
Train::Train(Printer & prt, NameServer & nameServer, unsigned int id, unsigned int maxNumStudents, unsigned int numStops):
    prt{prt}, nameServer{nameServer}, id{id}, maxNumStudents{maxNumStudents}, numStops{numStops} {
        if (id == 0) {
            direction = Clockwise;
            currentStop = 0;
        }
        else {
            direction = CounterClockwise;
            currentStop = ceil(numStops / 2);
        }
    travellingStudentsCond = new uCondition[maxNumStudents];
}
Train::~Train() {
    for (unsigned int id = 0; id < maxNumStudents; id++) travellingStudentsCond[id].signalBlock();
    boardingCond.signalBlock();
    capacityCond.signalBlock();

    delete [] travellingStudentsCond;
}

_Nomutex unsigned int Train::getId() const{
    return id;
}

TrainStop * Train::embark(unsigned int studentId, unsigned int destStop, WATCard& card) {
    prt.print(Printer::Kind::Train, id, 'E', studentId, destStop);
    prt.print(Printer::Kind::Student, studentId, 'E', id);

    travellingStudentPaid.push_back(std::make_tuple(studentId, card.paidForTicket())); // Push the pop status and student id as a tuple

    currentlyCarrying++; // if not enough space on train,make students wait for next one
    while (currentlyCarrying > maxNumStudents) capacityCond.wait();

    while (currentStop != destStop) {
        travellingStudentsCond[currentlyCarrying].wait();
        if (ejected) {
            ejected = false;
            throw Ejected();
        }
    }

    currentlyCarrying--;
    return trainStopList[destStop];
}

void Train::scanPassengers() {
    for (std::tuple<unsigned int, bool> studentPayment : travellingStudentPaid) {
        // if student did not pay
        if (!std::get<1>(studentPayment)) {
            currentlyCarrying--; // wake up students, set a boolean and check boolewan in embark and throw
            ejected = true;
            prt.print(Printer::Kind::Conductor, id, 'e', std::get<0>(studentPayment));
            travellingStudentsCond[std::get<0>(studentPayment)].signalBlock();
        }
    }
    travellingStudentPaid.clear();
    // all non-paying students are ejected, only paying students left, so no need to check

}

void Train::wakeUpStudents() {
    for (int i = 0; i < currentlyCarrying; i++) travellingStudentsCond[i].signal();
}

void Train::main() {
    prt.print(Printer::Kind::Train, id, 'S', currentStop, (char) direction);
    trainStopList = nameServer.getStopList(id);
    if (direction == Clockwise) {
        currentStop = (currentStop + 1) % numStops;
        while (currentStop != 0) { // Go to each stop
            numberWaiting = trainStopList[currentStop]->arrive(id, Clockwise, maxNumStudents - currentlyCarrying);
            prt.print(Printer::Kind::Train, id, 'A', currentStop, maxNumStudents - currentlyCarrying, currentlyCarrying);
            wakeUpStudents();
            currentStop = (currentStop + 1) % numStops;
        }

    }
    else if (direction == CounterClockwise) {
        currentStop = (currentStop - 1) % numStops;
        while (currentStop != ceil(numStops / 2)) { // Go to each stop
            numberWaiting = trainStopList[currentStop]->arrive(id, CounterClockwise, maxNumStudents - currentlyCarrying);
            prt.print(Printer::Kind::Train, id, 'A', currentStop, maxNumStudents - currentlyCarrying, currentlyCarrying);
            wakeUpStudents();
            currentStop = (currentStop - 1) % numStops;
        }
    }
    prt.print(Printer::Kind::Train, id, 'F');
}
