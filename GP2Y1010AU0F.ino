#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

#define BOTtoken "5352956410:AAHDAQXSQCsqO9-zKSWDqm6G14y1WOHx-cU"

#include "PageIndex.h"

#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#define MUX_A D0
#define MUX_B D1
#define MUX_C D2
#define ANALOG_INPUT A0


#define WIFI_SSID "Thesis22"
#define WIFI_PASS "Thesis22"
#define WEB_SERVER_PORT 80



ESP8266WebServer webServer(WEB_SERVER_PORT);

int ledPin = D4;
const int oneWireBus = 14;   
DHT dht(0, DHT11);
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

#define CHAT_ID "-1001782976731"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 500;
unsigned long lastTimeBotRan;

 
 
float voltsMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
float soilmost = 0;
float wtrlvl = 0;
 
void setup()
{
  Serial.begin(9600);
  setupWiFi();
  setupWebServer();
  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif
  dht.begin();
  pinMode(ledPin,OUTPUT);
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);     
  pinMode(MUX_C, OUTPUT);  
 
}
 void changeMux(int c, int b, int a) {
  digitalWrite(MUX_A, a);
  digitalWrite(MUX_B, b);
  digitalWrite(MUX_C, c);
}
 
void loop()
{
   webServer.handleClient();
   
   if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
      Serial.println("numNewMessages");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  
}

void setupWiFi()
{
  Serial.println("[WiFi] Setup");
  Serial.print("[WiFi] Connecting to: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("[WiFi] Connected!");
  Serial.print("[WiFi] IP: ");
  Serial.println(WiFi.localIP());
}
void setupWebServer()
{
  Serial.println("[WebServer] Setup");
  webServer.on("/", handleRoot);
  webServer.on("/readDustDensity", handleDD);
  webServer.on("/readSoilMost", handleSM);
  webServer.on("/readWaterLevel", handleWL);
  webServer.on("/readWeatherTemp", handleWT);
  webServer.on("/readWeatherHum", handleWH);
  webServer.on("/readSoilTemp", handleST);
  
  Serial.println("[WebServer] Starting..");
  webServer.begin();
  Serial.println("[WebServer] Running!");
}
void handleRoot() {
 String s = MAIN_page; //--> Read HTML contents
 webServer.send(200, "text/html", s); //--> Send web page
}

void handleDD() {
  digitalWrite(ledPin,LOW); // power on the LED
  delayMicroseconds(280);
  
  changeMux(LOW, LOW, LOW);
  voltsMeasured = analogRead(ANALOG_INPUT); // read the dust value
  delayMicroseconds(40);
  digitalWrite(ledPin,HIGH); // turn the LED off
  delayMicroseconds(9680);
 
  //measure your 5v and change below
  calcVoltage = voltsMeasured * (3.3 / 1024.0);
  dustDensity = 0.17 * calcVoltage - 0.1;
  
  String DustD = String(dustDensity);
 
  webServer.send(200, "text/plane", DustD); 
  if (isnan(dustDensity)) {
    Serial.println("Failed to read from Dust sensor !");
  }
  else {
    Serial.print(" Dust Density : ");
    Serial.println( dustDensity);
  }
}
void handleSM() {
  changeMux(LOW, HIGH, LOW); //mux-X2
  soilmost =  analogRead(ANALOG_INPUT); 
  String SM = String(soilmost);
 
  webServer.send(200, "text/plane", SM); 
  if (isnan(soilmost)) {
    Serial.println("Failed to read from Soil sensor !");
  }
  else {
    Serial.print(" Soil most : ");
    Serial.println( soilmost);
  }
}
void handleWL() {
  changeMux(LOW, LOW, HIGH); //mux-X1
  wtrlvl =  analogRead(ANALOG_INPUT);
  String WL = String(wtrlvl);
 
  webServer.send(200, "text/plane", WL); 
  if (isnan(wtrlvl)) {
    Serial.println("Failed to read from Wl sensor !");
  }
  else {
    Serial.print(" W  L : ");
    Serial.println( wtrlvl);
  }
}

void handleWH() {
  float h = dht.readHumidity();
  String Humidity_Value = String(h);
 
  webServer.send(200, "text/plane", Humidity_Value); 
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor !");
  }
  else {
    Serial.print("Humidity : ");
    Serial.println(h);
  }
}
void handleWT() {
  float t = dht.readTemperature();  
  String Temperature_Value = String(t);
 
  webServer.send(200, "text/plane", Temperature_Value);
  
  if (isnan(t)) { 
    Serial.println("Failed to read from DHT sensor!");
  }
  else {
    Serial.print("DHT11 || Temperature : ");
    Serial.print(t);
    Serial.print(" || ");
  }
}
void handleST() {
  sensors.requestTemperatures();
  float st = sensors.getTempCByIndex(0);  
  String Temperature_Value = String(st);
 
  webServer.send(200, "text/plane", Temperature_Value);
  
  if (isnan(st)) { 
    Serial.println("Failed to read from Soil Temp sensor!");
  }
  else {
    Serial.print(" Soil Temp || Temperature : ");
    Serial.print(st);
    Serial.print(" || ");
  }
}
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following command to get current readings.\n\n";
      welcome += "/readings \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/readings") {
      sensors.requestTemperatures();
  float st = sensors.getTempCByIndex(0);  
  
       String readings = "Dust dencity = " + String(dustDensity);
       readings += " \n Soil Mostur = " + String(soilmost);
       readings += " \n Water Level = " + String(wtrlvl);
       readings += " \n Weather Temp = " + String(dht.readTemperature());
       readings += " \n Weather Humidity = " + String(st);
      bot.sendMessage(chat_id, readings, "");
    }  
   
  }
}
