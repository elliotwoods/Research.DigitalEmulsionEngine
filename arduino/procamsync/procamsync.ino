int ledPin = 13;
int projectorPin = 2;
int cameraPin = 5;
int potentiometerPin = 0;

volatile unsigned long lastTimestamp = 0;
volatile long interval = 0;

volatile float potentiometerValue = 0.0f;
volatile unsigned long phaseDelay = 0;

volatile unsigned long nextTrigger = 0;
volatile bool toTrigger = false;

volatile float fps = 0.0f;

void setup() {
  Serial.begin(115200);
  
  pinMode(projectorPin, INPUT);
  digitalWrite(projectorPin, LOW); // turn off pull up resistor
  attachInterrupt(0, onProjectorRisingEdge, FALLING);
  
  pinMode(ledPin, OUTPUT);
  pinMode(cameraPin, OUTPUT);
}

void loop() {  
  fps = (float) 1e6  / (float) interval; 
  
  //--
  //Update delay
  //
  int rawPotentiometerValue = analogRead(potentiometerPin);
  potentiometerValue = (float) rawPotentiometerValue / (float) 1023;
  phaseDelay = potentiometerValue * (float) interval;
  //
  //--
  
  //--
  //Check timing
  //
  unsigned long currentTime = micros();
  if (currentTime >= nextTrigger && toTrigger) {
    shootCamera();
    Serial.println(fps);
    toTrigger = false;
  }
  //
  //--
}

void onProjectorRisingEdge() {    
  unsigned long currentTimestamp = micros();
  interval = currentTimestamp - lastTimestamp;
  lastTimestamp = currentTimestamp;
   
  nextTrigger = currentTimestamp + phaseDelay;
  toTrigger = true;
  
  //Serial.println("trigger");
}

void shootCamera() {
    digitalWrite(cameraPin, HIGH);
    //digitalWrite(ledPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(cameraPin, LOW);
    //digitalWrite(ledPin, LOW);
}

void printStatus() {
  Serial.print("Fps = ");
  Serial.print(fps);
  Serial.print("Hz \t");
  
  Serial.print("Pot value = ");
  Serial.print(potentiometerValue);
  Serial.print("\t (");
  Serial.print(phaseDelay);
  Serial.print("us)");
  
  Serial.println("");
}
