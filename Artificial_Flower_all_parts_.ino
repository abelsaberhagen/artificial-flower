/* Artificial Flower By Abel Saberhagen

The Artificial Flower is supposed to act like a flower,
it's characteristics are the petals that can wilt or rise
depending on the flower's health, and the color that 
will always remain dark orange at lowhealth but can be 
changed at high health.

Components:
Four 5V Wires
Five Super Long 5V Wires
Six GND Wires
Seven Super Long GND Wires
Five White Wires
Five Super Long White Wires
Nine Other Wires

RGB LED(should already be connected to four super long wires 
including one of the super long GND wires)
Soil Moisture Sensor(should already be connected to three super
long wires including one of the super long GND wires)
One Button
Four Resistors
One Photoresistor
One Temperature Sensor
Five Servos(should already each be connected to three super
long wires which includes one of the super long GND wires, is
also taped to a piece of cardboard at the top of the flower)

Wiring:
The 5V rail closest to the redboard is connected to 5V. Both
GND rails are connected to GND. The 5V rail further away
from the redboard is connected to VIN which is for extra
power supply so that there is enough current to power all the
servos.

For the RGB LED you should first push the end of the wires 
with the rgb led through the hole of the cardboard box at the
bottom of the flower. If you look through the hole you will
see another hole with wires coming out of it from the servos.
Push the rgb led up this hole until pops out at the top. Now
on the breadboard you can hook up the rgb led as you normally
would. The orange wire is connected the the red led, the
green wire is for the green led, and the blue wire is for the
blue led. Connect the orange, green, and blue wires each to a
resistor and connect each resistor to a normal sized wire which
connects to a PWM pin. The GND wire connects to one of the GND
rails.

The button, photoresistor, and temperature sensor are connected
as they normally would be.

The soil moisture sensor is attached to three wires. Connect
these wires to the breadboard and connect those wires with 
normal sized wires so that the soil moisture sensor can
stretch as far away from the breadboard as possible. Rather
than using 5V the soil moisture sensor uses a digital pin for
power so that it doesn't corrode while sitting in soil. 
The orange wire will be connected to a digital pin for power.
The yellow wire will be connected to an analog pin, and the
GND wire will be connected to GND.

The Servo wires should be connected to the servos and should be
coming out of the box at the bottom of the flower. There should
be five GND wires, five 5V wires, and five white wires. Connect
the five 5V wires to the power rail that is farthest away from
the breadboard. This power rail is connected to VIN which gives
the servos the extra current that they need. Connect the five
GND wires to GND, and connect the five white wires to five
normal sized white wires that connect to a digital pin(do not
use digital pins 9 or 10).

Once you've wired all the parts of the project hook up 
the extra power supplier and your computer to the redboard.

How to Use this Project

This flower is made up of the rgb led and the servos. The rgb
led is the flower's color and the servos are the flower's 
petals. The flower also has health and the color of the
flower and the degree that the servos are pointing in depends
on what the health of the flower is. The health of the flower
is based on three things - the amount of light it gets(analog
value of the photoresistor), the temperature of the room
(analog value of the temperature sensor), and the amount of
water the flower gets(analog value of the soil moisture 
sensor). These three values are averaged to get the flower's
health. If the flower has low health then the servos will point
downwards and the color of the rgb led will become more of a
dark orange color. If the flower has high health then the 
servos will point upwards and the color of the rgb led will be
of the player's choosing. The color that the user can choose
is defaulted to green but you can change it by pressing the
yellow button. However you will not notice any change if you
press the button and the flower is at low health. This is
because no matter what color is chosen by the user the flower
will have a dark orange color when at low health. The flower's
health is only updated every ten seconds. There is also a way
to kill the flower. If you leave the flower at very low health
for 50 seconds straight the rgb led will turn off and the
servos will point downwards.    

Note for the Soil Moisture Sensor

For the soil moisture sensor you technically don't actually 
need soil. You can cover you hand around it and that would be
enough to increase the flower's health.
*/

#include "rgb.h"
#include <Servo.h>

