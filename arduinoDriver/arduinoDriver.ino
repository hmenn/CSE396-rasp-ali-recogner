#include "arduinoStepMotor.h"

#include <SoftwareSerial.h>
//X 920 adım
//Y 610 adım
#define X_DIRECTION 4 //X icin yon
#define Y_DIRECTION 6 //Y icin yon
#define X_STEP 3 //X icin step pulse
#define Y_STEP 5 //Y icin step pulse
#define MAXSTEPX 920 //360/x=1.8 x=200 adım
#define MAXSTEPY 610

arduinoStepMotor stepMotorX(MAXSTEPX,X_DIRECTION,X_STEP);
arduinoStepMotor stepMotorY(MAXSTEPY,Y_DIRECTION,Y_STEP);
char data[50]={0};

void setup() {

  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(X_DIRECTION,OUTPUT);
  digitalWrite(X_DIRECTION,LOW);
  pinMode(Y_DIRECTION,OUTPUT);
  digitalWrite(Y_DIRECTION,LOW);
  pinMode(X_STEP,OUTPUT);
  digitalWrite(X_STEP,LOW);
  pinMode(Y_STEP,OUTPUT);
  digitalWrite(Y_STEP,LOW);
  stepMotorX.setSpeed(800);
  stepMotorY.setSpeed(800); 
  
 // while(!Serial);

}

bool makeHandshake(){

  char ch;
  ch = Serial.read();
  if(ch=='H'){
    Serial.write('C');
    return true;
  }else{
    return false;
 }
  
  
 }

int xStep,yStep;
char ch;

void loop() {

  if(Serial.available())
  {
    if(makeHandshake()){

      while(1){
        if(Serial.available()){
        String msg= Serial.readString();

        sscanf(msg.c_str(),"%c%c%d%c%c%c%d",&ch,&ch,&xStep,&ch,&ch,&ch,&yStep);
        stepMotorX.steps(xStep);
        stepMotorY.steps(yStep);
        delay(100);
        Serial.write(msg.c_str());
        delay(100);
        }
      }
    }    

  }
}

