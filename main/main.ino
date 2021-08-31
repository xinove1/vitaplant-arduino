#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <string.h>

//Pre setup ethernet
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
EthernetClient client;
int    HTTP_PORT   = 80;
String HTTP_METHOD = "GET"; // or "POST"
char   HOST_NAME[] = "08df-2001-1284-f016-3b60-6494-80d4-dc64-7b53.ngrok.io";
String PATH_NAME   = "/api/teste/get";

// Led
int LED[3] = {0, 0, 0};
int SensorHL = A0;
int rele = 7;
int pinLedR = 8;
int pinLedG = 9;
int pinLedB = 10;

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
    if(post_server())
        get_server();
    delay(60000);
}

int post_server()
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
        String c ;
        while(client.connected()) 
        {
            if(client.available())
            {
                // read an incoming byte from the server and print it to serial monitor:
                char ca = client.read();
                c += ca;
            }
        }
		Serial.println("|" + parse_http(c) + "|");
         if(parse_http(c) == "201")
             return(1);
         else
            return(0);
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
        Serial.println("string get: " + s);
        StaticJsonDocument<200> doc;
        deserializeJson(doc, s);
        int bomba = doc["bomb"];
        LED[0] = doc["ledR"];
        LED[1] = doc["ledG"];
        LED[2] = doc["ledB"];
        Serial.println("---------------------------------");

        bomba_ligar(bomba);
        led_liga();
        
    }
}

void bomba_ligar(int milisec)
{
    Serial.println("Bomba" + String(milisec));
    digitalWrite(rele, HIGH);
    delay(milisec);
    digitalWrite(rele, LOW);
} 

void led_liga(void)
{
    Serial.println("LedR :" + String(LED[0]));
    Serial.println("LedG :" + String(LED[1]));
    Serial.println("LedB :" + String(LED[2]));
    analogWrite(pinLedR, LED[0]);
    analogWrite(pinLedG, LED[1]);
    analogWrite(pinLedB, LED[2]);

}

void fill_data_send(String *data_send)
{
	char buffer[3];
    *data_send = String("{\"ledR\":")
               + itoa(LED[0], buffer, 10)
               +",\"ledG\":" 
               + itoa(LED[1], buffer, 10)
               +",\"ledB\":"
               + itoa(LED[2], buffer, 10)
               + ",\"humidity\":"
               + String(analogRead(SensorHL))
               + "}";
}

String    parse_http(String c)
{
    int i = 0;
    String dest;
    while (c[i] != '\n')
    {
        if (c[i] == ' ')
        {
			dest += c[i + 1];
			dest += c[i + 2];
			dest += c[i + 3];
			return (dest);
        }
        i++;
    }
    return ("No");
}
