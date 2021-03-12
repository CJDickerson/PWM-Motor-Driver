# PWM-Motor-Driver
Uses Beaglebone Black GPIO to drive a DC motor.
P9-14 is the PWM output, P9-15 and P9-17 control the motor direction.
The motor driver is TB6612 from Sparkfun. The main program ramps the motor
up and down in speed, changes direction, and repeats the ramps. Base on Derek Molloy's
code from his book: Exploring BB.
