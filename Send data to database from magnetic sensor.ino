// Code modified and written by Cory Thatcher, Ethan Long, Coby Moon
// Code adopted from Electro Peak on arduino project hub. Project: Connecting Arduino to Firebase to Send & Receive Data


#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiClient.h>
#include <ArduinoLowPower.h>

#include "arduino_secrets.h" 

#include "Firebase_Arduino_WiFiNINA.h"

#define FIREBASE_HOST "home-security-2-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AFC5lTirxWtocGHXPbqBQGArTchhBckmuJUSLSa6"
#define WIFI_SSID "RPI"
#define WIFI_PASSWORD "Th7nd3rL@b"

volatile bool magtriggered = 0;


char ssid[] = SECRET_SSID;        
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;    

//Define Firebase data object
FirebaseData firebaseData;
 int Arduino_state = 0;
  
void setup()
{
  ConfigInterrupt(0);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);
  ConfigInterrupt(0);

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


void loop() {

  

  LowPower.sleep(30000); //300,000 = 5 mins
  if(Arduino_state == 0){
    magtriggered = 0;
    Arduino_state = ReadWrite(magtriggered);
    if(Arduino_state == 1){
      ConfigInterrupt(1);
    }
  }
  else if(Arduino_state == 1){
    Arduino_state = ReadWrite(magtriggered);
    //if(magtriggered == 1){ // THIS IS FOR POWER SAVE STUFF
    //  Arduino_state = 0;
    //}
    if(Arduino_state == 0){
      ConfigInterrupt(0);
    }
    magtriggered = 0;
  }

 


}
int ReadWrite(int Trig)
{
  int x = 0;
   if (Firebase.setInt(firebaseData, "/ArduinoCheckIn1",1))
     {
     Serial.println("Insert");
     Serial.println("PATH: " + firebaseData.dataPath());
     Serial.println("TYPE: " + firebaseData.dataType());      
     Serial.print("VALUE: ");
     Serial.println(firebaseData.intData());
     }
    if(Trig == 1)
    {
      if (Firebase.setInt(firebaseData, "/ArduinoTriggered1",Trig))
      {
      Serial.println("Insert");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());      
      Serial.print("VALUE: ");
      Serial.println(firebaseData.intData());
      }
    }
  if(Firebase.getInt(firebaseData, "/Armed"))
  {
    if(firebaseData.dataType() == "int")
    x = firebaseData.intData();
    
  }
  return x;
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

void ISR()
{
  magtriggered = 1;
}

void ConfigInterrupt(int x){
  if(x==0){
    pinMode(A1, OUTPUT);
  }
  else{
    //pinMode(A1, INPUT);
    LowPower.attachInterruptWakeup(A1, ISR, RISING);
  }
  return;
}
