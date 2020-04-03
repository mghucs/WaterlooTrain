#include "student.h"

Student::Student(Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
                 unsigned int id, unsigned int numStops, unsigned int stopCost, unsigned int maxStudentDelay, unsigned int maxStudentTrips):
                 prt{prt}, nameServer{nameServer}, cardOffice{cardOffice},
                groupoff{groupoff}, id{id}, numStops{numStops}, stopCost{stopCost},
                maxStudentDelay{maxStudentDelay}, maxStudentTrips{maxStudentTrips} {}
Student::~Student(){}

void Student::main() {

    yield(maxStudentDelay);
}
