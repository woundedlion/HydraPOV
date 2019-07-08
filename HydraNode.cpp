#include "Display.h"
#include "HydraNode.h"
#include "Arduino.h"
#include "Barrier.h"
#include "FastLED.h"

HydraNode::HydraNode()
{
	pinMode(FRAME_SYNC_PIN_READY, OUTPUT);
	pinMode(FRAME_SYNC_PIN_WAIT, INPUT_PULLDOWN);
	delay(2000);
	digitalWriteFast(FRAME_SYNC_PIN_READY, HIGH);
	pinMode(PIN_ID0, INPUT_PULLUP);
	pinMode(PIN_ID1, INPUT_PULLUP);
	pinMode(PIN_ID2, INPUT_PULLUP);
	pinMode(PIN_SPI_CLOCK, OUTPUT);
	pinMode(PIN_SPI_DATA, OUTPUT);
	pinMode(PIN_TRIGGER, INPUT);
	pinMode(PIN_CLOCK, OUTPUT);

	SCB_SHPR3 = 0x20200000;  // Systick = priority 32 (defaults to zero)
	NVIC_SET_PRIORITY(IRQ_PORTA, 0);
	NVIC_SET_PRIORITY(IRQ_PORTB, 0);
	NVIC_SET_PRIORITY(IRQ_PORTC, 0);
	NVIC_SET_PRIORITY(IRQ_PORTD, 0);
	NVIC_SET_PRIORITY(IRQ_PORTE, 0);

	read_id();
	strip().init();
	FastLED.setMaxRefreshRate(strip().getMaxRefreshRate());
	start_clock();
}

void HydraNode::read_id() 
{
	id_ = ~(digitalReadFast(PIN_ID0)
		| (digitalReadFast(PIN_ID1) << 1)
		| (digitalReadFast(PIN_ID2) << 2)) & 0x7;
}

void HydraNode::start_clock()
{
	analogWriteFrequency(PIN_CLOCK, FREQ);
	analogWrite(PIN_CLOCK, 128);
}

void HydraNode::attach()
{
	attachInterrupt(PIN_TRIGGER, Display::instance_->draw_column_ISR, RISING);
	Serial.println("Clock interrupt attached");
}

void HydraNode::detach()
{
	detachInterrupt(PIN_TRIGGER);
	Serial.println("Clock interrupt detached");
}
