#include <SPI.h>
#include <ESP8266WiFi.h>

byte ledPin = 16;
char ssid[] = "Maddy's Phone";               // SSID of your home WiFi
char pass[] = "1qaz2wsx";               // password of your home WiFi
WiFiServer server(80);                    

IPAddress ip(192, 168, 0, 80);            // IP address of the server
IPAddress gateway(192,168,0,1);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network

boolean isOff = false;

void setup() {
  Serial.begin(115200);                   // only for debug
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  server.begin();                         // starts the server
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, isOff);
}

void loop () {
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      Serial.println(".");
      String request = client.readStringUntil('\r');    // receives the message from the client
      Serial.print("From client: "); Serial.println(request);
      client.flush();
      if(request.indexOf('l')>=0){
      isOff = !isOff;
      digitalWrite(ledPin, isOff);
      }
    }
    client.stop();                // tarminates the connection with the client
  }
}