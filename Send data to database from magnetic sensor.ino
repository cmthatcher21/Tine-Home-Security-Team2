#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoLowPower.h>

#include "arduino_secrets.h" 
//Ejemplo para acceder a Firebase

//Necesaria tener instalado WiFiNINA Library https://github.com/arduino-libraries/WiFiNINA esta en el repositorio de Arduino

#include "Firebase_Arduino_WiFiNINA.h"

#define FIREBASE_HOST "home-security-project-21ee6-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Inz3S5wczaXuRbcFVvQWF8FYmEZIglAW14slDwm7"
#define WIFI_SSID "RPI"
#define WIFI_PASSWORD "Th7nd3rL@b"

char ssid[] = SECRET_SSID;        
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;    

//Define Firebase data object
FirebaseData firebaseData;

void setup()
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");
  
  Serial.println("----------------------------------------");
  printData();
  Serial.println("----------------------------------------");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true); 

}


void loop()
{

       // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
   // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
// print out the value you read: remove in final
  String path = "/test"; //Path in the Database. We can hang by the root
  String jsonStr;
  if (voltage != 5.00)
  {
      if (Firebase.setInt(firebaseData, path + "/Int/Data1",0))
      {
      Serial.println("Insert");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: ");
      Serial.println(firebaseData.intData());
      
      }
      
  }
  else if (voltage == 5.00)
  {
    digitalWrite(LED_BUILTIN, LOW);
    if (Firebase.setInt(firebaseData, path + "/Int/Data1",1))
    {
      Serial.println("Insert");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: "); 
      Serial.println(firebaseData.intData());   
                   
  }
 
  
}

}

void printData() {
  Serial.println("Board Information:");
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println();
  Serial.println("Network Information:");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

