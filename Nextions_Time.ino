#include <WiFi.h>
#include "time.h"

const char* ssid       = "Nett";
const char* password   = "*********";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

String final_time_string;
String final_time_string2;
String final_date_string;
String final_date_string2;
String final_date_string3;
String x;

char last_letter;
char last_letter2;

float Ltime = 55;
void setup()
{
  Serial.begin(9600);

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
 
  printLocalTime();
  sendNextionTimeCommand();
  sendNextionDateCommand();
  //Serial.println();
  delay(1000);

}

void printLocalTime()
{
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
  Serial.print(command);
  endNextionCommand();

}

void sendNextionDateCommand() {
  //String command = "time.txt=\""+String(final_time_string+":"+final_time_string2)+"\""; 
  String command = "date.txt=\""+String(final_date_string+"."+final_date_string2+"."+final_date_string3)+"\""; 
  Serial.print(command);
  endNextionCommand();

}

void endNextionCommand()
{
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  
}
