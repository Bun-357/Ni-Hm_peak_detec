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
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
float outputValue = 0;        // value output to the PWM (analog out)
float OoutputValue = 0, varB = 0, varNow = 0;
float capHigh = 0;
int count = 0, cH = 0;


const int numReadings = 30; // number of analog read before checking battery status
const int multi = 1614; // multi coefficent for obtaining millivolts from analogread

long interval = 1000;     // interval for pulse charging and analog read - don't change this
long interval2 = 250;     // pulse off interval - you can adjust power with this. Use 100 for 2-4Ah battery packs, 500 for 1-2Ah battery pack
long interval2b = interval2;
long previousMillis, currentMillis, currentMillis2, trickleMillis = 0;

unsigned int readingtemp;
unsigned int total = 0;
 float average, medium, maxmedium = 0;

boolean executed, endcharge, trickle = false; // booleans for controlling various activities (for example "end of charge")

unsigned int myarray [7];   // array for keeping last 7 readings
int index, i = 0;




void pusharray() {
  // push the array
  for (i = 0; i <= 5; ++i) {
    myarray[i] = myarray[i + 1];
  }
  myarray[6] = average;
}


void voltread() {
  readingtemp = analogRead(analogInPin);  // read analog input
  total = total + readingtemp;
  index++;

  // if numReadings reached, calculate the average
  if (index == numReadings) {
    index = 0;
    //average = (((float)total / 1023.0) * (float)multi) / numReadings + 0.5;
    average = total / numReadings;
    // if (average<=70) endcharge=true;  // stop charge if battery is detached
    total = 0;
    pusharray(); // insert new average in array
    //
    medium = (float)(myarray[6] + myarray[5] + myarray[4] + myarray[3] + myarray[2] + myarray[1] + myarray[0]) / 7.0 + 0.5; // calculate the average of the last 7 readings
    medium = (medium/204.6)*1000;// miliVolte
    if (medium > maxmedium) maxmedium = medium; // save the value of highest medium in "maxmedium"
    Serial.print(medium);
    Serial.print(",");
    Serial.print(maxmedium);
    Serial.print(",");
    Serial.println(myarray[6]);
    if ( ((medium + 1.5) < maxmedium) && ((millis() / 60000) >= 0.5) ) { // if battery charged (average voltage is dropped 0.02v), but not in the firsts 11 mintues
      Serial.println("average voltage is dropped 0.015v");
      
      while(true){
        digitalWrite(13,HIGH);
      }
      
    }
  }
}


void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  currentMillis = millis();

  // executed every "interval" millis
  if (currentMillis - previousMillis > interval) {
    voltread();  // call reading and check volts function
    //digitalWrite(outputPin,LOW);  // temporaly stop charging
    previousMillis = currentMillis;
    executed = false; // boolean for setting and checking if has been yet turned ON charge

    //    // in the firsts 10 minutes and in the endings 15 minutes do a trickle charge (change OFF interval)
    //    if ( ( (trickle) && (((millis()-trickleMillis)/60000)<15) ) || ((millis()/60000)<10) ) {
    //      interval2=(interval-(interval-interval2b)/5);
    //    } else if ((millis()/60000)>=10) interval2=interval2b;  // after initial trickle charghe set back right time
    //    if ( (trickle) && (((millis()-trickleMillis)/60000)>=15) ) endcharge=true;  // if final trickle charge end, end charge
    //  }
    //
    //  currentMillis2 = millis();
    //
    //  // executed "interval2" millis after turning OFF charge
    //  if ((currentMillis2 - previousMillis > interval2) && (!executed)) {
    //    executed=true;
    //    if (!endcharge) {
    //      digitalWrite(outputPin,HIGH); // if battery is not charged, re-enable charging
    //    }
  }

}
