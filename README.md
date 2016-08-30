ESP_RTOS_REPRAP
===============

A proof of concept: ESP8266 alone is more than enough to run steppers, servos and PWMs. There is no need for an auxiliary, slow, ATMEGA.

[Youtube](https://www.youtube.com/watch?v=tURU9UKbYXw) video of it in action.
[RepRap Forum](http://forums.reprap.org/read.php?2,594898) discussion.

Setting Up
----------

1. You will need a linux machine.
2. Clone this repository, then create an empty bin directory.
3. Follow the remaining instructions [here](https://github.com/lhartmann/esp_rtos_zero).

Hardware
--------

* A breakout for ESP8266 that includes I2S pins. I used NodeMCU, ESP12 would do, ESP01 would not.
* 32-bit buffered output shift register: [Shift-out-32-HC595](https://github.com/lhartmann/Shift-out-32-HC595).
* A4988 or DRV8833 stepper drivers.
