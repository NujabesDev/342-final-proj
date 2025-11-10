# SD Card Pin Configuration Reference

## Your ST7796S Display SD Card Slot

The SD card slot on your display **shares SPI pins** with the TFT, so most pins are already connected!

### Shared Pins (Already Connected):

| SD Function | ESP32 Pin | Shared With |
|-------------|-----------|-------------|
| MOSI (DI)   | GPIO 23   | Display SDI |
| MISO (DO)   | GPIO 19   | Display SDO |
| SCK (CLK)   | GPIO 18   | Display SCK |

### SD Card Chip Select (CS) - IMPORTANT!

**This is the ONLY pin you need to identify!**

The SD card CS pin is **separate** from the display CS pin. Common options:

| Try This First | Pin Number |
|----------------|------------|
| **Most Common** | GPIO 5 |
| Alternative 1   | GPIO 0 |
| Alternative 2   | GPIO 21 |
| Alternative 3   | GPIO 22 |

### How to Find Your SD CS Pin:

**Method 1: Look at the Display PCB**

- Look for labels near the SD card slot
- Common labels: `SD_CS`, `SD_SS`, `TF_CS`
- May have a pin number written (like "D5" or "IO5")

**Method 2: Trial and Error**

In the code, try each pin:

```cpp
#define SD_CS 5   // Try this first
```

If SD card fails to initialize, try:
```cpp
#define SD_CS 0   // Then this
```

Then:
```cpp
#define SD_CS 21  // Then this
```

**Method 3: Multimeter/Continuity Test**

If you have a multimeter:
1. Set to continuity mode
2. Touch one probe to SD card CS pin (usually pin 1 or 2)
3. Touch other probe to ESP32 pins to find which connects

---

## Complete SD Card Wiring Reference

If using an **external SD card reader** (not the built-in slot):

| SD Card Pin | ESP32 Pin | Notes |
|-------------|-----------|-------|
| VCC         | 3.3V      | Power (NOT 5V!) |
| GND         | GND       | Ground |
| MISO (DO)   | GPIO 19   | Shared with TFT |
| MOSI (DI)   | GPIO 23   | Shared with TFT |
| SCK (CLK)   | GPIO 18   | Shared with TFT |
| CS (SS)     | GPIO 5    | Separate from TFT_CS |

**Important:** Most SD cards run at 3.3V, not 5V!

---

## Verifying SD Card is Working

When you upload the code and open Serial Monitor, you should see:

### ✅ Success:
```
=== Animated GIF Player ===
Initializing SD card...
SD Card initialized successfully

Files on SD card:
  test.gif (45678 bytes)
```

### ❌ Wrong CS Pin:
```
=== Animated GIF Player ===
Initializing SD card...
SD Card initialization failed!
```
**Solution:** Try a different SD_CS pin number

### ❌ SD Card Not Formatted:
```
SD Card initialization failed!
```
**Solution:** Format card as FAT32

### ❌ Bad SD Card:
```
SD Card initialization failed!
```
**Solution:** Try a different SD card (some cheap cards don't work)

---

## Quick Setup Checklist

- [ ] SD card formatted as FAT32
- [ ] GIF file copied to root of SD card
- [ ] AnimatedGIF library installed
- [ ] TFT_eSPI configured (from previous setup)
- [ ] SD_CS pin number set correctly in code
- [ ] SD card inserted into slot
- [ ] Code uploaded to ESP32
- [ ] Serial Monitor shows "SD Card initialized successfully"

---

## Common SD Card Slot Pin Labels

Your display's SD card slot pins might be labeled differently. Here's a translation guide:

| Common Label | What It Means | ESP32 Connection |
|--------------|---------------|------------------|
| SD_CS, TF_CS, SS | Chip Select | GPIO 5 (usually) |
| SD_MOSI, DI, CMD | Data In | GPIO 23 |
| SD_MISO, DO, DAT0 | Data Out | GPIO 19 |
| SD_SCK, CLK | Clock | GPIO 18 |
| VCC, 3V3 | Power | 3.3V |
| GND | Ground | GND |

**TF = TransFlash (old name for microSD)**

---

## Testing Without GIF Files

Want to test if SD card is working before adding GIFs?

Replace `loop()` with:

```cpp
void loop() {
  Serial.println("\n=== SD Card Test ===");

  // List all files
  File root = SD.open("/");
  File file = root.openNextFile();

  while (file) {
    if (!file.isDirectory()) {
      Serial.printf("File: %s (%d bytes)\n", file.name(), file.size());
    }
    file = root.openNextFile();
  }

  delay(5000); // Wait 5 seconds before listing again
}
```

This will just list all files on the SD card every 5 seconds.
