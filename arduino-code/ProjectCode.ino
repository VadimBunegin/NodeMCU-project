#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>


const int tachPIN=D4;
const unsigned long sampleTime=1000;

int totalimps = 0;

#define ON_Board_LED 2  
const char* ssid = "Beeline_5E79"; 
const char* password = "92515952"; 


const char* host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------

WiFiClientSecure client; //--> Create a WiFiClientSecure object.

String GAS_ID = "AKfycbyT1XyQYQawxhznzRkS0ZtZka2jwsYS7GRKZjv_MxrQ_fUf1SuF"; 
                   

void setup() {
  
  pinMode(tachPIN, INPUT);
  digitalWrite(tachPIN, HIGH); 
  
  Serial.begin(115200);
 
  
  WiFi.begin(ssid, password); 
  Serial.println("");
    
  pinMode(ON_Board_LED,OUTPUT); 
  digitalWrite(ON_Board_LED, HIGH); 

  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    
    digitalWrite(ON_Board_LED, LOW);
    
    digitalWrite(ON_Board_LED, HIGH);

  }
  
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  client.setInsecure();

  
}
void loop() {
  int rpm=getRPM();
   Serial.print("RPM: ");
   Serial.print(totalimps/100);
   Serial.print("\n");
   
   int h = totalimps/100;
   
  String Humi = "impuls: " + String(h);

  Serial.println(Humi);
  
  sendData(h); 
}

int getRPM(){

  int kount=0;
  boolean kflag=LOW;
  unsigned long currentTime=0;
  unsigned long startTime=millis();

  while (currentTime<=sampleTime){
    if (digitalRead(tachPIN)==HIGH){
      kflag=HIGH;
    }

    if (digitalRead(tachPIN)==LOW && kflag==HIGH){
      kount++;
      kflag=LOW;
    }

    currentTime=millis()-startTime;
  }

  int kount2rpm = int(60000./float(sampleTime))*kount;


  totalimps = totalimps + kount2rpm;

  return totalimps;


}
void sendData(int hum) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  

  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  
  String string_humidity =  String(hum, DEC); 
  String url = "https://script.google.com/macros/s/" + GAS_ID + "/exec?&humidity=" + string_humidity;
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