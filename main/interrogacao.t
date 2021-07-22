#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <string.h>



byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
int SensorHL = A0;
int rele = 7;

struct Data {
    int bomba;
    int led;
};
EthernetClient client;

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

  
  if(send_data())
    receive_data();
  delay(60000);
}


int send_data(void)
{
    if(client.connect(HOST_NAME, HTTP_PORT)) {
      
      // if connected:
      Serial.println("Connected to server");

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
        int i = 0;
        if(client.available() && i < 10){
          // read an incoming byte from the server and print it to serial monitor:
          char ca = client.read();
          c += ca;
          i++;
        }
      }
      
      Serial.println("sexo2.0:" + parse_http(c));
      if (parse_http(c) == "201")
      {
        Serial.println("201");
        return(1);
      }
    }
  Serial.println("200");
  return(0);
}

void receive_data(void)
{
  if(client.connect(HOST_NAME, HTTP_PORT)) {
      
      // if connected:
      Serial.println("Connected to server");

      String data_send;
      fill_data_send(&data_send);
      // make a HTTP request:
      // send HTTP header
      client.println("GET " + PATH_NAME + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Content-Type: application/json");
      client.println("Content-Length: " + String(data_send.length()));
      client.println("Connection: close");
      client.println(); // end HTTP header

      //Serial.println(data_send);
      client.println(data_send);      
      String c ;
    
      while(client.connected()) {
        int i = 0;
        if(client.available() && i < 10){
          // read an incoming byte from the server and print it to serial monitor:
          char ca = client.read();
          c += ca;
          i++;
        }
      }
      
      Serial.println("sexo5.0:" + parse_http(c));
    }
}

void bomba(int mili)
{
  digitalWrite(rele, HIGH);
  delay(mili);
  digitalWrite(rele, LOW);
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
    char *dest[4];
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
    return(String(dest[1]));
}

char    *fill(char *c)
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
