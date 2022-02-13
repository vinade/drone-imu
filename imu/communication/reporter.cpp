#ifndef REPORTER_CPP
#define REPORTER_CPP

#include "reporter.h"

Reporter::Reporter()
{
#ifdef REPORTER_WIFI
    esp_com = new ESP8266Com();
#endif
}

void Reporter::setup()
{
#ifdef REPORTER_WIFI
    esp_com->setup();
    Serial.println("WIFI CONFIGURADA");
    esp_com->send_cmd("AT+CIFSR", 300);

    delay(1000);
    while (esp_serial.available())
    {
        Serial.print((char)(esp_serial.read()));
    }

#endif
    this->last_check = millis();
}

void Reporter::send(StateData *state_data)
{
    uint8_t l;

    l = sizeof(*state_data);
    Serial.write((byte *)&l, 1);
    Serial.write((byte *)state_data, l);
    delay(300);
}

void Reporter::report(StateData *state_data, unsigned int interval)
{
    if (millis() - this->last_check > interval)
    {

#ifdef REPORTER_WIFI
        esp_com->send(state_data);
#else
        this->send(state_data);
#endif

        this->last_check = millis();
    }
}

#endif
