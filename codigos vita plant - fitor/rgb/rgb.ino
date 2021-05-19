int red = 11;
int green = 12;
int blue = 13;

int valr = 0;
int valg = 0;
int valb = 0;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
}

void loop() {
  digitalWrite(red, valr);
  digitalWrite(green, valg);
  digitalWrite(blue, valb);
}
