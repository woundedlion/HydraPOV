#pragma once

#include "Arduino.h"
#include <fastled.h>
#include "HydraNode.h"

class Effect
{	
public:

	Effect(HydraNode& node) : node_(node) {
	}
	virtual ~Effect() {};
	virtual void draw_frame() = 0;
	virtual bool show_bg() = 0;

	inline CRGBSet column(int x) {
		return CRGBSet(bufs_[prev_][x], HydraNode::H);
	}

	inline bool ready() {
		return prev_ == next_;
	}

	inline void swap() {
		prev_ = next_;
	}

protected:

	HydraNode & node_;
	volatile int prev_= 0, cur_ = 0, next_ = 0;
	CRGB bufs_[2][HydraNode::W][HydraNode::H];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////

class Spiral : public Effect
{
private:

	CRGBPalette256 palette_;

public:

	Spiral(HydraNode & node) : Effect(node)
	{
		Serial.println(node_.id());
		fill_rainbow(palette_.entries, 256, 0, 1);
	}

	bool show_bg() { return true; }

	FASTRUN	void draw_frame() {
		cur_ = !cur_;
		int y_offset = node_.y_offset();
		for (int x = 0; x < node_.W; ++x) {
			for (int y = y_offset; y < y_offset + node_.H; ++y) {
				bufs_[cur_][x][y - y_offset] = palette_[(x + y) % 256];
			}
		}
		next_ = cur_;
		Serial.println("Frame drawn");
	}
};