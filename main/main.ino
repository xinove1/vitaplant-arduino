#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <string.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
int SensorHL = A0;
int rele = 7;

EthernetClient client;
/*
struct dataReceive {
  int bomb;
  int led;
};*/

int    HTTP_PORT   = 80;
String HTTP_METHOD = "POST"; // or "POST"
char   HOST_NAME[] = "2ca7c3525874.ngrok.io"; // hostname of web server:
String PATH_NAME   = "/api/teste/get";

int LED[3] = {0, 0, 0};

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

  String httpRes = send_data();
  String httpResParsed = parse_http(httpRes);
  Serial.println("httpRes:" + httpRes);
  Serial.println("httpRes parsed:" + httpResParsed);
  
  if (httpResParsed == "201")
  {
     Serial.println("inside if parsed:" + httpResParsed);
    receive_data();
  }
  delay(60000);
}


String send_data(void)
{
  
    if(client.connect(HOST_NAME, HTTP_PORT)) {
      
      // if connected:
      Serial.println("Connected to server");

      //Cria e empacta dados para envio
      String data_send;
      fill_data_send(&data_send);

      // make a HTTP request:
      // send HTTP header
      client.println("POST " + PATH_NAME + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Content-Type: application/json");
      client.println("Content-Length: " + String(data_send.length()));
      client.println("Connection: close");
      client.println(); // end HTTP header

      //Serial.println(data_send);
      client.println(data_send);      
      
      String c ;
      while(client.connected()) {
        if(client.available()){
          // read an incoming byte from the server and print it to serial monitor:
          char ca = client.read();
          c += ca;
        }
      }
     // Serial.println("c:" + c);
      return(c);
      //Serial.println("sexo2.0:" + parse_http(c) + "a");
    }
}

void receive_data(void)
{
  Serial.println("A R D U I N O");
  if(client.connect(HOST_NAME, HTTP_PORT)) {
      
      // if connected:
      //Serial.println("Connected to server");

      // make a HTTP request:
      // send HTTP header
      client.println("GET " + PATH_NAME + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Connection: close");
      client.println(); // end HTTP header

      String c;    
      while(client.connected()) {
        if(client.available()){
          // read an incoming byte from the server and print it to serial monitor:
          char ca = client.read();
          c += ca;
        }
      }

    
    //Serial.println(c.substring(a, b+1));
   // Serial.println("\n" + c);
    StaticJsonDocument<1000> doc;
    deserializeJson(doc, c);
    Serial.println(c);
    int bomb = doc["bomb"];
    int led = doc["led"];
    //Serial.println("\n 2:" + c + "\n");

    
    Serial.println(String(bomb));
    Serial.println(String(led));
    
    // the server's disconnected, stop the client:
    client.stop();
    Serial.println("disconnected");
  } else {// if not connected:
    Serial.println("connection failed");
  }
}

void fill_data_send(String *data_send)
{
  //StaticJsonDocument<256> doc;
  //doc["led"] = LED;
  //doc["humidity"] = analogRead(SensorHL);
  //serializeJson(doc, data_send);
  LED[0] = 10;
  LED[1] = 20;
  LED[2] = 30;
  *data_send = String("{\"ledR\":") 
               + String(LED[0]) 
               +",\"ledG\":" 
               + String(LED[1])
               +",\"ledB\":" 
               + String(LED[2])
               + ",\"humidity\":"
               + String(analogRead(SensorHL))
               + "}";
}

String    parse_http(String c)
{
    int beg = 0;
    int i = 0;
    int j = 0;
    String dest[4];
    while (c[i] != '\n')
    {
        if (c[i] == ' ')
        {
            c[i] = '\0';
            dest[j] = fill(&c[beg]);
            j++; 
            i = i + 1;
            beg = i;
            continue;
        }
        i++;
    }
    return(dest[1]);
}

String    fill(char *c)
{
    //char cc[20];
    int i = 0;
    char dest[3];
    while (c[i] && i < 3)
    {
        dest[i] = c[i];
        i++;
    }
    return (dest);
}
