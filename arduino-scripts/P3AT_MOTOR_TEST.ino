#include <NewPing.h>

// H-BRIDGE PINS
int IN1A        = 3; 
int IN1B        = 5; 
int IN2A        = 6; 
int IN2B        = 11; 

// PROX SENSOR PINS 
#define trig_pin1 A1
#define echo_pin1 A0

#define trig_pin2 A3
#define echo_pin2 A2

#define trig_pin3 A5
#define echo_pin3 A4


#define MOTOR_DEFAULT_SPEED     1000

// RETURN CODES
#define DELIVEROID_SUCCESS        0
#define TIME_OUT_OF_RANGE_ERROR   1
#define SPEED_OUT_OF_RANGE_ERROR  2
   

// MAX VALUES
#define MAX_SPEED                 1000
#define MAX_TIME                  65536  
#define maximum_distance          200    

// prox sensor distance
int prox_distance = 100;
NewPing sonar(trig_pin1, echo_pin1, maximum_distance); //sensor function
NewPing sonar1(trig_pin2, echo_pin2, maximum_distance);
NewPing sonar2(trig_pin3, echo_pin3, maximum_distance);

//////////////////
// ERROR METHOD //
//////////////////
void ERROR(int return_code){
  Serial.print("ERROR[RETURN CODE: ");
  Serial.print(return_code);
  Serial.println("]");
}

void STOP_PROGRAM(){
  while(1){}
}
  

/////////////////////////
// BASIC MOTOR METHODS //
/////////////////////////

// A - LEFT
// B - RIGHT
// 1 - BACKWARDS
// 2 - FORWARDS

int deliveroid_stop(uint16_t duration){
  analogWrite(IN1A, 0);
  analogWrite(IN1B, 0);
  analogWrite(IN2A, 0);
  analogWrite(IN2B, 0);
  delay(duration);
  
  return DELIVEROID_SUCCESS;
} 

int deliveroid_move_forward(uint16_t speed, uint16_t duration){

  if (speed < 0 || speed > MAX_SPEED){
    return SPEED_OUT_OF_RANGE_ERROR;
  }

  digitalWrite(IN1A, LOW);
  digitalWrite(IN1B, LOW); //
  analogWrite(IN2A, speed); //
  analogWrite(IN2B, speed);
  delay(duration);
  deliveroid_stop(10);
  return DELIVEROID_SUCCESS;
}


int deliveroid_move_backwards(uint16_t speed, uint16_t duration){

  if (speed < 0 || speed > MAX_SPEED){
    return SPEED_OUT_OF_RANGE_ERROR;
  }

  digitalWrite(IN2A, LOW);
  digitalWrite(IN2B, LOW); //
  analogWrite(IN1A, speed); //
  analogWrite(IN1B, speed);
  delay(duration);
  deliveroid_stop(10);
  return DELIVEROID_SUCCESS;  
}

int deliveroid_turn_left(uint16_t speed, uint16_t duration){

  if (speed < 0 || speed > MAX_SPEED){
    return SPEED_OUT_OF_RANGE_ERROR;
  }

  digitalWrite(IN1A, LOW);
  digitalWrite(IN2B, LOW);
  analogWrite(IN1B, speed);
  analogWrite(IN2A, speed);
  delay(duration);
  deliveroid_stop(10);
  return DELIVEROID_SUCCESS;
}

int deliveroid_turn_right(uint16_t speed, uint16_t duration){

  if (speed < 0 || speed > MAX_SPEED){
    return SPEED_OUT_OF_RANGE_ERROR;
  }

  digitalWrite(IN1B, LOW);
  digitalWrite(IN2A, LOW);
  analogWrite(IN2B, speed);
  analogWrite(IN1A, speed);
  delay(duration);
  deliveroid_stop(10);
  return DELIVEROID_SUCCESS;
}


int deliveroid_move_forward_infinite(uint16_t speed){
  if (speed < 0 || speed > MAX_SPEED){
    return SPEED_OUT_OF_RANGE_ERROR;
  }

  digitalWrite(IN1A, LOW);
  digitalWrite(IN1B, LOW); //
  analogWrite(IN2A, speed); //
  analogWrite(IN2B, speed);

  delay(100);
  return DELIVEROID_SUCCESS;
}

////////////////////////////
// ADVANCED MOTOR METHODS //
////////////////////////////

void deliveroid_stop_1sec(){
  deliveroid_stop(1000);
}
  

int deliveroid_basic_motor_test(uint16_t speed, uint16_t duration){
  int return_code;

  return_code = deliveroid_move_forward(speed, duration);

  if (return_code != DELIVEROID_SUCCESS){
    ERROR(return_code);
  }

  deliveroid_stop_1sec();

  return_code = deliveroid_move_backwards(speed, duration);
  
  if (return_code != DELIVEROID_SUCCESS){
    ERROR(return_code);
  }

  deliveroid_stop_1sec();

  return_code = deliveroid_turn_right(speed, duration);
  
  if (return_code != DELIVEROID_SUCCESS){
    ERROR(return_code);
  }

  deliveroid_stop_1sec();

  return_code = deliveroid_turn_left(speed, duration);
  
  if (return_code != DELIVEROID_SUCCESS){
    ERROR(return_code);
  }

  deliveroid_stop_1sec();

  return_code = deliveroid_stop(duration);
  
  if (return_code != DELIVEROID_SUCCESS){
    ERROR(return_code);
  }

  deliveroid_stop_1sec();
return DELIVEROID_SUCCESS;
}


int deliveroid_roam(){
  int distanceRight, distanceLeft, distanceMiddle;

  while(1){
    deliveroid_move_forward_infinite(350);
    distanceMiddle = readsensorMiddle();
    
    distanceRight = readsensorRight();
    
    distanceLeft = readsensorLeft();
    
    if (distanceMiddle <= 60 || distanceRight <= 10 || distanceLeft <= 10){
      
      deliveroid_stop_1sec();
      delay(300);
      deliveroid_move_backwards(3500, 1500);
      delay(300);

      if (distanceRight <= distanceLeft){
        deliveroid_turn_right(400, 1000);
      }
      else{
        deliveroid_turn_left(400, 1000);
      }
    }
    

  } 
}

/////////////////////////
// PROX SENSOR METHODS //
/////////////////////////

int readsensorMiddle(){
  delay(70);
  int sensorMiddle = sonar.ping_cm();
  if (sensorMiddle==0){
    sensorMiddle=250;
  }
  return sensorMiddle;
}

int readsensorRight(){
  delay(70);
  int sensorRight = sonar1.ping_cm();
  if (sensorRight==0){
    sensorRight=250;
  }
  return sensorRight;
}

int readsensorLeft(){
  delay(70);
  int sensorLeft = sonar2.ping_cm();
  if (sensorLeft==0){
    sensorLeft=250;
  }
  return sensorLeft;
}


void setup() {
  // Serial.begin(9600);

  // SETTING H-BRIDGE PINS
  pinMode(IN1A, OUTPUT);
  pinMode(IN1B, OUTPUT);
  pinMode(IN2A, OUTPUT);
  pinMode(IN2B, OUTPUT);

}

void loop() {

  deliveroid_basic_motor_test(500, 500);
  deliveroid_roam();
  STOP_PROGRAM();

}
