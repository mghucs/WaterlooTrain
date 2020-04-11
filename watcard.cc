#include "watcard.h"

WATCard(): balance{0} {}

void WATCard::deposit(unsigned int amount) {
    balance += amount;
}

void WATCard::withdraw(unsigned int amount) {
    balance -= amount;
}

unsigned int WATCard::getBalance() {
    return balance;
}

bool WATCard::paidForTicket() {
    return paid;
}

void WATCard::resetPOP() {
    paid = !paid; // Make paid = true if paid is false and vice versa
}