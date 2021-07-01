#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <string.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
int SensorHL = A0;
int rele = 7;

EthernetClient client;

int    HTTP_PORT   = 80;
String HTTP_METHOD = "GET"; // or "POST"
char   HOST_NAME[] = "a3897fe14b76.ngrok.io"; // hostname of web server:
String PATH_NAME   = "/api/teste/get";

int LED = 0;

void setup() {
  Serial.begin(9600);
  pinMode(rele, OUTPUT);
    
  // initialize the Ethernet shield using DHCP:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to obtaining an IP address using DHCP");
    while(true);
  }

}

void loop() {
  String data_send;
  fill_data_send(&data_send);
  Serial.println(data_send);
  delay(60000);
  /*if(send_data())
    receive_data();
*/
}


int send_data(void)
{
    if(client.connect(HOST_NAME, HTTP_PORT)) {
      
      // if connected:
      Serial.println("Connected to server");
      
      // make a HTTP request:
      // send HTTP header
      client.println(HTTP_METHOD + " " + PATH_NAME + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Connection: close");
      client.println(); // end HTTP header


      String data_send;
      fill_data_send(&data_send);
      client.println(data_send);      
      
    }
  
  return(0);
}

void receive_data(void)
{
  
}

void fill_data_send(String *data_send)
{
  //StaticJsonDocument<256> doc;
  //doc["led"] = LED;
  //doc["humidity"] = analogRead(SensorHL);
  //serializeJson(doc, data_send);
  *data_send = String("{\"led\":") + String(LED) + String(",\"humidity\":") + String(analogRead(SensorHL))+ String("}");
}