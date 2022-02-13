#ifndef ESP8266_COM_H
#define ESP8266_COM_H

// https://create.arduino.cc/projecthub/neverofftheinternet/esp8266-setup-and-first-wifi-connection-76fc3c
// https://stackoverflow.com/questions/40438336/esp8266-how-to-send-tcp-messages-using-atcipsend-command

// #define ESPCOM_RATE_INITIAL 115200
#define ESPCOM_RATE_OPERATION 9600
// #define ESPCOM_AT "AT"
// #define ESPCOM_AT_UART "AT+UART_DEF=9600,8,1,0,0"
// #define ESPCOM_AT_CWMODE "AT+CWMODE=1"
// #define ESPCOM_AT_CWLAP "AT+CWLAP"
// #define ESPCOM_AT_CWJAP_PREFIX "AT+CWJAP=\""

#include <NeoSWSerial.h>
#include "../essential/util.h"

NeoSWSerial esp_serial(8, 9);

#define IP "192.168.0.8"
#define PORT "1973"

class ESP8266Com
{
public:
  void setup();
  void send(StateData *state_data);
  void send_cmd(String cmd, uint16_t wait_time);
};

#endif

/*

#include <SoftwareSerial.h>

const byte rxPin = 2;
const byte txPin = 3;

SoftwareSerial ESP8266 (rxPin, txPin);

unsigned long lastTimeMillis = 0;

void setup() {
  pinMode(5,OUTPUT);
  Serial.begin(9600);   
  ESP8266.begin(115200);
  delay(500);
  ESP8266.println("AT+CIPSTA=\"192.168.1.195\",\"192.168.1.1\",\"255.255.255.0\"");
  delay(500);
  ESP8266.println("AT+CIPMUX=1");
  delay(1000);   
  ESP8266.println("AT+CIPSERVER=1,11000");
  delay(1000);

  Serial.println("");
  Serial.println("Server is Ready");
  Serial.println("");   
}

void printResponse() {
  while (ESP8266.available()) {
    Serial.println(ESP8266.readStringUntil('\n'));
  }
}

void loop() {

    if ( ESP8266.available() ) 
    { 
       Serial.write( ESP8266.read()); 
       String result = ESP8266.readString();
       delay(500);
       Serial.println("salam: " +result);
       //if(result.indexOf("<EOM")>0)
       {
          if(result.indexOf("on")>0)
          {
            String str = "turned on";
            Serial.println(str);
            digitalWrite(5,LOW);
            //AT+CIPSEND=
            ESP8266.println("AT+CIPSEND=0,"+ str.length());
            delay(500);
            ESP8266.println("AT+CIPSEND=0,"+ str);
          }
          else if(result.indexOf("off")>0)
          {
            String str = "turned off";
            Serial.println(str);
            digitalWrite(5,HIGH);
           
            ESP8266.println("AT+CIPSEND=0,"+ str.length());
            delay(500);
            ESP8266.println("AT+CIPSEND=0,"+ str);
          }
          else
          {
      // do something
          }
       }
    }

    if ( Serial.available())
    { 
      ESP8266.write( Serial.read() );
    }

}

*/