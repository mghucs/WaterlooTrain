#include "conductor.h"
#include "train.h"

Conductor::Conductor(Printer & prt, unsigned int id, Train * train, unsigned int delay):
    prt{prt}, id{id}, train{train}, delay{delay} {
    prt.print(Printer::Kind::Conductor, 'S');
}

Conductor::~Conductor() {
    prt.print(Printer::Kind::Conductor, 'F');
}

void Conductor::main() {

    for (;;) {
        _Accept(~Conductor) {
            break;
        }
        _Else{
            yield(delay);
            train->scanPassengers();
        }

    }

}