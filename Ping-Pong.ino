//Flywheels
#define MotorDirection1 10    
#define MotorSpeed1 11
#define MotorDirection2 40
#define MotorSpeed2 12
int SpeedVal1 = 0;    
int SpeedVal2 = 0;

//Stepper Feeding
const int stepPinF = 23;   
const int dirPinF =  24;

//Stepper Horizontal
const int stepPinH = 44;   
const int dirPinH =  45;

//Stepper Vertical
const int stepPinV = 48;   
const int dirPinV =  49;

const int roundTOStepRate = 200;
const int stepperMovingStepsH [] = {10,0,10, 7,0,7, 4,0,4};
const int stepperMovingStepsV []= {2,0,2, 2,0,2, 2,0,2};

int stepperDirections [9][2] = {  // {HStepper , VStepper}, 0 for LOW, 1 for HIGH
  {0,0}, //P1
  {0,0}, //P2
  {1,0}, //P3
  {0,0}, //P4
  {0,0}, //P5
  {1,0}, //P6
  {0,1}, //P7
  {0,1}, //P8
  {1,1}  //P9
};

//LCD and KEYPAD
#include <Wire.h>           
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  Serial.begin(9600);

  //Stepper Feeding
  pinMode(stepPinF,OUTPUT); 
  pinMode(dirPinF,OUTPUT);

  //Stepper Horizontal
  pinMode(stepPinH,OUTPUT); 
  pinMode(dirPinH,OUTPUT);

  //Stepper Vertical
  pinMode(stepPinV,OUTPUT); 
  pinMode(dirPinV,OUTPUT);

  //Flywheels
  pinMode(MotorDirection1, OUTPUT); //Flywheels
  pinMode(MotorSpeed1, OUTPUT);
  pinMode(MotorDirection2, OUTPUT);
  pinMode(MotorSpeed2, OUTPUT);

}

void loop() {
  //Get inputs from key pad
  //LCD
  lcd.backlight();    
  lcd.init(); 
  lcd.print("Enter Combo:");

  char combo[4][3];

  int c = 0;

  for(int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 3;j++)
    {
      while((combo[i][j] = customKeypad.getKey())== NO_KEY)
      {
        delay(10);
      }
      while(customKeypad.getKey()!= NO_KEY)
      {
        delay(10);
      }
       lcd.setCursor(c, 1);
       lcd.print(combo[i][j]);
       c=c+1;
    }
    
    c=c+1;
  } 
  //Show inputs
  Serial.println("combo entered");
  Serial.print(combo[0][0]);
  Serial.print(combo[0][1]);
  Serial.println(combo[0][2]);
  
  Serial.print(combo[1][0]);
  Serial.print(combo[1][1]);
  Serial.println(combo[1][2]);
  
  Serial.print(combo[2][0]);
  Serial.print(combo[2][1]);
  Serial.println(combo[2][2]);
  
  Serial.print(combo[3][0]);
  Serial.print(combo[3][1]);
  Serial.println(combo[3][2]);  

  //Iteration through each combo
  for(int index = 0; index < 4; index++)
  {
    //For test use print index before each iteration
    Serial.println("Current iteration: "+index);
    
    //Step 0: Stop Flywheels before making moves
    if (customKeypad.getKey()!= NO_KEY)
    {
      while( (SpeedVal1 > 0) || (SpeedVal2 > 0))
      {
        lcd.setCursor(0, 1);
        lcd.print("Stopping");
//        if ((SpeedVal1 > 0) && (SpeedVal2 > 0))
//        {
//          SpeedVal1=SpeedVal1-2;
//          SpeedVal2=SpeedVal2-2;
//          analogWrite(MotorSpeed1,SpeedVal1);
//          analogWrite(MotorSpeed2,SpeedVal2);
//          delay(100);
//        }
        if (SpeedVal1  > 0)
        {
          SpeedVal1=SpeedVal1-2;
          analogWrite(MotorSpeed1,SpeedVal1);
          delay(100);
        }
        if (SpeedVal2 >0)
        {
          SpeedVal2=SpeedVal2-2;
          analogWrite(MotorSpeed2,SpeedVal2);
          delay(100);
        }
      }
    break;
    }

    // Run moving function
    Moving(combo[index]);   
  }  
}

