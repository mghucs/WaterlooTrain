#include "student.h"
#include "global.h"
#include "train.h"
#include "trainstop.h"
#include "nameserver.h"
#include "cardoffice.h"
#include <cstdlib>
#include <iostream>
#include "groupoff.h"

Student::Student(Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
                 unsigned int id, unsigned int numStops, unsigned int stopCost, unsigned int maxStudentDelay, unsigned int maxStudentTrips):
                 prt{prt}, nameServer{nameServer}, cardOffice{cardOffice},
                groupoff{groupoff}, id{id}, numStops{numStops}, stopCost{stopCost},
                maxStudentDelay{maxStudentDelay}, maxStudentTrips{maxStudentTrips} {
}
Student::~Student(){}


void Student::main() {
    unsigned int numTrips = mprng(maxStudentTrips);
    prt.print(Printer::Kind::Student, 'S', numTrips);
    int start = mprng(numStops - 1);
    int end;
    while (start == end) end = mprng(numStops - 1);

    TrainStop * nextTrain;

    WATCard::FWATCard watCard = cardOffice.create(id, numStops / 2);
    WATCard::FWATCard giftCard = groupoff.giftCard();

    for (int i = 0; i < numTrips; i++) {
        yield(maxStudentDelay);
        unsigned int maxStops;
        // Take train 0 if clockwise is faster, train 1 is counter clockwise is faster

        Train::Direction direction;
        unsigned int distance = abs(end - start); // The distance the student will travel
        if (distance < distance % numStops) { // If clockwise path is shorter
            direction = Train::Direction::Clockwise;
            nextTrain = nameServer.getStop(id, direction);
            maxStops = distance;
        } else { // If counter clockwise path is shorter
            direction = Train::Direction::CounterClockwise;
            nextTrain = nameServer.getStop(id, direction);
            maxStops = distance % numStops;
        }
        // Pay for the train
        unsigned int randomPay = mprng(9); // 50% chance of not paying for 1 stop and 30% chance of paying for other
        prt.print(Printer::Kind::Student, 'T', start, end, (unsigned int) direction);
        try {
            _Select( giftCard )
            {
                if (numStops == 1) {
                    if (randomPay >= 5) { // Do pay
                        nextTrain->buy(maxStops, *giftCard());
                        prt.print(Printer::Kind::Student, 'G', maxStops * stopCost, giftCard() -> getBalance());
                        paid = true;
                    }
                    // Else we don't pay
                } else {
                    if (randomPay > 2) { // Do pay
                        nextTrain->buy(maxStops, *giftCard());
                        prt.print(Printer::Kind::Student, 'G', maxStops * stopCost, giftCard() -> getBalance());
                        paid = true;
                    }
                    // Else we don't pay
                }
            }
            _Select(watCard) {
                if (numStops == 1) {
                    if (randomPay >= 5) { // Do pay
                        nextTrain->buy(maxStops, *watCard());
                        prt.print(Printer::Kind::Student, 'B', maxStops * stopCost, giftCard() -> getBalance());
                        paid = true;
                    }
                    else {
                        prt.print(Printer::Kind::Student, 'f');
                    }
                    // Else we don't pay
                }
                else {
                    if (randomPay > 2) { // Do pay
                        nextTrain->buy(maxStops, *watCard());
                        prt.print(Printer::Kind::Student, 'B', maxStops * stopCost, giftCard()->getBalance());
                        paid = true;
                    }
                    // Else we don't pay
                    else {
                        prt.print(Printer::Kind::Student, 'f');
                    }
                }
            }
        }
        catch (TrainStop::Funds &funds) {
            cardOffice.transfer(id, funds.amount, watCard);
        }
        catch (WATCardOffice::Lost &) {
            prt.print(Printer::Kind::Student, 'L');
        }
        Train * arrivedTrain = nextTrain->wait(id, direction);

        try {
            TrainStop * destinationStop = arrivedTrain->embark(id, end, *watCard());
            prt.print(Printer::Kind::Student, 'E', maxStops * stopCost, giftCard()->getBalance());

            destinationStop->disembark(id);
        }
        catch (Train::Ejected &) {
            prt.print(Printer::Kind::Student, 'e');
            return;
        }
        start = end;
        while (start == end) end = mprng(numStops - 1);

        paid = false;
    }
}


