#include <Servo.h>
 
int yaw_servoPin = 8;
int roll_servoPin = 9;
int pitch_servoPin = 10; 
Servo yaw_servo;  
int yaw_angle = 0;   // servo position in degrees 
Servo roll_servo;  
int roll_angle = 0;   // servo position in degrees 
Servo pitch_servo;  
int pitch_angle = 0;   // servo position in degrees 

int angle = 0; //testing

int yaw_neutral = 80;
int pitch_neutral = 70;
int roll_neutral = 40;

int sensor_yaw_neutral = 0;
int sensor_pitch_neutral = 0;
int sensor_roll_neutral = 0; 
int sensor_yaw_dir = 1;
int sensor_pitch_dir = 1;
int sensor_roll_dir = 1;

int yaw_max = 150;
int yaw_min = 10;
int pitch_min = 10;
int pitch_max = 110;
int roll_min = 0;
int roll_max = 180;
  
byte buffer[12]; // get the Yaw/Pitch/Roll angles, 4 bytes each

//Union to store bytes and float on top of each other
typedef union {
    unsigned char b[4];
    float f;
} bfloat;

bfloat yaw;
bfloat pitch;
bfloat roll;

int isCalibrated = 0;
 
void setup() { 
  Serial.begin(57600);
  yaw_servo.attach(yaw_servoPin);
  roll_servo.attach(roll_servoPin);
  pitch_servo.attach(pitch_servoPin); 

  // Go to neutral positions:
  yaw_servo.write(yaw_neutral);
  pitch_servo.write(pitch_neutral);
  roll_servo.write(roll_neutral);

  //clear buffer:
  int i;
  for (i = 0; i < 12; i++) buffer[i] = 0;
} 
 
