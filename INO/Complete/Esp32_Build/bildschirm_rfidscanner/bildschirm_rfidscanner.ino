#include <MFRC522.h> //library responsible for communicating with the module RFID-RC522
#include <SPI.h> //library responsible for communicating of SPI bus
#include <WiFi.h>
#include "time.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>


const char* ssid = "CableLeachim";
const char* password = "*************";
StaticJsonDocument<200> doc;
//Your Domain name with URL path or IP address with path
String serverName = "https://gymhogautest.000webhostapp.com/DatenbankTest.php";   //old name http://192.168.178.36/db.php



#define SS_PIN    5
#define RST_PIN   15
#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16
#define greenPin     12
#define redPin       32
//used in authentication
MFRC522::MIFARE_Key key;
//authentication return status code
MFRC522::StatusCode status;
// Defined pins to module RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);


bool cardWasRead;


//////// Time /////////////////////////////
#include "time.h"
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

///////////////////////////////////////////

//////// Bildschirm /////////////////////////////
String final_time_string;
String final_time_string2;
String final_date_string;
String final_date_string2;
String final_date_string3;
String x;

char last_letter;
char last_letter2;
float Ltime = 55;

#define TXD2 17
#define RXD2 16
/////////////////////////////////////////////////

/////////////////LED_STRIP////////////////
/*include <Adafruit_NeoPixel.h>
#define PIN 13
#define NUM_LEDS 85
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
*/

//////////////////////////////////////////
String build= "Version 0.2";
void setup()
{
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
    Serial.begin(9600);
    
    
  //connect to WiFi
    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    wifiMeldungNextion1();
    delay(500);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    wifiMeldungNextion2();
    delay(1000);
    


    SPI.begin(); // Init SPI bus
    //pinMode(greenPin, OUTPUT);
    //pinMode(redPin, OUTPUT);

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //printLocalTime();

    // Init MFRC522
    mfrc522.PCD_Init();
    Serial.println("Approach your reader card...");
    Serial.println();

    //strip.begin();
    //strip.show(); // Initialize all pixels to 'off'

    //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  resetNextionText();
    
}

void loop()
{
  ////Bildschirm////////////////////
    printLocalTime();
  sendNextionTimeCommand();
  sendNextionDateCommand();
  //Serial.println();
  verzoegerung(1000);
  ///////////////////////////////////////

  



}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  
 
  final_time_string=timeinfo.tm_hour;
  if(final_time_string.length() == 1){
    final_time_string = "0" + final_time_string;
  }
  final_time_string2=timeinfo.tm_min;
  if(final_time_string2.length() == 1){
    final_time_string2 = "0" + final_time_string2;
  }

  final_date_string=timeinfo.tm_mday;
  if(final_date_string.length() == 1){
    final_date_string = "0" + final_date_string;
  }
  final_date_string2=timeinfo.tm_mon+1;
  if(final_time_string2.length() == 1){
    final_time_string2 = "0" + final_time_string2;
  }
  final_date_string3=timeinfo.tm_year+1900;

  int length = final_date_string3.length();
  final_date_string3[length] = last_letter;   // we need to get the last two numbers of 2019 because of lack of space
  final_date_string3[length] = last_letter2;
    //Serial.println(last_letter);
    //Serial.println(last_letter2);
    //Serial.println(final_date_string3[length-1]);
    //Serial.println(final_date_string3[length-2]);
}
void sendNextionTimeCommand() {
  String command = "time.txt=\""+String(final_time_string+":"+final_time_string2)+"\""; 
  //String command = "time.txt=\""+&timeinfo, "%A, %B %d %Y %H:%M:%S"+"\""; 
  Serial2.print(command);
  Serial.println(command);

  endNextionCommand();
  //Serial.println();

}
void rfid(){
  String UID = String(mfrc522.uid.uidByte[0], HEX) + String(mfrc522.uid.uidByte[1], HEX) + String(mfrc522.uid.uidByte[2], HEX) + String(mfrc522.uid.uidByte[3], HEX);
    //Serial.println("card:" + UID);
    sendReadText();
    String serverPath = serverName + "?RF=" + UID;
    cardWasRead = true;
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        //printLocalTime();


        // Your Domain name with URL path or IP address with path
        http.begin(serverPath.c_str());

        // Send HTTP GET request
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            //Serial.print("HTTP Response code: ");
            //Serial.println(httpResponseCode);
            String payload = http.getString();
            DeserializationError error = deserializeJson(doc, payload);
            if (error) {
                ///Serial.print(F("deserializeJson() failed: "));
                ///Serial.println(error.f_str());
                return;
            }
            //Serial.println(String(doc["name"]));
          //  Serial.println(String(doc["valid"]));
            long uu = doc["valid"];
            const char* Username = doc["name"];
            const char* Klasse = doc["klasse"];
            //Serial.println(uu);
            //Serial.println(Username);
            
            if (uu == 1) {
                //Serial.println("OK!!! ");
                //Serial.print("User: "); Serial.print(Username); Serial.print(" wurder erkannt. Eintritt erlaubt. Zeit: "); printLocalTime();
                //setAll(0,255,0);
                //delay(2000);
                //setAll(0,0,0);
                sendNextionValue1Command(Username);
                verzoegerung(2000);
                resetNextionText();
                
            } else if (uu==0){
                ///Serial.print("User: "); Serial.print(Username); Serial.print(" wurder erkannt. Eintritt NICHT erlaubt. Zeit: "); printLocalTime();
                //setAll(255,0,0);
                //delay(2000);
                //setAll(0,0,0);
                sendNextionValue0Command();
                verzoegerung(2000);
                resetNextionText();
              }
            // Serial.println(payload);
            
        }
        else
        {
            ///Serial.print("Error code: ");
            ///Serial.println(httpResponseCode);
        }
        // Free resources
        http.end();
    }
    else {
        //Serial.println("WiFi Disconnected");
    }

    if (cardWasRead) {
      
        //delay(1000);
        cardWasRead = false;
    }
}


