/*
▪ * Team Id: eYRC#804
▪ * Author List:  
▪ * Filename: LQR_controller.h
▪ * Theme: Biped Patrol
▪ * Functions: lqr(), move_motor()
▪ */ 
#include "I2Cdev.h"
#include "MPU6050.h"
#include<Wire.h>
#include<math.h>
#include<TimerOne.h>
#include<TimerThree.h>
#include"motors.h"
#include"accel_gyro.h"
#include"remote.h"
#include"electromagnet.h"
#include"LQR_controller.h"

long prevtime = 0;
float left_prev_count = 0, right_prev_count = 0;
float meanrpm = 0, circumference = 0.2041;
float left_RPM = 0, right_RPM = 0;

void readSensor()
{
  sei();
  read_accel();
  read_gyro();
  complimentary_filter_roll();
}

void rpm()
{
  //sei();
  volatile float left_current_count = left_encoder_count;
  volatile float right_current_count = right_encoder_count;

  left_RPM = (float)(((left_current_count - left_prev_count) * 60) / (0.005 * 540 * 2));
  right_RPM = (float)(((right_current_count - right_prev_count) * 60) / (0.005 * 540 * 2));

  left_prev_count = left_current_count;
  right_prev_count = right_current_count;

  meanrpm = (left_RPM + right_RPM) / 2.0;

  velocity = (meanrpm * circumference) / 60;
  distance += (velocity * 0.005);
  omega = g[1] * (3.14 / 180);
}

void setup()
{
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  Serial.begin(9600);

  mpu_init();
  motor_init();
  MAG_init();
  Timer1.initialize(3000);
  Timer1.attachInterrupt(readSensor);
  Timer3.initialize(5000);
  Timer3.attachInterrupt(rpm);
}

void loop()
{
  read_joystick();
  if ((micros() - prevtime) >= 10000)
  {
    lqr();
    prevtime = micros();
  }
}
