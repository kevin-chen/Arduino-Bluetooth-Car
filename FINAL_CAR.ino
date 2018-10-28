//DO NOT TOUCH THESE 3 VARIABLES
const int SETUP_TIME = 5000;
const int AUTONOMOUS_TIME = 15000;
const int STOP_TIME = 2000;

//Time
long runtime = millis();

//Motors
int speedM = 220; //255 for CVS; 200 for Duracell; 200 amazonNew; 230,255 amazonFast
                  //FULL POWER: 255 200
long time90 = 240; //for regular teleop moveForward; 640; 350-Good for Low Battery; 210-Beginning of 3/24; 250-End of 3/25; -VIDEOS; In spite of the battery levels, the turn of the car has to match the videos
                   //340 (-320) for CVS Alkaline Battery; 300 for Duracell; 320 amazonNew; 340,360 amazonFast
                   //FULL POWER: 200 280
int motorL1 = 5; //PWM Pin- Motor 2
int motorL2 = 6; //PWM Pin
int motorR1 = 9; //PWM Pin - Motor 1
int motorR2 = 10; //PWM Pin

//LED's - Headlights
int frontLeft = 8; //Normal Pin - White
int frontRight = 7; //Normal Pin - White
int backLeft = 3; //Normal Pin - Red
int backRight = 4; //Normal Pin - Red

//TeleOp
int photoF = A0; //Analog In Pin
int photoB = A1; //Analog In Pin
int photoL = A2; //Analog In Pin
int photoR = A3; //Analog In Pin

//Autonomous
int distanceLT = 2; //Normal Pin - RED
int distanceLE = 12; //Normal Pin - WHITE
long durationL = 0; //Variable
int distanceL = 0;
int flexL = A4; //Analog In Pin
int flexR = A5; //Analog In Pin
int disLeftBound = 11; //20
int disRightBound = 13; //30
int flexLMin = 800; //Variable: 780, 790, 810, 820, 830
int flexRMin = 800; //Variable: (Tighter, More) 780, 795, 810, 820, 860
long startTime = 0; //Variable
int tier = 1; //Variable
long timePerCenti = 19; //Variable: 1700/30, 47-in class, 51-at home; 19 milliseconds / centimeter Date 3/2
boolean touchingWall = false; //Variable
boolean moveNormal = true; //Variable
boolean resetStartTime = true; //Variable

void setup() {
  Serial.begin(9600);
  
  runtime = millis();

  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);

  pinMode(frontLeft, OUTPUT);
  pinMode(frontRight, OUTPUT);
  pinMode(backLeft, OUTPUT);
  pinMode(backRight, OUTPUT);

  pinMode(photoF, INPUT);
  pinMode(photoB, INPUT);
  pinMode(photoL, INPUT);
  pinMode(photoR, INPUT);
  
  pinMode(distanceLT, OUTPUT);
  pinMode(distanceLE, INPUT);

  pinMode(flexL, INPUT);
  pinMode(flexR, INPUT);
}

void loop() {
  //DO NOT TOUCH THIS CODE
  long mil = millis();
  runtime = millis();
  
  if(mil<SETUP_TIME)
  {
    //LEAVE THIS BLANK
  }
  else if(mil<AUTONOMOUS_TIME+SETUP_TIME)
  {
    autonomousMode();
  }
  else if(mil<AUTONOMOUS_TIME+SETUP_TIME+STOP_TIME)
  {
    fullStop();
  }
  else
  {
    teleOpMode();
  }
  
  //flexAutonomous();
  //bluetooth();
  //teleOpMode();
  //autonomousMode();
  /*
  digitalWrite(distanceLT, LOW);
  delayMicroseconds(2);

  digitalWrite(distanceLT, HIGH);
  delayMicroseconds(10);
  digitalWrite(distanceLT, LOW);

  durationL = pulseIn(distanceLE, HIGH);
  distanceL = durationL*0.034/2;

  Serial.print("Distance: ");
  Serial.println(distanceL);
  */
}

