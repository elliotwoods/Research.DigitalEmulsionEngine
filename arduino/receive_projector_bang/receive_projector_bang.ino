int ledPin = 13;
int projectorPin = 2;

volatile unsigned long lastTimestamp = 0;
volatile long interval = 0;

volatile float fps = 0.0f;

void setup() {
  Serial.begin(115200);
  
  pinMode(projectorPin, INPUT);
  digitalWrite(projectorPin, LOW); // turn off pull up resistor
  attachInterrupt(0, onProjectorRisingEdge, FALLING);
  
  pinMode(ledPin, OUTPUT);
}

void loop() {  
  delay(10);
}

void onProjectorRisingEdge() {    
  unsigned long currentTimestamp = micros();
  interval = currentTimestamp - lastTimestamp;
  lastTimestamp = currentTimestamp;
  
  fps = (float) 1e6  / (float) interval; 
  
  Serial.println(fps);
}

void shootCamera() {
    digitalWrite(cameraPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(1);
    digitalWrite(cameraPin, LOW);
    digitalWrite(ledPin, LOW);
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
