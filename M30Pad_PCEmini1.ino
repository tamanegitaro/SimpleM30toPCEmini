#include <Joystick.h>

#define BUTTON_UP 0
#define BUTTON_DOWN 1
#define BUTTON_LEFT 2
#define BUTTON_RIGHT 3
#define BUTTON_A 4
#define BUTTON_B 5
#define BUTTON_C 6
#define BUTTON_X 7
#define BUTTON_Y 8
#define BUTTON_Z 9
#define BUTTON_S 10
#define BUTTON_M 11
#define BUTTON_SPECIAL 12

#define PAD_3B 0
#define PAD_6B 1

//Configure joystick as JOYSTICK type, 10 buttons, and X & Y axis enabled
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 14, 1, 
  true, true, true, false, false, true, false, false, false, false, false);

void setup() 
{
  //Configure pins
  pinMode(3, INPUT_PULLUP); //D0
  pinMode(4, INPUT_PULLUP); //D1
  pinMode(5, INPUT_PULLUP); //D2
  pinMode(6, INPUT_PULLUP); //D3
  pinMode(7, INPUT_PULLUP); //D4
  pinMode(8, INPUT_PULLUP); //D5
  digitalWrite(9, LOW); //Set SELECT pin as low
  pinMode(9, OUTPUT); //SELECT
    
  // initialize Timer1 to trigger 8ms interval interrupts
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 500; // compare match register 16MHz/256/125Hz 8ms interval
//  OCR1A = 250; // compare match register 16MHz/256/250Hz 4ms interval
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12); // 256 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts(); // enable all interrupts

  //Set Joystick default state
  Joystick.begin(0); //Begin Joystick with Auto sendstate disabled
  Joystick.setXAxisRange(0, 255);
  Joystick.setYAxisRange(0, 255);
  Joystick.setXAxis(0x7F); //Set X axis as neutral
  Joystick.setYAxis(0x7F); //Set Y axis as neutral

  Joystick.setZAxisRange(0, 255);
  Joystick.setZAxis(0x7F);
  Joystick.setRzAxisRange(0, 255);
  Joystick.setRzAxis(0x7F);

  Joystick.setHatSwitch(0, JOYSTICK_HATSWITCH_RELEASE);
  
  Joystick.setButton(0, 0); //Set all buttons low
  Joystick.setButton(1, 0); //Set all buttons low
  Joystick.setButton(2, 0); //Set all buttons low
  Joystick.setButton(3, 0); //Set all buttons low
  Joystick.setButton(4, 0); //Set all buttons low
  Joystick.setButton(5, 0); //Set all buttons low
  Joystick.setButton(6, 0); //Set all buttons low
  Joystick.setButton(7, 0); //Set all buttons low
  Joystick.setButton(8, 0); //Set all buttons low
  Joystick.setButton(9, 0); //Set all buttons low
  Joystick.setButton(10, 0); //Set all buttons low
  Joystick.setButton(11, 0); //Set all buttons low
  Joystick.setButton(12, 0); //Set all buttons low
  Joystick.setButton(13, 0); //Set all buttons low
  Joystick.sendState();
}

int Get_6B_Pad(int *button)
{
  int ret = PAD_3B;
  //Count 0
  button[BUTTON_S] = !digitalRead(8);
  button[BUTTON_A] = !digitalRead(7);
  
  //Set SELECT HIGH Advance counter to 1
  digitalWrite(9, HIGH);
  button[BUTTON_C] = !digitalRead(8);
  button[BUTTON_B] = !digitalRead(7);
  button[BUTTON_RIGHT] = !digitalRead(6);
  button[BUTTON_LEFT] = !digitalRead(5);
  button[BUTTON_DOWN] = !digitalRead(4);
  button[BUTTON_UP] = !digitalRead(3);    

  //Set SELECT LOW Advance counter to 2
  digitalWrite(9, LOW);

  //Set SELECT HIGH Advance counter to 3
  digitalWrite(9, HIGH);

  //Set SELECT LOW Advance counter to 4
  digitalWrite(9, LOW);

  //Make sure that all 4 bits are LOW
  if((digitalRead(3) == 0x0) && (digitalRead(4) == 0x0) && (digitalRead(5) == 0x0) && (digitalRead(6) == 0x0))
  {
    //Continue operation
    //Set SELECT HIGH Advance counter to 5
    digitalWrite(9, HIGH);
  
    //Get data
    button[BUTTON_M] = !digitalRead(6);
    button[BUTTON_X] = !digitalRead(5);
    button[BUTTON_Y] = !digitalRead(4);
    button[BUTTON_Z] = !digitalRead(3);    

    //Set SELECT LOW Advance counter to 6
    digitalWrite(9, LOW);
  
    //Get special button if needed
    button[BUTTON_SPECIAL] = !digitalRead(3);

    //Confirm if D1, D2 and D3 are all HIGH
    if((digitalRead(4) == 0x1) && (digitalRead(5) == 0x1) && (digitalRead(6) == 0x1))
    {
      ret = PAD_6B;
    }
    
    //Set SELECT HIGH Advance counter to 7
    //digitalWrite(9, HIGH);
  
    //Set SELECT LOW Counter will be 0
    //digitalWrite(9, LOW);

  }

  return ret;
}


