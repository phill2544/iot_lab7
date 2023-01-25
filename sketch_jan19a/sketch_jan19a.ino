
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <iostream>
#include <string.h>
#include <string> 

ESP8266WebServer server;
uint8_t pin_led = 2;
char* ssid = "phill";
char* password = "00000000";
int sw = 12;
int status_led;
//const char* host = "http://192.168.8.101";
//String get_host = "http://192.168.8.101";
//char* host = "172.20.10.3";
//char* host = "172.24.135.150";
char* host = "172.20.10.3" ;
WiFiClient client;
int httpPort = 3030;
void setup()
{
  SPIFFS.begin();
  pinMode(pin_led, OUTPUT);
  pinMode(sw, INPUT);
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", serveIndexFile);
  server.on("/toggle_led.html", serveToggleLedFile);
  server.on("/ledstate", getLEDState);
  server.on("/ledstate/:status", getLEDState);
  server.begin();



}

void loop()
{

  if (!client.connect(host, httpPort)) {
    Serial.print("failed");
    return;
  }

  client.print(String("GET ") + "/getLedStatus" + " HTTP/1.1\r\n" + "Host:" + host + "\r\n" + "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 500) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  Serial.println();
  while (client.available()) {
    String line = client.readStringUntil('\r');
    //    status_led = int(line);
    //    Serial.println(a);
    Serial.println("line" + line);
    if (line == "1" || line == "0") {
      status_led = line.toInt();
    }
    //    status_led = int(line[10]);
    //    Serial.println("status_led" + status_led);
    //    Serial.println(line[10]);
    //    Serial.println();
    //    Serial.println(int(line[10]));
    Serial.print("status_led ==>" + status_led);
    //        Serial.print(status_led);
    getLEDStateFromFecth();
  }
  if (digitalRead(sw) == 0) {
    getLEDState();
    delay(500);
  }
  server.handleClient();
}

void serveIndexFile()
{
  File file = SPIFFS.open("/index.html", "r");
  server.streamFile(file, "text/html");
  file.close();
}

void serveToggleLedFile()
{
  File file = SPIFFS.open("/toggle_led.html", "r");
  server.streamFile(file, "text/html");
  file.close();
}

void toggleLED()
{
  digitalWrite(pin_led, !digitalRead(pin_led));
}

void getLEDState()
{
  toggleLED();
  String led_state = digitalRead(pin_led) ? "Off" : "on";
  server.send(200, "text/plain", led_state);
}


void getLEDStateFromFecth()
{
  digitalWrite(pin_led, status_led);
  String led_state = digitalRead(pin_led) ? "Off" : "on";
  server.send(200, "text/plain", led_state);
}