int mappedLight; //the mapped value of the light variable
int light; //analog value of the photoresistor
int photoPin = A0; //analog pin of the photoresistor

int tempPin = A1; //analog pin of the temperature sensor
int temp; //analog value of the temperature sensor
float healthyTemp; //stores how healthy the temperature is for the flower, if the temperature is > 69 then it is the healthiest possible temperature for the flower.
float voltage; //helps turn the temp variable into celsius
float degreesC; //degrees celsius of the temp variable
float degreesF; //degress fahrenheit of the temp variable

int soilPin = A2; //analog pin of the soil moisture sensor
int soilPower = 9; //instead of using 5V to power the soil we use a digital pin so that the soil moisture sensor does not corrode if it is left outside
int soilMoisture = 0; //analog value of the soil moisture sensor
int mappedSoilMoisture; //mapped value of the soilMoisture variable

float flowerHealth = 100; //health of the flower, defaults to 100 because the flower starts with full health
float prevFlowerHealth; //the health of the flower before the health of the flower updated
int intFlowerHealth; //the integer value of the flowerHealth variable
int intPrevFlowerHealth; //the integer value of the prevFlowerHealth variable 
int lowHealthCycles = 0; //how many times has the flower had low health in a row
int incrementedFlowerHealth = 100; //this is a variable that increments from the intPrevFlowerHealth varaible to the intFlowerHealth variable

int red; //red value for the rgb led
int green; //green value for the rgb led
int blue; //blue value for the rgb led
int chosenColor = 5; //the bright color that is chosen by the user
const byte colorButtonPin = 2; //pin number of the color changing button, is a const byte so that it can become an interrupt

int servoAngle; //angle for all the servos
int lenServoArray = 5; //length of the servo array 
Servo petal1;  
Servo petal2;
Servo petal3;
Servo petal4;
Servo petal5;
Servo petals[5] = {petal1, petal2, petal3, petal4, petal5}; //array of servos so that it is easier to make them all turn to the same angle
int petalPins[5] = {7, 4, 8, 11, 12}; //pin numbers of the servos

void setup() {
  setupRGBPins(); //sets the pin mode of the rgb led pins
  pinMode(photoPin, INPUT); //sets the pin mode of the photoresister to input
  pinMode(tempPin, INPUT); //sets the pin mode of the temperature sensor to input
  pinMode(soilPower, OUTPUT); //sets the pin mode of the pin that gives power to the soil moisture sensor to output
  pinMode(colorButtonPin, INPUT_PULLUP); //sets the pin mode of the color changing button to INPUT_PULLUP so I don't have to use a resistor for the button in my circuit 
  digitalWrite(soilPower, LOW); //turns off the pin that gives power to the soil moisture sensor since the soil moisture sensor doesn't need to be on yet(it only needs to be on when I need the analog value of the soil moisture sensor) 
  Serial.begin(9600); //begins the serial
  attachInterrupt(digitalPinToInterrupt(colorButtonPin), changeBrightColor, FALLING); //turns the color changing button from a digital pin to an interrupt and makes it so that when the button is pressed it runs the changeBrightColor() function
  ledColor(incrementedFlowerHealth, 50, 100); //This runs the ledColor() function which takes a number and turn it into a red, green, and blue value. For example if the number was 100, that would be the highest health the flower can have, so if the chosen color was green it would turn the number 100 into the red value which would be 0, the green value which would be 255, and the blue value which would be 0. The other two arguments are the fromLow and fromHigh values that the map() function will use in the ledColor() function. 
  for (int i = 0; i < lenServoArray; i++) { //Attaches all the servos to their corresponding pin number in the petalPins[] array and writes all of the servos to angle 0(where the servos are pointing upwards) 
    petals[i].attach(petalPins[i]);
    petals[i].write(0);
  }
}

