/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255
  and uses the result to set the pulsewidth modulation (PWM) of an output pin.
  Also prints the results to the serial monitor.

  The circuit:
   potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
   LED connected from digital pin 9 to ground

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the battery1 value
int readCurrent1Pin = A1;

const int analogInPin2 = A2;  // Analog input pin that the battery1 value
int readCurrent2Pin = A3;

int outputValueC1 = 0;        // value output to the current batt1
int outputValueC2 = 0;        // value output to the current batt1

const int numReadings = 30; // number of analog read before checking battery status
const int multi = 1614; // multi coefficent for obtaining millivolts from analogread

long interval = 1000;     // interval for pulse charging and analog read - don't change this

long previousMillis, currentMillis;

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;

int ledT1, ledT2;

int ledState1;
int ledState2;

int c2Zero = 0, c1Zero = 0;

float dV = 2;

int numCb1 = 0, numCb2 = 0;

unsigned int readingtemp;
unsigned int readingtemp2;
unsigned int total = 0;
unsigned int total2 = 0;
float average = 0.000f, medium = 0.000f, maxmedium = 0.000f;
float average2 = 0.000f, medium2 = 0.000f, maxmedium2 = 0.000f;

unsigned int myarray [7];   // array for keeping last 7 readings
unsigned int myarray2 [7];   // array for keeping last 7 readings
int index = 0, index2 = 0; // i = 0;

boolean batt1Full = false, batt1Out = false;
boolean batt2Full = false, batt2Out = false;


void ledStateBatt1() {
  // blink the LED.
  //unsigned long currentMillis = millis();
  if (batt1Out == true) {
    ledT1 = 2;
  }

  if (millis() - previousMillis1 >= ledT1) {
    // save the last time you blinked the LED
    previousMillis1 = millis();

    // if the LED is off turn it on and vice-versa:
    if (ledState1 == LOW) {
      ledState1 = HIGH;
    } else {
      ledState1 = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(10, ledState1);
  }
}



void ledStateBatt2() {
  // blink the LED.
  //unsigned long currentMillis = millis();
  if (batt2Out == true) {
    ledT2 = 2;
  }
  if (millis() - previousMillis2 >= ledT2) {
    // save the last time you blinked the LED
    previousMillis2 = millis();

    // if the LED is off turn it on and vice-versa:
    if (ledState2 == LOW) {
      ledState2 = HIGH;
    } else {
      ledState2 = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(11, ledState2);
  }
}





void pusharray() {
  // push the array
  for (int i = 0; i <= 5; ++i) {
    myarray[i] = myarray[i + 1];
  }
  myarray[6] = (int)average;
}

void pusharray2() {
  for (int i = 0; i <= 5; ++i) {
    myarray2[i] = myarray2[i + 1];
  }
  myarray2[6] = (int)average2;
}




void voltread() {
  readingtemp = analogRead(analogInPin);  // read analog input

  total = total + readingtemp;
  index++;
  Serial.print("           index = ");
  Serial.println( index);
  // if numReadings reached, calculate the average
  if (index >= numReadings) {
    c1Zero = 0;
    index = 0;
    //average = (((float)total / 1023.0) * (float)multi) / numReadings + 0.5;
    average = total / numReadings;
    // if (average<=70) endcharge=true;  // stop charge if battery is detached
    total = 0;

    //
    pusharray(); // insert new average in array
    //
    medium = (float)(myarray[6] + myarray[5] + myarray[4] + myarray[3] + myarray[2] + myarray[1] + myarray[0]) / 7.0f + 0.5f; // calculate the average of the last 7 readings
    medium = (medium / 204.6f) * 1000.0f; // miliVolte
    //
    if (medium > maxmedium) maxmedium = medium; // save the value of highest medium in "maxmedium"

    Serial.print("medium: ");
    Serial.print(medium);
    Serial.print("  maxmedium: ");
    Serial.print(maxmedium);
    Serial.print(" Battery1 : ");
    Serial.print(myarray[6] / 204.6f);
    Serial.println(" V");



    if ( ((medium + dV) < maxmedium) && ((millis() / 60000.0f) >= 0.5f) ) { // if battery charged (average voltage is dropped 0.02v), but not in the firsts 11 mintues
      //Serial.println("average voltage is dropped 0.004v");
      Serial.println("battery 1 full");
      batt1Full = true;
      dV = 0.6;
      numCb1++;

    }


  }
}// end function voltread

void voltread2() {
  readingtemp2 = analogRead(analogInPin2);  // read analog input
  total2 = total2 + readingtemp2;
  index2++;
  Serial.print(" index2 = ");
  Serial.println( index2);
  // if numReadings reached, calculate the average
  if (index2 >= numReadings) {
    c2Zero = 0;
    index2 = 0;

    average2 = total2 / numReadings;
    total2 = 0;

    pusharray2(); // insert new average in array

    medium2 = (float)(myarray2[6] + myarray2[5] + myarray2[4] + myarray2[3] + myarray2[2] + myarray2[1] + myarray2[0]) / 7.0f + 0.5f; // calculate the average of the last 7 readings
    medium2 = (medium2 / 204.6f) * 1000.0f; // miliVolte

    if (medium2 > maxmedium2) maxmedium2 = medium2; // save the value of highest medium in "maxmedium"

    Serial.print("medium2: ");
    Serial.print(medium2);
    Serial.print("  maxmedium2: ");
    Serial.print(maxmedium2);
    Serial.print(" Battery2 : ");
    Serial.print(myarray2[6] / 204.6f);
    Serial.println(" V");


    if ( ((medium2 + dV) < maxmedium2) && ((millis() / 60000.0f) >= 0.5f) ) { // if battery charged (average voltage is dropped 0.02v), but not in the firsts 11 mintues
      //Serial.println("average voltage is dropped 0.004v");
      Serial.println("battery 2 full");
      batt2Full = true;
      dV = 0.6;
      numCb2++;

    }
  }
}


void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);
  pinMode(13, OUTPUT);

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  analogWrite(5, 0); // trun on lm2596
  analogWrite(6, 0); // trun on lm2596_2
  Serial.println("start work");
}

