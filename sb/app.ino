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