void loop() {
  delaySeconds(10); //waits ten seconds, because the flower health is supposed to only update every ten seconds
  
  light = analogRead(photoPin); //sets the light variable to the analog value of the photoresistor
  mappedLight = map(light, 0, 1023, 0, 100); //maps the light variable to 0, 100 because the flower health ranges from 0 to 100  

  temp = analogRead(tempPin); //sets the temp variable to the analog value of the temperature sensor
  turnToFarenheit(); //function that turns the temp variable into fahrenheit

  readSoil(); //readSoil() function gets the analog value of the soil moisture sensor
  mappedSoilMoisture = map(soilMoisture, 0, 880, 0, 100); //maps the soilMoisture variable to 0, 100 because the flower health ranges from 0 to 100   

  if (degreesF > 69) { //if the temperature is over 69 degrees fahrenheit then the temperature is as healthy as it can be, if not then how healthy the temperature is is dependant on the temperature
    
    healthyTemp = 100.0;
  }
  else {
    healthyTemp = degreesF;
  }

  Serial.println(healthyTemp);
  prevFlowerHealth = flowerHealth; //sets the previous flower health to the current flower health before the current flower health updates
  flowerHealth = (healthyTemp + mappedLight + mappedSoilMoisture) / 3; //averages the three mapped values of the three inputs that the flower's health is based on
  intFlowerHealth = round(flowerHealth); //sets the intFlowerHealth variable to the rounded value of the flowerHealth variable
  intPrevFlowerHealth = round(prevFlowerHealth); //sets the intFlowerHealth variable to the rounded value of the prevFlowerHealth variable
  Serial.println(flowerHealth);

  if (flowerHealth < 55.0) { //death system, if the flower has less than 55 health for 50 seconds straight then it dies and the rgb led turns off and the servos point as downwards as it can and then the project does nothing forever
    lowHealthCycles++;
    if (lowHealthCycles == 5) {
      flowerHealth = 0;
      rgbLedOff();
      for (int i = 0; i < lenServoArray; i++) {
        petals[i].write(50);
      }
      while (true) {
      }
    }
  }
  else {
    lowHealthCycles = 0;
  }

  if (intFlowerHealth < 50) { //makes sure the intFlowerHealth variable is not less than 50 because the map() function in the ledColor() function maps from 50 to 100 and if the intFlowerHealth variable is less than 50 then the code won't know what to do 
    intFlowerHealth = 50;
  }
  if (intPrevFlowerHealth < 50) { //makes sure the intPrevFlowerHealth variable is not less than 50 for the same reasons as the intFlowerHealth variable
    intPrevFlowerHealth = 50;
  }

  //this part of the code is what allows the rgb to slowly transition from one color to another color and it allows the servos to slowly turn
  incrementedFlowerHealth = intPrevFlowerHealth; //sets the incrementedFlowerHealth variable to the intPrevFlowerHealth variable because the incrementedFlowerHealth variable increments from the flower's previous health to it's current health
  while (incrementedFlowerHealth != intFlowerHealth) { //incrementedFlowerHealth variable increments until it is equal to the current flower health
    if (incrementedFlowerHealth < intFlowerHealth) {
      incrementedFlowerHealth++;
    }
    else { 
      incrementedFlowerHealth--;
    }
    ledColor(incrementedFlowerHealth, 50, 100); //every time the incrementedFlowerHealth variable increments it runs these functions 
    turnServo(incrementedFlowerHealth, 50, 100);
    delay(100);
  }
  ledColor(incrementedFlowerHealth, 50, 100); //these functions run one more time to complete the color transition for the rgb led and the servo angle transition for the servos  
  turnServo(incrementedFlowerHealth, 50, 100);
}

void turnToFarenheit() { //turns the temp variable into degrees fahrenheit
  voltage = temp * 0.0048828125;
  degreesC = (voltage - 0.5) * 100;
  degreesF = degreesC * 1.8 + 32;
}

int readSoil() { //gets the analog value of the soil moisture sensor
  digitalWrite(soilPower, HIGH); //turns on so that it can read the anlog value of the soil moisture sensor
  delay(10);
  soilMoisture = analogRead(soilPin);
  digitalWrite(soilPower, LOW); //turns off because the soil moisture sensor isn't being used unless you need the analog value of the soil moisture sensor 
}

