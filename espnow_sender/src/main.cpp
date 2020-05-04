/*
Mengirim data ke 2 node : 
node 1 dengan mac address :0xA4, 0xCF, 0x12, 0x05, 0xDF, 0x00
node 2 dengan mac address :0x30, 0xAE, 0xA4, 0x04, 0x3D, 0x98

*/
#include <esp_now.h>
#include <WiFi.h>
// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress1[] = {0xA4, 0xCF, 0x12, 0x05, 0xDF, 0x00};
uint8_t broadcastAddress2[] = {0x30, 0xAE, 0xA4, 0x04, 0x3D, 0x98};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Menambah Mac address node yang dituju node 1       
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
// Menambah Mac address node yang dituju node 2
  memcpy(peerInfo.peer_addr,broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Set values to send
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = "Hello";
  myData.e = false;

  // Send message via ESP-NOW
  //esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData));    // fungsi untuk mengirim data yaang sama ke semua node peer yang terdaftar. 
  esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) &myData, sizeof(myData));
  esp_err_t result2 = esp_now_send(broadcastAddress2, (uint8_t *) &myData, sizeof(myData));
  if (result == ESP_OK) {
    Serial.println("Sent with success node 1");
  }
  else {
    Serial.println("Error sending the data node 1");
  }
  if (result2 == ESP_OK) {
    Serial.println("Sent with success node 2");
  }
  else {
    Serial.println("Error sending the data node 2");
  }
  delay(2000);
}