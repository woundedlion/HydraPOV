#ifndef _HYDRANODE_h
#define _HYDRANODE_h

#include <FastLED.h>
#include "Barrier.h"

class HydraNode
{
private:

	static const int FRAME_SYNC_PIN_READY = 24;
	static const int FRAME_SYNC_PIN_WAIT = 26;
	static const int PIN_ID0 = 21;
	static const int PIN_ID1 = 22;
	static const int PIN_ID2 = 23;
	static const int PIN_SPI_CLOCK = 13;
	static const int PIN_SPI_DATA = 11;
	static const int PIN_TRIGGER = 2;
	static const int PIN_CLOCK = 5;
	static const int FREQ = 266 * 16;

	int id_ = -1;
	APA102Controller<PIN_SPI_DATA, PIN_SPI_CLOCK, BGR, DATA_RATE_MHZ(16)> strip_;

	void read_id();
	void start_clock();

public:

	static const int W = 288;
	static const int H = 48;
	static const int PIXELS = 144;
	typedef Barrier<FRAME_SYNC_PIN_READY, FRAME_SYNC_PIN_WAIT> FrameSync;

	HydraNode();
	int id() { return id_; }
	int y_offset() { return id_ * H; }
	CLEDController & strip() { return strip_; }
	void attach();
	void detach();
};

#endif

