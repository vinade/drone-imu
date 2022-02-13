#ifndef LED_ACTOR_CPP
#define LED_ACTOR_CPP

#include "led.h"

LEDActor::LEDActor(uint8_t pin)
{
    this->pin = pin;
}

void LEDActor::set(uint8_t state)
{
    this->state = state;
    this->update_led();
}

void LEDActor::setup(uint8_t led_type)
{
    this->setup(led_type, LED_DEFAULT_INITIAL, LED_DEFAULT_INTERVAL);
}

void LEDActor::setup(uint8_t led_type, uint8_t initial)
{
    this->setup(led_type, initial, LED_DEFAULT_INTERVAL);
}

void LEDActor::setup(uint8_t led_type, uint8_t initial, uint8_t interval)
{
    this->led_type = led_type;
    this->state = initial;
    this->interval = interval;

    pinMode(this->pin, OUTPUT);
    this->update_led();
}

void LEDActor::update_led()
{
    digitalWrite(this->pin, this->state ? HIGH : LOW);
    this->led_state = this->state;
    this->last_update = millis();
}

void LEDActor::run()
{
    if (this->led_type == LED_TYPE_STATIC)
    {
        if (this->state == this->led_state)
        {
            return;
        }

        this->update_led();
        return;
    }

    if (this->led_type == LED_TYPE_BLINK)
    {
        unsigned int last_interval = millis() - this->last_update;
        if (last_interval > this->interval)
        {
            this->state = !this->state;
            this->update_led();
        }

        return;
    }
}

#endif