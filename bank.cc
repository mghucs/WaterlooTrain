#include "bank.h"
#include <iostream>

Bank::Bank(unsigned int numStudents): numStudents{numStudents} {
    studentBank = new unsigned int[numStudents];
    studentCond = new uCondition[numStudents];
    for (unsigned int id = 0; id < numStudents; ++id) {
        studentBank[id] = 0;
    }
}

void Bank::deposit(unsigned int id, unsigned int amount) {
    studentBank[id] += amount;
    studentCond[id].signal();
}

void Bank::withdraw(unsigned int id, unsigned int amount) {

    while(studentBank[id] < amount) studentCond[id].wait();

    studentBank[id] -= amount;
}

Bank::~Bank() {
    for (int id = 0; id < numStudents; id++) studentCond[id].signalBlock();
    delete [] studentBank;
    delete [] studentCond;
}