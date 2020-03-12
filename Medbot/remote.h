/*
  ▪ * Team Id: eYRC#804
  ▪ * Author List:
  ▪ * Filename: remote.h
  ▪ * Theme: Biped Patrol
  ▪ * Functions: read_joystick()
  ▪ */
/*Global Variables*/
int x_axis_val = 0, y_axis_val = 0;
int button_1 = 0, button_2 = 0, button_3 = 0, button_4 = 0;
bool forward_flag = false;
bool backward_flag = false;
bool right_flag = false;
bool left_flag = false;
bool slow_flag = false;


//joystick limits
const int x_upper = 600, x_lower = 400, y_upper = 700, y_lower = 300;
/*****************/

/*
 * Function Name: read_joystick
 * Input: None
 * Output: None
 * Logic: To read the data from remote controller and accordingly set flags for differents modes
 * Example Call: read_joystick();
 */ 
void read_joystick()
{
  // Controller Variables 
  int sum = 131;
  unsigned char byte_discard, checksum;
  unsigned char digital_data = 0, digital_data_1 = 0;
  unsigned char AD0[2] = {0, 0}, AD1[2] = {0, 0};

  // Check frame length
  if (Serial2.available() >= 18)
  {
    // Check Start Byte of the API Frame
    if (Serial2.read() == 0x7E)
    {
       // Read and discard 2 bytes
      for (int i = 0; i < 2; i++)
        byte_discard = Serial2.read();

      // Check Frame Type
      if (Serial2.read() != 0x83) return;

      // Read and discard 7 bytes
      for (int i = 0; i < 7; i++)
        sum += Serial2.read();

      //Read Digital Byte
      digital_data_1 = Serial2.read();
      button_1 = digital_data_1;
      sum += digital_data_1;

      //Read Digital Byte
      digital_data = Serial2.read();
      sum += digital_data;

      // Read ADC Data
      AD0[1] = Serial2.read();
      AD0[0] = Serial2.read();
      AD1[1] = Serial2.read();
      AD1[0] = Serial2.read();
      sum = sum + AD0[0] + AD0[1] + AD1[0] + AD1[1];

      // Read Checksum
      checksum = 0xFF - (0xFF & (unsigned char)sum);
      byte_discard = Serial2.read();

      // Discard frame if checksum does not match
      if (byte_discard != checksum) return;

      // Update Controller Variables
      if ((digital_data & 0x08) == 0x08)
        button_2 = 1;
      else
        button_2 = 0;

      if ((digital_data & 0x10) == 0x10)
        button_3 = 1;
      else
        button_3 = 0;

      if ((digital_data & 0x04) == 0x04)
        button_4 = 1;
      else
        button_4 = 0;

      // Combine bytes and extract input from joystick
      y_axis_val = AD0[0] + (AD0[1] * 256);
      x_axis_val = AD1[0] + (AD1[1] * 256);

      // Set flags according to the data
      if (y_axis_val >= y_upper)
      {
        forward_flag  = false;
        backward_flag = false;
        right_flag    = true;             //Fr right rotation
        left_flag     = false;
        slow_flag    = false;
      }
      else if (y_axis_val <= y_lower)
      {
        forward_flag  = false;
        backward_flag = false;
        right_flag    = false;
        left_flag     = true;             //For left rotation
        slow_flag    = false;
      }
      else if (x_axis_val >= x_upper)
      {
        forward_flag  = true;             //For forward movement
        backward_flag = false;
        right_flag    = false;
        left_flag     = false;
        slow_flag    = false;
      }
      else if (x_axis_val <= x_lower)
      {
        forward_flag  = false;
        backward_flag = true;             //For backward movement
        right_flag    = false;
        left_flag     = false;
        slow_flag    = false;
      }
      else
      {
        //normal balancing when nothing pressed 
        forward_flag  = false;
        backward_flag = false;
        right_flag    = false;
        left_flag     = false;
        slow_flag    = false;
      }

      if (button_1)
      { 
        //slow mode for pickup and drop zone 
        slow_flag = true;
      }
      else
      {
        slow_flag = false;
      }
    }
  }
}
