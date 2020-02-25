/*
▪ * Team Id: eYRC#804
▪ * Author List: 
▪ * Filename: motors.h
▪ * Theme: Biped Patrol
▪ * Functions: motor_init(), motor_pin_config(),encoder_pin_config(), left_encoder_interrupt(), leftB_encoder_interrupt(), right_encoder_interrupt(), rightB_encoder_interrupt()  
▪ */ 

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
/****************/
void motor_pin_config()
{
  pinMode(InAL, OUTPUT);
  pinMode(InBL, OUTPUT);
  pinMode(InAR, OUTPUT);
  pinMode(InBR, OUTPUT);
  pinMode(PWML, OUTPUT);
  pinMode(PWMR, OUTPUT);
}

void left_encoder_interrupt()
{
  int state = digitalRead(encodPinAL);
  if (digitalRead(encodPinBL))
    state ? left_encoder_count-- : left_encoder_count++;
  else
    state ? left_encoder_count++ : left_encoder_count--;
}

void right_encoder_interrupt()
{
  int state = digitalRead(encodPinAR);
  if (digitalRead(encodPinBR))
    state ? right_encoder_count++ : right_encoder_count--;
  else
    state ? right_encoder_count-- : right_encoder_count++;
}

void leftB_encoder_interrupt()
{
  int state = digitalRead(encodPinBL);
  if (digitalRead(encodPinAL))
    state ? left_encoder_count++ : left_encoder_count--;
  else
    state ? left_encoder_count-- : left_encoder_count++;
}

void rightB_encoder_interrupt()
{
  int state = digitalRead(encodPinBR);
  if (digitalRead(encodPinAR))
    state ? right_encoder_count-- : right_encoder_count++;
  else
    state ? right_encoder_count++ : right_encoder_count--;
}
void encoder_pin_config()
{
  pinMode(encodPinAL, INPUT_PULLUP);
  pinMode(encodPinBL, INPUT_PULLUP);
  pinMode(encodPinAR, INPUT_PULLUP);
  pinMode(encodPinBR, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encodPinAL), left_encoder_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encodPinAR), right_encoder_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encodPinBL), leftB_encoder_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encodPinBR), rightB_encoder_interrupt, CHANGE);

}

void motor_init()
{
  motor_pin_config();
  encoder_pin_config();
}
