#include "cardoffice.h"
#include "printer.h"

WATCardOffice::WATCardOffice(Printer & prt, Bank & bank, unsigned int numCouriers):
    prt{prt}, bank{bank}, numCouriers{numCouriers} {
    prt.print(Printer::Kind::WATCardOffice, 'S');

    for (int id = 0; i < numCouriers; id++) {
        couriers[id] = new Courier(prt, bank, *this, id);
    }
}

WATCardOffice::~WATCardOffice() {
    prt.print(Printer::Kind::WATCardOffice, 'F');

    for (int id = 0; i < numCouriers; id++) {
        delete couriers[id];
    }
}

//A student performs an asynchronous call to create to create a “real” WATCard with an initial balance. A future
//WATCard is returned and sufficient funds are subsequently obtained from the bank (see Parent task) via a courier
//to satisfy the create request.
WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
    struct Job * job = new WATCardOffice::Job(id, amount, new WATCard());
    jobQueue.push_back(job);
    jobCond.signal();

    return job->result;
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard * card) {
    struct Job * job = new WATCardOffice::Job(id, amount, card);
    prt.print(Printer::Kind::WATCardOffice, 't', sid, amount);
    jobQueue.push_back(job);
    jobCond.signal();
    prt.print(Printer::Kind::WATCardOffice, 'T', sid, amount);

    return job->result;
}

Job * WATCard::requestWork() {
    if (jobQueue.empty()) jobCond.wait();

    if (jobQueue.empty()) return nullptr;

    Job * jobToDo = jobQueue.front();
    jobQueue.pop();
    return jobToDo;
}

WATCardOffice::Courier::Courier(Printer & prt, Bank & bank, WATCardOffice & cardOffice, unsigned int id):
prt{prt}, bank{bank}, cardOffice{cardOffice}, id{id} {
    prt.printer(Printer::Kind::WATCardOfficeCourier, 'S');
}

WATCardOffice::Courier::~Courier() {
    prt.printer(Printer::Kind::WATCardOfficeCourier, 'F');
}

void WATCardOffice::Courier::main() {
    for (;;) {
        _Accept(~Courier) {
            break;
        }
        _Else {
            Job * jobToDo = cardOffice.requestWork();

            bank.withdraw(jobToDo->id, jobToDo->amount);
            jobToDo->watcard->deposit(jobToDo->id, jobToDo->amount);

            unsigned int randomLost = mprng(5);
            if (randomLost == 3) {
                prt.print(Printer::Kind::WATCardOfficeCourier, jobToDo->id);
                jobToDo->result.exception(new Lost());
                delete jobToDo->watcard;
            }
            else {
                jobToDo->result.delivery(jobToDo->watcard);
            }
            delete jobToDo
        }
    }
}
