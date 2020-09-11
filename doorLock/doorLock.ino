#include <Servo.h>
#include <ESP8266WiFi.h>

const char* ssid     = "CenturyLink3302";
const char* password = "rmps9qx4nuq2rk"; 

WiFiServer server(80);

Servo door_servo;
const int servoPin = 2;
const int buttonPin = 15;

int buttonState = 0;
int locked = 0;

Servo servo;

void setup() {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);

  servo.attach(servoPin);
  servo.write(160);
  delay(3000);
  servo.write(160);
  delay(3000);
  servo.detach();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  Serial.println(WiFi.localIP()); 

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    client.flush();
    
    while (client.connected()) {
      buttonState = digitalRead(buttonPin);
      if (buttonState == 1) {
        delay(100);
        buttonState = digitalRead(buttonPin);
        if (buttonState == 1) {
          Serial.println("HERE");
          changeState();
        }
      }

      if(client.available()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
        if (line.equals("LOCK")) {
          changeState();
        }
        client.flush();
      }
      
    }
    
    delay(1);
    client.stop();
  }
  else {
    buttonState = digitalRead(buttonPin);
    if (buttonState == 1) {
      changeState();
    }
  }
}

void changeState() {
  if (locked == 0) {
    servo.attach(servoPin);
    delay(100);
    servo.write(0);
    delay(2000);
    servo.detach();
    locked = 1;
  }
  else {
    servo.attach(servoPin);
    servo.write(160);
    delay(1500);
    servo.detach();
    locked = 0;
  }  
}
