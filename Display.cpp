#include "Display.h"

Display *Display::instance_ = nullptr;

FASTRUN void Display::draw_column_ISR()
{
	Display & d = *Display::instance_;
	CLEDController & strip = d.node().strip();

	CRGB *leds = d.effect().column(d.x_);
	strip.show(leds, HydraNode::H, 255);
	if (d.effect().show_bg()) {
		strip.showColor(CRGB::Black, HydraNode::H, 0); 
	}
	if (++d.x_ == HydraNode::W) {
		d.x_ = 0;
		d.effect_->advance_display();
	}
 }
