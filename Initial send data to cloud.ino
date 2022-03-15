

#include <SPI.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h" 


#include "Firebase_Arduino_WiFiNINA.h"

#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

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
  String path = "/test"; //Path in the Database. We can hang by the root
  String jsonStr;

   //--------------------------  Insert and read a whole number -----------------------------

  Serial.println("Inserting an integer into specified field");
  
   if (Firebase.setInt(firebaseData, path + "/Int/Data1",2022))
    {
      Serial.println("Insert");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: ");
      if (firebaseData.dataType() == "int") // is done to ensure that if the stored type is integer we get the value correctly and not garbage
        Serial.println(firebaseData.intData()); // We are really reading the entered value with this
    }
    else
    {
      Serial.println("ERROR : " + firebaseData.errorReason());
      Serial.println();
    }
	
	 Serial.println("----------------------------------------");
   Serial.println("Reading the inserted value");
	
   if (Firebase.getInt(firebaseData, path + "/Int/Data1"))
    {
      Serial.println("Reading result");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: ");
      if (firebaseData.dataType() == "int")
        Serial.println(firebaseData.intData()); //Here is the read value
    }
    else
    {
      Serial.println("ERROR: " + firebaseData.errorReason());
      Serial.println();
    }
    Serial.println("------------------------------------------------------------------------------------");
//--------------------------  Insert and Read a string -----------------------------
  

 Serial.println("Inserting a string into specified field");
  
   if (Firebase.setString(firebaseData, path + "/String/text1","Hello Firebase"))
    {
      Serial.println("Insert");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: ");
      if (firebaseData.dataType() == "string") 
        Serial.println(firebaseData.stringData()); 
    }
    else
    {
      Serial.println("ERROR : " + firebaseData.errorReason());
      Serial.println();
    }
 Serial.println("----------------------------------------");	
 Serial.println("Reading the inserted string");
	
   if (Firebase.getString(firebaseData, path + "/String/text1"))
    {
      Serial.println("Reading result");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: ");
      if (firebaseData.dataType() == "string")
        Serial.println(firebaseData.stringData()); 
    }
    else
    {
      Serial.println("ERROR: " + firebaseData.errorReason());
      Serial.println();
    }
}


void loop()
{

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
