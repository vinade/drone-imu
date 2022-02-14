#ifndef ESP8266_COM_H
#define ESP8266_COM_H

#define ESPCOM_RATE_OPERATION 9600

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
