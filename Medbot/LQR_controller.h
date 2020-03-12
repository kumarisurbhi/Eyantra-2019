
/*Global Variables*/
float reqVelocity = 0, reqDistance = 0, reqRoll = 0, reqOmega = 0;
float errorVelocity = 0, errorDistance = 0, errorRoll = 0, errorOmega = 0;
const float angle_offset = (0.01745 * 3);
int dummy = 0;
float U = 0, U_new = 0;
/******************/

void lqr()
{
  //Gain matrix optained from octave for sampling time of 7 ms
  float k[4] = { -34.120, -30.453, -119.949, -10.274};        // { distance, velocity, angle, angular velocity } 

  // Slow flag for precise movement in pick-place area
  if (slow_flag)                     
  {
    // Move the bot backward
    if (backward_flag)               
    { 
      k[1] += k[0];                                           // distance constrain is added to velocity constrain
      k[0] = 0;                                               // distance constrain made zero
      dummy = map(x_axis_val, 600, 1023, 0, -25 );            // mapped values received from joystick to rpm for variable speed
      reqVelocity = (circumference * dummy) / 60 ;            // compute velocity from rpm
      distance = 0;                                           // set distance covered by the bot to zero
    }

    // Move the bot backward
    else if (forward_flag)          
    {
      k[1] += k[0];                 
      k[0] = 0;
      dummy = map(x_axis_val, 0, 400, 25, 0);
      reqVelocity = (circumference * dummy) / 60;
      distance = 0;
    }

    // Rotate the robot to the right
    else if (right_flag)       
    {
      k[0] = 0;
      reqVelocity = 0.0;
      dummy = map(y_axis_val, 0, 400, 50, 0);
      distance = 0;
    }

    // Rotate the robot to the right
    else if (left_flag)
    {
      k[0] = 0;
      reqVelocity = 0.00;       
      dummy = map(y_axis_val, 600, 1023, 0, 50 );
      distance = 0;
    }

    else
    {
      // For normal balancing
      k[1] += k[0];
      k[0] = 0;
      reqDistance = 0;
      reqVelocity = 0;
      reqRoll = 0;
      reqOmega = 0;
    }
  }
  else                                
  {
    // When traversing in the normal arena
    reqRoll = 0;
    if (forward_flag)
    {
      k[1] += k[0];
      k[0] = 0;
      dummy = map(x_axis_val, 0, 400, 70, 0);
      reqVelocity = (circumference * dummy) / 60;
      distance = 0;
    }
    else if (backward_flag)                
    {
      k[1] += k[0];
      k[0] = 0;
      dummy = map(x_axis_val, 600, 1023, 0 , -90 );    
      reqVelocity = (circumference * dummy) / 60 ;
      distance = 0;
    }
    else if (right_flag)
    {
      k[0] = 0;
      reqVelocity = 0.0;
      dummy = map(y_axis_val, 0, 400, 70, 0);
      distance = 0;
    }
    else if (left_flag)
    {
      k[0] = 0;
      reqVelocity = 0.00;
      dummy = map(y_axis_val, 600, 1023, 0 , 70 );
      distance = 0;
    }
    else          
    {
      k[1] += k[0];
      k[0] = 0;
      reqDistance = 0;
      reqVelocity = 0;
      reqRoll = 0;
      reqOmega = 0;
    }
  }
  
  errorDistance = (distance - reqDistance);           //Error in the distance covered
  errorVelocity = (velocity - reqVelocity);           //Error in the velocity
  errorRoll = (roll - reqRoll) + angle_offset ;       //Error in the tilt angle
  errorOmega = (omega - reqOmega);                    //Error in angular velocity

  U = ( - k[0] * errorDistance - k[1] * errorVelocity - k[2] * errorRoll - k[3] * errorOmega);
  U_new = constrain(U * 255 / 12 , -255, 255);

  if (right_flag)       
  {
    // Offset is added to left part and subtracted from right part 
    moveMotor(U_new + dummy, U_new - dummy) ; 
  }
  else if (left_flag)  
  {
    // Offset is added to right part and subtracted from left part
    moveMotor(U_new - dummy, U_new + dummy) ;
  }
  else
  {
    // Update the PWM  and direction
    moveMotor(U_new, U_new);
  }
}