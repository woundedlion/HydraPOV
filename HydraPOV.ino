#include "Display.h"
#include "Effect.h"
#include <fastled.h>

void setup()
{
	Serial.begin(9600);
	Serial.println("Initializing...");
	Display::instance_ = new Display();
	Serial.print("ID ");
	Serial.print(Display::instance_->node().id());
	Serial.println("Initialized");
}

void loop()
{
	Display& d = *Display::instance_;
	d.run<Spiral>(15);
}
