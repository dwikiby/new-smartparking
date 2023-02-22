// Source Code SCP (Smart Car Parking) IoT
// Mikrokontroler Wemos D1 R1 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#include <ESP8266HTTPClient.h>
#include <b64.h>
#include <HttpClient.h>
#include <HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>      //library untuk format data sensor menjadi json
WiFiClient wifiClient;
#include "FirebaseESP8266.h"  //library untuk firebase esp8266

String wifiSSID = "przival";        //SSID
String wifiPassword = "rahasia123"; // PASS SSID

const int ir1 = D3;           //deklarasi pin sensor 
const int ir2 = D4;
const int ir3 = D5;
const int ir4 = D6;

//Deklarasi fungsi
void connectWifi();
void postHttp();
#define FIREBASE_HOST "nodemcudht11-14d34-default-rtdb.firebaseio.com" //link firebase
#define FIREBASE_AUTH "nxrTlBv5ETOgBCRhEBcnLcPBlKPfUEXmrXWgnpwc" //firebase token

FirebaseData firebaseData;
FirebaseJson json;

int S1=0, S2=0, S3=0, S4=0; //slot parkir
//int flag1=0, flag2=0;  
int slot = 4; 

void setup() {
  Serial.begin(9600);
  lcd.init();         // initialize the lcd 
  lcd.backlight();

  lcd.begin(16, 2);  
  lcd.backlight();
  lcd.setCursor (0,0);
  lcd.print("SMART CAR PARKING");
  lcd.setCursor (0,1);
  lcd.print("IOT SYSTEM");
  delay (2000);
  lcd.clear();

  Read_Sensor();
  
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(ir4, INPUT);
  connectWifi();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

void loop() {

  // menampilkan informasi slot di lcd 
Read_Sensor();

lcd.setCursor (0,0);
lcd.print("   Have Slot: "); 
lcd.print(slot);
//Serial.println(slot);
////Slot.println(slot);
lcd.print("    ");  

lcd.setCursor (0,1); // informasi slot parkir 1
if(S1==1 ){
  lcd.print("P1:");
  lcd.setCursor(3,1);
  lcd.print("P");
  slot = 3;}

     else{
      lcd.setCursor (0,1);
      lcd.print("P1:");
      lcd.setCursor(3,1);
      lcd.print("K");
      slot = 4;}

lcd.setCursor (5,1); // informasi slot parkir 2 
if(S2==1){
  lcd.print("P2:");
  lcd.setCursor(8,1);
  lcd.print("P");
  slot = 3;}
  
     else{lcd.print("P2:");
     lcd.setCursor(8,1);
     lcd.print("K");
     slot = 4;}

lcd.setCursor (10,1); // informasi slot parkir 3
if(S3==1){
  lcd.print("P3:");
  lcd.setCursor(12,1);
  lcd.print("P");
  slot = 3;}
  
     else{lcd.print("P3:");
     lcd.setCursor(12,1);
     lcd.print("K");
     slot = 4;}

lcd.setCursor (14,1); // informasi slot parkir 4
if(S4==1){
  lcd.print("P4:");
  lcd.setCursor(16,1);
  lcd.print("P");
  slot = 3;}
  
     else{lcd.print("P4:");
     lcd.setCursor(16,1);
     lcd.print("K");
     slot = 4;}

  firebaseUpdate();   //kirim data sensor ke firebase realtime database
  delay(1000);
  kirimSlot1();       // kirim data slot 1-4 ke cluster yacanet
  kirimSlot2();
  kirimSlot3();
  kirimSlot4();
  delay(2000);
}

void connectWifi(){
  Serial.println("Connecting to WiFi");
  WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("Wifi Connected");
  Serial.println(WiFi.SSID());
  Serial.println(WiFi.RSSI());
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
}

void firebaseUpdate(){                // Pengiriman data sensor ke firebase
  String slot1, slot2, slot3, slot4 ;
  slot1 = digitalRead(ir1);
  slot2 = digitalRead(ir2);
  slot3 = digitalRead(ir3);
  slot4 = digitalRead(ir4);

  // kirim data slot parkir (1)
    if (Firebase.setString(firebaseData, "/IOT/slotparkir1", slot1))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  // kirim data slot parkir (2)

    if (Firebase.setString(firebaseData, "/IOT/slotparkir2", slot2))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  // kirim data slot parkir(3)

  if (Firebase.setString(firebaseData, "/IOT/slotparkir3", slot3))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  } 

  // kirim data slot parkir(4)

  if (Firebase.setString(firebaseData, "/IOT/slotparkir4", slot4))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  } 
}

