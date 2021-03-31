#include "pwm.h"
#include "gpio.h"

#include <iostream>
#include <unistd.h>
#include <cstdio>

using namespace myNameSpace;
using namespace std;

int main()
{
    printf("hello from %s!\n", "ConsoleApplication1");

	string path;
	string pin_state;

	cout << endl;
	cout << "PWM Program for PWM1A" << endl;

	// constructors
	PWM pwm;
	GPIO P9_15("P9_15", 48);
	GPIO P9_17("P9_17", 5);

	cout << "Setup PWM and GPIO pins" << endl;

	// Set state of P9.14 to pwm
	pwm.setPWMPinState("pwm");

	// Set state of P9.15 (gpio 48) and P9.17 (gpio 5) to gpio
	P9_15.setPinState("gpio");
	P9_17.setPinState("gpio");

	// Set direction of P9.15 (gpio 48) and P9.17 (gpio 5) to out
	P9_15.setDirection(OUTPUT);
	P9_17.setDirection(OUTPUT);

	// set initial motor direction
	pwm.setMotorDirection(CW, P9_15.getNumber(), P9_17.getNumber());

	// setup PWM
	pwm.setPeriod(50000);
	pwm.setDutyCyclePct(0.0f);

	pwm.enablePWM();
	cout << "Enable PWM" << endl;

	//=====================start running motor==============================

	cout << "Start CW Ramp" << endl;

	// ramp to 90% over 5 seconds
	pwm.rampSpeed(90.0f, 5);

	// ramp to 0% over 5 seconds
	pwm.rampSpeed(0.0f, 5);

	// Change motor direction
	pwm.setMotorDirection(CCW, P9_15.getNumber(), P9_17.getNumber());
	cout << "Change motor direction " << endl;

	cout << "Start CCW Ramp" << endl;

	// ramp to 90% over 3 seconds
	pwm.rampSpeed(90.0f, 2);

	// ramp to 0% over 3 seconds
	pwm.rampSpeed(0.0f, 2);

	// Set inputs to stop, turn off PWM when done
	pwm.setMotorDirection(STOP, P9_15.getNumber(), P9_17.getNumber());
	pwm.disablePWM();
	cout << "Disable PWM, stop motor " << endl;
}
