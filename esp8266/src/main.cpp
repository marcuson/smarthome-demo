#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

ESP8266WebServer server(80);

StaticJsonDocument<250> jsonDocument;
char buffer[250];

bool isOn = false;

void sendState()
{
  jsonDocument.clear();
  jsonDocument["state"] = isOn ? "on" : "off";
  serializeJson(jsonDocument, buffer);
  Serial.print("Sending back state response: ");
  Serial.println(buffer);
  server.send(200, "application/json", buffer);
}

void getState()
{
  Serial.println("Received GET state request");
  sendState();
}

void handleControl()
{
  Serial.println("Received POST command");
  String body = server.arg("plain");
  Serial.print("Body is: ");
  Serial.println(body);
  deserializeJson(jsonDocument, body);
  String newState = jsonDocument["state"];
  isOn = newState == "on" ? true : false;
  sendState();
}

void updateLedState()
{
  digitalWrite(LED_BUILTIN, isOn ? LOW : HIGH);
}

void wifiConnect()
{
  File file = LittleFS.open("/wifi.txt", "r");
  String ssid = file.readStringUntil('|');
  String password = file.readStringUntil('|');
  file.close();

  Serial.print("Trying WiFi connection, ssid: ");
  Serial.print(ssid);
  Serial.print(", password: ");
  Serial.println(password);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("WiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}

void setupRouting()
{
  server.on("/state", getState);
  server.on("/control", HTTP_POST, handleControl);
  server.begin();
}

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  if (!LittleFS.begin())
  {
    Serial.println("Error has occurred while mounting LittleFS.");
    return;
  }

  wifiConnect();
  setupRouting();

  updateLedState();
}

void loop()
{
  server.handleClient();
  updateLedState();
}