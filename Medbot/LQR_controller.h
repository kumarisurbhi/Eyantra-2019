/*
  ▪ * Team Id: eYRC#804
  ▪ * Author List:
  ▪ * Filename: LQR_controller.h
  ▪ * Theme: Biped Patrol
  ▪ * Functions: lqr(), move_motor()
  ▪ */

float reqVelocity = 0, reqDistance = 0, reqRoll = 0, reqOmega = 0;
float errorV = 0, errorD = 0, errorR = 0, errorO = 0;
float distance = 0, velocity = 0, omega = 0;
float U = 0, U_new = 0;
float SLOPE_ANGLE = (0.01745 * 15);
int dummy = 0;
int left = 0, right = 0;

void moveMotor()
{
  if (left > 0)
  {
    digitalWrite(InAL, HIGH);
    digitalWrite(InBL, LOW);
  }
  else if (left < 0)
  {
    digitalWrite(InAL, LOW);
    digitalWrite(InBL, HIGH);
  }
  else
  {
    digitalWrite(InAL, LOW);
    digitalWrite(InBL, LOW);
  }

  if (right > 0)
  {
    digitalWrite(InAR, HIGH);
    digitalWrite(InBR, LOW);
  }
  else if (right < 0)
  {
    digitalWrite(InAR, LOW);
    digitalWrite(InBR, HIGH);
  }
  else
  {
    digitalWrite(InAR, LOW);
    digitalWrite(InBR, LOW);
  }

  left = map(abs(left), 0, 255, 20, 255);
  right = map(abs(right), 0, 255, 30, 255);
  analogWrite(PWML, left);
  analogWrite(PWMR, right);
}
void lqr()
{
  float k[4] = { -25.5764, -27.4429, -70.7773, -9.1915};

  if (forward_flag)
  {
    k[0] = 0;
    dummy = map(x_axis_val, 0, 400, -100, 0);
    reqVelocity = (circumference * dummy) / 60;
    //reqVelocity = 0.3;
    distance = 0;
  }

  else if (backward_flag)
  {
    k[0] = 0;
    dummy = map(x_axis_val, 600, 1023, 0, 70);
    reqVelocity = (circumference * dummy) / 60 ;
    //reqVelocity = -0.3;
    distance = 0;
  }

  else if (right_flag)
  {
    k[0] = 0;
    reqVelocity = 0.03;
    distance = 0;
  }

  else if (left_flag)
  {
    k[0] = 0;
    reqVelocity = 0.03;
    distance = 0;
  }

  else
  {
    k[0] = 0;
    reqDistance = 0;
    reqVelocity = 0;
    reqRoll = 0;
    reqOmega = 0;
  }

  if (slope_flag)
  {
    //reqDistance+= (0.0305/4);
    reqRoll -= 0.0174 * 0.5 ;
    //reqVelocity = -0.3;
    reqRoll = constrain(reqRoll, 0, SLOPE_ANGLE);
    distance = 0;
  }
  errorD = (distance - reqDistance);
  errorV = (velocity - reqVelocity);
  errorR = (roll_rad - reqRoll) + 0.0174 * 3 ;
  errorO = (omega - reqOmega);

  U = ( - k[0] * errorD - k[1] * errorV - k[2] * errorR - k[3] * errorO);
  U_new = constrain(U * 255 / 12 , -255, 255);
  if (right_flag)
  {
    left = (U_new - 50);
    right = (U_new + 50) ;
  }

  else if (left_flag)
  {
    left = (U_new + 50);
    right = (U_new - 50) ;
  }
  else
  {
    left = U_new;
    right = U_new;
  }

  moveMotor();
}
