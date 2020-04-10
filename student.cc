#include "student.h"
#include "global.h"
#include "train.h"
#include "trainstop.h"
#include "nameserver.h"
#include "cardoffice.h"
#include <cstdlib>
#include <iostream>

Student::Student(Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
                 unsigned int id, unsigned int numStops, unsigned int stopCost, unsigned int maxStudentDelay, unsigned int maxStudentTrips):
                 prt{prt}, nameServer{nameServer}, cardOffice{cardOffice},
                groupoff{groupoff}, id{id}, numStops{numStops}, stopCost{stopCost},
                maxStudentDelay{maxStudentDelay}, maxStudentTrips{maxStudentTrips} {}
Student::~Student(){}


void Student::main() {
    unsigned int numTrips = mprng(maxStudentTrips);
    int start = mprng(numStops - 1);
    unsigned int randomTripStop = mprng(1);
    int end = randomTripStop ? mprng(start - 1) : mprng(start + 1, numStops - 1);

    TrainStop * trainStopTicket;

    WATCard watCard = cardOffice.create(id, numStops / 2);

    try {
        for (int i = 0; i < numTrips; i++) {
            yield(maxStudentDelay);
            std::cout << "Start: " << start << " end: " << end << std::endl;
            unsigned int maxStops;
            // Take train 0 if clockwise is faster, train 1 is counter clockwise is faster

            Train::Direction direction;
            unsigned int distance = abs(end - start); // The distance the student will travel
            if (distance < distance % numStops) { // If clockwise path is shorter
                direction = Train::Direction::Clockwise;
                nextTrain = nameServer.getStop(id, direction);
                maxStops = distance;
            } else {
                direction = Train::Direction::CounterClockwise;
                nextTrain = nameServer.getStop(id, direction); // If counter clockwise path is shorter
                maxStops = distance % numStops;
            }
            unsigned int amountDue = stopCost * maxStops;
            // Pay for the train
            unsigned int randomPay = mprng(9); // 50% chance of not paying for 1 stop and 30% chance of paying for other
            if (numStops == 1) {
                if (randomPay >= 5) { // Do pay
                    trainStopTicket->buy(maxStops, watCard);
                }
                // Else we don't pay
            } else {
                if (randomPay > 2) { // Do pay
                    trainStopTicket->buy(maxStops, watCard);
                }
                // Else we don't pay
            }
            Train * arrivedTrain = trainStopTicket->wait(id, direction);

//            TrainStop * destinationStop = arrivedTrain->embark(id, end, watCard);
//
//            destinationStop.disembark(id);

            start = end;
            randomTripStop = mprng(1);
            end = randomTripStop ? mprng(start - 1) : mprng(start + 1, numStops - 1);
        }
    }
    catch (TrainStop::Funds & funds){
        // Print the funds needed?
        // Get the funds from bank
    }


}
