#include "bank.h"
#include "cardoffice.h"
#include "conductor.h"
#include "config.h"
#include "global.h"
#include "groupoff.h"
#include "MPRNG.h"
#include "nameserver.h"
#include "parent.h"
#include "printer.h"
#include "student.h"
#include "timer.h"
#include "trainstop.h"
#include "watcard.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char * argv[]) {
    MPRNG mprng;
    string configFile = "lrt.config";
//    char configFile[] = {'l', 'r', 't', '.', 'c', 'o', 'n', 'f', 'i', 'g'};
    unsigned int seed = getpid();
    try {
        switch (argc) {
            case 3: // get seed
                if (strcmp(argv[1], "d") != 0) {
                    seed = stoi(argv[2]);
                    if (seed <= 0) throw 1;
                }
            case 2: // get config file name
                configFile = argv[1];
        }
    }
    catch(...) {
        cerr << "Invalid Usage: " << argv[0]
             << " [ config-file "
             << " [ Seed > 0 "
             << "] ]" << endl;
        exit(EXIT_FAILURE);                // TERMINATE
    }

    mprng.set_seed(seed);
    ConfigParms configParms;
    processConfigFile(const_cast<char*>(configFile.c_str()), configParms);

    unsigned int maxTripCost = configParms.stopCost * (configParms.numStops / 2);
    Printer prt(configParms.numStudents, 2, configParms.numStops, configParms.numCouriers);
    Bank bank(configParms.numStudents);
    WATCardOffice cardOffice(prt, bank, configParms.numCouriers);
    Groupoff groupoff(prt, configParms.numStudents, configParms.stopCost, configParms.groupoffDelay);
    Parent parent(prt, bank, configParms.numStudents, configParms.parentalDelay, maxTripCost);
    NameServer nameServer(prt, configParms.numStops, configParms.numStudents);

    TrainStop * trainStops[configParms.numStops];

    Train * trains[2];
    Conductor * conductors[2];
    Student * students[configParms.numStudents];

    for (int id = 0; id < configParms.numStops; ++id) {
        trainStops[id] = new TrainStop(prt, nameServer, id, configParms.stopCost);
    }

    Timer * timer = new Timer(prt, nameServer, configParms.timerDelay);
    for (int id = 0; id < 2; ++id) {
        trains[id] = new Train(prt, nameServer, id, configParms.maxNumStudents, configParms.numStops);
        conductors[id] = new Conductor(prt, id, trains[id], configParms.conductorDelay);
    }

    for (int id = 0; id < configParms.numStudents; ++id) {
        students[id] = new Student(prt, nameServer, cardOffice, groupoff, id, configParms.numStops,
                configParms.stopCost, configParms.maxStudentDelay, configParms.maxStudentTrips);
    }

    // Delete all of the students first to show that no more people are waiting for the trains
    for (int id = 0; id < configParms.numStudents; ++id) {
        delete students[id];
    }

    // Delete the timer first so the trainstops nor train will be ticked
    delete timer;

    for (int id = 0; id < configParms.numStops; ++id) {
        delete trainStops[id];
    }

    for (int id = 0; id < 2; ++id) {
        delete conductors[id];
        delete trains[id];
    }

}