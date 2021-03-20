#include <MFRC522.h> //library responsible for communicating with the module RFID-RC522
#include <SPI.h> //library responsible for communicating of SPI bus
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


const char* ssid = "Nett";
const char* password = "T5Multivan";
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

/////////////////LED_STRIP////////////////
/*#include <Adafruit_NeoPixel.h>
#define PIN 13
#define NUM_LEDS 85
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
*/

//////////////////////////////////////////


void setup()
{
    Serial.begin(115200);

    
    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());


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
    
}

void loop()
{
  
   
    // Aguarda a aproximacao do cartao
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

    String UID = String(mfrc522.uid.uidByte[0], HEX) + String(mfrc522.uid.uidByte[1], HEX) + String(mfrc522.uid.uidByte[2], HEX) + String(mfrc522.uid.uidByte[3], HEX);
    Serial.println("card:" + UID);
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
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                return;
            }
            //Serial.println(String(doc["name"]));
          //  Serial.println(String(doc["valid"]));
            long uu = doc["valid"];
            const char* Username = doc["name"];
            //Serial.println(uu);
            //Serial.println(Username);
            
            if (uu == 1) {
                //Serial.println("OK!!! ");
                Serial.print("User: "); Serial.print(Username); Serial.print(" wurder erkannt. Eintritt erlaubt. Zeit: "); printLocalTime();
                //setAll(0,255,0);
                //delay(2000);
                //setAll(0,0,0);
                
            } else if (uu==0){
                Serial.print("User: "); Serial.print(Username); Serial.print(" wurder erkannt. Eintritt NICHT erlaubt. Zeit: "); printLocalTime();
                //setAll(255,0,0);
                //delay(2000);
                //setAll(0,0,0);
              }
            // Serial.println(payload);
            
        }
        else
        {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }
        // Free resources
        http.end();
    }
    else {
        Serial.println("WiFi Disconnected");
    }

    if (cardWasRead) {
        delay(1000);
        cardWasRead = false;
    }



}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
