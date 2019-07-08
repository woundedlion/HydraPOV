#pragma once

#include "Arduino.h"
#include <fastled.h>
#include "HydraNode.h"

class Canvas;
class Effect;

class Effect
{	
	friend class Canvas;

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

	inline bool buffer_free() {
		return prev_ == next_;
	}

	inline void advance_display() {
		Serial.print("advance display: ");
		Serial.println(millis());
		prev_ = next_;
	}

	inline void advance_buffer() {
		Serial.print("advance buffer: ");
		Serial.println(millis());
		cur_ = !cur_;
	}

	inline void queue_frame() {
		Serial.print("queue frame: ");
		Serial.println(millis());
		next_ = cur_;
	}

protected:

	HydraNode & node_;

private:

	volatile int prev_= 0, cur_ = 0, next_ = 0;
	CRGB bufs_[2][HydraNode::W][HydraNode::PIXELS];
};

class Canvas
{
public:

	Canvas(Effect& effect) : 
		effect_(effect)
	{
		while (!effect_.buffer_free()) {}
		effect_.advance_buffer();
	}

	~Canvas() {
		sync_.wait();
		effect_.queue_frame();
	}

	inline CRGB(&operator()())[HydraNode::W][HydraNode::PIXELS]{
		return effect_.bufs_[effect_.cur_];
	}

private:

	Effect & effect_;
	HydraNode::FrameSync sync_;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////

class Test1 : public Effect
{
private:

	CRGBPalette256 palette_;
	int frame_ = 0;

public:

	Test1(HydraNode & node) : Effect(node) {
		fill_rainbow(palette_.entries, 256, 0, 1);
	}

	bool show_bg() { return true; }

	FASTRUN	void draw_frame() {
		Canvas buf(*this);
		int y_offset = node_.y_offset();
		frame_ = !frame_;
		for (int x = 0; x < node_.W; ++x) {
			for (int y = y_offset; y < y_offset + node_.H; ++y) {
				buf()[x][y] = frame_ ? CRGB::Red : CRGB::Blue;
			}
		}
	}
};

class Test2 : public Effect
{
private:

	CRGBPalette256 palette_;
	int frame_ = 0;

public:

	Test2(HydraNode & node) : Effect(node) {
		fill_rainbow(palette_.entries, 256, 0, 1);
	}

	bool show_bg() { return true; }

	FASTRUN	void draw_frame() {
		Canvas buf(*this);
		int y_offset = node_.y_offset();
		frame_ = !frame_;
		for (int x = 0; x < node_.W; ++x) {
			for (int y = y_offset; y < y_offset + node_.H; ++y) {
				buf()[x][y] = frame_ ? CRGB::Yellow : CRGB::Green;
			}
		}
	}
};