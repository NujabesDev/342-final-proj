#include <WiFi.h>
#include <esp_now.h>

// Eric's Upload - Serial Output Only Version
uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0x2D, 0x18, 0xD8};

bool currentState = false;
bool receivedState = false;
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 2000; // Send every 2 seconds

typedef struct struct_message {
  bool state;
} struct_message;

struct_message stateIn;
struct_message stateOut;

esp_now_peer_info_t peerInfo;

void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("\r\n[ERIC] Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&stateIn, incomingData, sizeof(stateIn));
  Serial.print("[ERIC] Bytes received: ");
  Serial.println(len);
  receivedState = stateIn.state;
  Serial.print("[ERIC] Received state: ");
  Serial.println(receivedState ? "TRUE" : "FALSE");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n[ERIC] ESP-NOW Serial Test - Starting...");

  WiFi.mode(WIFI_STA);

  Serial.print("[ERIC] MAC Address: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("[ERIC] Error initializing ESP-NOW");
    return;
  }

  Serial.println("[ERIC] ESP-NOW initialized successfully");

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("[ERIC] Failed to add peer");
    return;
  }

  Serial.println("[ERIC] Peer added successfully");

  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("[ERIC] Ready to send/receive data\n");
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastSendTime >= sendInterval) {
    lastSendTime = currentTime;
    currentState = !currentState;
    stateOut.state = currentState;

    Serial.print("[ERIC] Sending state: ");
    Serial.println(currentState ? "TRUE" : "FALSE");

    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &stateOut, sizeof(stateOut));

    if (result == ESP_OK) {
      Serial.println("[ERIC] Sent with success");
    } else {
      Serial.println("[ERIC] Error sending the data");
    }
  }
}
