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

extern MPRNG mprng;
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
}