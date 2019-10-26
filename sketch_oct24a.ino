// importing necessary libraries
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
// library imported from : https://gitlab.com/MakerRobotics/_M_MQ5 to exploit MQ5's data
#include <maker_mq5.h>


// variable declarations for circuit connections with the Arduino UNO:
// lcd for LCD screen, data for data collected from MQ135 sensor
// 
 
LiquidCrystal lcd(13, 12, 6, 5, 4, 3);


float t=0;
char data = 0;
// TODO: insert IoT platform's channel's (thingspeak for example) API key
String apiKey = "8NBNB4VQ9F2EEWQM";
// connect 10 to TX of Serial USB
// connect 11 to RX of serial USB
SoftwareSerial ser(10,11); // RX, TX


void setup() 
{                
  // enable debug serial 
  Serial.begin(9600);
  // enable software serial
  ser.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0); 
  lcd.print("Engineers Garage");
  lcd.setCursor(0,1);
  lcd.print("               "); 
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("    IOT AIR");
  lcd.setCursor(0,1);
  lcd.print("QUALITY MONITOR");  
  delay(3000); 
 // pinMode(12, INPUT);
  
  // reset ESP8266 WiFi connection AT+CIPMUX=1 AT+CWJAP
  ser.println("AT");
  delay(1000);
  ser.println("AT+GMR");
  delay(1000);
  ser.println("AT+CWMODE=3");
  delay(1000);
  ser.println("AT+RST");
  delay(5000);
  ser.println("AT+CIPMUX=1");
  delay(1000);
  String cmd="AT+CWJAP=\"EngineersGarage\",\"egP@$$w0rd?\"";
  ser.println(cmd);
  delay(1000);
  ser.println("AT+CIFSR");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("      WIFI");
  lcd.setCursor(0,1);
  lcd.print("   CONNECTED");
 }




 
void loop()
{
  delay(1000);
  t = analogRead(A0);
  Serial.print("Airquality = ");
  Serial.println(t);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  SENDING DATA");
  lcd.setCursor(0,1);
  lcd.print("    TO CLOUD");
  esp_8266();
}



void esp_8266()
{
   // TCP connection AT+CIPSTART=4,"TCP","184.106.153.149",80 
    String cmd = "AT+CIPSTART=4,\"TCP\",\"";
    cmd += "184.106.153.149"; // api.thingspeak.com
    cmd += "\",80";
    ser.println(cmd);
    Serial.println(cmd); 
    if(ser.find("Error"))
    {
      Serial.println("AT+CIPSTART error");
      return;
    }
  
  // prepare GET string GET https://api.thingspeak.com/update?api_key=LHAG4NSIYJ5UWS6U&field1=0\r\n\r\n
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  //getStr +="&field1=";
  //getStr +=String(h);
  getStr +="&field1=";
  getStr +=String(t);
  getStr += "\r\n\r\n";
  // send data length
  cmd = "AT+CIPSEND=4,";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);
  delay(1000);
  ser.print(getStr);
  Serial.println(getStr);
     
  // thingspeak needs 15 sec delay between updates
  delay(16000);  
}
