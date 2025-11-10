# ST7796S Display Setup Instructions

## Step 1: Install TFT_eSPI Library

1. Open Arduino IDE
2. Go to **Sketch → Include Library → Manage Libraries**
3. Search for **TFT_eSPI**
4. Install **TFT_eSPI by Bodmer**

## Step 2: Configure TFT_eSPI for ST7796S

After installing the library, you need to configure it:

### Option A: Replace User_Setup.h (Recommended)

1. Find the TFT_eSPI library folder:
   - **Mac:** `~/Documents/Arduino/libraries/TFT_eSPI/`
   - **Windows:** `Documents/Arduino/libraries/TFT_eSPI/`
   - **Linux:** `~/Arduino/libraries/TFT_eSPI/`

2. Backup the original file:
   ```bash
   # In the TFT_eSPI folder:
   mv User_Setup.h User_Setup_backup.h
   ```

3. Copy the `User_Setup.h` file from this folder to the TFT_eSPI library folder

### Option B: Manual Configuration

Open `Arduino/libraries/TFT_eSPI/User_Setup.h` and make these changes:

1. **Uncomment this line:**
   ```cpp
   #define ST7796_DRIVER
   ```

2. **Comment out any other driver (like ILI9341):**
   ```cpp
   // #define ILI9341_DRIVER
   ```

3. **Set the pins (around line 200):**
   ```cpp
   #define TFT_MISO 19
   #define TFT_MOSI 23
   #define TFT_SCLK 18
   #define TFT_CS   15
   #define TFT_DC   2
   #define TFT_RST  4
   ```

4. **Set touch pin (if using touch):**
   ```cpp
   #define TOUCH_CS 21
   ```

## Step 3: Wire the Display

Connect the display to your ESP32 according to the wiring guide in `WIRING_GUIDE.md`

## Step 4: Upload Test Sketch

1. Open `tft_display_test.ino` in Arduino IDE
2. Select your ESP32 board (**Tools → Board → ESP32 Dev Module**)
3. Select the correct port
4. Click **Upload**

## Step 5: What You Should See

If everything is working correctly:
- Red, green, blue color fills
- Text saying "ST7796S Display Test"
- Rectangles and circles
- Diagonal lines
- A rainbow bar animating at the bottom

## Troubleshooting

### White screen only
- Check wiring (especially CS, DC, RESET pins)
- Verify User_Setup.h has ST7796_DRIVER defined
- Try lowering SPI_FREQUENCY to 20000000

### Nothing displayed
- Check power connections (VCC and GND)
- Verify backlight is connected (LED pin)
- Try connecting RESET pin to 3.3V instead of GPIO

### Garbled display
- Wrong driver selected in User_Setup.h
- SPI frequency too high - try 27MHz or lower
- Check MOSI and SCLK connections

### Colors are wrong
- Some ST7796S variants need color inversion
- Add `tft.invertDisplay(true);` after `tft.init();`

## Next Steps

Once the test sketch works, you can:
1. Add touch screen functionality
2. Integrate with ESP-NOW messenger
3. Create a custom UI for your project
