# Dot Survivor
## Dot Survivor is a game (mainly) for Arduino.

# Example setup:
### Screen
I recommend a 128 by 64 OLED display, but some others will work too. Start by connecting the GND pin on the OLED to GND on your Arduino. Connect VCC to 5V, SDA to SDA, and SCL to SCL.

### Joystick
You will need a joystick with X and Y analog outputs, and SW digital output. start by connecting 5V to 5V on your Arduino, then connect it to ground, then connect VRX to A0, and VRY to A1. Then connect SW to pin 2 on your Arduino. 

### Speaker (optional)
You will need a buzzer with a + and - pin. Connect + on the buzzer to a digital pin on your Arduino (default 8). Connect - to ground. REMEMBER: if you don't use a buzzer or used another pin than 8, you need to specify this in the config. 

### Reset (optional)
Start by connecting the Arduino's reset pin to digital pin 4 through a 220 ohm resistor. Also connect the reset pin to 5V through a 5k to 50k ohm resistor. REMEMBER: if you don't use the reset pin or used another pin than 4, you need to specify this in the config.