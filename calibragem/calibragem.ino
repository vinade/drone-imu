#include <EEPROM.h>

#define CALIBRATION_SCRIPT

#include "sensors/mpu6050.cpp"
#include "sensors/qmc5883l.cpp"
#include "actors/led.cpp"

#define LED_STATUS_PIN 4
#define EEPROM_GYRO_B_OFFSET 1
#define EEPROM_MAGNETO_OFFSET 2

QMC5883LSensor *compass;
MPU6050Sensor *gyro_a;
MPU6050Sensor *gyro_b;
LEDActor *led_status;

void log_offset(String label, vec3float *offset)
{
  Serial.print(label);
  Serial.print(offset->x);
  Serial.print(offset->y);
  Serial.println(offset->z);
}

void write_offsets()
{
  log_offset("giroscopio A:", &gyro_a->gyro_offset);
  write_on_eeprom(EEPROM_GYRO_A_OFFSET, &gyro_a->gyro_offset);

  log_offset("giroscopio B:", &gyro_b->gyro_offset);
  write_on_eeprom(EEPROM_GYRO_B_OFFSET, &gyro_b->gyro_offset);

  log_offset("magnetometro:", &compass->offset);
  write_on_eeprom(EEPROM_MAGNETO_OFFSET, &compass->offset);
}

void initialization()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  } // On attend que le port série soit disponible
  delay(300);

  Wire.begin();
  led_status = new LEDActor(LED_STATUS_PIN);
  led_status->setup(LED_TYPE_STATIC);

  delay(300);
}

void gauge_gyroscope()
{
  led_status->set(LED_ON);

  gyro_a = new MPU6050Sensor(I2C_ADDRESS_MPU6050_A);
  gyro_a->setup_instance();
  gyro_a->calibrate(MPU6050_STATIC_SAMPLE);

  gyro_b = new MPU6050Sensor(I2C_ADDRESS_MPU6050_B);
  gyro_b->setup_instance();
  gyro_b->calibrate(MPU6050_STATIC_SAMPLE);
}

void transition()
{
  led_status->set(LED_OFF);
  delay(3000);
}

void gauge_magnetometer()
{
  led_status->set(LED_ON);

  compass = new QMC5883LSensor();
  compass->setup_instance();

  compass->calibrate();
}

void done()
{
  led_status->led_type = LED_TYPE_BLINK;
  led_status->interval = 1000;
}

void setup()
{
  Serial.println("inicio");
  initialization();
  Serial.println("giro");
  gauge_gyroscope();
  Serial.println("transition");
  transition();
  Serial.println("magneto");
  gauge_magnetometer();
  Serial.println("write");
  write_offsets();
  Serial.println("transition");
  transition();
  Serial.println("done");
  done();
  Serial.println("inicio do loop");
}

void loop()
{
  led_status->run();
}
