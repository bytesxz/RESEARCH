//set what the pins will do 

int led0 = 13;
int led = 12;
int led1 = 11;


void setup() //runs once, runs if reset is pressed
{
	//init a digital pin to be an output:
    pinMode(led, OUTPUT);
    pinMode(led1, OUTPUT);
    pinMode(led0, OUTPUT);
}

void loop()//self explanatory
{
	digitalWrite(led, HIGH);
    digitalWrite(led1, LOW);
    digitalWrite(led0, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    digitalWrite(led1, HIGH);
    digitalWrite(led0, LOW);
    delay(1000);
}


//read serial
// digital pin 2 has a pushbutton attached to it. Give it a name:
int pushButton = 2;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  int buttonState = digitalRead(pushButton);
  // print out the state of the button:
  Serial.println(buttonState);
  delay(1);        // delay in between reads for stability
}    


//read from a potentiometer/ 
void setup() {
    //init communication @ 9600bps
    Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  //read input on pin A0
  int sensorValue = analogRead(A0);
  //return to serial what sensor shows
  Serial.println(sensorValue);
  delay(1); //apparently added for "stability"
}  


//idk but yes

//pwm from pot value

const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to
const int analogOutPin0 = 10;

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  // change the analog out value:
  analogWrite(analogOutPin, outputValue);
  analogWrite(analogOutPin0, outputValue);

  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
}

//tryna make the sensor werk
/*

list of shit I had to do to make this work:
1) declare what pins are going to be used
2) create unsigned variables that can store a lot of data
3) create float/doubles = ratio of particles, concentration in air
4) 
*/


//declare vars
int pin = 8;
unsigned long duration, startime;
unsigned long sampletime_ms = 2000; // start sampling, send to serial every 2s.
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
//fuck this

void wait() { //intended for sens' resistor to start heating up, thus moving air.
  Serial.println("wait 60s for sensor warmup.")
  sleep(6000)
}


//yes
void setup() {
  Serial.begin(9600);//establish serial at 9600 bps :)) ./.
  pinMode(pin, INPUT); // let pin 8 act as an input
}

void loop() {
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;
  if ((millis() - startime) >= sampletime_ms) {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; //what the hecc
    Serial.print("Concentration = ");
    Serial.print(concentration);
    Serial.println(" pcs/0.01cf");
    Serial.println("\n");
    lowpulseoccupancy = 0;
    startime = millis();
  }
}