#ifndef _BARRIER_h
#define _BARRIER_h
#include "Arduino.h"

template <int PIN_READY, int PIN_WAIT>
class Barrier
{
public:

	Barrier(int id) :
		pin_wait_(PIN_WAIT + id),
		pin_ready_(PIN_READY + id)
	{
		digitalWriteFast(pin_ready_, HIGH);
	}

	void wait()
	{
		digitalWriteFast(pin_ready_, LOW);
		while (digitalReadFast(pin_wait_)) {}
	}

private:

	int pin_wait_;
	int pin_ready_;
};

#endif

