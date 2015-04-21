//  libraries
#include <Stepper.h>
 
//  buffer containing the data
char inData[50];
//  index used for buffer
byte index;
//  set start and end of packet of data sent
#define SOP '<'
#define EOP '>'
//  and it's semaphore lights 
bool started = false;
bool ended = false;
//  led
int led = 13;
//  the 3 variables of interest (default values)
int _dir = 0, _speed = 60, _steps = 200;
//  the stepper
Stepper myStepper(_steps, 2,3);
 
 
//  setup 
void setup(){
  myStepper.setSpeed(_speed);
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  memset(inData, '\0', 50);
}
 
 
//  loop
void loop() {
  
  //  saving serial data into buffer
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    
    if (inChar == SOP)
    {
      index = 0;
      inData[index] = '\0';
      started = true;
      ended = false;
    } 
    else if (inChar == EOP)
    {
      ended = true;
      break;
    }
    else
    {
      if (index < 49) 
      {
        inData[index] = inChar;
        index++;
        inData[index] = '\0';
      }
    }
  }
  
  //  check packet
  if (started && ended)
  {
    //  process the packet
    
    //  get direction (first char)
    _dir = inData[0] - '0';
    
    //  get speed (next 2 chars)
    char speedTemp[3];
    speedTemp[0] = inData[1];
    speedTemp[1] = inData[2];
    speedTemp[2] = '\0';
    _speed = atoi(speedTemp);
    
    //  get number of steps (next - the last - 3 chars)
    char stepsTemp[4];
    stepsTemp[0] = inData[3];
    stepsTemp[1] = inData[4];
    stepsTemp[2] = inData[5];
    stepsTemp[3] = '\0';
    _steps = atoi(stepsTemp);
    
    //  Print check for values
    Serial.print("direction: ");
    Serial.print(_dir);
    Serial.print(" speed: ");
    Serial.print(_speed);
    Serial.print(" steps: ");
    Serial.println(_steps);
    
    //  command the stepper
    myStepper.setSpeed(_speed);
    if (_dir == 0) {
      myStepper.step(_steps);
      delay(500);
    } else {
      myStepper.step(-_steps);
      delay(500);
    }
    
    //  reset for the next packet
    started = false;
    ended = false;
    index = 0;
    inData[index] = '\0';
  }
  
}
