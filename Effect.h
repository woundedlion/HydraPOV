#pragma once

#include "Arduino.h"
#include <fastled.h>
#include "HydraNode.h"

class Cursor;
class Effect;

class Effect
{	
	friend class Cursor;

public:

	Effect(HydraNode& node) : node_(node) {
		memset(bufs_, 0, sizeof(bufs_));
	}
	virtual ~Effect() {};
	virtual void draw_frame() = 0;
	virtual bool show_bg() = 0;
	
	inline CRGB* column(int x) {
		return &bufs_[prev_][x][node_.y_offset()];
	}

	inline bool ready() {
		return prev_ == next_;
	}

	inline void advance() {
		Serial.print("swap: ");
		Serial.println(millis());
		prev_ = next_;
	}

protected:

	HydraNode & node_;

private:

	volatile int prev_= 0, cur_ = 0, next_ = 0;
	CRGB bufs_[2][HydraNode::W][HydraNode::PIXELS];
};

class Cursor
{
public:

	Cursor(Effect& effect) : effect_(effect),
		sync_(!effect_.cur_)
	{
		effect_.cur_ = !effect_.cur_;
	}

	~Cursor() {
		effect_.next_ = effect_.cur_;
		while (!effect_.ready()) {}
		sync_.wait();
	}

	inline CRGB(&operator()())[HydraNode::W][HydraNode::PIXELS]{
		return effect_.bufs_[effect_.cur_];
	}

private:

	Effect & effect_;
	HydraNode::Sync sync_;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////

class Spiral : public Effect
{
private:

	CRGBPalette256 palette_;

public:

	Spiral(HydraNode & node) : Effect(node) {
		fill_rainbow(palette_.entries, 256, 0, 1);
	}

	bool show_bg() { return true; }

	FASTRUN	void draw_frame() {
		Cursor buf(*this);
		int y_offset = node_.y_offset();
		for (int x = 0; x < node_.W; ++x) {
			for (int y = y_offset; y < y_offset + node_.H; ++y) {
				buf()[x][y] = palette_[(x + y) % 256];
			}
		}
	}
};