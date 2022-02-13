#ifndef LED_ACTOR_H
#define LED_ACTOR_H

#define LED_TYPE_STATIC 0
#define LED_TYPE_BLINK 1

#define LED_OFF false
#define LED_ON true

#define LED_DEFAULT_TYPE 0
#define LED_DEFAULT_INITIAL false
#define LED_DEFAULT_INTERVAL 1000
#define LED_MINIMUM_INTERVAL 10

#include "../essential/util.h"

class LEDActor
{
private:
    uint8_t pin;
    long last_update;
    boolean state;
    boolean led_state;

public:
    uint8_t led_type;
    uint8_t interval;

    LEDActor(uint8_t pin);
    void set(uint8_t state);

    void setup(uint8_t led_type);
    void setup(uint8_t led_type, uint8_t initial);
    void setup(uint8_t led_type, uint8_t initial, uint8_t interval);

    void run();
    void update_led();
};

#endif