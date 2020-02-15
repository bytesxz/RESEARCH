/* RESEARCHHHHHHH
Button:
  - pushbutton attached to pin 12 from +5V 
  - 10K/1K resistor attached to pin 12 from ground
Motor:
  
Sensor:
  -
*/
//DSM
int parM1 = 11; // for 1um particles, pin 8 is a bitch
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
const byte PIN_EN1 = 6; // not gonna be used, unless roller will also have variable speed.
const byte PIN_EN2 = 7;
//switch
int bPin = 12;
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

int trackbutton(int BUTTON_PIN) {// will loop until it detects a button press, idk if this is the best approach
  int run = 0;
  buttonstate = digitalRead(BUTTON_PIN);

  if (buttonstate != lstate) {
    //if state change, do shit
    if (buttonstate == HIGH) {
      counter++;
      Serial.println("on");
      run = 1;
    }
    else {
      // if cstate is LOW, then it's not pressed
      Serial.println("off");
      run = 0;
    }
    delay(50); // so that it doesn't bounce, whatever that is
    lstate = buttonstate;
  }

  return run;
}

void trackbuttonalt(int peen) { // this needs to be fixed.
    // read the pushbutton input pin:
  buttonstate = digitalRead(peen);

  // compare the buttonState to its previous state
  if (buttonstate != lstate) {
    // if the state has changed, increment the counter
    if (buttonstate == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      counter++;
      Serial.println("on");
      Serial.print("number of button pushes:  ");
      Serial.println(counter);
    } else {
      // if the current state is LOW then the button
      // wend from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state,
  //for next time through the loop
  lstate= buttonstate;
}

void speedSetting(byte val) { //sets speed for motor
  analogWrite(PIN_EN2, val); 
}

void start_motors() {
  //move motor 1
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  //move motor 2 (vac)
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
}

void stop_motors() {
  digitalWrite(PIN_IN1,LOW);
  digitalWrite(PIN_IN2,LOW);
  digitalWrite(PIN_IN3,LOW);
  digitalWrite(PIN_IN4,LOW);  
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
  speedSetting(128);
  
}

void loop() {
  
}