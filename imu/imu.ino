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

//  Serial.println("[setup] done.");
}

void loop()
{

  imu->try_update();

  Serial.print("\tconst_max:");
  Serial.print(1);
  Serial.print("\tconst_min:");
  Serial.print(-1);
  Serial.print("\tsize:");
  Serial.print(sqrt(device_state.raw_magneto.x*device_state.raw_magneto.x + device_state.raw_magneto.y*device_state.raw_magneto.y +device_state.raw_magneto.z*device_state.raw_magneto.z));
  Serial.print("\tangleX:");
  Serial.print(device_state.magneto.x);
  Serial.print("\tangleY:");
  Serial.print(device_state.magneto.y);
  Serial.print("\tangleZ:");
  Serial.print(device_state.magneto.z);
  Serial.print("\tinitX:");
  Serial.print(device_state.initial_mag.x);
  Serial.print("\tinitY:");
  Serial.print(device_state.initial_mag.y);
  Serial.print("\tinitZ:");
  Serial.println(device_state.initial_mag.z);
//  Serial.print("\traw_angleX:");
//  Serial.print(device_state.raw_magneto.x);
//  Serial.print("\traw_angleY:");
//  Serial.print(device_state.raw_magneto.y);
//  Serial.print("\traw_angleZ:");
//  Serial.println(device_state.raw_magneto.z);

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
