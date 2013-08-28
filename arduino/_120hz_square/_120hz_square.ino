int cameraPin = 5;
int ledPin = 13;
int frequency = 120;

void setup() {
  pinMode(cameraPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int halfPeriod = 1e6 / frequency / 2;
  
  digitalWrite(cameraPin, HIGH);
  digitalWrite(ledPin, HIGH);    
  delayMicros(halfPeriod);
  
  digitalWrite(cameraPin, LOW);
  digitalWrite(ledPin, LOW);    
  delayMicros(halfPeriod);
}

void delayMicros(int duration) {
  delay(duration / 1000);
  delayMicroseconds(duration % 1000);
}
