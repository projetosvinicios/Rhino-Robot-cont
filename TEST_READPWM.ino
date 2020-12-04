
  int throttle = 5;
  int steering = 6;

  int RightMotor = 9;
  int LeftMotor = 10; 
  int R_dir = 8;
  int L_dir = 12;
  
 const int MIN_THROTTLE = 1250, MAX_THROTTLE = 1800;
 const int MIN_STEERING = 1100, MAX_STEERING = 1850;
 const int THROTTLE_DEAD_ZONE = 50;
 const int STEERING_DEAD_ZONE = 20;
  
 int throttleHalfway = (MIN_THROTTLE + MAX_THROTTLE) / 2;
 int steeringHalfway = (MIN_STEERING + MAX_STEERING) / 2;
 
  void setup() {
    Serial.begin(9600);

//  reducing the pwm freq
    TCCR1B = TCCR1B & 0B11111000 | 0B00000100;
      
    pinMode(RightMotor,OUTPUT);
    pinMode(LeftMotor,OUTPUT);
    pinMode(R_dir,OUTPUT);
    pinMode(L_dir,OUTPUT);  
    
    pinMode(throttle, INPUT);
    pinMode(steering, INPUT);
  }

  void loop() {

    //CHECK IF IS RECEIVING SIGNAL
    if (pulseIn(throttle, HIGH) == 0 || pulseIn(steering, HIGH) == 0){
      stopMotors();
      Serial.println("Turn on the controller");
    }
    else {
      checkInputs();  
    } 
  }


  int checkInputs(){
    
    int throttlePosition = pulseIn(throttle, HIGH);
    int steeringPosition = pulseIn(steering, HIGH);

    bool isTurning = ((steeringPosition <= (steeringHalfway - STEERING_DEAD_ZONE)) || (steeringPosition > (steeringHalfway + STEERING_DEAD_ZONE)));
    bool isReving = ((throttlePosition <= (throttleHalfway - THROTTLE_DEAD_ZONE)) || (throttlePosition > (throttleHalfway + THROTTLE_DEAD_ZONE)));

    
    if ((throttlePosition > throttleHalfway + THROTTLE_DEAD_ZONE)) {
       //FORWARD
      int pwmSpeed = map(throttlePosition, throttleHalfway + THROTTLE_DEAD_ZONE, MAX_THROTTLE, 0, 255);

      if (isTurning){
        
      } else {
        forward(correctPWM(pwmSpeed), 0, 0);
      }
             
    } 
    
    else if ((throttlePosition < throttleHalfway - THROTTLE_DEAD_ZONE)) {
      //BACKWARDS
      int pwmSpeed = map(throttlePosition, MIN_THROTTLE, throttleHalfway - THROTTLE_DEAD_ZONE, 255, 0);

      if (isTurning){
        
      } else {
        backward(correctPWM(pwmSpeed), 0, 0);
      }
    }

    //TURN IN ITS AXIS
    else if (isTurning && !isReving){
      
      // CHECK WHICH WAY IS TURNING
      if ((steeringPosition > steeringHalfway + STEERING_DEAD_ZONE)) {
        //RIGHT
        int pwmSpeed = map(steeringPosition, steeringHalfway + STEERING_DEAD_ZONE, MAX_STEERING, 0, 255);
        right(correctPWM(pwmSpeed));
      }
      else{
        //LEFT
        int pwmSpeed = map(steeringPosition, MIN_STEERING, steeringHalfway - STEERING_DEAD_ZONE, 255, 0);
        left(correctPWM(pwmSpeed));
      } 
    }
    
    //JOYSTIC IN DEAD ZONE  
    else{
      stopMotors();
    }  
  }

  bool isTurningLeft(int steeringPosition){
    return (steeringPosition < steeringHalfway - STEERING_DEAD_ZONE);
  }

  bool isTurningRight(int steeringPosition){
    return (steeringPosition > steeringHalfway + STEERING_DEAD_ZONE);
  }
  
  int correctPWM(int pwm){
    int pwmValue = pwm;
    if (pwmValue < 0) {pwmValue = 0;}
    if (pwmValue > 255) {pwmValue = 255;}
    return pwmValue;
  }
    
  void forward(unsigned int pwm, int decelerationL, int decelerationR){
    digitalWrite(R_dir,LOW); 
    digitalWrite(L_dir,LOW);
    analogWrite(RightMotor, pwm - decelerationR);
    analogWrite(LeftMotor, pwm - decelerationL);
  }
    
  void backward(unsigned int pwm, int decelerationL, int decelerationR){
    digitalWrite(R_dir,HIGH);
    digitalWrite(L_dir,HIGH);
    analogWrite(RightMotor,pwm);
    analogWrite(LeftMotor,pwm);
  }
   
  void left(unsigned int pwm){
    digitalWrite(R_dir,LOW);
    digitalWrite(L_dir,HIGH);
    analogWrite(RightMotor,pwm);
    analogWrite(LeftMotor,pwm);
  }

  void right(unsigned int pwm){
    digitalWrite(R_dir,HIGH);
    digitalWrite(L_dir,LOW);
    analogWrite(RightMotor, pwm);
    analogWrite(LeftMotor, pwm); 
  }
     
  void stopMotors(){
    digitalWrite(RightMotor,LOW);
    digitalWrite(LeftMotor,LOW);
  } 