void loop() {
  currentMillis = millis();

  // executed every "interval" millis
  if (currentMillis - previousMillis > interval) {
    if (numCb1 == numCb2) {
      dV = 2;
    }
    previousMillis = currentMillis;
    outputValueC1 = analogRead(readCurrent1Pin);  // read analog input
    // outputValueC1 = (outputValueC1 / 204.6f) * 10000.0f;

    outputValueC2 = analogRead(readCurrent2Pin);  // read analog input
    //outputValueC2 = (outputValueC2 / 204.6) * 10000;

    //Serial.print("  current : ");
    //Serial.print(outputValueC1);
    //Serial.println(" mA ");
    if (outputValueC1 <= 1) {
      c1Zero++;
    }

    if (outputValueC1 > 0 && batt1Full == false) {
      Serial.print("        outputValueC1 : ");
      Serial.println( outputValueC1 );
      voltread();  // call reading and check volts function
      ledT1 = 1000;
    } else {
      if (c1Zero >= 5) {
        c1Zero = 0;
        Serial.println(" Current pin1 = 0 !!!!!!!!!!!!!!! ");
        index = 0;
        average = 0;
        medium = 0;
        maxmedium = 0;
        myarray[0] = 0;
        myarray[1] = 0;
        myarray[2] = 0;
        myarray[3] = 0;
        myarray[4] = 0;
        myarray[5] = 0;
        myarray[6] = 0;
        ledT1 = 2;
      }
    }

    if (outputValueC2 <= 1) {
      c2Zero++;
    }

    if (outputValueC2 > 0 && batt2Full == false) {
      Serial.print(" outputValueC2 :");
      Serial.println( outputValueC2);
      voltread2();  // call reading and check volts function
      ledT2 = 1000;
    } else {
      //c2Zero++;
      if (c2Zero >= 5) {
        c2Zero = 0;
        Serial.println(" Current pin2 = 0 >>>>>>>>>>>>>>>>> ");
        index2 = 0;
        average2 = 0;
        medium2 = 0;
        maxmedium2 = 0;
        myarray2[0] = 0;
        myarray2[1] = 0;
        myarray2[2] = 0;
        myarray2[3] = 0;
        myarray2[4] = 0;
        myarray2[5] = 0;
        myarray2[6] = 0;
        ledT2 = 2;
      }
    }


    if (batt1Full == true) {

      //digitalWrite(13, HIGH);
      analogWrite(5, 255); // cut off lm2596
      //interval = 995;
      //delay(10);

      int x = analogRead(analogInPin);
      if (x < 20 && batt1Out == false && outputValueC1 == 0 ) {//check batt full but in case


        batt1Out = true;
        Serial.println("battery1 in case has get out");
        ledT1 = 2;
      } else {
        ledT1 = 100;
      }

      if (x > 20 && batt1Out == true) { // new batt

        batt1Out = false;
        batt1Full = false;
        //dV = 2.5;

        Serial.println("found new battery 1");
        analogWrite(5, 0); // trun on lm2596
        digitalWrite(13, LOW);
      }
    }


    if (batt2Full == true) {

      //digitalWrite(13, HIGH);
      analogWrite(6, 255); // cut off lm2596
      //interval = 995;
      //delay(10);

      int x = analogRead(analogInPin2);
      if (x < 20 && batt2Out == false && outputValueC2 == 0 ) {//check batt full but in case


        batt2Out = true;
        Serial.println("battery2 in case  get has out");
        ledT2 = 2;
      } else {
        ledT2 = 100;
      }

      if (x > 20 && batt2Out == true) { // new batt

        batt2Out = false;
        batt2Full = false;
        //dV = 2.5;

        Serial.println("found new battery2");
        analogWrite(6, 0); // trun on lm2596
        digitalWrite(13, LOW);
      }
    }



  }
  ledStateBatt1();
  ledStateBatt2();

}
