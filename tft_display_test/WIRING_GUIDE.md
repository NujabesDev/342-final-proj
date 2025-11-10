# ST7796S 4.0" TFT Display Wiring Guide for ESP32

## Display to ESP32 Connections

| Display Pin | ESP32 Pin | Description |
|-------------|-----------|-------------|
| VCC         | 5V or 3.3V| Power (ESP32 can provide both) |
| GND         | GND       | Ground |
| CS          | GPIO 15   | Chip Select |
| RESET       | GPIO 4    | Reset |
| DC/RS       | GPIO 2    | Data/Command |
| SDI (MOSI)  | GPIO 23   | SPI Data In (Master Out) |
| SCK         | GPIO 18   | SPI Clock |
| LED         | 3.3V      | Backlight (or use a resistor to dim) |
| SDO (MISO)  | GPIO 19   | SPI Data Out (for SD card) |

## Touch Screen Connections (Optional - if using touch)

| Touch Pin   | ESP32 Pin | Description |
|-------------|-----------|-------------|
| T_CLK       | GPIO 18   | Shared with TFT SCK |
| T_CS        | GPIO 21   | Touch Chip Select |
| T_DIN       | GPIO 23   | Shared with TFT MOSI |
| T_DO        | GPIO 19   | Shared with TFT MISO |
| T_IRQ       | GPIO 22   | Touch Interrupt (optional) |

## Notes:
- VCC can be 3.3V or 5V (the display accepts both)
- If backlight is too bright, add a 100-220 ohm resistor between LED pin and 3.3V
- These are the default SPI pins for ESP32
- You can change CS, RESET, and DC pins in the TFT_eSPI User_Setup.h file
