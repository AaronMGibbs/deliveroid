///On Board Wifi Module (Receiver)

#include <ESP8266WiFi.h>
#include <espnow.h>




// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0xA4, 0xCF, 0x12, 0xC5, 0x17, 0x18};



//these variables determine the robot's state
int state1;





// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct receive_message {
    int ste;
    
} receive_message;


// Create a struct_message to hold incoming value from the host
recieve_message incomingReadings;


// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  state=receive_message.ste;
  
}
  
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);


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
 


