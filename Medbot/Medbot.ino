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
#include"accel_gyro.h"
#include"remote.h"
#include"electromagnet.h"
#include"motors.h"
#include"LQR_controller.h"

/*Global Variables*/
long prevtime = 0;
/*****************/

/*
   Function Name: readSensor
   Input: None
   Output: None
   Logic: To read roll and angular velocity of the bot 
   Example Call: readSensor();
*/
void readSensor()
{
  sei();                           //To read data from sensor when used as interrupt
  read_accel();      
  read_gyro();
  complimentary_filter_roll();
}

/*
   Function Name: setup
   Input: None
   Output: None
   Logic: To initialise all the hardware components 
*/
void setup()
{
  // Initialize I2C bus
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  Serial2.begin(9600);                    //Initialize Serial communication

  mpu_init();                             // Initialize GY-87
  motor_init();                           // Initialize motors
  MAG_init();                             // Initialize electromagnets
  rgb_init();                             // Initialize RGB LED
  buzzer_init();                          // Initilalize buzzer
  Timer1.initialize(3000);                // Timer of 3 ms for reading GY-87
  Timer1.attachInterrupt(readSensor);
  Timer3.initialize(5000);                // Timer of 5 ms for rpm measurement
  Timer3.attachInterrupt(rpm);
}

/*
   Function Name: loop
   Input: None
   Output: None
   Logic: 
*/
void loop()
{
  read_joystick();                 // Reads data from remote

  led_buzzer();                   // To control buzzer
  magnet();                      // To control electromagnets

  // Calculates effort every 7 ms and update motors
  if ((micros() - prevtime) >= 7000)
  {
    lqr();
    prevtime = micros();
  }

}
