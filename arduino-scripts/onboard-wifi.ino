///On Board Wifi Module

#include <ESP8266WiFi.h>
#include <espnow.h>




// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0xA4, 0xCF, 0x12, 0xC5, 0x17, 0x18};

//these variables determine where the robot will turn and enable it to go
uint8_t state;
bool globalenable;

// Define variables to store incoming readings
// int for incoming obstacle is used to determine what direction the obstruction is in (1 for left, 2 for center, 3 for right)
int incomingobstacle;



// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct recieve_message {
    uint8_t ste;
    
} send_message;

typedef struct send_message {
    int obst;
} recieve_message;

// Create a struct_message to hold directions to send to host
send_message obstacle;

// Create a struct_message to hold incoming value from the host
recieve_message incomingReading;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  state=recieve_message.ste;
  
}
//////////////////This is where we'll do obstacle calculations//////////////////////////
void obstaclecalc(){
  obst= 0;
}


    
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  Serial1.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  bool globalenable;
  
  if (globalenable==0) {
    

    //Get obstacle message
    obstaclecalc();

    //Set values to send
    send_message.obst = incomingobstacle;
  

    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &obstacle, sizeof(obstacle));

 
  }
}
 


