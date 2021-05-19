#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoJson.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;

int    HTTP_PORT   = 80;
String HTTP_METHOD = "GET"; // or "POST"
char   HOST_NAME[] = "c0d745a96603.ngrok.io"; // hostname of web server:
String PATH_NAME   = "/api/teste/get";

struct Data {
    int bomba;
    int led;
};

void setup() {
  Serial.begin(9600);

  // initialize the Ethernet shield using DHCP:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to obtaining an IP address using DHCP");
    while(true);
  }

  // connect to web server on port 80:
  if(client.connect(HOST_NAME, HTTP_PORT)) {
    // if connected:
    Serial.println("Connected to server");
    // make a HTTP request:
    // send HTTP header
    client.println(HTTP_METHOD + " " + PATH_NAME + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header

    Data DadosServer;

    while(client.connected()) {
      if(client.available()){
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        StaticJsonDocument<256> doc;
        deserializeJson(doc, c);
        DadosServer.bomba = doc["bomba"];
        DadosServer.led = doc["led"];

        Serial.print(DadosServer.bomba);
        Serial.print(DadosServer.led);
      }
    }

    // the server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  } else {// if not connected:
    Serial.println("connection failed");
  }
}

void loop() {

}

void json(){
  StaticJsonDocument<256> doc;

}
