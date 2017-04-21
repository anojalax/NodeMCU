
#include <ESP8266WiFi.h>
#include <MQTTClient.h>

int BUTTON_PIN = D2; //button is connected to GPIO pin D1
const char* ssid = "";
const char* password = "";
const char* mqtt_server = "ec2-45-205-193-143.compute-1.amazonaws.com";

WiFiClient net;
MQTTClient client;
unsigned long lastMillis = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  client.begin(mqtt_server, net);
  connect();
  client.publish("/switch", "initialised");
}
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "try", "try")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
}


void loop() {
  if (!client.connected()) {
    connect();
  }
  int status;
  status=digitalRead(BUTTON_PIN);
  String msg="Button status: ";
     if(status==HIGH )
     {
         msg= msg+ "Pressed";
         char message[58];
         msg.toCharArray(message,58);
         Serial.println(message);
         
         if(millis() - lastMillis > 5) 
         {
          lastMillis = millis();
          client.publish("/switch", "Switch Pressed");
         }
     }
     else
     {
         msg= msg+ " Not Press";
         char message[58];
         msg.toCharArray(message,58);
         Serial.println(message);
         
         if(millis() - lastMillis > 5)
           {
           lastMillis = millis();
           client.publish("/switch", "Switch not Pressed");
           }
     }
   
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
}
