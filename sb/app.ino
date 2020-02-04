/* RESEARCHHHHHHH
Button:
  - pushbutton attached to pin 22 from +3V (5v already used for DSM)
  - 10K resistor attached to pin 22 from ground
Motor:
  - vacuum motor connected to d12
  - roller/eraser to d13
Sensor:
  -
*/
//DSM
int parM1 = 8;
int parM2 = 9;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 5000; // rate @ which the values will be measured and given.
unsigned long LPulseOc = 0;
float ratio = 0;
float concentration = 0;
float ratio25 = 0;
float concentration25 = 0;
unsigned long concenOT25 = 0;
unsigned long currentConcentration = 0;
//Motor
int vacMotor = 12; // controlled by pwm output, based on DSM readings
int eRoller = 13; // just 1
int outputvalue
//switch
int bPin = 2;
int buttonstate = 0;

//declare functions
unsigned long sizeOne() { // unsigned long functions... what the fuck
  duration = pulseIn(parM1, LOW);
  LPulseOc = LPulseOc + duration;
  if ((millis()-starttime) >= sampletime_ms) //if the sample time is less than 5s
  {
    ratio = LPulseOc/(sampletime_ms*10.0);  
    Serial.print("Concentration = ");
    Serial.print(concentration);
    Serial.println(" pcs/0.01cf");
    Serial.println("\n"); 
    LPulseOc = 0;
    starttime = millis();
  }
  return concentration;
}

unsigned long sizeTwoFive() { // get 2.5 
  duration = pulseIn(parM2, LOW);
  LPulseOc = LPulseOc + duration;
  if ((millis()-starttime) >= sampletime_ms) 
  {
    ratio25 = LPulseOc/(sampletime_ms*10.0);  
    concentration25 = 1.1*pow(ratio25,3)-3.8*pow(ratio25,2)+520*ratio25+0.62;
    Serial.print("Concentration25 = ");
    Serial.print(concentration25);
    Serial.println(" pcs/0.025cf");
    Serial.println("\n"); 
    LPulseOc = 0;
    starttime = millis();
  }
  return concentration;
}


void wait(int timew) {
  for (int j = 0; j <= 29; j++) { 
      sizeTwoFive();
      Serial.print("waiting " );
      Serial.print(timew-j);
      Serial.println("s for sensor warmup");
      delay(1000);
    }
}

unsigned long getaverage(int size, int limit) {
  unsigned long baselinevalue [limit];
  unsigned long averagevalue = 0;
  if (size == 1) {
    for (int i = 0; i <= limit; i++ ) {
      baselinevalue [i] = sizeOne();
    }
  }
  else if (size == 2) {
    for (int i = 0; i <= limit; i++) {
      baselinevalue [i] = sizeTwoFive();
    }
  }
  for (int j = 0; j <= limit; j++) {
    averagevalue = averagevalue + baselinevalue [j];
  }
  averagevalue = averagevalue / limit;
  return averagevalue;
}

int trackbutton(int BUTTON_PIN) {// will loop until it detects a button press, idk if this is the best approach
  int run = 0;
  buttonstate = digitalRead(BUTTON_PIN); //read button state
  if (buttonstate == HIGH) {
    run = 1;
  }
  else {
    run = 0;
  }

  return run;
}

void start() {
 int state = trackbutton(bPin);
 if (state == 1) {
   Serial.println("1");
   digitalWrite(eRoller, HIGH); // turns on roller motor
   
 }
 else {
   Serial.println("0");
 }
}



//run
void setup() {
  Serial.begin(9600);
  pinMode(parM1, INPUT);
  pinMode(parM2, INPUT);
  starttime = millis(); //start to read time in ms
  pinMode(bPin, INPUT); // state button pin as an input
}

void loop() {
 // concenOT25 = getaverage(2.5, 10); // gets baseline dust amount.
  start();


}