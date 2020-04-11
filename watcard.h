#pragma once

#include <uFuture.h>

class WATCard {
	bool paid = false;
	unsigned int balance;

	WATCard( const WATCard & ) = delete;				// prevent copying
	WATCard & operator=( const WATCard & ) = delete;

  public:
	typedef Future_ISM<WATCard *> FWATCard;				// future watcard pointer
	WATCard();
	~WATCard();
	void deposit( unsigned int amount );
	void withdraw( unsigned int amount );
	unsigned int getBalance();
	bool paidForTicket();
  	void resetPOP();
}; // WATCard
