#ifndef BASIC_SENSOR_H
#define BASIC_SENSOR_H

#include "../essential/util.cpp"
#include "../essential/state.h"

#define SENSOR_HEALTH 0
#define SENSOR_NOT_HEALTH 1
#define SENSOR_NOT_USEFUL 2
#define SENSOR_IMPRECISE 4

#define SENSOR_AVAILABLE true
#define SENSOR_NOT_AVAILABLE false

#define SENSOR_FAMILY_BATTERY_LEVEL 0
#define SENSOR_FAMILY_BATTERY_USAGE 1
#define SENSOR_FAMILY_GYROSCOPE 2
#define SENSOR_FAMILY_ACCELEROMETER 4
#define SENSOR_FAMILY_ALTITUDE 8
#define SENSOR_FAMILY_POSITION 16
#define SENSOR_FAMILY_GPS 32
#define SENSOR_FAMILY_MAGNETOMETER 64
#define SENSOR_FAMILY_VELOCITY 128
#define SENSOR_FAMILY_ULTRASONIC 256

#define SENSOR_DEFAULT_UPDATE_INTERVAL 10
#define SENSOR_MINIMUM_UPDATE_INTERVAL 1

class BasicSensor
{
protected:
    uint32_t last_update = 0;
    BasicSensor *fallback;
    uint8_t family;
    unsigned int *fail_state;
    unsigned int *warning_state;

public:
    static uint8_t minimum_update_interval;
    static boolean static_setup_ready;

    uint8_t warning = 0;
    boolean available = SENSOR_AVAILABLE;

    void *read();
    void set_fallback(BasicSensor *fallback_sensor);
    void set_fail_state(unsigned int *sensor_fail_state);
    void set_warning_state(unsigned int *sensor_warning_state);
    void fail();
    void warn();
    void try_update();

    // to overwrite
    static void setup(){};
    virtual void update(){};
    virtual void setup_instance(){};
    virtual void *get_data() { return NULL; };
};

#endif