void loop() { 

  Serial.readBytes(buffer, 12); //#YPR 

  yaw.b[0] = buffer[0];
  yaw.b[1] = buffer[1];
  yaw.b[2] = buffer[2];
  yaw.b[3] = buffer[3];

  pitch.b[0] = buffer[4];
  pitch.b[1] = buffer[5];
  pitch.b[2] = buffer[6];
  pitch.b[3] = buffer[7];

  roll.b[0] = buffer[8];
  roll.b[1] = buffer[9];
  roll.b[2] = buffer[10];
  roll.b[3] = buffer[11];

  float yaw_angle = (float)yaw.f;
  float pitch_angle = (float)pitch.f;
  float roll_angle = (float)roll.f;
  
  Serial.print("Y/P/R: " ); 
  Serial.print(yaw_angle); Serial.print(" / ");
  Serial.print(pitch_angle); Serial.print(" / ");
  Serial.print(roll_angle); Serial.println(""); 

  if (isCalibrated == 0) {
    sensor_yaw_neutral = (int)yaw_angle;
    sensor_pitch_neutral = (int)pitch_angle;
    sensor_roll_neutral = (int)roll_angle;    
    isCalibrated = 1;  
  }

  Serial.print("Y/P/R: " ); 
  Serial.print(sensor_yaw_neutral); Serial.print(" / ");
  Serial.print(sensor_pitch_neutral); Serial.print(" / ");
  Serial.print(sensor_pitch_neutral); Serial.println("");
  
  char Dword = '0'; // not using
  
  if(Dword=='q'){ //allow re-setting of sensor neutral
    sensor_yaw_neutral = (int)yaw_angle;
    sensor_pitch_neutral = (int)pitch_angle;
    sensor_roll_neutral = (int)roll_angle;      
  }
  // calibrate directions:
  if(Dword=='a'){
    sensor_yaw_dir = sensor_yaw_dir * -1;
  }
  if(Dword=='s'){
    sensor_pitch_dir = sensor_pitch_dir * -1;
  }
  if(Dword=='d'){
    sensor_roll_dir = sensor_roll_dir * -1;
  }

  // angle changes from neutral:
  int yaw_delta = sensor_yaw_dir*((int)yaw_angle - (int)sensor_yaw_neutral);
  int pitch_delta = sensor_pitch_dir*((int)pitch_angle - (int)sensor_pitch_neutral);
  int roll_delta = sensor_roll_dir*((int)roll_angle - (int)sensor_roll_neutral);

  // set new angles:
  int new_yaw = yaw_neutral + yaw_delta;
  if (new_yaw > yaw_max) new_yaw = yaw_max; if (new_yaw < yaw_min) new_yaw = yaw_min;

  int new_pitch = pitch_neutral + pitch_delta;
  if (new_pitch > pitch_max) new_pitch = pitch_max; if (new_pitch < pitch_min) new_pitch = pitch_min;

  int new_roll = roll_neutral + roll_delta;
  if (new_roll > roll_max) new_roll = roll_max; if (new_roll < roll_min) new_roll = roll_min;

  // write angles:
  yaw_servo.write(new_yaw);
  pitch_servo.write(new_pitch);
  roll_servo.write(new_roll);

  //Serial.print("Y/P/R: " ); 
  //Serial.print(yaw_delta); Serial.print(" / ");
  //Serial.print(pitch_delta); Serial.print(" / ");
  //Serial.print(roll_delta); Serial.println(""); 
    
  

  /*char Dword=Serial.read();
  
  if(Dword=='q'){
    angle = angle + 10; 
    if (angle > 180) angle = 180;
  }

  if(Dword=='w'){
    angle = angle - 10;
    if (angle < 0) angle = 0; 
  }

  // Write angles:
  if(Dword=='a'){
    yaw_servo.write(angle);
    delay(10); 
  }
  if(Dword=='s'){
    pitch_servo.write(angle);
    delay(10); 
  }
  if(Dword=='d'){
    roll_servo.write(angle);
    delay(10); 
  }

  // Print info:
  if(Dword=='f'){
    Serial.print("Current angle: "); Serial.println(angle);
  }
  */
  
  /*
  ///////////////////////// YAW ////////////////////////////
  // scan from 0 to 180 degrees
  for(yaw_angle = 0; yaw_angle < 180; yaw_angle++)  
  {                                  
    yaw_servo.write(yaw_angle);               
    delay(15);                   
  }
  Serial.print("yaw_angle: "); Serial.println(yaw_angle);
  // now scan back from 180 to 0 degrees
  for(yaw_angle = 180; yaw_angle > 0; yaw_angle--)    
  {                                
    yaw_servo.write(yaw_angle);           
    delay(15);       
  }
  Serial.print("yaw_angle: "); Serial.println(yaw_angle);

  ////////////////////// ROLL /////////////////////////////
  // scan from 0 to 180 degrees
  for(roll_angle = 0; roll_angle < 180; roll_angle++)  
  {                                  
    roll_servo.write(roll_angle);               
    delay(15);                   
  }
  Serial.print("roll_angle: "); Serial.println(roll_angle);
  // now scan back from 180 to 0 degrees
  for(roll_angle = 180; roll_angle > 0; roll_angle--)    
  {                                
    roll_servo.write(roll_angle);           
    delay(15);       
  }
  Serial.print("roll_angle: "); Serial.println(roll_angle);
  
  //////////////////////// PITCH ////////////////////////////
  // scan from 0 to 180 degrees
  for(pitch_angle = 0; pitch_angle < 180; pitch_angle++)  
  {                                  
    pitch_servo.write(pitch_angle);               
    delay(15);                   
  }
  Serial.print("pitch_angle: "); Serial.println(pitch_angle);
  // now scan back from 180 to 0 degrees
  for(pitch_angle = 180; pitch_angle > 0; pitch_angle--)    
  {                                
    pitch_servo.write(pitch_angle);           
    delay(15);       
  }
  Serial.print("pitch_angle: "); Serial.println(pitch_angle); 
  */ 
} 
