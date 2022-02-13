#ifndef REPORTER_H
#define REPORTER_H

#include "../essential/util.h"

#ifdef REPORTER_WIFI
#include "esp8266.cpp"
ESP8266Com *esp_com;
#endif

class Reporter
{
private:
    long last_check;

public:
    Reporter();
    void setup();
    void send(StateData *state_data);
    void report(StateData *state_data, unsigned int interval);
};

#endif
