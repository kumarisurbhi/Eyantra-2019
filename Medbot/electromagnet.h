/*
  ▪ * Team Id: eYRC#804
  ▪ * Author List:
  ▪ * Filename: electromagnet.h
  ▪ * Theme: Biped Patrol
  ▪ * Functions: MAG_init()
  ▪
*/

//Pin definitions
#define Mag1            31
#define Mag2            8
#define buzzer          42
#define red             33
#define green           36
#define blue            39
#define common          34

/*Global Variables*/
long starttimer = 0, stoptimer = 0;
int count = 0; 
/*****************/

/*
 * Function Name: MAG_init
 * Input: None
 * Output: None
 * Logic: Initialize the electromagnet
 * Example Call: MAG_init();
 */
void MAG_init()
{
  pinMode(Mag1, OUTPUT);
  pinMode(Mag2, OUTPUT);
  digitalWrite(Mag1, LOW);
  digitalWrite(Mag2, LOW);
}

/*
 * Function Name: rgb_init
 * Input: None
 * Output: None
 * Logic: Initialize the RGB Led
 * Example Call: rgb_init();
 */ 
void rgb_init()
{
  pinMode(red, OUTPUT);
  pinMode(common, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
  digitalWrite(blue, HIGH);
  digitalWrite(common, HIGH);
}

/*
 * Function Name: buzzer_init
 * Input: None
 * Output: None
 * Logic: Initialize the buzzer
 * Example Call: buzzer_init();
 */
void buzzer_init()
{
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
}

/*
 * Function Name: magnet
 * Input: None
 * Output: None
 * Logic: Activates the magnet when received from remote
 * Example Call: magnet();
 */
void magnet()
{
  if (button_2 == 1)
  {
    digitalWrite(Mag1, HIGH);
  }
  else
  {
    digitalWrite(Mag1, LOW);
  }

  if (button_4 == 1)
  {
    digitalWrite(Mag2, HIGH);
  }
  else
  {
    digitalWrite(Mag2, LOW);
  }
  
}

/*
 * Function Name: led_buzzer
 * Input: None
 * Output: None
 * Logic: To initiate the buzzer and LED for 1000ms 
 * Example Call: led_buzzer();
 */
void led_buzzer()
{
if (button_3 == 1)
  {
    if (count == 0)
    {
      count = 1;
      starttimer = micros();
    }
    if (count == 2)
    {
      count = 3;
      stoptimer = micros();
    }
  }

  if (count == 1)
  {
    if (micros() - starttimer <= 1000000)
    {
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
      digitalWrite(buzzer, LOW);
    }
    else
    {
      count = 2;
      digitalWrite(green, HIGH);
      digitalWrite(red, HIGH);
      digitalWrite(buzzer, HIGH);
    }
  }
  else if (count == 3)
  {
    if (micros() - stoptimer <= 1000000)
    {
      digitalWrite(green, HIGH);
      digitalWrite(red, LOW);
      digitalWrite(buzzer, LOW);
    }
    else
    {
      count = 0;
      digitalWrite(green, HIGH);
      digitalWrite(red, HIGH);
      digitalWrite(buzzer, HIGH);
    }
  }
}