void verzoegerung(int x){
 int setmillis;
  setmillis = millis();
  while(millis() - setmillis<x){
     checkRfid(); 
  }

    
}

void sendNextionDateCommand() {
  String cmd;
  cmd += "\"";
  //String command = "time.txt=\""+String(final_time_string+":"+final_time_string2)+"\""; 
  String command = "date.txt=\""+String(final_date_string+"."+final_date_string2+"."+final_date_string3)+"\""; 
  //Serial.print("date.txt=" + cmd + final_date_string + final_date_string2 + final_date_string3 + cmd);
  Serial.println(command);
  Serial2.print(command);
  //Serial.print("date.txt=" + cmd + "01.05.2021" + cmd);
  endNextionCommand();

}

void sendNextionValue1Command(String nameY) {
  String cmd;
  String x = "Guten Appetit";
  cmd += "\"";
  Serial.println("meet.txt=" + cmd + String(x) + cmd);
  Serial2.print("meet.txt=" + cmd + String(x) + cmd);
  //String command = "time.txt=\""+String(final_time_string+":"+final_time_string2)+"\""; 
  //String command = "meet.txt=\""+String(value1)+"\""; 
  //Serial.print(command);
  endNextionCommand();
  Serial.println("meet.pco=9985");
  Serial2.print("meet.pco=9985");
  endNextionCommand();
  delay(1000);
  Serial.println("meet.txt=" + cmd + String(nameY) + cmd);
  Serial2.print("meet.txt=" + cmd + String(nameY) + cmd);
  endNextionCommand();
  

}

void sendNextionValue0Command() {
  //String command = "time.txt=\""+String(final_time_string+":"+final_time_string2)+"\""; 
  String cmd;
  cmd += "\"";
  Serial.println("meet.txt=" + cmd + "Kein Zutritt" + cmd);
  Serial2.print("meet.txt=" + cmd + "Kein Zutritt" + cmd);
  //String value0 = "NICHT";
  
  //Serial.print(command);
  endNextionCommand();
  Serial.println("meet.pco=63488");
  Serial2.print("meet.pco=63488");
  endNextionCommand();

}

void endNextionCommand(){
  
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  
}

void checkRfid(){
    //waiting the card approach
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return;
    }

    // Select a card
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }

    rfid();

  
}

void resetNextionText(){
  String cmd;
  cmd += "\"";
  Serial2.print("meet.pco=0000");
  endNextionCommand();
  Serial2.print("instructions.pco=0000");
  endNextionCommand();
  Serial2.print("meet.txt=" + cmd + "Willkommen" + cmd);
  endNextionCommand();
  Serial2.print("instructions.txt=" + cmd + "Bitte halten Sie ihre RFID Karte an den Scanner" + cmd);
  endNextionCommand();
  Serial2.print("error.pco=0000");
  endNextionCommand();
  Serial2.print("error.txt=" + cmd + " "+ cmd);
  endNextionCommand();
  Serial2.print("version.txt=" + cmd + build + cmd);
  endNextionCommand();
  Serial.println("bildschirm reset");
}

void wifiMeldungNextion1(){
  String cmd;
  cmd += "\"";  
  Serial.println("instructions.pco=64512");
  Serial2.print("instructions.pco=64512");
  endNextionCommand();  
  delay(100);
  Serial.println("instructions.txt=" + cmd + "connecting to Wifi..." + cmd);
  Serial2.print("instructions.txt=" + cmd + "connecting to Wifi..." + cmd);
  endNextionCommand();
  
  
}

void wifiMeldungNextion2(){
  String cmd;
  String x = "Connected to WiFi network with IP Address:";
  cmd += "\"";
  Serial.println("instructions.pco=9985");
  Serial2.print("instructions.pco=9985");
  endNextionCommand();  
  delay(100);
  Serial.println("instructions.txt=" + cmd + String(x + WiFi.localIP()) + cmd);
  Serial2.print("instructions.txt=" + cmd + String(x + WiFi.localIP()) + cmd);
  endNextionCommand();
  Serial.println("wifi.txt=" +cmd + "connected to wifi" + cmd );
  Serial2.print("wifi.txt=" +cmd + "connected to wifi" + cmd );
  endNextionCommand();  
  
}

void sendReadText(){
  String cmd;
  cmd += "\"";
  Serial.println("instructions.pco=0000");
  Serial2.print("instructions.pco=0000");
  endNextionCommand();  
  Serial.println("meet.pco=0000");
  Serial2.print("meet.pco=0000");
  endNextionCommand();  
  Serial.println("instructions.txt=" + cmd + "Karte erfolgreich gelesen!" + cmd);
  Serial2.print("instructions.txt=" + cmd + "Karte erfolgreich gelesen!" + cmd);
  endNextionCommand();
  Serial.println("meet.txt=" + cmd + "Loading..." + cmd);
  Serial2.print("meet.txt=" + cmd + "Loading..." + cmd);
  endNextionCommand();
}
