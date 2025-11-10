# EXACT Step-by-Step Setup Instructions for ST7796S Display

## What You Need:
- ESP32-WROOM-32 board
- ST7796S 4.0" TFT display (Hosyond brand)
- USB cable to connect ESP32 to computer
- Jumper wires (female-to-female recommended)

---

## Step 1: Install TFT_eSPI Library

1. Open **Arduino IDE**
2. Click **Sketch** → **Include Library** → **Manage Libraries...**
3. In the search box, type: `TFT_eSPI`
4. Find **TFT_eSPI by Bodmer**
5. Click **Install**
6. Wait for installation to complete

---

## Step 2: Find and Edit User_Setup.h

### On Mac:
```bash
# Open Finder, press Cmd+Shift+G and paste this path:
~/Documents/Arduino/libraries/TFT_eSPI/
```

### On Windows:
```
Navigate to: Documents\Arduino\libraries\TFT_eSPI\
```

### On Linux:
```bash
cd ~/Arduino/libraries/TFT_eSPI/
```

You should see a file called **User_Setup.h**

---

## Step 3: Backup Original File

**Before editing, backup the original:**

### Mac/Linux Terminal:
```bash
cd ~/Documents/Arduino/libraries/TFT_eSPI/
mv User_Setup.h User_Setup_ORIGINAL_BACKUP.h
```

### Windows (in Command Prompt):
```
cd Documents\Arduino\libraries\TFT_eSPI\
ren User_Setup.h User_Setup_ORIGINAL_BACKUP.h
```

### Or manually:
- Right-click `User_Setup.h` → Rename to `User_Setup_ORIGINAL_BACKUP.h`

---

## Step 4: Copy New Configuration File

### Option A: Manual Copy (Easiest)

1. In your project folder, go to: `342-final-proj/tft_display_test/`
2. Find the file: `User_Setup.h`
3. **Copy** this file
4. **Paste** it into: `~/Documents/Arduino/libraries/TFT_eSPI/`

### Option B: Command Line

#### Mac/Linux:
```bash
# Copy the new config file
cp ~/path/to/342-final-proj/tft_display_test/User_Setup.h ~/Documents/Arduino/libraries/TFT_eSPI/User_Setup.h
```

#### Windows:
```
copy "path\to\342-final-proj\tft_display_test\User_Setup.h" "Documents\Arduino\libraries\TFT_eSPI\User_Setup.h"
```

---

## Step 5: Wire the Display to ESP32

**Get your jumper wires ready.** Connect as follows:

| Display Pin | → | ESP32 Pin | Wire Color (your choice) |
|-------------|---|-----------|--------------------------|
| VCC         | → | 5V        | Red (power)              |
| GND         | → | GND       | Black (ground)           |
| CS          | → | GPIO 15   | Any color                |
| RESET       | → | GPIO 4    | Any color                |
| DC/RS       | → | GPIO 2    | Any color                |
| SDI (MOSI)  | → | GPIO 23   | Any color                |
| SCK         | → | GPIO 18   | Any color                |
| LED         | → | 3.3V      | Any color (backlight)    |
| SDO (MISO)  | → | GPIO 19   | Any color (optional)     |

**Touch pins (if you want touch later):**

| Touch Pin   | → | ESP32 Pin |
|-------------|---|-----------|
| T_CS        | → | GPIO 21   |
| T_CLK       | → | GPIO 18   | (shared with SCK)
| T_DIN       | → | GPIO 23   | (shared with MOSI)
| T_DO        | → | GPIO 19   | (shared with MISO)
| T_IRQ       | → | Not connected for now |

---

## Step 6: Set Up Arduino IDE for ESP32

1. **Select Board:**
   - **Tools** → **Board** → **ESP32 Arduino** → **ESP32 Dev Module**

2. **Configure Settings:**
   - **Tools** → **Upload Speed** → **115200** (or 921600 if it works)
   - **Tools** → **Flash Frequency** → **80MHz**
   - **Tools** → **Partition Scheme** → **Default 4MB with spiffs**

3. **Select Port:**
   - **Tools** → **Port** → Choose your ESP32's port
   - On Mac: `/dev/cu.usbserial-XXXX` or `/dev/cu.SLAB_USBtoUART`
   - On Windows: `COM3`, `COM4`, etc.

---

## Step 7: Open and Upload Test Sketch

1. In Arduino IDE: **File** → **Open**
2. Navigate to: `342-final-proj/tft_display_test/tft_display_test.ino`
3. Click **Upload** (arrow button)
4. Wait for upload to complete

---

## Step 8: What You Should See

If everything works correctly, the display should show:

1. **Red screen** (½ second)
2. **Green screen** (½ second)
3. **Blue screen** (½ second)
4. **Black screen with:**
   - White text: "ST7796S Display Test"
   - Yellow text: "Display is working!"
   - Colored rectangles and circles
   - Diagonal white lines
   - **Rainbow bar animating** at the bottom

---

## Troubleshooting

### Problem: White Screen Only

**Solution:**
- Check all wiring connections (especially CS, DC, RESET)
- Make sure VCC is connected to 5V (not 3.3V)
- Verify LED (backlight) is connected to 3.3V

### Problem: Display Shows Nothing / Stays Black

**Solution:**
- Check power: VCC to 5V, GND to GND
- Check LED backlight is connected
- Try connecting RESET to 3.3V instead of GPIO 4

### Problem: Garbled Display / Wrong Colors

**Solution 1:** Add this line after `tft.init();` in the sketch:
```cpp
tft.invertDisplay(true);
```

**Solution 2:** Lower SPI frequency in User_Setup.h:
```cpp
#define SPI_FREQUENCY  20000000  // Try 20MHz instead of 27MHz
```

### Problem: Compilation Error - "ST7796_DRIVER not defined"

**Solution:**
- Make sure you copied the new User_Setup.h correctly
- Restart Arduino IDE
- Check that User_Setup.h is in the correct folder

### Problem: Upload Failed

**Solution:**
- Hold down **BOOT** button on ESP32 during upload
- Select correct port in Tools → Port
- Try lower upload speed (115200)

---

## Success Checklist

- [ ] TFT_eSPI library installed
- [ ] User_Setup.h backed up
- [ ] New User_Setup.h copied to library folder
- [ ] Display wired to ESP32 (9 connections minimum)
- [ ] ESP32 board selected in Arduino IDE
- [ ] Correct port selected
- [ ] Test sketch uploaded successfully
- [ ] Display shows colored screens and graphics

---

## Next Steps

Once the test works:
1. ✅ Display is confirmed working
2. 🔜 Integrate ESP-NOW messenger to show messages on screen
3. 🔜 Add touch screen functionality
4. 🔜 Create custom UI with chat bubbles

**Ready to test?** Follow these steps and let me know if you hit any issues!
