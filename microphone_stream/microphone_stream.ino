/*
 * ESP32 I2S Microphone WiFi Audio Streamer
 *
 * Captures audio from Adafruit SPH0645LM4H I2S MEMS microphone
 * and streams it over WiFi via UDP for real-time monitoring
 *
 * Hardware:
 * - ESP32-WROOM-32
 * - Adafruit SPH0645LM4H I2S MEMS Microphone
 *
 * Connections (as per CLAUDE.md):
 * - BCLK  -> GPIO26
 * - LRCL  -> GPIO25
 * - DOUT  -> GPIO22
 * - SEL   -> GND (Left channel)
 * - GND   -> GND
 * - 3V    -> 3.3V
 */

#include <WiFi.h>
#include <WiFiUdp.h>
#include <driver/i2s.h>

// ===== WiFi Configuration =====
const char* ssid = "YOUR_WIFI_SSID";        // Change this to your WiFi name
const char* password = "YOUR_WIFI_PASSWORD"; // Change this to your WiFi password

// ===== UDP Configuration =====
const char* udpAddress = "192.168.1.100";    // Change to your computer's IP
const int udpPort = 5555;                    // UDP port to send audio

WiFiUDP udp;

// ===== I2S Configuration =====
#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE 16000  // 16kHz sample rate (good for voice)
#define I2S_BITS_PER_SAMPLE 32 // SPH0645LM4H outputs 18-bit in 32-bit frames
#define I2S_CHANNEL_COUNT 1    // Mono

// Pin definitions (from CLAUDE.md)
#define I2S_BCK_PIN 26   // Bit Clock
#define I2S_WS_PIN 25    // Word Select (LRCL)
#define I2S_DATA_PIN 22  // Data Out

// Buffer configuration
#define DMA_BUF_COUNT 4
#define DMA_BUF_LEN 512
#define SAMPLES_PER_PACKET 256

// Audio buffers
int32_t i2s_samples[SAMPLES_PER_PACKET];
int16_t audio_packet[SAMPLES_PER_PACKET];

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== ESP32 I2S Microphone WiFi Streamer ===");

  // Initialize WiFi
  initWiFi();

  // Initialize I2S for microphone
  initI2S();

  Serial.println("\nReady! Streaming audio over UDP...");
  Serial.printf("Target: %s:%d\n", udpAddress, udpPort);
  Serial.println("Sample Rate: 16kHz, Mono, 16-bit");
}

void loop() {
  size_t bytes_read = 0;

  // Read audio samples from I2S microphone
  esp_err_t result = i2s_read(I2S_PORT,
                              (void*)i2s_samples,
                              SAMPLES_PER_PACKET * sizeof(int32_t),
                              &bytes_read,
                              portMAX_DELAY);

  if (result == ESP_OK) {
    int samples_read = bytes_read / sizeof(int32_t);

    // Convert 32-bit I2S samples to 16-bit audio
    // SPH0645LM4H has 18-bit data in upper bits of 32-bit word
    for (int i = 0; i < samples_read; i++) {
      // Shift right to get 16-bit audio (takes upper 16 bits)
      audio_packet[i] = (int16_t)(i2s_samples[i] >> 16);
    }

    // Send audio packet via UDP
    udp.beginPacket(udpAddress, udpPort);
    udp.write((uint8_t*)audio_packet, samples_read * sizeof(int16_t));
    udp.endPacket();

    // Optional: Print audio level indicator every ~1 second
    static unsigned long last_print = 0;
    if (millis() - last_print > 1000) {
      int32_t sum = 0;
      for (int i = 0; i < samples_read; i++) {
        sum += abs(audio_packet[i]);
      }
      int16_t avg_level = sum / samples_read;
      Serial.printf("Audio Level: %d | ", avg_level);

      // Visual level indicator
      int bars = map(avg_level, 0, 5000, 0, 20);
      for (int i = 0; i < bars; i++) Serial.print("█");
      Serial.println();

      last_print = millis();
    }
  }
}

void initWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("ESP32 IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi connection failed!");
    Serial.println("Please check SSID and password");
  }
}

void initI2S() {
  Serial.println("Initializing I2S microphone...");

  // I2S configuration for SPH0645LM4H
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = DMA_BUF_COUNT,
    .dma_buf_len = DMA_BUF_LEN,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  // Pin configuration
  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_BCK_PIN,
    .ws_io_num = I2S_WS_PIN,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_DATA_PIN
  };

  // Install and configure I2S driver
  esp_err_t err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("Failed to install I2S driver: %d\n", err);
    return;
  }

  err = i2s_set_pin(I2S_PORT, &pin_config);
  if (err != ESP_OK) {
    Serial.printf("Failed to set I2S pins: %d\n", err);
    return;
  }

  // Clear I2S buffer
  i2s_zero_dma_buffer(I2S_PORT);

  Serial.println("I2S microphone initialized successfully!");
}
