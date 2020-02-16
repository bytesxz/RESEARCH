/* RESEARCHHHHHHH
Button:
  - pushbutton attached to pin 12 from +5V 
  - 10K/1K resistor attached to pin 12 from ground
Motor:
 - motorspeed takes in a byte value, which ranges from 0 to 255. currently set to 128.
Sensor:
  -
*/
//DSM
int parM1 = 11; // for 1um particles, pin 8 is a bitch (not working/code wasn't working.)
int parM2 = 9; // for 2.5um particles
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
//Motor + motor control
const byte PIN_IN1 = 2;
const byte PIN_IN2 = 3;
const byte PIN_IN3 = 4;
const byte PIN_IN4 = 5;
const byte PIN_EN1 = 6; // not gonna be used, unless roller will also have variable speed. jumper placed on pins for constant speed
const byte PIN_EN2 = 7;
//switch
const int bPin = 12;
const int ledPin = 13; //temporary, only for diag
int buttonstate = 0;
int lstate = 0; 
int counter = 0;

//declare functions
unsigned long sizeOne() { 
  duration = pulseIn(parM1, LOW);
  LPulseOc = LPulseOc + duration;
  if ((millis()-starttime) >= sampletime_ms) 
  {
    ratio25 = LPulseOc/(sampletime_ms*10.0);  
    concentration = 1.1*pow(ratio25,3)-3.8*pow(ratio25,2)+520*ratio25+0.62;
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
  return concentration25;
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

unsigned long getaverage(int size, int limit) {// note that size can only be 1 or 2, since other values will just be ignored by this function
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
    Serial.println("getting baseline values. Please wait");
  }
  averagevalue = averagevalue / limit;
  return averagevalue;
}

void speedSetting(byte val) { //sets speed for motor
  analogWrite(PIN_EN2, val); 
}

void start_motors() {
  //move motor 1
  digitalWrite(PIN_IN1, LOW); // PIN_IN1 low, PIN_IN2 HIGH = reverse direction
  digitalWrite(PIN_IN2, HIGH);
  //move motor 2 (vac)
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
}

void vac_motor() {
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
}

void roller_motor() {
  digitalWrite(PIN_IN1, LOW); 
  digitalWrite(PIN_IN2, HIGH);
}

void vac_off() {
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
}

void stop_motors() {
  digitalWrite(PIN_IN1,LOW);
  digitalWrite(PIN_IN2,LOW);
  digitalWrite(PIN_IN3,LOW);
  digitalWrite(PIN_IN4,LOW);  
}

void reverse_motors() {
  digitalWrite(PIN_IN1, HIGH); 
  digitalWrite(PIN_IN2, LOW);
  //move motor 2 (vac)
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
}

void trackbuttonalt(int peen) { //diag, will show lstate and bstate in serial
  buttonstate = digitalRead(peen);
  digitalWrite(ledPin, buttonstate);
  lstate = buttonstate;
  Serial.println("lstate: ");
  Serial.println(lstate);
  Serial.println("bstate");
  Serial.println(buttonstate);
}

void trackbutton(int peen) { //diag, will show lstate and bstate in serial
  buttonstate = digitalRead(peen);
  digitalWrite(ledPin, buttonstate);
  lstate = buttonstate;
}


void novarM(int button) {
  trackbuttonalt(button);
  if (lstate == 1) {
    start_motors();
  }
  else {
    stop_motors();
  }
}

void withvarM(int button) { // run this AFTER readings are being taken (if taking the average is the opted way to do it)
  trackbutton(button);
  currentConcentration = sizeTwoFive();
  if (lstate == 1) {
    roller_motor();
   
    if (currentConcentration >= 1000) {
      speedSetting(255);
      vac_motor();
    }
    else {
      speedSetting(128);
      vac_off();
    }

    
  }
  else {
    stop_motors();
  }
}

void test() { // use this function to test if switch works. it does :))
  novarM(bPin);
}

void motortest() {

}

void motorsweep() {
  speedSetting(1);
  delay(2000);
  

  for (int i = 0; i <= 255; i= i+10) {
    speedSetting(i);
    vac_motor();
    delay(1000);
    stop_motors();
    delay(1000);
    Serial.println(i);
  }
  

}

//run
void setup() {
  Serial.begin(9600);
  pinMode(parM1, INPUT);
  pinMode(parM2, INPUT);
  pinMode(PIN_IN1,OUTPUT);
  pinMode(PIN_IN2,OUTPUT);
  pinMode(PIN_IN3,OUTPUT);
  pinMode(PIN_IN4,OUTPUT);
  pinMode(PIN_EN1,OUTPUT);
  pinMode(PIN_EN2,OUTPUT);
  starttime = millis(); //start to read time in ms
  pinMode(bPin, INPUT); 
  pinMode(ledPin, OUTPUT);
  speedSetting(128); // initial speed for vac motor upon startup
  
}

void loop() { 
  withvarM(bPin);

}