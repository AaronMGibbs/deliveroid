///On Board Wifi Module (Receiver)

#include <ESP8266WiFi.h>
#include <espnow.h>


// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0x8C, 0xAA, 0xB5, 0x0C, 0xDF, 0x34};



//these variables determine the robot's state
//the states are represented like this: state1 state2 state3 EX: if state1=1 and state2=0 and state3=0, this output is 100
char state[4];
int state1;
int state2;
int state3;



// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct receive_message {
     char ste1[4];
  
    
} receive_message;


// Create a struct_message to hold incoming value from the host
receive_message incomingReadings;


// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  strcpy(state, incomingReadings.ste1);
  
  state1=state[0]-'0';
  state2=state[1]-'0';
  state3=state[2]-'0';
  if (state1==1){
     digitalWrite(D1, 0);
  }
  else{
     digitalWrite(D1, 1);
  }
  
  if (state2==3){
     digitalWrite(D2, 0);
  }
  else{
     digitalWrite(D2, 1);
  }

  if (state3==5){
    digitalWrite(D5, 0);
  }
  else{
    digitalWrite(D5, 1);
  }
  
}
  
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println(WiFi.macAddress());


  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  
  
  
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  }
