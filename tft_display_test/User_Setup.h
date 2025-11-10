// USER_SETUP.H FOR ST7796S 4.0" DISPLAY
// Copy this file to: Arduino/libraries/TFT_eSPI/User_Setup.h
// Or rename User_Setup.h to User_Setup_backup.h and use this one

// ##################################################################################
// Driver selection
// ##################################################################################
#define ST7796_DRIVER  // ST7796S 480x320 display

// ##################################################################################
// Display resolution
// ##################################################################################
#define TFT_WIDTH  320
#define TFT_HEIGHT 480

// ##################################################################################
// ESP32 pin configuration
// ##################################################################################
#define TFT_MISO 19  // (leave TFT SDO disconnected if not using)
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15  // Chip select control pin
#define TFT_DC    2  // Data Command control pin
#define TFT_RST   4  // Reset pin (could connect to RST pin on ESP32)

// ##################################################################################
// Optional: Touch screen pins (XPT2046)
// ##################################################################################
#define TOUCH_CS 21  // Chip select pin (T_CS) of touch screen

// ##################################################################################
// Fonts
// ##################################################################################
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

// ##################################################################################
// SPI frequency
// ##################################################################################
#define SPI_FREQUENCY  27000000  // 27 MHz (safe for most displays)
// #define SPI_FREQUENCY  40000000  // 40 MHz (faster, try if 27MHz works)
// #define SPI_FREQUENCY  80000000  // 80 MHz (maximum, may cause issues)

#define SPI_READ_FREQUENCY  20000000  // For reading from display
#define SPI_TOUCH_FREQUENCY  2500000  // For XPT2046 touch controller