//Autonomous
void autonomousMode(){

  runtime = millis();
  
  //flexAutonomous();
  redA();
  //blueA();
  /*
  if(tier == 1){
    rotateRightFor(time90);
  }
  else{
    fullStop();
  }
  */
}
//TeleOp
void teleOpMode(){

  runtime = millis();
  
  int frontLight = analogRead(photoF);
  int backLight = analogRead(photoB);
  int leftLight = analogRead(photoL);
  int rightLight = analogRead(photoR);

  Serial.println("");
  Serial.print("frontLight: ");
  Serial.println(frontLight);
  Serial.print("backLight: ");
  Serial.println(backLight);
  Serial.print("leftLight: ");
  Serial.println(leftLight);
  Serial.print("rightLight: ");
  Serial.println(rightLight);
  Serial.println("");
  
  int thresholdF = 800;
  int thresholdB = 800;
  int thresholdL = 800;
  int thresholdR = 800;
  int limit = 1000;
  
  if(frontLight > limit || backLight > limit || leftLight > limit || rightLight > limit){
    fullStop();
    Serial.println("Limit");
  }
  else if(frontLight > thresholdF){
    moveForward();
    //slowMoveForward();
    Serial.println("Forward");
  }
  else if(backLight > thresholdR){
    moveBackward();
    //slowMoveBackward();
    Serial.println("Backward");
  }
  else if(leftLight > thresholdL){
    //rotateLeft();
    slowRotateLeft();
    Serial.println("Left");
  }
  else if(rightLight > thresholdB){
    //rotateRight();
    slowRotateRight();
    Serial.println("Right");
  }
  else{
    fullStop();
    Serial.println("None");
  }
}

