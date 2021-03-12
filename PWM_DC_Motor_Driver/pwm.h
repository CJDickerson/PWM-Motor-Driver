/*
 * pwm.h
 *
 *  Created on: Mar 1, 2021
 *      Author: chuck
 */

#ifndef PWM_H_
#define PWM_H_

#include <string>

using std::string;

#define PWM_PATH "/sys/devices/platform/ocp/48302000.epwmss/48302200.pwm/pwm"
#define PWM_PIN_PATH "/sys/devices/platform/ocp/ocp:P9_14_pinmux/"
#define GPIO_PATH "/sys/class/gpio/"

#define PWM_ENABLE "enable"
#define PWM_PERIOD "period"
#define PWM_DUTY_CYCLE "duty_cycle"
#define PWM_POLARITY "polarity"
#define PIN_STATE "state"
#define PIN_VALUE "value"

#define ONE_MS 1000

namespace myNameSpace {

enum MOTOR_DIRECTION {CW, CCW, STOP};

class PWM {
public:

private:
	string pathToPWM;
	string pathToPWMPin;
	string pathToGPIOPin;

public:
	PWM();
	static string readFile(string path, string filename);
	static int writeFile(string path, string filename, int value);
	static int writeFile(string path, string filename, string value);
	static string buildPath();
	string getPinState();
	int setPWMPinState(string value);
	string getGPIOPinValue(string pin);
	unsigned int getEnable();
	unsigned int getPeriod();
	unsigned int getDutyCycle();
	float getDutyCyclePct();
	string getPolarity();
	int setPeriod(unsigned int value);
	int setDutyCycle(unsigned int value);
	int setDutyCyclePct(float percentage);
	int enablePWM();
	int disablePWM();
	void rampSpeed(float desiredSpeed, unsigned int time);
	void setMotorDirection(MOTOR_DIRECTION direction, int pin1, int pin2);
};

}

#endif /* PWM_H_ */
