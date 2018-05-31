#ifndef _DISPLAY_h
#define _DISPLAY_h
#include "Effect.h"
#include "HydraNode.h"

class Display
{
private:
	
	static const int W = 288;
	static const int H = 144;
	int x_ = 0;

public:

	static Display *instance_;
	static void draw_column_ISR();
	HydraNode node_;
	Effect *effect_;

	HydraNode & node() { return node_; }
	Effect & effect() { return *effect_; }
	template <typename E>
	void run(unsigned int secs);
};

template <typename E>
void Display::run(unsigned int secs)
{
	E effect(node_);
	effect_ = &effect; // pointer for ISR access
	node_.attach();
	elapsedMillis duration;
	while (duration < (static_cast<unsigned>(secs * 1000))) {
		HydraNode::Sync sync;
		elapsedMicros duration2;
		effect.draw_frame();
		Serial.println("Wait");
		sync.wait();
		Serial.println(duration2);
		Serial.println("wait done");
		while (!effect.ready()) {}
		Serial.println("Frame advance");
	}
	node_.detach();
	}
#endif

