/*
 * pwm.cpp
 *
 *  Created on: Mar 1, 2021
 *      Author: chuck
 */
#include "pwm.h"
#include "gpio.h"

#include <iostream>
#include <fstream>
#include <dirent.h>
#include <cstring>
#include <unistd.h>

using namespace std;

namespace myNameSpace {

// constructor
PWM::PWM() {
	this->pathToPWM = PWM::buildPath();
	this->pathToPWMPin = PWM_PIN_PATH;
	this->pathToGPIOPin = GPIO_PATH;
}

string PWM::readFile(string path, string filename) {
	ifstream fs;
	fs.open((path + filename).c_str());
	if (!fs.is_open()) {
		cout << "read failed to open file" << endl;
	}
	string input;
	getline(fs,input);
	fs.close();
	return input;
}

int PWM::writeFile(string path, string filename, int value) {
	ofstream fs;
	fs.open((path + filename).c_str());
	if (!fs.is_open()) {
		cout << "write failed to open file" << endl;
		return -1;
	}
	fs << value;
	fs.close();
	return 0;
}

int PWM::writeFile(string path, string filename, string value) {
	ofstream fs;
	fs.open((path + filename).c_str());
	if (!fs.is_open()) {
		cout << "write failed to open file" << endl;
		return -1;
	}
	fs << value;
	fs.close();
	return 0;
}

string PWM::buildPath() {
	struct dirent *de;
	string chip, str1, str2;
	int len;

	// find folder where the desired PWM is located
	// There should be only 3 items in the folder:
	// ".","..", and "pwmchipN", so look for the string
	// which is 8 chars long
	DIR *dr = opendir(PWM_PATH);
	if (dr == NULL) {
		cout << "Could not open directory " << PWM_PATH << endl;
	}
	while ((de = readdir(dr)) != NULL) {
		chip = de->d_name;
		len = strlen(chip.c_str());
		if (len == 8)
			break;
	}
	closedir(dr);

	// use chip number to build the rest of the path
	// /pwmchipN/pwm-N:0/
	str1 = PWM_PATH;
    str1 = str1 + "/" + chip + "/";
    // get chip number, last char in chip string
    char ch = chip.at(7);
    str2 = "pwm-";
    str2 += ch;
    str2 += ":0/";
    str1 += str2;

	return str1;
}

unsigned int PWM::getEnable() {
	string enable = readFile(this->pathToPWM, PWM_ENABLE);
	return stoi(enable);
}

unsigned int PWM::getPeriod() {
	string period = readFile(this->pathToPWM, PWM_PERIOD);
	return stoi(period);
}

unsigned int PWM::getDutyCycle() {
	string duty_cycle = readFile(this->pathToPWM, PWM_DUTY_CYCLE);
	return stoi(duty_cycle);
}

float PWM::getDutyCyclePct() {
	unsigned int duty_cycle = this->getDutyCycle();
	unsigned int period = this->getPeriod();
	float dcPct = 100.0f * (float)duty_cycle/(float)period;
	return dcPct;
}

string PWM::getPolarity() {
	string polarity = readFile(this->pathToPWM,PWM_POLARITY);
	return polarity;
}

int PWM::setPeriod(unsigned int value) {
	return writeFile(this->pathToPWM, PWM_PERIOD, value);
}

int PWM::setDutyCycle(unsigned int value) {
	unsigned int period = getPeriod();
	if (period > value) {
		return writeFile(this->pathToPWM, PWM_DUTY_CYCLE, value);
	}
	else {
		cout << "Duty Cycle Too High" << endl;
		return 0;
	}
}

int PWM::setDutyCyclePct(float percentage) {
	if (percentage > 100.0f)
		percentage = 100.0f;
	else if (percentage < 0.0f)
		percentage = 0.0f;
	unsigned int period_ns = getPeriod();
	float duty_ns = (float)period_ns * (percentage/100.0f);
    return writeFile(this->pathToPWM, PWM_DUTY_CYCLE, (unsigned int)duty_ns);
}

int PWM::enablePWM() {
	return writeFile(this->pathToPWM, PWM_ENABLE, 1);
}

int PWM::disablePWM() {
	return writeFile(this->pathToPWM, PWM_ENABLE, 0);
}

string PWM::getPinState() {
	string state = readFile(this->pathToPWMPin, PIN_STATE);
	return state;
}

int PWM::setPWMPinState(string value) {
	return writeFile(this->pathToPWMPin, PIN_STATE, value);
}

string PWM::getGPIOPinValue(string pin) {
	string pin_string = this->pathToGPIOPin + "gpio" + pin + "/";
	string state = readFile(pin_string, PIN_VALUE);
	return state;
}

void PWM::setMotorDirection(MOTOR_DIRECTION direction, int pin1, int pin2) {
	string pin1_string = this->pathToGPIOPin + "gpio" + to_string(pin1) + "/";
	string pin2_string = this->pathToGPIOPin + "gpio" + to_string(pin2) + "/";
	if (direction == CW) {
		writeFile(pin1_string, "value", "0");
		writeFile(pin2_string, "value", "1");
	}
	else if (direction == CCW) {
		writeFile(pin1_string, "value", "1");
		writeFile(pin2_string, "value", "0");
	}
	else if (direction == STOP) {
		writeFile(pin1_string, "value", "0");
		writeFile(pin2_string, "value", "0");
	}
}

void PWM::rampSpeed(float desiredPct, unsigned int time) {
	// keep same direction
	// desiredSpeed is duty cycle in %, time is seconds

	// timesteps = time/time step size
	unsigned int timesteps = (unsigned int)((float)(1e6*time)/(float)ONE_MS);

	// get current speed
	float currentPct = this->getDutyCyclePct();

	// calculate step size: (desiredPct - currentPct)/timesteps
	float stepSize = (desiredPct - currentPct)/(float)timesteps;

	if (stepSize >= 0)
		for (float dc = currentPct; dc <= desiredPct; dc+= stepSize) {
			this->setDutyCyclePct(dc);
			usleep(ONE_MS);
		}
	else
		for (float dc = currentPct; dc >= desiredPct; dc+= stepSize) {
			this->setDutyCyclePct(dc);
			usleep(ONE_MS);
		}
}
}