void ledColor(int color, int fromLow, int fromHigh) { //This takes a number(the color parameter) and creates a red, green, and blue value from it. For example if the number was 100, that would be the highest health the flower can have, so if the chosen color was green it would turn the number 100 into the red value which would be 0, the green value which would be 255, and the blue value which would be 0. The other two arguments are the fromLow and fromHigh values that the map() function will use in the ledColor() function. 
  if (chosenColor == 0) { //pink
    red = 255; //the red value will always be 255
    green = map(color, fromLow, fromHigh, 20, 0); //At the flower's lowest health the green value will be 20 and at the flower's highest health the green value will be 0.
    blue = map(color, fromLow, fromHigh, 0, 100); //At the flower's lowest health the blue value will be 0 and at the flower's highest health the blue value will be 100.
  }
  if (chosenColor == 1) { //blue
    red = map(color, fromLow, fromHigh, 255, 0); //At the flower's lowest health the red value will be 255 and at the flower's highest health the red value will be 0.
    green = map(color, fromLow, fromHigh, 20, 0); //At the flower's lowest health the green value will be 20 and at the flower's highest health the green value will be 0.
    blue = map(color, fromLow, fromHigh, 0, 255); //At the flower's lowest health the blue value will be 0 and at the flower's highest health the blue value will be 255.
  }
  if (chosenColor == 2) { //yellow
    red = 255; //the red value will always be 255
    green = map(color, fromLow, fromHigh, 20, 175); //At the flower's lowest health the green value will be 20 and at the flower's highest health the green value will be 175.
    blue = 0; //the blue value will always be 0
  }
  if (chosenColor == 3) { //red
    red = 255; //the red value will always be 255
    green = map(color, fromLow, fromHigh, 20, 0); //At the flower's lowest health the green value will be 20 and at the flower's highest health the green value will be 0.
    blue = 0; //the blue value will always be 0
  }
  if (chosenColor == 4) { //purple
    red = 255; //the red value will always be 255
    green = map(color, fromLow, fromHigh, 20, 0); //At the flower's lowest health the green value will be 20 and at the flower's highest health the green value will be 0.
    blue = map(color, fromLow, fromHigh, 0, 255); //At the flower's lowest health the blue value will be 0 and at the flower's highest health the blue value will be 255.
  }
  if (chosenColor == 5) { //green
    red = map(color, fromLow, fromHigh, 255, 0); //At the flower's lowest health the red value will be 255 and at the flower's highest health the red value will be 0.
    green = map(color, fromLow, fromHigh, 20, 255); //At the flower's lowest health the green value will be 20 and at the flower's highest health the green value will be 255.
    blue = 0; //the blue value will always be 0
  }
  RGB(red, green, blue); //runs the RGB() function from the rgb.h file and actually apply the red, green, and blue
}

void turnServo(int mapVar, int fromLow, int fromHigh) { //Turns all the servos to the same angle. The mapVar parameter is the number that the map() function uses to map. The fromLow and fromHigh parameters are the numbers the map() function maps from.
  servoAngle = map(mapVar, fromLow, fromHigh, 50, 0); //Sets the servoAngle variable to a mapped value of the mapVar variable from (fromLow, fromHigh) to (50, 0). 50 is the lowest the servos's angle can be and 0 is the highest.
  for (int i = 0; i < lenServoArray; i++) { //Turns all the servos's angle to the servoAngle variable.
    petals[i].write(servoAngle);
  }
  Serial.println(servoAngle);
}

void delaySeconds(int seconds) { //delays the program for a number of seconds
  delay(1000 * seconds);
}

void changeBrightColor() { //Changes the bright color of the flower when the flower is healthy. This function runs when the button is pressed.
  if (chosenColor == 5) { //Numbers are used to represent colors. For example green is 5 and purple is 4.
    chosenColor = 0;
  }
  else {
    chosenColor++;
  }
  Serial.println(chosenColor);
  ledColor(incrementedFlowerHealth, 50, 100); //Updates the color of the rgb led so it changes it's bright color 
}