void Joystick_Updatestate(int *button, int pad)
{
  int button_update;

  //Button Mapping 
  //A, B, C, X, Y, Z, START, MENU
  //2, 1, 5, 3, 0, 4, 9,     8

#if 0
  //Set neutral axis and set value if button is pressed
  Joystick.setXAxis(0x7F);
  if(button[BUTTON_LEFT]) Joystick.setXAxis(0x00);
  if(button[BUTTON_RIGHT]) Joystick.setXAxis(0xFF);
  
  //Set neutral axis and set value if button is pressed
  Joystick.setYAxis(0x7F);
  if(button[BUTTON_UP]) Joystick.setYAxis(0x00);
  if(button[BUTTON_DOWN]) Joystick.setYAxis(0xFF);
#endif

  if(button[BUTTON_LEFT] & button[BUTTON_UP]) Joystick.setHatSwitch(0, 315);
  else if(button[BUTTON_RIGHT] & button[BUTTON_UP]) Joystick.setHatSwitch(0, 45);
  else if(button[BUTTON_LEFT] & button[BUTTON_DOWN]) Joystick.setHatSwitch(0, 225);
  else if(button[BUTTON_RIGHT] & button[BUTTON_DOWN]) Joystick.setHatSwitch(0, 135);
  else if(button[BUTTON_UP]) Joystick.setHatSwitch(0, 0);
  else if(button[BUTTON_DOWN]) Joystick.setHatSwitch(0, 180);
  else if(button[BUTTON_LEFT]) Joystick.setHatSwitch(0, 270);
  else if(button[BUTTON_RIGHT]) Joystick.setHatSwitch(0, 90);
  else Joystick.setHatSwitch(0, JOYSTICK_HATSWITCH_RELEASE);
  

  //Button I
  if(button[BUTTON_Z] || button[BUTTON_C])
  {
    Joystick.setButton(2, 1); //I pressed
  }
  else
  {
    Joystick.setButton(2, 0); //I released    
  }
  //Button II
  if(button[BUTTON_Y] || button[BUTTON_B])
  {
    Joystick.setButton(1, 1); //I pressed
  }
  else
  {
    Joystick.setButton(1, 0); //I released    
  }


  Joystick.setButton(8, button[BUTTON_M]);

  //Button RUN
  if(button[BUTTON_X] || button[BUTTON_S] || button[BUTTON_A])
  {
    Joystick.setButton(9, 1);
  }
  else
  {
    Joystick.setButton(9, 0);
  }

  
  if(button[BUTTON_SPECIAL])
  {
    Joystick.setButton(9, 1);
    Joystick.setButton(8, 1);
  }

#if 0
  //Update button states
  Joystick.setButton(2, button[BUTTON_A]);
  Joystick.setButton(1, button[BUTTON_B]);
  Joystick.setButton(5, button[BUTTON_C]);
  Joystick.setButton(9, button[BUTTON_S]);

  //Special button for 8bitDo M30 pad
  Joystick.setButton(6, button[BUTTON_SPECIAL]);
  
  if(pad == PAD_6B)
  {
    Joystick.setButton(3, button[BUTTON_X]);
    Joystick.setButton(0, button[BUTTON_Y]);
    Joystick.setButton(4, button[BUTTON_Z]);
    Joystick.setButton(8, button[BUTTON_M]);
  }
  else
  {
    Joystick.setButton(3, 0);
    Joystick.setButton(0, 0);
    Joystick.setButton(4, 0);
    Joystick.setButton(8, 0);
  }
  
#endif
  //Send Joystick status through USB
  Joystick.sendState();
}

ISR(TIMER1_COMPA_vect) // timer compare interrupt service routine
{
  int button[13];
  int pad;

  //Get 6B button pad status
  pad = Get_6B_Pad(button);

    //Update joystick state
  Joystick_Updatestate(button, pad);
}

void loop()
{
}

