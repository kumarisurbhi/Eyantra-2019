/*
  ▪ * Team Id: eYRC#804
  ▪ * Author List: Simrat Singh Chitkara, 
  ▪ * Filename: motors.h
  ▪ * Theme: Biped Patrol
  ▪ * Functions: motor_init(), motor_pin_config(),encoder_pin_config(), left_encoder_interrupt(), leftB_encoder_interrupt(), right_encoder_interrupt(), rightB_encoder_interrupt()
  ▪ */

//Pin definitions
#define InAL            9
#define InBL            10
#define PWML            6
#define encodPinAL      2
#define encodPinBL      3

#define InAR            11
#define InBR            12
#define PWMR            4
#define encodPinAR      18
#define encodPinBR      19

/*Global Variables*/
volatile long left_encoder_count = 0, right_encoder_count = 0;
int left = 0, right = 0;
float left_RPM = 0, right_RPM = 0;
const int mode = 4, ppr = 270;
float meanrpm = 0, circumference = 0.2041;
float left_prev_count = 0, right_prev_count = 0;
float distance = 0, velocity = 0;
/*****************/

/*
 * Function Name: motor_pin_config
 * Input: None
 * Output: None
 * Logic: To configure the motor pins
 * Example Call: motor_pin_config();
 */ 
void motor_pin_config()
{
  pinMode(InAL, OUTPUT);
  pinMode(InBL, OUTPUT);
  pinMode(InAR, OUTPUT);
  pinMode(InBR, OUTPUT);
  pinMode(PWML, OUTPUT);
  pinMode(PWMR, OUTPUT);
}

/*
 * Function Name: left_encoder_interrupt
 * Input: None
 * Output: None
 * Logic: Handles the interrupt for left encoder channel A
 * Example Call: left_encoder_interrupt();
 */
void left_encoder_interrupt()
{
  int state = digitalRead(encodPinAL);
  if (digitalRead(encodPinBL))
    state ? left_encoder_count-- : left_encoder_count++;
  else
    state ? left_encoder_count++ : left_encoder_count--;
}

/*
 * Function Name: right_encoder_interrupt
 * Input: None
 * Output: None
 * Logic: Handles the interrupt for right encoder channel A
 * Example Call: right_encoder_interrupt();
 */
void right_encoder_interrupt()
{
  int state = digitalRead(encodPinAR);
  if (digitalRead(encodPinBR))
    state ? right_encoder_count++ : right_encoder_count--;
  else
    state ? right_encoder_count-- : right_encoder_count++;
}


/*
 * Function Name: leftB_encoder_interrupt
 * Input: None
 * Output: None
 * Logic: Handles the interrupt for left encoder channel B
 * Example Call: leftB_encoder_interrupt();
 */
void leftB_encoder_interrupt()
{
  int state = digitalRead(encodPinBL);
  if (digitalRead(encodPinAL))
    state ? left_encoder_count++ : left_encoder_count--;
  else
    state ? left_encoder_count-- : left_encoder_count++;
}

/*
 * Function Name: rightB_encoder_interrupt
 * Input: None
 * Output: None
 * Logic: Handles the interrupt for right encoder channel B
 * Example Call: rightB_encoder_interrupt();
 */
void rightB_encoder_interrupt()
{
  int state = digitalRead(encodPinBR);
  if (digitalRead(encodPinAR))
    state ? right_encoder_count-- : right_encoder_count++;
  else
    state ? right_encoder_count++ : right_encoder_count--;
}

/*
 * Function Name: encoder_pin_config
 * Input: None
 * Output: None
 * Logic: To configure the encoder pins(in 4x mode) - 2 Interrupts for each channel
 * Example Call: encoder_pin_config();
 */
void encoder_pin_config()
{
  // Set as input pins and enable internal pull-up
  pinMode(encodPinAL, INPUT_PULLUP);
  pinMode(encodPinBL, INPUT_PULLUP);
  pinMode(encodPinAR, INPUT_PULLUP);
  pinMode(encodPinBR, INPUT_PULLUP);

  // Attach interrupts for the encoder input pins
  attachInterrupt(digitalPinToInterrupt(encodPinAL), left_encoder_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encodPinAR), right_encoder_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encodPinBL), leftB_encoder_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encodPinBR), rightB_encoder_interrupt, CHANGE);

}

/*
 * Function Name: motor_init
 * Input: None
 * Output: None
 * Logic: Initialize the motors and encoders
 * Example Call: motor_init();
 */
void motor_init()
{
  motor_pin_config();
  encoder_pin_config();
}

/*
 * Function Name: rpm
 * Input: None
 * Output: None
 * Logic: Calculates the rpm of motors using encoder count difference
 * Example Call: rpm();
 */

void rpm()
{
  volatile long left_current_count = left_encoder_count;
  volatile long right_current_count = right_encoder_count;

  //Calculated RPM of motors using change in encoder count in a particular time period
  left_RPM = (float)(((left_current_count - left_prev_count) * 60) / (0.005 * ppr * mode));
  right_RPM = (float)(((right_current_count - right_prev_count) * 60) / (0.005 * ppr * mode));

  //Store the current count 
  left_prev_count = left_current_count;
  right_prev_count = right_current_count;

  meanrpm = (left_RPM + right_RPM) / 2.0;     //Compute mean RPM
  velocity = (meanrpm * circumference) / 60;  //Compute velocity
  distance += (velocity * 0.005);             //Compute distance
}

/*
 * Function Name: moveMotor
 * Input: PWM values for left and right motors 
 * Output: None
 * Logic: Update the motors with direction and PWM values
 * Example Call: moveMotor(U_new, U_new);
 */
void moveMotor(int left, int right)
{
  //To move left motor forward
  if (left > 0)
  {
    digitalWrite(InAL, HIGH);
    digitalWrite(InBL, LOW);
    left = map(abs(left), 0, 255, 0, 230);
  }
  //To move left motor backward
  else if (left < 0)
  {
    digitalWrite(InAL, LOW);
    digitalWrite(InBL, HIGH);
    left = map(abs(left), 0, 255, 0, 230);
  }
  else
  {
    //To stop the motor
    digitalWrite(InAL, LOW);
    digitalWrite(InBL, LOW);
  }

  if (right > 0)
  {
    //To move right motor forward
    digitalWrite(InAR, HIGH);
    digitalWrite(InBR, LOW);
    right = map(abs(right), 0, 255, 17, 255);
  }
  else if (right < 0)
  {
    //To move left motor backward
    digitalWrite(InAR, LOW);
    digitalWrite(InBR, HIGH);
    right = map(abs(right), 0, 255, 17 , 255);
  }
  else
  {
    //To stop the motor
    digitalWrite(InAR, LOW);
    digitalWrite(InBR, LOW);
  }
  //Update motors with pwm calculated using LQR
  analogWrite(PWML, left);
  analogWrite(PWMR, right);
}
