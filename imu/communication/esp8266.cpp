#ifndef ESP8266_COM_CPP
#define ESP8266_COM_CPP

#include "esp8266.h"

void ESP8266Com::setup()
{
    esp_serial.begin(ESPCOM_RATE_OPERATION);
    delay(500);

    esp_serial.print("AT+CIPSTART=\"TCP\",\"");
    esp_serial.print(IP);
    esp_serial.print("\",");
    esp_serial.print(PORT);
    esp_serial.print("\r\n");

    delay(2000);
    // while (esp_serial.available())
    // {
    //     Serial.print((char)(esp_serial.read()));
    // }
    // Serial.println("");
}

void ESP8266Com::send(StateData *state_data)
{
    uint8_t l = sizeof(*state_data);

    esp_serial.print(F("AT+CIPSEND="));
    esp_serial.print(l);
    esp_serial.print(F("\r\n"));
    delay(10);

    esp_serial.write((byte *)state_data, l);
    delay(10);
}

void ESP8266Com::send_cmd(String cmd, uint16_t wait_time)
{
    cmd += "\r\n";
    esp_serial.print(cmd);
    delay(wait_time);
    // while (esp_serial.available())
    // {
    //     Serial.print((char)esp_serial.read());
    // }
    // Serial.println("");
}

#endif
