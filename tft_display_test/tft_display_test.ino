#include <TFT_eSPI.h>

// Create TFT instance
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  Serial.println("ST7796S Display Test Starting...");

  // Initialize the display
  tft.init();
  tft.setRotation(1); // Landscape mode (0-3 for different orientations)

  // Test 1: Fill screen with colors
  Serial.println("Test 1: Color fill test");
  tft.fillScreen(TFT_RED);
  delay(500);
  tft.fillScreen(TFT_GREEN);
  delay(500);
  tft.fillScreen(TFT_BLUE);
  delay(500);
  tft.fillScreen(TFT_BLACK);

  // Test 2: Draw text
  Serial.println("Test 2: Text test");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("ST7796S Display Test");
  tft.setCursor(10, 40);
  tft.println("Resolution: 480x320");
  tft.setCursor(10, 70);
  tft.setTextColor(TFT_YELLOW);
  tft.println("Display is working!");

  // Test 3: Draw shapes
  Serial.println("Test 3: Shape test");
  tft.drawRect(10, 100, 100, 80, TFT_CYAN);
  tft.fillRect(120, 100, 100, 80, TFT_MAGENTA);
  tft.drawCircle(280, 140, 40, TFT_GREEN);
  tft.fillCircle(380, 140, 40, TFT_ORANGE);

  // Test 4: Draw lines
  Serial.println("Test 4: Line test");
  for (int i = 0; i < 480; i += 20) {
    tft.drawLine(0, 200, i, 320, TFT_WHITE);
  }

  Serial.println("Display test complete!");
  Serial.println("If you see colors and shapes, the display is working correctly.");
}

void loop() {
  // Rainbow cycle animation
  static uint16_t hue = 0;

  tft.fillRect(0, 280, 480, 40, tft.color565(
    (1.0 + sin(hue * 0.01)) * 127.5,
    (1.0 + sin(hue * 0.01 + 2.094)) * 127.5,
    (1.0 + sin(hue * 0.01 + 4.189)) * 127.5
  ));

  hue += 5;
  delay(50);
}
