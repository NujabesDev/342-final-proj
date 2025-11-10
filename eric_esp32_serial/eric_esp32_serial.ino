#include <WiFi.h>
#include <esp_now.h>

// Eric's Upload - Interactive Message Version
uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0x2D, 0x18, 0xD8};

typedef struct struct_message {
  char text[200];
} struct_message;

struct_message incomingMessage;
struct_message outgoingMessage;

esp_now_peer_info_t peerInfo;

void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  if (status != ESP_NOW_SEND_SUCCESS) {
    Serial.println("[ERIC] Delivery Failed!");
  }
}

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));
  Serial.print("[RYAN → ERIC]: ");
  Serial.println(incomingMessage.text);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n[ERIC] ESP-NOW Interactive Messenger");

  WiFi.mode(WIFI_STA);

  Serial.print("[ERIC] MAC Address: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("[ERIC] Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("[ERIC] Failed to add peer");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("[ERIC] Ready! Type a message and press Enter to send.\n");
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
        Serial.print("[ERIC → RYAN]: ");
        Serial.println(outgoingMessage.text);
      } else {
        Serial.println("[ERIC] Error sending message");
      }
    }
  }
}
