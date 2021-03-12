/*
 * gpio.cpp
 *
 *  Created on: Mar 1, 2021
 *      Author: chuck
 */
#include "gpio.h"
#include "pwm.h"
#include <stdlib.h>

using namespace std;

namespace myNameSpace {
	// constructor
GPIO::GPIO(string pin,int number) {
	this->number = number;
	string name = "gpio";
	name += to_string(number);
	this->name = name;
	this->path = GPIO_PATH + this->name + "/";
	this->muxPath = GPIO_PINMUX_PATH + pin + "_pinmux/";
}

string GPIO::getPinState() {
	string state = PWM::readFile(this->muxPath, PIN_STATE);
	return state;
}

int GPIO::setPinState(string value) {
	return PWM::writeFile(this->muxPath, PIN_STATE, value);
}

GPIO_DIRECTION GPIO::getDirection() {
	string input = PWM::readFile(this->path, "direction");
	if (input == "in")
		return INPUT;
	else return OUTPUT;
}

GPIO_VALUE GPIO::getValue() {
	string input = PWM::readFile(this->path, "value");
	if (input == "0") return LOW;
	else return HIGH;
}

int GPIO::setDirection(GPIO_DIRECTION dir) {
	switch (dir) {
	case INPUT: return PWM::writeFile(this->path, "direction", "in");
		break;
	case OUTPUT: return PWM::writeFile(this->path, "direction", "out");
		break;
	}
	return -1;
}

int GPIO::setValue(GPIO_VALUE value) {
	switch (value) {
	case LOW: return PWM::writeFile(this->path, "value", "0");
		break;
	case HIGH: return PWM::writeFile(this->path, "value", "1");
		break;
	}
	return -1;
}

int GPIO::toggleValue() {
	GPIO_VALUE value = this->getValue();
	if (value == HIGH)
		this->setValue(LOW);
	else
		this->setValue(HIGH);
	return 0;
}

int GPIO::getNumber() {
	return this->number;
}

GPIO::~GPIO() {}

}



