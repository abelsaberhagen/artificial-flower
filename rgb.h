/* Variables and Functions that allows us to more
*  easily use our rgb led.
*
* Variables are only mentioned in this tab/file
*/
int redLed = 5;
int greenLed = 6;
int blueLed = 3;
//int potPin = A0;


void setupRGBPins() { 
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  //pinMode(potPin, INPUT);
}

void whiteLight() { 
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, HIGH);
  digitalWrite(blueLed, HIGH);
}

void rgbLedOff() {
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, LOW);
}

void RGB(int r, int g, int b) {
  analogWrite(redLed, r);
  analogWrite(greenLed, g);
  analogWrite(blueLed, b);
//  Serial.println(r);
//  Serial.println(g);
//  Serial.println(b);
}

//h stands for hue
//takes a value for the hue in the color circle < 765
void colorWheel(int h){
  h = h % 765;
  
  if(h < 255){
    RGB(255 - h, h, 0);
  }
  else if(h < 510){
    RGB(0, 510 - h, h - 255);
  }
  else{
    RGB(h - 510, 0, 765 - h);
  }
  
}
