#include "DHT.h"
#include <ESP8266WiFi.h>

#define DHTTYPE DHT22

#define dhtCOLD_dpin 0
#define dhtHOT_dpin 2

const char* ssid     = "CenturyLink3302";
const char* password = "rmps9qx4nuq2rk"; 

WiFiServer server(80);

DHT dhtHOT(dhtHOT_dpin, DHTTYPE);
DHT dhtCOLD(dhtCOLD_dpin, DHTTYPE);
 
void setup(void)
{ 
  Serial.begin(9600);
  
  dhtCOLD.begin();
  dhtHOT.begin();
  delay(2000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  server.begin();
}

void loop() {
    WiFiClient client = server.available();
  
    if (client) {
      client.flush();
      
      while (client.connected()) {
        float h_cold = dhtCOLD.readHumidity();
        float t_cold = dhtCOLD.readTemperature() * 1.8 + 32;
        float h_hot = dhtHOT.readHumidity();
        float t_hot = dhtHOT.readTemperature() * 1.8 + 32;

        String data = "CH," + String(h_cold) + ",CT," + String(t_cold) + ",HH," + String(h_hot) + ",HT," + String(t_hot) + ",";
        client.print(data);
        delay(5000);
        client.flush();
      }
      
      delay(1);
      client.stop();
    }
    
    delay(2000);
}
