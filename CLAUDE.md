# Project Hardware Specifications

## Project Components Overview

This document contains the complete technical specifications for the microphone-driven ESP32 display project.

---

## 1. Adafruit I2S MEMS Microphone Breakout - SPH0645LM4H

### Product Information
- **Model**: SPH0645LM4H
- **Product ID**: 3421
- **Type**: Digital I2S MEMS Microphone
- **Manufacturer**: Adafruit Industries

### Key Features
- **Digital Output**: Pure I2S digital interface (no analog output)
- **Frequency Range**: 50Hz - 15KHz
- **Microphone Type**: Single mono element, bottom ported
- **Power Requirements**: 1.6V - 3.6V (NOT compatible with 5V logic)
- **Interface Type**: I2S (Inter-IC Sound)

### Pin Configuration
- **BCLK** (Bit Clock): High-frequency clock driven by I2S controller
- **DOUT** (Data Out): Digital audio data output
- **LRCL** (Left-Right Clock / Word Select): Channel selection clock
- **SEL** (Select): Channel selection pin
  - Connect to GND for Left channel
  - Connect to 3V for Right channel
- **GND**: Ground
- **3V**: Power (3.3V)

### Physical Specifications
- **Dimensions**: 16.7mm x 12.7mm x 1.8mm (0.7" x 0.5" x 0.1")
- **Weight**: 0.4g (0.0oz)
- **Mounting**: 2 circular mounting holes
- **Port Orientation**: Bottom ported (hole must face sound source)

### Important Notes
- Requires I2S peripheral support (common on Cortex M-series and ESP32)
- Can create stereo setup with two microphones (one SEL high, one SEL low)
- Best compatibility: Arduino Zero, Feather M0, Raspberry Pi, **ESP32**
- Revision: Updated PCB with Adafruit Pinguin silkscreen (as of Nov 23, 2022)

### Technical Specifications
- **Bit Depth**: Configurable via I2S protocol
- **Sample Rate**: Configurable via I2S controller
- **SNR**: Standard for MEMS microphones in this class
- **Sensitivity**: Optimized for voice and general audio

---

## 2. Hosyond 4.0" TFT Touch Screen LCD Display Module

### Product Information
- **Model**: MSP4022
- **Brand**: Hosyond
- **Type**: SPI TFT LCD with Resistive Touch

### Display Specifications
- **Size**: 4.0 inches diagonal
- **Resolution**: 480 x 320 pixels
- **Driver IC**: ST7796S
- **Display Type**: TFT (Thin Film Transistor)
- **Color Support**: RGB 65K colors (16-bit color depth)
- **Active Area**: 55.68mm x 83.52mm

### Interface
- **Display Interface**: 4-wire SPI serial bus
- **Touch Interface**: SPI (XPT2046 touch controller)
- **Additional Features**: SD card slot for expansion

### Pin Configuration
#### Display SPI Pins
- **MOSI** (Master Out Slave In): Data input
- **MISO** (Master In Slave Out): Data output (for SD card)
- **SCK** (Serial Clock): SPI clock
- **CS** (Chip Select): Display chip select
- **DC** (Data/Command): Command/data selection
- **RST** (Reset): Hardware reset
- **BL** (Backlight): Backlight control

#### Touch Screen Pins (XPT2046)
- **T_CLK**: Touch clock
- **T_CS**: Touch chip select
- **T_DIN**: Touch data in
- **T_DO**: Touch data out
- **T_IRQ**: Touch interrupt (optional)

### Power Specifications
- **Operating Voltage**: 3.3V - 5V (flexible power input)
- **Logic Level**: 3.3V compatible (safe for ESP32)
- **Power Consumption**: TBD (typical for 4" TFT backlit display)

### Physical Specifications
- **PCB Dimensions**: 61.74mm x 108.04mm
- **PCB Size (inches)**: 4.25"L x 2.43"W x 2.43"H
- **Weight**: 3.5 oz (99g)
- **Touch Type**: Resistive (works with stylus, gloves, any pressure)

### Compatibility
- Arduino R3, Nano, Mega2560
- ESP32 (3.3V native compatibility)
- Any microcontroller with SPI support

### Included Components
- 1x 4.0" touch screen module
- 1x touch pen/stylus

### Software Support
- Rich sample programs provided
- Driver libraries available for ST7796S
- Works with:
  - TFT_eSPI library (recommended for ESP32)
  - Adafruit GFX library
  - LVGL (graphics library)

---

## 3. ESP32 Development Board (ESP-WROOM-32)

### Product Information
- **Model**: ESP-WROOM-32 / ESP32-S
- **Chip**: ESP32-D0WD or ESP32-D0WD-V3
- **Manufacturer**: Espressif Systems
- **Brand**: AITRIP / HiLetgo / Generic

### Processor Specifications
- **CPU**: Dual-core Xtensa LX6
- **Clock Speed**: Up to 240 MHz (adjustable)
- **Cores**: 2 independent cores (Core 0 and Core 1)
- **Architecture**: 32-bit RISC
- **Manufacturing Process**: TSMC 40nm ultra-low power

### Memory
- **SRAM**: 520 KB (512 KB usable)
- **ROM**: 448 KB
- **Flash**: 4 MB (typical, varies by board)
- **External PSRAM**: Not included on WROOM-32 (available on WROVER variant)
- **RTC Memory**: 8 KB SRAM (for deep sleep data retention)

### Wireless Capabilities
- **Wi-Fi**: 802.11 b/g/n (2.4 GHz)
  - Modes: Station (STA), Access Point (AP), STA+AP
  - Security: WPA/WPA2/WPA2-Enterprise/WPS
  - Built-in antenna with RF balun, power amplifier, filters
- **Bluetooth**:
  - Classic Bluetooth v4.2 BR/EDR
  - Bluetooth Low Energy (BLE)

### Power Specifications
- **Operating Voltage**: 3.3V (internal logic)
- **Input Voltage**: 5V via USB or VIN pin
- **Deep Sleep Current**: ~10 μA
- **Modem Sleep Current**: ~3 mA
- **Active Current**: 80-160 mA (depending on Wi-Fi/BT usage)
- **Max Current per GPIO**: 12 mA (40 mA absolute max)

### GPIO and Peripherals
- **Total GPIO**: 34 (some are input-only)
- **ADC**: 18 channels, 12-bit SAR ADC
- **DAC**: 2 channels, 8-bit
- **Touch Sensors**: 10 capacitive touch GPIOs
- **SPI**: 4 SPI interfaces (HSPI and VSPI available for use)
- **I2C**: 2 I2C interfaces
- **I2S**: 2 I2S interfaces (perfect for audio)
- **UART**: 3 UART interfaces
- **PWM**: 16 channels (LED PWM controller)
- **CAN**: CAN 2.0 controller

### Special Features
- **Hall Effect Sensor**: Built-in
- **Temperature Sensor**: Built-in (approximate, ±2°C accuracy)
- **Cryptographic Hardware Acceleration**: AES, SHA-2, RSA, ECC
- **RTC**: Real-time clock with alarm
- **Watchdog Timers**: Multiple WDTs for reliability

### Physical Specifications
- **Dimensions**: ~52mm x 28mm (varies by manufacturer)
- **Pin Count**: 30 pins (38 on some DevKit variants)
- **Pin Pitch**: 2.54mm (0.1" standard)
- **USB Interface**: Micro USB or USB-C (depending on model)
- **USB-to-Serial**: CP2102 or CH340G chip

### Pin Layout Notes
- **Boot Mode Pins**:
  - GPIO0: Must be LOW during boot for flash mode
  - GPIO2: Must be floating or HIGH during boot
  - GPIO15: Must be HIGH during boot
- **Input Only Pins**: GPIO34, 35, 36, 39 (no internal pull-up/down)
- **Reserved/Special**: GPIO6-11 (connected to flash, do not use)

### Development Support
- **IDE Support**:
  - Arduino IDE (via ESP32 board package)
  - PlatformIO
  - ESP-IDF (Espressif IoT Development Framework)
- **Programming Languages**:
  - C/C++ (Arduino framework)
  - MicroPython
  - Lua
- **Operating System**: FreeRTOS (built-in)

### Compatibility
- Works with Arduino libraries
- Compatible with ESP32 shields and breakout boards
- 3.3V logic (safe for I2S microphone and TFT display)

---

## Project Integration Notes

### Voltage Compatibility
✅ **All components are 3.3V compatible**
- ESP32: 3.3V logic native
- SPH0645LM4H Microphone: 1.6V-3.6V (safe at 3.3V)
- ST7796S Display: 3.3V-5V (works perfectly at 3.3V)

### Interface Mapping

#### I2S Microphone → ESP32
The ESP32 has dedicated I2S hardware support:
- ESP32 I2S0 or I2S1 peripheral can be used
- Recommended pins (configurable in software):
  - BCLK → GPIO26 (or any GPIO)
  - LRCL → GPIO25 (or any GPIO)
  - DOUT → GPIO22 (or any GPIO)
  - SEL → GND (Left channel) or 3.3V (Right channel)

#### SPI Display → ESP32
The ESP32 has HSPI and VSPI available:
- Recommended VSPI pins (default for most libraries):
  - MOSI → GPIO23 (VSPI MOSI)
  - MISO → GPIO19 (VSPI MISO - for SD card)
  - SCK → GPIO18 (VSPI CLK)
  - CS → GPIO5 (or any GPIO)
  - DC → GPIO2 (or any GPIO)
  - RST → GPIO4 (or any GPIO)
  - BL → GPIO15 (or any GPIO with PWM)

- Touch (XPT2046):
  - Can share SPI bus with display
  - Separate CS pin required
  - T_CS → GPIO21 (or any GPIO)

### Power Considerations
- ESP32 can source 3.3V from its onboard regulator
- TFT backlight may draw significant current (50-100mA)
- Consider external 3.3V regulator if powering all from battery
- Total current: ~200-300mA typical, ~500mA peak

### Pin Conflict Avoidance
- Do not use GPIO6-GPIO11 (flash connected)
- GPIO0, GPIO2, GPIO15 have boot mode requirements
- Careful with GPIO34-39 (input only, no pull-up)
- Keep strapping pins in mind during design

---

## Recommended Pin Assignment

```
ESP32 Pin Assignment for This Project:

I2S Microphone (SPH0645LM4H):
├─ BCLK  → GPIO26
├─ LRCL  → GPIO25
├─ DOUT  → GPIO22
├─ SEL   → GND (Left) or 3.3V (Right)
├─ GND   → GND
└─ 3V    → 3.3V

TFT Display (ST7796S - VSPI):
├─ MOSI  → GPIO23
├─ MISO  → GPIO19 (for SD card)
├─ SCK   → GPIO18
├─ CS    → GPIO5
├─ DC    → GPIO2
├─ RST   → GPIO4
├─ BL    → GPIO15 (PWM for brightness control)
└─ VCC   → 3.3V or 5V

Touch Screen (XPT2046):
├─ T_CLK → GPIO18 (shared with display SCK)
├─ T_CS  → GPIO21
├─ T_DIN → GPIO23 (shared with display MOSI)
├─ T_DO  → GPIO19 (shared with display MISO)
└─ T_IRQ → GPIO27 (optional interrupt)

Power:
├─ ESP32 powered via USB (5V → 3.3V regulator onboard)
├─ All peripherals powered from ESP32 3.3V pin
└─ Common GND for all components
```

---

## Software Libraries Required

### Arduino IDE Setup
1. Install ESP32 board support (Espressif Systems)
2. Install required libraries:
   - **TFT_eSPI** (for ST7796S display)
   - **XPT2046_Touchscreen** (for touch input)
   - **ESP32 I2S** (built-in, for microphone)
   - **driver/i2s.h** (ESP-IDF I2S driver)

### Configuration Notes
- TFT_eSPI requires User_Setup.h configuration for ST7796S
- I2S requires sample rate configuration (typical: 16kHz, 22kHz, or 44.1kHz)
- Consider using FreeRTOS tasks for parallel processing:
  - Task 1: I2S audio capture
  - Task 2: Display rendering
  - Task 3: Touch input handling

---

## Performance Expectations

### I2S Audio
- **Sample Rate**: Up to 44.1kHz (CD quality possible)
- **Bit Depth**: 16-bit or 32-bit (I2S protocol)
- **Latency**: <10ms with proper buffer configuration
- **CPU Load**: ~5-10% at 16kHz (one core)

### Display
- **Refresh Rate**: 20-60 FPS (depends on SPI clock speed)
- **SPI Speed**: Up to 40 MHz (ST7796S supports up to 80MHz)
- **Typical Speed**: 27 MHz for stability
- **Full Screen Update**: ~33ms at 27MHz

### Overall System
- Dual-core ESP32 allows:
  - Core 0: Handle Wi-Fi/Bluetooth + I2S audio
  - Core 1: Handle display rendering + main app logic
- Sufficient RAM for audio buffers + frame buffers
- Can achieve real-time audio visualization

---

## Project Capabilities

With this hardware combination, the project can:

1. **Real-time Audio Capture**
   - Capture high-quality digital audio via I2S
   - Process audio with FFT for frequency analysis
   - Detect sound levels, frequencies, patterns

2. **Visual Display**
   - Real-time waveform visualization
   - Spectrum analyzer display
   - VU meters and level indicators
   - Interactive touch controls

3. **Wireless Connectivity**
   - Stream audio over Wi-Fi
   - Remote control via Bluetooth
   - Web-based interface for configuration
   - MQTT for IoT integration

4. **Storage & Logging**
   - Save settings to SD card
   - Log audio events
   - Store audio samples (with compression)

---

## Safety & Best Practices

1. **Always use 3.3V for the microphone** (never 5V)
2. **Common ground for all components** is critical
3. **Keep I2S wires short** (<6 inches) to minimize noise
4. **Add decoupling capacitors** (0.1µF) near power pins
5. **Use level shifters if mixing 5V components**
6. **Test with USB power before battery operation**
7. **Monitor current draw** to prevent brownouts

---

## Reference Links

### Datasheets
- [SPH0645LM4H Datasheet](https://cdn-learn.adafruit.com/assets/assets/000/049/977/original/SPH0645LM4H-B.pdf)
- [ST7796S Display Driver Datasheet](https://www.displayfuture.com/Display/datasheet/controller/ST7796s.pdf)
- [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
- [ESP32 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)

### Guides
- [Adafruit I2S MEMS Microphone Guide](https://learn.adafruit.com/adafruit-i2s-mems-microphone-breakout)
- [ESP32 I2S Audio Tutorial](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2s.html)
- [TFT_eSPI Library Documentation](https://github.com/Bodmer/TFT_eSPI)

---

*Document created: 2025-11-24*
*Hardware components verified and documented for ESP32 audio visualization project*