void kirimSlot1(){ // pengiriman data sensor slot 
  String slot1; //deklarasi tipe variabel
  slot1 = digitalRead(ir1);
  
  Serial.println("MENGIRIM DATA SLOT 1");
  String url = "http://scp-28-boot.sttindonesia.ac.id:8888/api/sensor"; //url target
  HTTPClient http ;
  String response ;

  StaticJsonDocument<200> buff;
  String jsonParams ;
  buff["sensorId"]="1";
  
  if (slot1 == "1" ){ // jika slot parkir kosong kirim status 1
    buff["data"]= "{\"status\":\"1\"}"; 
  }
  else {
    buff["data"]= "{\"status\":\"2\"}"; // jika slot parkir terisi kirim status 2
  }
  serializeJson(buff, jsonParams);
  Serial.println(jsonParams);

  http.begin(wifiClient, url);
  http.addHeader("Content-Type", "application/json");
  http.POST(jsonParams); // METHOD POST 
  response = http.getString();
  Serial.println(response);
  Serial.println("------------------------------------");
  Serial.println();
}

void kirimSlot2(){ // pengiriman data sensor slot 2
  String slot2; //deklarasi tipe variabel
  slot2 = digitalRead(ir2);
  
  Serial.println("MENGIRIM DATA SLOT 2");
  String url = "http://scp-28-boot.sttindonesia.ac.id:8888/api/sensor"; //url target
  HTTPClient http ;
  String response ;

  StaticJsonDocument<200> buff;
  String jsonParams ;
  buff["sensorId"]="2"; // sensor id 2
  
  if (slot2 == "1" ){
    buff["data"]= "{\"status\":\"1\"}"; 
  }
  else {
    buff["data"]= "{\"status\":\"2\"}"; 
  }

  serializeJson(buff, jsonParams);
  Serial.println(jsonParams);

  http.begin(wifiClient, url);
  http.addHeader("Content-Type", "application/json");
  http.POST(jsonParams); // METHOD POST 
  response = http.getString();
  Serial.println(response);
  Serial.println("------------------------------------");
  Serial.println();
}

void kirimSlot3(){ // pengiriman data slot 3 
  String slot3; //deklarasi tipe variabel
  slot3 = digitalRead(ir3);
  
  Serial.println("SENDING..");
  String url = "http://scp-28-boot.sttindonesia.ac.id:8888/api/sensor"; //url target
  HTTPClient http ;
  String response ;

  StaticJsonDocument<200> buff;
  String jsonParams ;
  buff["sensorId"]="3"; // sensor id 3

  if (slot3 == "1" ){
    buff["data"]= "{\"status\":\"1\"}"; 
  }
  else {
    buff["data"]= "{\"status\":\"2\"}"; 
  }
  
  serializeJson(buff, jsonParams);
  Serial.println(jsonParams);

  http.begin(wifiClient, url);
  http.addHeader("Content-Type", "application/json");
  http.POST(jsonParams); // METHOD POST 
  response = http.getString();
  Serial.println(response);
  Serial.println("------------------------------------");
  Serial.println();
}

void kirimSlot4(){ // pengiriman data slot 4
  String slot4; //deklarasi tipe variabel
  slot4 = digitalRead(ir4);
  
  Serial.println("MENGIRIM DATA SLOT 4");
  String url = "http://scp-28-boot.sttindonesia.ac.id:8888/api/sensor"; //url target
  HTTPClient http ;
  String response ;

  StaticJsonDocument<200> buff;
  String jsonParams ;
  
  buff["sensorId"]="4"; // sensor id 4
  if (slot4 == "1" ){
    buff["data"]= "{\"status\":\"1\"}"; 
  }
  else {
    buff["data"]= "{\"status\":\"2\"}"; 
  }

  serializeJson(buff, jsonParams);
  Serial.println(jsonParams);

  http.begin(wifiClient, url);
  http.addHeader("Content-Type", "application/json");
  http.POST(jsonParams); // METHOD POST 
  response = http.getString();
  Serial.println(response);
  Serial.println("------------------------------------");
  Serial.println();
}

void Read_Sensor(){
S1=0, S2=0, S3=0, S4=0;
if(digitalRead(ir1) == 0){S1=1;}
if(digitalRead(ir2) == 0){S2=1;}
if(digitalRead(ir3) == 0){S3=1;}
if(digitalRead(ir4) == 0){S4=1;}  
}
