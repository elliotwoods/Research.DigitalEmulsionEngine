int ledPin = 13;
int projectorPin = 2;
int cameraPin = 7;
int potentiometerPin = 0;

volatile unsigned long lastTimestamp = 0;
volatile unsigned long interval = 0;

volatile float potentiometerValue = 0.0f;
volatile unsigned long phaseDelay = 0;

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
  
  if (interval == 0) {
    Serial.println("Interval = 0");
    delay(100);
    return;
  }
  
  //--
  //Update delay
  //
  potentiometerValue = (float) analogRead(potentiometerPin) / (float) 1023 - 0.5f;
  long phaseDelaySigned = potentiometerValue * (float) interval;
  if (phaseDelaySigned < 0) {
    phaseDelay = phaseDelaySigned + interval;
  } else {
    phaseDelay = phaseDelaySigned;
  }
  //
  //--
  
  //--
  //Check if we need to set high
  unsigned long timeMicros = micros();
  if (micros() % interval > (phaseDelay + lastTimestamp) % interval) {
    digitalWrite(cameraPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(20);
    digitalWrite(cameraPin, LOW);
    digitalWrite(ledPin, LOW);
    
    printStatus();
    /*
    char message[200];
    sprintf(message, "Trigger at %uus \t Phase delay = %d / 100 (%uus) \t fps = %dmHz", micros(), (int) (potentiometerValue * 100.0f), phaseDelay, (int) (fps * 1000.0f));
    Serial.println(message);
    Serial.println(fps);
    */
  }
  //
  //--
}

void onProjectorRisingEdge() {
  unsigned long currentTimestamp = micros();
  interval = currentTimestamp - lastTimestamp;
  lastTimestamp = currentTimestamp;
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
