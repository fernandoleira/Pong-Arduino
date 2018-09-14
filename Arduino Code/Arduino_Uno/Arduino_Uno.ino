// Pins for accelerometer
const int xPin = A0;
const int yPin = A1;
const int zPin = A2;

char readed;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int xP = analogRead(xPin);
  
  int yP = analogRead(yPin);
  if (yP < 265) yP = 265;
  else if(yP > 395) yP = 395;
  int yV = map(yP, 265, 395, 4200, -3500);
  
  int zP = analogRead(zPin);

  String message = String(yV) + "," + String(xP) + "," + String(zP);
  
  if (Serial.available() > 0){
    readed = Serial.read();
    // Send command to Serial
    if (readed == 's'){
      Serial.println(message);
    }
  }

  //Serial.println(message);
}
