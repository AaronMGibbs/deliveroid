#include <Servo.h>
#include <NewPing.h>

// H-BRIDGE PINS
int IN1A        = 3; 
int IN1B        = 5; 
int IN2A        = 6; 
int IN2B        = 11; 

// PROX SENSOR PINS 
#define trig_pin A1
#define echo_pin A0 

// SERVO MOTOR
Servo servo_motor; //our servo name


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
NewPing sonar(trig_pin, echo_pin, maximum_distance); //sensor function

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
  int distanceRight, distanceLeft;

  while(1){
    deliveroid_move_forward_infinite(350);
    if (prox_distance <= 60){
      
      deliveroid_stop_1sec();
      delay(300);
      deliveroid_move_backwards(3500, 1500);
      delay(300);
      distanceRight = lookRight();
      delay(300);
      distanceLeft = lookLeft();
      delay(300);

      if (distanceRight <= distanceLeft){
        deliveroid_turn_right(400, 1000);
      }
      else{
        deliveroid_turn_left(400, 1000);
      }
    }
    prox_distance = readPing();
  } 
}

/////////////////////////
// PROX & SERVO METHODS //
/////////////////////////

int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}

int lookRight(){
  servo_motor.write(0);
  delay(500);
  int distance = readPing();
  delay(100);
  return distance;
}

int lookLeft(){
  servo_motor.write(180);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(90);
  return distance;
  delay(100);
}

void setup() {
  // Serial.begin(9600);

  // SETTING H-BRIDGE PINS
  pinMode(IN1A, OUTPUT);
  pinMode(IN1B, OUTPUT);
  pinMode(IN2A, OUTPUT);
  pinMode(IN2B, OUTPUT);

  // SETTING SERVO PINS
  servo_motor.attach(13); 
}

void loop() {

  deliveroid_basic_motor_test(500, 500);
  deliveroid_roam();
  STOP_PROGRAM();

}
