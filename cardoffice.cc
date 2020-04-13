#include "cardoffice.h"
#include "bank.h"

using namespace std;

WATCardOffice::WATCardOffice(Printer & prt, Bank & bank, unsigned int numCouriers):
    prt{prt}, bank{bank}, numCouriers{numCouriers} {
    prt.print(Printer::Kind::WATCardOffice, 'S');

    couriers = new Courier *[numCouriers];
    for (int id = 0; id < numCouriers; id++) {
        couriers[id] = new Courier(prt, bank, *this, id);
    }
}

WATCardOffice::~WATCardOffice() {
    prt.print(Printer::Kind::WATCardOffice, 'F');

    for (int id = 0; id < numCouriers; id++) {
        delete couriers[id];
    }
    for (unsigned int id = 0; id < numCouriers; id++) {
        jobCond.signalBlock();
    }
    delete [] couriers;
}

//A student performs an asynchronous call to create to create a “real” WATCard with an initial balance. A future
//WATCard is returned and sufficient funds are subsequently obtained from the bank (see Parent task) via a courier
//to satisfy the create request.
WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
    struct Job * job = new WATCardOffice::Job(sid, amount, new WATCard());
    jobQueue.push(job);
    jobCond.signal();
    prt.print(Printer::Kind::WATCardOffice, 'C', sid, amount);

    return job->result;
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard * card) {
    struct Job * job = new WATCardOffice::Job(sid, amount, card);
    jobQueue.push(job);
    jobCond.signal();
    prt.print(Printer::Kind::WATCardOffice, 'T', sid, amount);

    return job->result;
}

WATCardOffice::Job * WATCardOffice::requestWork() {
    if (jobQueue.empty()) jobCond.wait();

    if (jobQueue.empty()) return nullptr;

    WATCardOffice::Job * jobToDo = jobQueue.front();
    jobQueue.pop();
    prt.print(Printer::Kind::WATCardOffice, 'W');
    return jobToDo;
}

void WATCardOffice::main() {
    for (;;) {
        try {
            _Accept(~WATCardOffice) {break;}
            or _Accept(create, transfer, requestWork) {}
        }
        catch (uMutexFailure::RendezvousFailure &) {}
    }
}

void WATCardOffice::Courier::main() {
    for (;;) {
        _Accept(~Courier) {
            break;
        }
        _Else {
            WATCardOffice::Job * jobToDo = cardOffice.requestWork();

            prt.print(Printer::Kind::WATCardOfficeCourier, 't', jobToDo->id, jobToDo->amount);
            bank.withdraw(jobToDo->id, jobToDo->amount);
            jobToDo->watcard->deposit(jobToDo->amount);
            prt.print(Printer::Kind::WATCardOfficeCourier, 'T', jobToDo->id, jobToDo->amount);

            unsigned int randomLost = mprng(5);
            if (randomLost == 3) {
                prt.print(Printer::Kind::WATCardOfficeCourier, 'L', jobToDo->id);
                jobToDo->result.exception(new Lost());
                delete jobToDo->watcard;
            }
            else {
                jobToDo->result.delivery(jobToDo->watcard);
            }
            delete jobToDo;

        }
    }
}
