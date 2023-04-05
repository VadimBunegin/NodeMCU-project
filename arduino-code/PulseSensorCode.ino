#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <TimerMs.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define SENSOR_PIN  2 //D4

OneWire oneWire(SENSOR_PIN);
DallasTemperature DS18B20(&oneWire);
TimerMs tmr(10000, 1, 1);
WiFiClientSecure client;


float tempC; 
const char* host = "web-production-4880.up.railway.app";
const int httpsPort = 443;
int Threshold = 570; 
int count = 0;
const char* ssid = "Beeline_5E79"; 
const char* password = "92515952"; 


void setup() {
  Serial.begin(115200);
  DS18B20.begin();
  tmr.setTimerMode();
  WiFi.begin(ssid, password); 
  Serial.println("");
    

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  client.setInsecure();

  
}
void loop() {

  if(analogRead(0)>Threshold){
      count ++;
      while (analogRead(0) > Threshold-5){
        count = count;
      }
      Serial.println(count);
    }


  if (tmr.tick()){
     Serial.println(count);
     DS18B20.requestTemperatures();
    tempC = DS18B20.getTempCByIndex(0);
    sendData(count*6, tempC);
    count = 0;
    tmr.start();
  }
}


void sendData(int pulse, float temp) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String string_pulse =  String(pulse, DEC); 
  String string_temp =  String(temp, DEC); 
  String url = "https://web-production-4880.up.railway.app/get_request/?v1=" + string_pulse + "&v2=" + string_temp;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");

 
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI has failed");
  } else {
    Serial.println("esp8266/Arduino CI successfull!");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
 
}