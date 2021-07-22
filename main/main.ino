#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <string.h>

//Pre setup ethernet
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
EthernetClient client;
int    HTTP_PORT   = 80;
String HTTP_METHOD = "GET"; // or "POST"
char   HOST_NAME[] = "2ca7c3525874.ngrok.io"; 
String PATH_NAME   = "/api/teste/get";

// Led
int LED[3] = {0, 0, 0};
int SensorHL = A0;
int rele = 7;
int pinLedR = 11;
int pinLedG = 12;
int pinLedB = 13;

void setup ()
{
    Serial.begin(9600);
    pinMode(rele, OUTPUT);
    pinMode(pinLedR, OUTPUT);
    pinMode(pinLedG, OUTPUT);
    pinMode(pinLedB, OUTPUT);

    if (Ethernet.begin(mac) == 0) 
    {
        Serial.println("Failed to obtaining an IP address using DHCP");
        while(true);
    }
}

void loop()
{
    //post_server();
    get_server();
    delay(60000);
}

void post_server()
{
    client.stop();
    if(client.connect(HOST_NAME, HTTP_PORT)) {
    
        Serial.println("Connected to server post");

        String data_send;
        fill_data_send(&data_send);
        Serial.println(data_send);
        client.println("POST " + PATH_NAME + " HTTP/1.1");
        client.println("Host: " + String(HOST_NAME));
        client.println("Content-Type: application/json");
        client.println("Content-Length: " + String(data_send.length()));
        client.println("Connection: close");
        client.println(); // end HTTP header

        client.println(data_send);      
    }
}

void get_server()
{
    client.stop();
    if(client.connect(HOST_NAME, HTTP_PORT))
    {
        Serial.println("connected to server get");

        client.println("GET " + PATH_NAME + " HTTP/1.1");
        client.println("Host: " + String(HOST_NAME));
        client.println("Connection: close");
        client.println(); // end HTTP heade r
        
        String s = "";
        int i = 0;
        while(client.connected()) {
            if(client.available()){
                char c = client.read();
                if (c == '{') { i = 1; }
                if (c == '}') { i = 0; }
                if (i == 1)   { s += c; }
            }     
        }
        s += '}';
        Serial.println(s);
        StaticJsonDocument<200> doc;
        deserializeJson(doc, s);
        int bomba = doc["bomb"];
        int ledR = doc["ledR"];
        int ledG = doc["ledG"];
        int ledB = doc["ledB"];
        Serial.println("Bomba" + String(bomba));
        Serial.println("LedR :" + String(ledR));
        Serial.println("LedG :" + String(ledG));
        Serial.println("LedB :" + String(ledB));
        
        
    }
}

void bomba(int milisec)
{
    digitalWrite(rele, HIGH);
    delay(milisec);
    digitalWrite(rele, LOW);
} 

void ledRGB()

void fill_data_send(String *data_send)
{
    //valores led teste, por coisa global?
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
