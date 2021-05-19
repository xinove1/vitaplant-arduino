int rele = 7;

void setup() {
  pinMode(rele, OUTPUT);
}

void loop() {
  digitalWrite(7, HIGH);
  delay(2000);
  digitalWrite(7, LOW);
  delay(2000);
}
