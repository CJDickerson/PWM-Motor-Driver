/*
 * gpio.h
 *
 *  Created on: Mar 12, 2021
 *      Author: chuck
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <fstream>

using std::string;
using std::ofstream;

#define GPIO_PATH "/sys/class/gpio/"
#define GPIO_PINMUX_PATH "/sys/devices/platform/ocp/ocp:"

namespace myNameSpace {

enum GPIO_DIRECTION { INPUT, OUTPUT};
enum GPIO_VALUE { LOW=0, HIGH=1};

class GPIO {
public:

private:
	int number;	// GPIO number of the pin
	string name; // name of the pin e.g. gpio50
	string muxName; // name of the pinmux e.g. ocp:P9_15_pinmux
	string path; // full path to the pin
	string muxPath;

public:
	GPIO(string pin,int number);
	string getPinState();
	int setPinState(string value);
	GPIO_DIRECTION getDirection();
	GPIO_VALUE getValue();
	virtual int setDirection(GPIO_DIRECTION);
	int setValue(GPIO_VALUE value);
	int toggleValue();
	int getNumber();
	~GPIO();
private:
};
}





#endif /* GPIO_H_ */
