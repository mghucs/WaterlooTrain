#include "config.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

void processConfigFile( const char * configFile, ConfigParms & cparms ) {
    ifstream inFile(configFile);

    string line;
//    cout << configFile << endl;
    while (getline(inFile, line)) {

        istringstream iss(line);
        string name;
        int num;

        iss >> name;
        iss >> num;

        if (name.compare("StopCost") == 0) cparms.stopCost = num;
        else if (name.compare("NumStudents") == 0) cparms.numStudents = num;
        else if (name.compare("NumStops") == 0) cparms.numStops = num;
        else if (name.compare("MaxNumStudents") == 0) cparms.maxNumStudents = num;
        else if (name.compare("TimerDelay") == 0) cparms.timerDelay = num;
        else if (name.compare("MaxStudentDelay") == 0) cparms.maxStudentDelay = num;
        else if (name.compare("MaxStudentTrips") == 0) cparms.maxStudentTrips = num;
        else if (name.compare("GroupoffDelay") == 0) cparms.stopCost = num;
        else if (name.compare("ConductorDelay") == 0) cparms.conductorDelay = num;
        else if (name.compare("ParentalDelay") == 0) cparms.parentalDelay = num;
        else if (name.compare("NumCouriers") == 0) cparms.numCouriers = num;
    }

}

