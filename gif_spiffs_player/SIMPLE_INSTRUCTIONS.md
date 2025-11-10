# SPIFFS GIF Player - Simple Instructions

## What is SPIFFS?
SPIFFS = SPI Flash File System. It's like a tiny hard drive built into your ESP32 (about 1-2MB of space).

---

## Step 1: Install Library

1. Arduino IDE → **Sketch** → **Include Library** → **Manage Libraries**
2. Search: **AnimatedGIF**
3. Install **AnimatedGIF by Larry Bank**

---

## Step 2: Install SPIFFS Upload Tool

### For Arduino IDE 1.x:
1. Download: https://github.com/me-no-dev/arduino-esp32fs-plugin/releases
2. Download **ESP32FS-1.1.zip**
3. Unzip to: `~/Documents/Arduino/tools/` (create `tools` folder if needed)
4. Restart Arduino IDE
5. You should now see: **Tools** → **ESP32 Sketch Data Upload**

### For Arduino IDE 2.x:
1. Download: https://github.com/earlephilhower/arduino-esp32fs-plugin/releases
2. Download **esp32fs.vsix**
3. In Arduino IDE 2.x: Click the puzzle icon (Extensions)
4. Click **Install from VSIX**
5. Select the downloaded file
6. You should now see: **Tools** → **ESP32 Sketch Data Upload**

---

## Step 3: Prepare Your GIF

### Make it small:
- **Resolution:** 320x240 or smaller
- **File size:** Under 500KB (preferably under 200KB)
- Use https://ezgif.com/resize to resize
- Use https://ezgif.com/optimize to compress

### Example GIFs to try:
- Simple animations work best
- Pixel art GIFs are perfect
- Keep it under 10 seconds

---

## Step 4: Add GIF to Project

1. **Rename** your GIF to: `animation.gif`
2. Put it in the **data** folder inside `gif_spiffs_player/`
   ```
   gif_spiffs_player/
   ├── gif_spiffs_player.ino
   └── data/
       └── animation.gif  ← Your GIF goes here
   ```
3. That's it!

---

## Step 5: Upload

### Upload the GIF to ESP32:
1. Open `gif_spiffs_player.ino` in Arduino IDE
2. Select your ESP32 board and port
3. **Tools** → **ESP32 Sketch Data Upload**
4. Wait 30-60 seconds (it will erase and upload SPIFFS)
5. You'll see: "SPIFFS Image Uploaded"

### Upload the code:
1. Click the normal **Upload** button
2. Wait for upload to complete

---

## Step 6: Watch it Play!

1. Open Serial Monitor (115200 baud)
2. You should see:
   ```
   === SPIFFS GIF Player ===
   Mounting SPIFFS...
   SPIFFS mounted successfully

   Files in SPIFFS:
     /animation.gif (123456 bytes)

   Starting GIF playback...
   Playing animation.gif
   ```
3. **Your GIF should be playing on the display!**

---

## Troubleshooting

### "No GIF files found"
- Make sure file is named exactly `animation.gif`
- Must be in the `data/` folder
- Run **ESP32 Sketch Data Upload** again

### "SPIFFS Mount Failed"
- In Arduino IDE: **Tools** → **Partition Scheme** → **Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)**
- Upload code again

### GIF plays slowly
- Your GIF is too big
- Resize to 240x180 or smaller
- Reduce colors to 128

### Arduino IDE doesn't have "ESP32 Sketch Data Upload"
- You need to install the SPIFFS plugin (see Step 2)
- Make sure you restart Arduino IDE after installing

---

## Adding Multiple GIFs

Want to play different GIFs? Update the code:

```cpp
void loop() {
  playGif("/animation1.gif");
  delay(1000);

  playGif("/animation2.gif");
  delay(1000);
}

void playGif(const char *path) {
  if (gif.open(path, GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw)) {
    while (gif.playFrame(true, NULL)) { }
    gif.close();
  }
}
```

Then add multiple GIFs to the `data/` folder!

---

## How Much Space Do You Have?

Add this to `setup()` to check:

```cpp
Serial.printf("SPIFFS Total: %d bytes\n", SPIFFS.totalBytes());
Serial.printf("SPIFFS Used: %d bytes\n", SPIFFS.usedBytes());
Serial.printf("SPIFFS Free: %d bytes\n", SPIFFS.totalBytes() - SPIFFS.usedBytes());
```

**Typical:** ~1.5MB total space available

---

## That's It!

SPIFFS is perfect for storing 1-3 small GIF animations without needing an SD card!
