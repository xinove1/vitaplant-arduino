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


void setup ()
{
    Serial.begin(9600);

    if (Ethernet.begin(mac) == 0) 
    {
        Serial.println("Failed to obtaining an IP address using DHCP");
        while(true);
    }
}

void loop()
{
    post_server();
    delay(60000);
}

void post_server()
{
    client.stop();
    if(client.connect(HOST_NAME, HTTP_PORT)) {
    
        Serial.println("Connected to server");

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