//Autonomous Methods
void redA(){ //Chan's Room - LEFT TO RIGHT

  runtime = millis();
  
  digitalWrite(distanceLT, LOW);
  delayMicroseconds(2);

  digitalWrite(distanceLT, HIGH);
  delayMicroseconds(10);
  digitalWrite(distanceLT, LOW);

  durationL = pulseIn(distanceLE, HIGH);
  distanceL = durationL*0.034/2;

  Serial.print("Distance: ");
  Serial.println(distanceL);
  
  //int flexStateL = analogRead(flexL);
  //int flexStateR = analogRead(flexR);
  
  //School: 100, 190 Home: 110, 150; 130, 160; 110, 140; 100, 130; 100, 190; 60, 160; 
  if((!touchingWall) && (distanceL >= disLeftBound && distanceL <= disRightBound)){ //&& (flexStateL >= flexLMin && flexStateR >= flexRMin)
    touchingWall = true;
    fullStop();
  }

  if(touchingWall){ //rotateRightFor depends on Battery Level/Timing for 90 Turns
    if(tier == 1){ //B1
      rotateRightFor(time90);
      if(tier != 1){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 2){ //B2
      rotateRightFor(time90);
      if(tier != 2){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 3){ //B4
      rotateLeftFor(time90);
      if(tier != 3){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 4){ //B5
      rotateLeftFor(time90);
      if(tier != 4){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 5){ //B7
      rotateRightFor(time90);
      if(tier != 5){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 6){ //B9
      rotateRightFor(time90);
      if(tier != 6){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 7){ //C1
      rotateRightFor(time90);
      if(tier != 7){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 8){ //C2
      rotateLeftFor(time90);
      if(tier != 8){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 9){ //C3
      rotateLeftFor(time90);
      if(tier != 9){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 10){ //C6
      rotateLeftFor(time90);
      if(tier != 10){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 11){ //C7
      rotateLeftFor(time90);
      if(tier != 11){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 12){ //C8
      rotateRightFor(time90);
      if(tier != 12){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 13){ //C9
      rotateRightFor(time90);
      if(tier != 13){
        touchingWall = false;
        fullStop();
      }
    }
    else{
      touchingWall = false;
      fullStop();
    }
  }
  else{
    flexAutonomous(); //driveForward();
  }
  
}

void blueA(){ //Suriel's Room - RIGHT TO LEFT

  runtime = millis();
  
  digitalWrite(distanceLT, LOW);
  delayMicroseconds(2);

  digitalWrite(distanceLT, HIGH);
  delayMicroseconds(10);
  digitalWrite(distanceLT, LOW);

  durationL = pulseIn(distanceLE, HIGH);
  distanceL = durationL*0.034/2;

  Serial.print("Distance: ");
  Serial.println(distanceL);
  
  //int flexStateL = analogRead(flexL);
  //int flexStateR = analogRead(flexR);
  //School: 100, 190; Home: 130, 160; 110, 140; 100, 130; 100, 190; 60, 160; 
  if((!touchingWall) && (distanceL >= disLeftBound && distanceL <= disRightBound)){ //&& (flexStateL >= flexLMin && flexStateR >= flexRMin)
    touchingWall = true;
    fullStop();
  }

  if(touchingWall){
    if(tier == 1){ //B1
      rotateLeftFor(time90);
      if(tier != 1){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 2){ //B2
      rotateLeftFor(time90);
      if(tier != 2){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 3){ //B4
      rotateRightFor(time90);
      if(tier != 3){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 4){ //B5
      rotateRightFor(time90);
      if(tier != 4){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 5){ //B7
      rotateLeftFor(time90);
      if(tier != 5){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 6){ //B9
      rotateLeftFor(time90);
      if(tier != 6){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 7){ //C1
      rotateLeftFor(time90);
      if(tier != 7){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 8){ //C2
      rotateRightFor(time90);
      if(tier != 8){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 9){ //C3
      rotateRightFor(time90);
      if(tier != 9){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 10){ //C6
      rotateRightFor(time90);
      if(tier != 10){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 11){ //C7
      rotateRightFor(time90);
      if(tier != 11){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 12){ //C8
      rotateLeftFor(time90);
      if(tier != 12){
        touchingWall = false;
        fullStop();
      }
    }
    else if(tier == 13){ //C9
      rotateLeftFor(time90);
      if(tier != 13){
        touchingWall = false;
        fullStop();
      }
    }
    else{
      tier = 1;
    }
  }
  else{
    flexAutonomous(); //driveForward();
  }
}




























//Everything off
void shutDown(){
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, LOW);

  digitalWrite(frontLeft, LOW);
  digitalWrite(frontRight, LOW);
  digitalWrite(backLeft, LOW);
  digitalWrite(backRight, LOW);
}
//Bluetooth
void bluetooth(){
  //check whether arduino is reciving signal or not 
  while(Serial.available() == 0);
  char val = Serial.read() ;//reads the signal
  Serial.print(val);

  /*********Driving (Motors)*********/
  if(val == 'F'){
    moveForward();
  }
  else if(val == 'B'){
    moveBackward();
  }
  else if(val == 'L'){
    rotateLeft();
  }
  else if(val == 'R'){
    rotateRight();
  }
  else{
    fullStop();
  }
}
//Flex
void flexAutonomous(){

  runtime = millis();
  
  int flexStateL = analogRead(flexL);
  int flexStateR = analogRead(flexR);
  
  Serial.print("Left Flex State: ");
  Serial.print(flexStateL);
  Serial.print(" | Right Flex State: ");
  Serial.println(flexStateR);
  
  if(flexStateL >= flexLMin){
    //slowRotateRight();
    rotateRight();
  }
  else if(flexStateR >= flexRMin){
    //slowRotateLeft();
    rotateLeft();
  }
  else if(flexStateL < flexLMin && flexStateR < flexRMin){
    //slowMoveForward();
    moveForward();
  }
  else{
    
  }
  
}
void handleFlexL(){
  //getting the flex state and displaying it
  int flexState = analogRead(flexL);
  Serial.print("Left Flex State: ");
  Serial.println(flexState);
}
void handleFlexR(){
  //getting the flex state and displaying it
  int flexState = analogRead(flexR);
  Serial.print("Right Flex State: ");
  Serial.println(flexState);
}
//Time Driving
void driveForwardFor(long duration){
  if(resetStartTime){
    resetStartTime = false;
    startTime = runtime;
  }
  
  if(runtime < startTime + duration){
    moveForward();
  }
  else{
    fullStop();
    tier++;
    resetStartTime = true;
  }
}
void driveBackwardFor(long duration){
  if(resetStartTime){
    resetStartTime = false;
    startTime = runtime;
  }
  
  if(runtime < startTime + duration){
    moveBackward();
  }
  else{
    fullStop();
    tier++;
    resetStartTime = true;
  }
}
void rotateLeftFor(long duration){ //600 is 90 degrees
  if(resetStartTime){
    resetStartTime = false;
    startTime =  runtime;
  }
  
  if(runtime < startTime + duration){
    rotateLeft();
  }
  else{
    fullStop();
    tier++;
    resetStartTime = true;
  }
}
void rotateRightFor(long duration){
  if(resetStartTime){
    resetStartTime = false;
    startTime = runtime;
  }
  
  if(runtime < startTime + duration){
    rotateRight();
  }
  else{
    fullStop();
    tier++;
    resetStartTime = true;
  }
}
//Normal Driving
void moveForward(){
  //frontLight();
  frontBlinker();
  
  analogWrite(motorL1, speedM);
  analogWrite(motorL2, 0);
  
  analogWrite(motorR1, speedM);
  analogWrite(motorR2, 0);
  
  /*
  digitalWrite(motorL1,HIGH);  digitalWrite(motorR1,HIGH);
  digitalWrite(motorL2,LOW);   digitalWrite(motorR2,LOW);
  */
}
void moveBackward(){
  //backLight();
  backBlinker();
  
  analogWrite(motorL1, 0);
  analogWrite(motorL2, speedM);
  
  analogWrite(motorR1, 0);
  analogWrite(motorR2, speedM);
  
  /*
  digitalWrite(motorL1,LOW);  digitalWrite(motorR1,LOW);
  digitalWrite(motorL2,HIGH);   digitalWrite(motorR2,HIGH);
  */
}
void rotateLeft(){
  //leftLight();
  leftBlinker();
  
  analogWrite(motorL1, 0);
  analogWrite(motorL2, speedM);
  
  analogWrite(motorR1, speedM);
  analogWrite(motorR2, 0);
  
  /*
  digitalWrite(motorL1,LOW);  digitalWrite(motorR1,HIGH);
  digitalWrite(motorL2,HIGH);   digitalWrite(motorR2,LOW);
  */
}
void rotateRight(){
  //rightLight();
  rightBlinker();
  
  analogWrite(motorL1, speedM);
  analogWrite(motorL2, 0);
  
  analogWrite(motorR1, 0);
  analogWrite(motorR2, speedM);
 
  /*
  digitalWrite(motorL1,HIGH);  digitalWrite(motorR1,LOW);
  digitalWrite(motorL2,LOW);   digitalWrite(motorR2,HIGH);
  */
}

//Slow Driving
void slowMoveForward(){
  //frontLight();
  frontBlinker();
  
  analogWrite(motorL1, 175);
  analogWrite(motorL2, 0);
  
  analogWrite(motorR1, 175);
  analogWrite(motorR2, 0);
  
  /*
  digitalWrite(motorL1,HIGH);  digitalWrite(motorR1,HIGH);
  digitalWrite(motorL2,LOW);   digitalWrite(motorR2,LOW);
  */
}
void slowMoveBackward(){
  //backLight();
  backBlinker();
  
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 125);
  
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 125);
  
  /*
  digitalWrite(motorL1,LOW);  digitalWrite(motorR1,LOW);
  digitalWrite(motorL2,HIGH);   digitalWrite(motorR2,HIGH);
  */
}
void slowRotateLeft(){
  //leftLight();
  leftBlinker();
  
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 200);
  
  analogWrite(motorR1, 200);
  analogWrite(motorR2, 0);
  
  /*
  digitalWrite(motorL1,LOW);  digitalWrite(motorR1,HIGH);
  digitalWrite(motorL2,HIGH);   digitalWrite(motorR2,LOW);
  */
}
void slowRotateRight(){
  //rightLight();
  rightBlinker();
  
  analogWrite(motorL1, 200);
  analogWrite(motorL2, 0);
  
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 200);
  
  /*
  digitalWrite(motorL1,HIGH);  digitalWrite(motorR1,LOW);
  digitalWrite(motorL2,LOW);   digitalWrite(motorR2,HIGH);
  */
}
void fullStop(){
  allLightsOn();
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 0);
  
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 0);
  /*
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, LOW);
  */
}
//Lights
void allLightsOn(){
  digitalWrite(frontLeft, HIGH);
  digitalWrite(frontRight, HIGH);
  digitalWrite(backLeft, HIGH);
  digitalWrite(backRight, HIGH);
}
void allLightsOff(){
  digitalWrite(frontLeft, LOW);
  digitalWrite(frontRight, LOW);
  digitalWrite(backLeft, LOW);
  digitalWrite(backRight, LOW);
}
void blinking(){
  long t = millis()/100;
  if(t % 2 == 0){
    allLightsOn();
  }
  else{
    allLightsOff();
  }
}
void frontBlinker(){
  long t = millis()/100;
  if(t % 2 == 0){
    digitalWrite(frontLeft, HIGH);
    digitalWrite(frontRight, HIGH);
    digitalWrite(backLeft, LOW);
    digitalWrite(backRight, LOW);
  }
  else{
    allLightsOff();
  }
}
void backBlinker(){
  long t = millis()/100;
  if(t % 2 == 0){
    digitalWrite(frontLeft, LOW);
    digitalWrite(frontRight, LOW);
    digitalWrite(backLeft, HIGH);
    digitalWrite(backRight, HIGH);
  }
  else{
    allLightsOff();
  }
}
void leftBlinker(){
  long t = millis()/100;
  if(t % 2 == 0){
    digitalWrite(frontLeft, HIGH);
    digitalWrite(frontRight, LOW);
    digitalWrite(backLeft, HIGH);
    digitalWrite(backRight, LOW);
  }
  else{
    allLightsOff();
  }
}
void rightBlinker(){
  long t = millis()/100;
  if(t % 2 == 0){
    digitalWrite(frontLeft, LOW);
    digitalWrite(frontRight, HIGH);
    digitalWrite(backLeft, LOW);
    digitalWrite(backRight, HIGH);
  }
  else{
    allLightsOff();
  }
}
void frontLight(){
  digitalWrite(frontLeft, HIGH);
  digitalWrite(frontRight, HIGH);
  digitalWrite(backLeft, LOW);
  digitalWrite(backRight, LOW);
}
void backLight(){
  digitalWrite(frontLeft, LOW);
  digitalWrite(frontRight, LOW);
  digitalWrite(backLeft, HIGH);
  digitalWrite(backRight, HIGH);
}
void leftLight(){
  digitalWrite(frontLeft, HIGH);
  digitalWrite(frontRight, LOW);
  digitalWrite(backLeft, HIGH);
  digitalWrite(backRight, LOW);
}
void rightLight(){
  digitalWrite(frontLeft, LOW);
  digitalWrite(frontRight, HIGH);
  digitalWrite(backLeft, LOW);
  digitalWrite(backRight, HIGH);
}

//Distance
/**
 * Required method. Returns how long it took the sound to travel from trigger, to the object, and back to echo.
 */
long getRawDistanceL()
{
  digitalWrite(distanceLT, LOW);
  delayMicroseconds(2);
  digitalWrite(distanceLT, HIGH); // Trigger pin to HIGH
  delayMicroseconds(10); // 10us high
  digitalWrite(distanceLT, LOW); // Trigger pin to HIGH
  
  durationL = pulseIn(distanceLE,HIGH); // Waits for the echo pin to get high
  return durationL;
}
/**
 * returns the distance in millimeters
 */
long distanceInMML()
{
  long time1 = getRawDistanceL();
  // ((time)*(Speed of sound))/ toward and backward of object) * 10
  long DistanceCalc; // Calculation variable
  DistanceCalc = ((time1 /2.9) / 2); // Actual calculation in mm
  return DistanceCalc; // return calculated value
}
/*
 * returns the distance in inches, always rounds down!
 */
long distanceInInches1L()
{
  
  long time2 = getRawDistanceL();
  long DistanceCalc; // Calculation variable

  DistanceCalc = time2 / 74 / 2; // Actual calculation in inches
  return DistanceCalc; // return calculated value
}
/*
 * returns the distance in inches as a double(as a decimal)
 */
double distanceInInches2L()
{
  long time3 = getRawDistanceL();
  double DistanceCalc; // Calculation variable
  DistanceCalc = (double)time3 / 74.0 / 2.0; // Actual calculation in inches
  return DistanceCalc; // return calculated value
}
