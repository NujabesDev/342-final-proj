#include <WiFi.h>
#include <esp_now.h>

// Ryan's Upload - Interactive Message Version
uint8_t broadcastAddress[] = {0xEC, 0xE3, 0x34, 0x1A, 0x7F, 0x38};

typedef struct struct_message {
  char text[200];
} struct_message;

struct_message incomingMessage;
struct_message outgoingMessage;

esp_now_peer_info_t peerInfo;

void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  if (status != ESP_NOW_SEND_SUCCESS) {
    Serial.println("[RYAN] Delivery Failed!");
  }
}

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));
  Serial.print("[ERIC → RYAN]: ");
  Serial.println(incomingMessage.text);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n[RYAN] ESP-NOW Interactive Messenger");

  WiFi.mode(WIFI_STA);

  Serial.print("[RYAN] MAC Address: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("[RYAN] Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("[RYAN] Failed to add peer");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("[RYAN] Ready! Type a message and press Enter to send.\n");
}

void loop() {
  // Check if there's data available from Serial Monitor
  if (Serial.available() > 0) {
    String inputString = Serial.readStringUntil('\n');
    inputString.trim(); // Remove any whitespace

    if (inputString.length() > 0) {
      // Copy the message to the outgoing struct
      inputString.toCharArray(outgoingMessage.text, sizeof(outgoingMessage.text));

      // Send the message
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &outgoingMessage, sizeof(outgoingMessage));

      if (result == ESP_OK) {
        Serial.print("[RYAN → ERIC]: ");
        Serial.println(outgoingMessage.text);
      } else {
        Serial.println("[RYAN] Error sending message");
      }
    }
  }
}
