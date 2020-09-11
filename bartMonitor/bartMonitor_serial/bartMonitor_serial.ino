#include "DHT.h"
#include <ESP8266WiFi.h>

#define DHTTYPE DHT11

#define dhtCOLD_dpin 0
#define dhtHOT_dpin 2

const char* ssid     = "CenturyLink3302";
const char* password = "rmps9qx4nuq2rk"; 

WiFiServer server(80);

DHT dhtHOT(dhtHOT_dpin, DHTTYPE);
DHT dhtCOLD(dhtCOLD_dpin, DHTTYPE);
 
void setup(void)
{ 
  dhtCOLD.begin();
  dhtHOT.begin();
  Serial.begin(9600);
  Serial.println("Humidity and temperature\n\n");
  delay(700);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("...");
  }

  
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
    WiFiClient client = server.available();
  
    if (!client) {
      Serial.println("connection failed");
    }
    else
    {
      client.flush();
      Serial.println("\n[Client connected]");
      while (client.connected())
      {
        float h_cold = dhtCOLD.readHumidity();
        float t_cold = dhtCOLD.readTemperature();
        float h_hot = dhtHOT.readHumidity();
        float t_hot = dhtHOT.readTemperature();

        /*
            Serial.print("Current cold humidity = ");
            Serial.print(h_cold);
            Serial.print("%  ");
            Serial.print("cold temperature = ");
            Serial.print(t_cold); 
            Serial.println("C  ");
        
            Serial.print("Current hot humidity = ");
            Serial.print(h_hot);
            Serial.print("%  ");
            Serial.print("hot temperature = ");
            Serial.print(t_hot); 
            Serial.println("C  ");
        */
        
        //client.println(prepareHtmlPage(h_cold, t_cold, h_hot, t_hot));
        client.print(String(h_cold));
        delay(5000);
      }
      
      delay(1);
      client.stop();
      Serial.println("[Client disconnected]");
    }
    
    delay(2000);
}

String prepareHtmlPage(float h_cold, float t_cold, float h_hot, float t_hot)
{
  String htmlPage =
     String("HTTP/1.1 200 OK\r\n") +
            "Content-Type: text/html\r\n" +
            "Connection: close\r\n" +  // the connection will be closed after completion of the response
            "Refresh: 5\r\n" +  // refresh the page automatically every 5 sec
            "\r\n" +
            "<!DOCTYPE HTML>" +
            "<html>" +
            "Hot Temp:  " + t_hot + "<br>" +
            "Hot Humidity: " + h_hot + "<br>" +
            "Cold Temp: " + t_cold + "<br>" +
            "Cold Humidity: " + h_cold + "<br>" +
            "</html>" +
            "\r\n";
  return htmlPage;
}
