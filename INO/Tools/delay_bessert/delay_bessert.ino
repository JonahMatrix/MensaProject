
  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  verzoegerung(500);  
}

void verzoegerung(int x){
  int vergangen;
  int setmillis;
  setmillis = millis();
  while(millis() - setmillis<x){
     Serial.println(millis() - setmillis);
     Serial.println(setmillis);
     Serial.println();
  }
  vergangen = 0;
  Serial.println(vergangen);
}
