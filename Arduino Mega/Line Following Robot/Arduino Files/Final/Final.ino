#include <LiquidCrystal.h>


// ------------------------------------ PIN CONFIGURATIONS ------------------------------------
// -------- Motor --------
//Front Left
const int FL1 = 13;
//Back Left
const int BL1 = 12;
//Back Right
const int BR1 = 11;
//Front Right
const int FR1 = 10;
//Speed Control
int maxSpeed = 255;
int turnSpeed = 100;
int flag = 0;

// -------- InfraRed Sensor --------
const int IRLeft = 9;
const int IRRight = 8;
#define checkLeft digitalRead(IRLeft)
#define checkRight digitalRead(IRRight)

// -------- LCD Display --------
LiquidCrystal lcd(14,15,16,17,18,19); //lcd(RS,E,DB4,DB5,DB6,DB7);

// -------- UltraSonic Sensor --------
const int TriggerL = 7;
const int EchoL = 6;
int mappedL;
int durL;
int cmL;

// -------- Load --------
const int Load = A0;
int pot;
int mappot;
// ------------------------------------ END OF PIN CONFIGURATIONS ------------------------------------


// ------------------------------------ INITIALIZATIONS ------------------------------------
// -------- Motor --------
void motor_init()
{
  pinMode(FL1,OUTPUT);
  pinMode(BL1,OUTPUT);
  pinMode(BR1,OUTPUT);
  pinMode(FR1,OUTPUT);
}

// -------- InfraRed Sensor --------
void ir_init()
{
  pinMode(IRLeft,INPUT);
  pinMode(IRRight,INPUT);
}

// -------- LCD Display --------
void lcd_init()
{
   lcd.begin(16,2); 
   lcd.clear();
   lcd.setCursor(4,0);
   lcd.print("WELCOME");
   delay(100);
}

// -------- UltraSonic Sensor --------
void us_init()
{
  pinMode(TriggerL,OUTPUT);
  pinMode(EchoL,INPUT);
}

// -------- Load --------
void load_init()
{
  pinMode(Load,INPUT);
}
// ------------------------------------ END OF INITIALIZATIONS ------------------------------------


// ------------------------------------ FUNCTIONS ------------------------------------ 
// -------- Motor Direction --------
// Forward
void Forward()
{
  analogWrite(FL1,maxSpeed);
  analogWrite(BL1,maxSpeed);
  analogWrite(BR1,maxSpeed);
  analogWrite(FR1,maxSpeed);
}
// Left
void Left()
{
  analogWrite(FL1,turnSpeed);
  analogWrite(BL1,turnSpeed);
  analogWrite(BR1,maxSpeed);
  analogWrite(FR1,maxSpeed);
}
// Right
void Right()
{
  analogWrite(FL1,maxSpeed);
  analogWrite(BL1,maxSpeed);
  analogWrite(BR1,turnSpeed);
  analogWrite(FR1,turnSpeed);
}
// Stop
void Stop()
{
  analogWrite(FL1,0);
  analogWrite(BL1,0);
  analogWrite(BR1,0);
  analogWrite(FR1,0);

}
// End
void End()
{
  Stop();
}

// -------- LCD Texts --------
//Message for Left Turn
void MsgLeft()
{
   lcd.setCursor(0,0);
   lcd.print("TURNING LEFT");
}
//Message for Right Turn
void MsgRight()
{
   lcd.setCursor(0,0);
   lcd.print("TURNING RIGHT");
}
//Message for Moving Forward
void MsgForward()
{
  lcd.setCursor(0,0);
  lcd.print("MOVING STRAIGHT");
}
//Message for Obstacle
void MsgObstacle()
{
  lcd.setCursor(0,0);
  lcd.print("OBSTRUCTED    ");
}
//Message for End
void MsgEnd()
{
  lcd.setCursor(0,0);
  lcd.print("END");
}
//Clear Texts
void clearLCD()
{
  lcd.clear();
}
//Obstacle Distance
void Distance()
{
  lcd.setCursor(2,1);
  lcd.print("  ");
  lcd.setCursor(0,1);
  lcd.print("D:");
  lcd.print(mappedL);
  lcd.setCursor(4,1);
  lcd.print("CM");
}
//Overload
void MsgLoad()
{
  lcd.setCursor(0,0);
  lcd.print("OVER LOAD    ");
}
//Load Kilogram
void Kg()
{
  lcd.setCursor(12,1);
  lcd.print("  ");
  lcd.setCursor(7,1);
  lcd.print("LOAD:");
  lcd.print(mappot);
  lcd.setCursor(14,1);
  lcd.print("KG");
}
// -------- UltraSonic Obstacle Check --------
bool ObstacleCheck()
{
  digitalWrite(TriggerL,LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerL,HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerL,LOW);
  durL = pulseIn(EchoL,HIGH);
  cmL = (durL/29)/2;
  if(cmL >= 0)
    mappedL = map(cmL,4,560,0,15);
  else
    mappedL = map(cmL,-557,-7,16,30);
  Distance();
  if(mappedL < 10)
  {
    MsgObstacle();
    Stop();
    return true;
  }
  else
    return false;
}
// -------- Load Check --------
bool LoadCheck()
{
  pot = analogRead(A0);
  mappot = map(pot,0,1023,0,20);
  Kg();
  if(mappot > 10)
  {
    MsgLoad();
    Stop();
    return true;
  }
  else
   return false;
}
// ------------------------------------ END OF FUNCTIONS ------------------------------------


// -------- VOID SETUP --------
void setup()
{
  motor_init();
  ir_init(); 
  lcd_init();
  us_init();
  load_init();
}


// -------- VOID LOOP --------
void loop()
{
  if(!ObstacleCheck() and !LoadCheck())
  {
    if(checkLeft == LOW and checkRight == HIGH)
    {
      Right();
      flag = 1;
      MsgRight();
      while(ObstacleCheck() == false and LoadCheck() == false and checkLeft == LOW and checkRight == HIGH);
      clearLCD();
    }  
    if(checkLeft == HIGH and checkRight == LOW)
    {
      Left();
      flag = 1;
      MsgLeft();
      while(ObstacleCheck() == false and LoadCheck() == false and checkLeft == HIGH and checkRight == LOW);
      clearLCD();
    }
    if(checkLeft == HIGH and checkRight == HIGH)
    {
      Forward();
      flag = 1;
      MsgForward();
      while(ObstacleCheck() == false and LoadCheck() == false and checkLeft == HIGH and checkRight == HIGH);
      clearLCD();
    }
    if(ObstacleCheck() == false and checkLeft == LOW and checkRight == LOW)
    {
      if(flag == 1)
      {
        End();
        MsgEnd();
        while(ObstacleCheck() == false and LoadCheck() == false and checkLeft == LOW and checkRight == LOW);
        clearLCD();
      }
    }
  }
}
