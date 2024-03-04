/*
December 2023
Hayden McKenna
This is supposed to act as an automatic blind opener for windows.
It uses a PIR sensor to detect movement, photoresistor to detect
light levels and, if the light levels are low enough with motion detected, 
the blinds will be opened via a stepper motor.
*/

int PIRpin = 2; //digital 2 to PIR
int Pin1 = 3; //digital 3 control the IN1
int Pin2 = 4; //digital 4 control the IN2
int Pin3 = 5; //digital 5 control the IN3
int Pin4 = 6; //digital 6 control the IN4
int photocellPin = A0; //photoresistor to A0 
int sensorValue; //value from photoresistor
int step = 512; //stepper motor value for 360º rotation
int speed; //stepper speed
bool lock = false; //boolean value to check whether the blinds are already open
bool rtrn = false; //boolean value to check whether the conditions to close the blinds are met
bool reset = false; // boolean value to check if the timer should restart
long timeStart; // start time of timer 
long timeCurrent; //current time of timer 
long pause = 10000; //millisecond value for 10 seconds for timer to compare to


void setup() {
  Serial.begin(9600);
  pinMode(PIRpin,INPUT); //initialize PIR pin as input
  pinMode(Pin1, OUTPUT); //digital 3 port mode, OUTPUT for the output
  pinMode(Pin2, OUTPUT); //digital 4 port mode, OUTPUT for the output
  pinMode(Pin3, OUTPUT); //digital 5 port mode, OUTPUT for the output
  pinMode(Pin4, OUTPUT); //digital 6 port mode, OUTPUT for the output
}

void loop() {
  sensorValue = analogRead(photocellPin);
  if(digitalRead(PIRpin)==HIGH && sensorValue<=400 && lock==false){//opens blinds if they are closed, motion is detected, and light level low enough
    Serial.println("Opening...");
    Speed(15);
    Step(512);
    lock = true;
    reset = true;
    timeStart = millis();
    Serial.println("Open");
  }
  else if(lock == true && rtrn == false){//if blinds opened and haven't been closed
    while(lock == true && reset == true){//if blinds opened and motion has been detected again
      if(digitalRead(PIRpin)==LOW){//if no motion has been detected
        timeCurrent = millis();
        if(timeCurrent - timeStart >= pause){//if enough time has passed (10sec) since the last motion
          reset = false;
          rtrn = true;
        }
      }
      else if(digitalRead(PIRpin)==HIGH){//if motion is again detected
        reset = true;
        Serial.println("Timer reset to 10 seconds");
        timeStart = millis();
      } 
    }
  }
  else if(lock==true && rtrn==true){//closes blinds if they are open and rtrn requirements of the timer are met
    Serial.println("Closing...");
    Speed(15);
    Step(-512);
    lock = false;
    rtrn = false;
    Serial.println("Closed");
  }
}

void Step(int _step){//Stepper motor rotation
  if(_step>=0){  // Stepper motor forward
    for(int i=0;i<_step;i++){   
      setStep(1, 0, 0, 1);
      delay(speed); 
      setStep(1, 0, 0, 0);
      delay(speed);
      setStep(1, 1, 0, 0);
      delay(speed);
      setStep(0, 1, 0, 0);
      delay(speed);
      setStep(0, 1, 1, 0);
      delay(speed);
      setStep(0, 0, 1, 0);
      delay(speed);
      setStep(0, 0, 1, 1);
      delay(speed); 
      setStep(0, 0, 0, 1);
      delay(speed);
    }
  } else{ // Stepper motor backward
     for(int i=_step;i<0;i++){  
      setStep(0, 0, 0, 1);
      delay(speed);
      setStep(0, 0, 1, 1);
      delay(speed);
      setStep(0, 0, 1, 0);
      delay(speed);
      setStep(0, 1, 1, 0);
      delay(speed);
      setStep(0, 1, 0, 0);
      delay(speed);
      setStep(1, 1, 0, 0);
      delay(speed);
      setStep(1, 0, 0, 0);
      delay(speed);
      setStep(1, 0, 0, 1);
      delay(speed);
    }
  }
}
void setStep(int a, int b, int c, int d){  
  digitalWrite(Pin1, a);     
  digitalWrite(Pin2, b);     
  digitalWrite(Pin3, c);     
  digitalWrite(Pin4, d);    
} 
void Speed(int stepperspeed){//set Stepper speed 
  speed = 15 - stepperspeed;
  if(speed<1){
    speed = 1;
  }
  if(speed>15){
    speed = 15;
  }
}

