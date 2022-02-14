#define REPORTER_WIFI

// #include "sensors/basicSensor.cpp"
#include "sensors/bmp180.cpp"
#include "sensors/hcsr04.cpp"
#include "sensors/gps.cpp"
#include "sensors/imu.cpp"

#include "communication/reporter.cpp"

BMP180Sensor *barometer;
HCSR04Sensor *ultrasonic;
IMUSensor *imu;
GPSSensor *gps;
Reporter *communication;

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;

  Wire.begin();

  barometer = new BMP180Sensor();
  barometer->setup_instance();

  ultrasonic = new HCSR04Sensor(12, 13);
  ultrasonic->setup_instance();

  gps = new GPSSensor();
  gps->setup();

  imu = new IMUSensor();
  imu->setup_instance();

  communication = new Reporter();
  communication->setup();
}

void loop()
{

  imu->try_update();

  Serial.print("\tangleX:");
  Serial.print(device_state.angle.x);
  Serial.print("\tangleY:");
  Serial.print(device_state.angle.y);
  Serial.print("\tangleZ:");
  Serial.print(device_state.angle.z);

//  gps->try_update();
  //  if (gps->ready)
  //  {
  //    Serial.println("====== COORDS:");
  //    Serial.println(gps->coords.x);
  //    Serial.println(gps->coords.y);
  //    Serial.println(gps->coords.z);
  //  }

  communication->report(&device_state, 20);
}