void Moving(char curCombo[])
{ 
//  //checking input
//  if(isDigit(curCombo[0]) 
//    && isDigit(curCombo[2]) 
//    && (curCombo[1] == 'A' || curCombo[1] == 'B'))
//  {
//  //Getting position, mode and number of servings.
//  int curPosition = curCombo[0]-'0';
//  curPosition = curPosition-1;
//  char curMode = curCombo[1];
//  int curNum = curCombo[2]-'0';
//  }
//  else
//  {
//    //Showing error message
//    return;
//  }
  int curPosition = curCombo[0]-'0';
  curPosition = curPosition-1;
  char curMode = curCombo[1];
  int curNum = curCombo[2]-'0';
  //Step1: Moving H&V stepper according to given position
  SettingHVDirection(curPosition);
  MovingStepper(stepperMovingStepsV[(curPosition)],stepPinV);
  MovingStepper(stepperMovingStepsH[(curPosition)],stepPinH);
  /* Update for including moving rate
  MovingStepperNew(stepperMovingStepsV[(curPosition)],stepPinV,roundTOStepRate);
  MovingStepperNew(stepperMovingStepsH[(curPosition)],stepPinH,roundTOStepRate);
  */

  
  // Step 2: Setting flywheels' speed according to given mode
  if(curMode == 'A')
  {
    digitalWrite(MotorDirection1, HIGH);   
    digitalWrite(MotorDirection2, HIGH);
    SpeedVal1= 50;
    SpeedVal2 =30;
    analogWrite(MotorSpeed1,SpeedVal1);
    analogWrite(MotorSpeed2,SpeedVal2);
    delay (1000);
  }
  else if(curMode == 'B')
  {
    digitalWrite(MotorDirection1, HIGH);   
    digitalWrite(MotorDirection2, HIGH);
    SpeedVal1= 50;  //Change to speed for mode B
    SpeedVal2 =30;  //Change to speed for mode B  
    analogWrite(MotorSpeed1,SpeedVal1);
    analogWrite(MotorSpeed2,SpeedVal2);
    delay (1000);
  }


  // Step3: Setting feeding time according to given serving number
  digitalWrite(dirPinF,HIGH);
  MovingStepper(curNum,stepPinF);
  /* Update for including moving rate
  MovingStepperNew(curNum,stepPinF,feedingMovingRate);
  */
  
  // Finishing serving for current combo. Adding delay time if needed
  // delay(1000);

   
  // Step4: Stop flywheels
  //analogWrite(MotorSpeed1,0);
  //analogWrite(MotorSpeed2,0);
  while( (SpeedVal1 > 0) || (SpeedVal2 > 0))
      {
//        if ((SpeedVal1 > 0) && (SpeedVal2 > 0))
//        {
//          SpeedVal1=SpeedVal1-2;
//          SpeedVal2=SpeedVal2-2;
//          analogWrite(MotorSpeed1,SpeedVal1);
//          analogWrite(MotorSpeed2,SpeedVal2);
//          delay(100);
//        }
        if (SpeedVal1  > 0)
        {
          SpeedVal1=SpeedVal1-2;
          analogWrite(MotorSpeed1,SpeedVal1);
          delay(100);
        }
        if (SpeedVal2 >0)
        {
          SpeedVal2=SpeedVal2-2;
          analogWrite(MotorSpeed2,SpeedVal2);
          delay(100);
        }
      }
  delay (1000);


  // Step5: Return H&V Stepper to starting position
  SettingHVDirectionReverse(curPosition);
  MovingStepper(stepperMovingStepsV[(curPosition)],stepPinV);
  MovingStepper(stepperMovingStepsH[(curPosition)],stepPinH);
  /* Update for including moving rate
  MovingStepperNew(stepperMovingStepsV[(curPosition)],stepPinV,roundTOStepRate);
  MovingStepperNew(stepperMovingStepsH[(curPosition)],stepPinH,roundTOStepRate);
  */
}

int ReverseDirection(int curDirection){
  if(curDirection == 1){
    return 0;
  }
  return 1;
}

void SettingHVDirectionReverse(int curPosition){
  digitalWrite(dirPinH, ReverseDirection( stepperDirections[curPosition][0]) );
  digitalWrite(dirPinV, ReverseDirection( stepperDirections[curPosition][1]) );
}

