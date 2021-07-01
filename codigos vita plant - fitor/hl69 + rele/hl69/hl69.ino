int SensorHL = A0;
int valor = 0;
int rele = 7;
  
void setup() {
  Serial.begin(9600);
  pinMode(rele, OUTPUT);
}

void loop() {
  valor = analogRead(SensorHL);
  Serial.print("Nivel de umidade: ");
  Serial.println(valor);
  
  if(valor > 600) {
    Serial.println("Seco, ligando bomba...");
    digitalWrite(rele, HIGH);
  } else {
        Serial.println("Umido, desligando...");
        digitalWrite(rele, LOW);
    }
    
  delay(2000);
}
