#ifndef _BARRIER_h
#define _BARRIER_h
#include "Arduino.h"

template <int PIN_READY, int PIN_WAIT>
class Barrier
{
public:

	Barrier()
	{
		reset();
	}

	~Barrier()
	{
		reset();
	}

	void wait()
	{
		digitalWriteFast(PIN_READY, LOW);
		while (digitalReadFast(PIN_WAIT)) {}
	}

	void reset()
	{
		digitalWriteFast(PIN_READY, HIGH);
	}
};

#endif

