#include "bank.h"
Bank::Bank(unsigned int numStudents): numStudents{numStudents} {
    studentBank = new unsigned int[numStudents];
    for (unsigned int i = 0; i < numStudents; ++i) {
        studentBank[i] = 0;
    }
}

void Bank::deposit(unsigned int id, unsigned int amount) {
    studentBank[id] += amount;
}

void Bank::withdraw(unsigned int id, unsigned int amount) {
    for (;;) {
        _When (studentBank[id] < amount) _Accept(deposit);
        _Else break;
    }
    studentBank[id] -= amount;
}

Bank::~Bank() {
    delete [] studentBank;
}