/*
  ▪ * Team Id: eYRC#804
  ▪ * Author List:
  ▪ * Filename: accel_gyro.h
  ▪ * Theme: Biped Patrol
  ▪ * Functions: mpu_init(), test(), set_offsets(), read_accel(), read_gyro(), low_pass_filter(float a[0], a[1], a[2]), high_pass_filter(g[0], g[1], g[2]), complimentary_filter_roll()
  ▪ */

/*Global Variables*/
int16_t ax, ay, az, gx, gy, gz, gnx, gny, gnz;
float a[3] = {0, 0, 0}, g[3] = {0, 0, 0};

const float pi = 3.14159265359, f_cut = 5, dT = 0.003, comp_alpha = 0.01;
float Tau = 1 / (2 * pi*f_cut);   // Time constant
float alpha = Tau / (Tau + dT);
float roll_deg = 0;
float roll = 0, omega = 0;

const float accel_sf = 16384, gyro_sf = 131; //scaling factor for accelerometer and gyroscope
float lpx = 0, lpy = 0, lpz = 0, hpx = 0, hpy = 0, hpz = 0;
int m = 1, n = 1;
/******************/

// Created object of class MPU6050
MPU6050 mpu;

/*
 * Function Name: test
 * Input: None
 * Output: None
 * Logic: To check the connections of the sensor
 * Example Call: test();
 */ 
void test()
{
  Serial2.println("Testing device connections...");
  Serial2.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

/*
 * Function Name: set_offsets
 * Input: None
 * Output: None
 * Logic: To calibrate the accelerometer and gyroscope and set offsets
 * Example Call: set_offsets();
 */
void set_offsets()
{
  // Offsets calculated by putting gy-87 on a flat surface for 5 minutes
  mpu.setXAccelOffset(-5699);   // Set offset for X acceleration
  mpu.setYAccelOffset(-645);    // Set offset for Y acceleration
  mpu.setZAccelOffset(1237);    // Set offset for Z acceleration

  mpu.setXGyroOffset(-104);     //Set offset for X gyro
  mpu.setYGyroOffset(15);       //Set offset for Y gyro
  mpu.setZGyroOffset(-10);      //Set offset for Z gyro
}

/*
 * Function Name: low_pass_filter
 * Input: accelerometer values in 3 axis
 * Output: None
 * Logic: To filter the accelerometer readings
 * Example Call: low_pass_filter(a[0], a[1], a[2]);
 */ 
void low_pass_filter(float Ax, float Ay, float Az)
{
  if (n == 1)
  {
    lpx = (1 - alpha) * Ax;    //Low Pass filtered AccelX
    lpy = (1 - alpha) * Ay;    //Low Pass filtered AccelY
    lpz = (1 - alpha) * Az;    //Low Pass filtered AccelZ
    n++;
  }
  else
  {
    lpx = (1 - alpha) * Ax + alpha * lpx;
    lpy = (1 - alpha) * Ay + alpha * lpy;
    lpz = (1 - alpha) * Az + alpha * lpz;
  }
}

/*
 * Function Name: high_pass_filter
 * Input: gyroscope values in 3 axis
 * Output: None
 * Logic: To filter the gyroscope readings
 * Example Call: high_pass_filter(g[0], g[1], g[2]);
 */
void high_pass_filter(float Gx, float Gy, float Gz)
{
  if (m == 1)
  {
    hpx = (1 - alpha) * Gx;     //High Pass filtered GyroX
    hpy = (1 - alpha) * Gy;     //High Pass filtered GyroY
    hpz = (1 - alpha) * Gz;     //High Pass filtered GyroZ
    m++;
  }
  else
  {
    hpx = (1 - alpha) * (hpx + Gx - gnx);
    hpy = (1 - alpha) * (hpy + Gy - gny);
    hpz = (1 - alpha) * (hpz + Gz - gnz);
  }
  gnx = Gx;
  gny = Gy;
  gnz = Gz;
}

/*
 * Function Name: read_accel
 * Input: None
 * Output: None
 * Logic: Reads the acceleration along X,Y,Z axes
 * Example Call: read_accel()
 */ 

void read_accel()
{
  mpu.getAcceleration(&ax, &ay, &az);       //To get raw acceleration values 
  a[0] = (ax / accel_sf);                   //Divide by scaling factor
  a[1] = (ay / accel_sf); 
  a[2] = (az / accel_sf);
  low_pass_filter(a[0], a[1], a[2]);
}

/*
 * Function Name: read_gyro
 * Input: None
 * Output: None
 * Logic: To read the angular velocity from Gyroscope
 * Example Call: read_gyro()
 */
void read_gyro()
{
  mpu.getRotation(&gx, &gy, &gz);          //To get raw gyro values 
  g[0] = (gx / gyro_sf);                   //Divide by scaling factor
  g[1] = (gy / gyro_sf);
  g[2] = (gz / gyro_sf);
  high_pass_filter(g[0], g[1], g[2]);
}

/*
 * Function Name: complimentary_filter_roll
 * Input: None
 * Output: None
 * Logic: Fuse the accel ang gyro values
 * Example Call: complimentary_filter_roll()
 */
void complimentary_filter_roll()
{
  // Compute the roll angle by fusing angles from accel and gyro 
  roll_deg = (1 - comp_alpha) * (roll_deg + g[1] * dT) + (comp_alpha) * (atan(a[0] / abs(a[2]))) * (180 / 3.14); 
  // Convert the angle to radians
  roll = roll_deg * (3.14 / 180);
  // Calculate angular velocity 
  omega = g[1] * (pi / 180);        
}

/*
 * Function Name: mpu_init
 * Input: None
 * Output: None
 * Logic: Initialize the accelerometer and gyroscope  
 * Example Call: read_accel()
 */ 
void mpu_init()
{
  mpu.initialize();                // Initialise GY-87
  test();                          // Test connections
  mpu.setFullScaleAccelRange(0);   // Set Accel range to 2g
  mpu.setFullScaleGyroRange(0);    // Set Gyro range to 131 
  set_offsets();                   // Set optimum offsets
}