void SettingHVDirection(int curPosition){
  digitalWrite(dirPinH, stepperDirections[curPosition][0]);
  digitalWrite(dirPinV, stepperDirections[curPosition][1]);
}

void MovingStepper(int steps,int StepperName){
    for(int x=0; x < (roundTOStepRate*steps); x++)
    {
      digitalWrite(StepperName, HIGH);
      delayMicroseconds(1000);
      digitalWrite(StepperName, LOW);
      delayMicroseconds(1000);
    }
    delay(1000);
}

/* New stepper Moving method, in case moving rate of H/V stepper and feeding stepper are different
void MovingStepperNew(int steps,int StepperName, int movingRate){
    for(int x=0; x < (movingRate*steps); x++)
    {
      digitalWrite(StepperName, HIGH);
      delayMicroseconds(1000);
      digitalWrite(StepperName, LOW);
      delayMicroseconds(1000);
    }
    delay(1000);
}
*/

//
//void MovingHStepper(int steps){
//  for(int x=0; x < (roundToStepRate*steps); x++)
//    {
//      digitalWrite(stepPinH, HIGH);
//      delayMicroseconds(1000);
//      digitalWrite(stepPinH, LOW);
//      delayMicroseconds(1000);
//    }
//    delay(1000);
//}
//  //Position 1
  
//  if(curPosition == 1)
//  {
//    digitalWrite(dirPinH, LOW);
//    digitalWrite(dirPinV, LOW);
//
//    MovingVStepper( stepperMovingStepsV[(curPosition)] );
//    MovingHStepper( stepperMovingStepsH[(curPosition)] );
//  }
//  
//  //Position 2
//  else if(curPosition == 2)
//  {
//    digitalWrite(dirPinH, LOW);
//    digitalWrite(dirPinV, LOW);
//
//    MovingVStepper( stepperMovingStepsV[(curPosition)] );
//    MovingHStepper( stepperMovingStepsH[(curPosition)] );
//  }
//
//  //Position 3
//  else if(curPosition == 3)
//  {
//    digitalWrite(dirPinH, HIGH);
//    digitalWrite(dirPinV, LOW);
//
//    MovingVStepper( stepperMovingStepsV[(curPosition)] );
//    MovingHStepper( stepperMovingStepsH[(curPosition)] );
//  }
//
//  //Position 4
//  else if(curPosition == 4)
//  {
//    digitalWrite(dirPinH, LOW);
//    digitalWrite(dirPinV, LOW);
//
//    MovingVStepper( stepperMovingStepsV[(curPosition)] );
//    MovingHStepper( stepperMovingStepsH[(curPosition)] );
//  }
//
//  //Position 5
//  else if(curPosition == 5)
//  {
//    digitalWrite(dirPinH, LOW);
//    digitalWrite(dirPinV, LOW);
//
//    MovingVStepper( stepperMovingStepsV[(curPosition)] );
//    MovingHStepper( stepperMovingStepsH[(curPosition)] );
//  }
//
//  //Position 6
//  else if(curPosition == 6)
//  {
//    digitalWrite(dirPinH, HIGH);
//    digitalWrite(dirPinV, LOW);
//
//    MovingVStepper( stepperMovingStepsV[(curPosition)] );
//    MovingHStepper( stepperMovingStepsH[(curPosition)] );
//  }
//
//  //Position 7
//  else if(curPosition == 7)
//  {
//    digitalWrite(dirPinH, LOW);
//    digitalWrite(dirPinV, HIGH);
//
//    MovingVStepper( stepperMovingStepsV[(curPosition)] );
//    MovingHStepper( stepperMovingStepsH[(curPosition)] );
//  }
//
//  //Position 8
//  else if(curPosition == 8)
//  {
//    digitalWrite(dirPinH, LOW);
//    digitalWrite(dirPinV, HIGH);
//
//    MovingVStepper( stepperMovingStepsV[(curPosition)] );
//    MovingHStepper( stepperMovingStepsH[(curPosition)] );
//  }
//
//  //Position 9
//  else if(curPosition == 9)
//  {
//    digitalWrite(dirPinH, HIGH);
//    digitalWrite(dirPinV, HIGH);
//
//    MovingVStepper( stepperMovingStepsV[(curPosition)] );
//    MovingHStepper( stepperMovingStepsH[(curPosition)] );
//  }
