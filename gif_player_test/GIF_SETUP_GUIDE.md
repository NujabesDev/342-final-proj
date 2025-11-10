# Animated GIF Player Setup Guide

## What You Need:

1. **MicroSD Card** (2GB-32GB, FAT32 formatted)
2. **SD Card Reader** (to connect to your computer)
3. **GIF files** (prepared according to specs below)
4. **ST7796S Display** (already working!)
5. **ESP32** (already set up!)

---

## Step 1: Install AnimatedGIF Library

1. Open **Arduino IDE**
2. Go to **Sketch** → **Include Library** → **Manage Libraries**
3. Search for: **AnimatedGIF**
4. Find **AnimatedGIF by Larry Bank**
5. Click **Install**

---

## Step 2: Prepare Your GIF Files

### GIF Requirements:

| Property | Recommended | Maximum |
|----------|-------------|---------|
| **Resolution** | 320x240 or smaller | 480x320 (display size) |
| **File Size** | Under 1MB | Under 5MB |
| **Frame Rate** | 10-15 fps | 20 fps |
| **Colors** | 256 colors or less | 256 colors |
| **Duration** | 1-5 seconds loop | Any |

### Why These Limits?

- **ESP32 has limited RAM** (512KB total)
- Larger/faster GIFs may stutter or fail to load
- Smaller files = smoother playback

---

## Step 3: Convert/Resize GIFs

### Option A: Online Tool (Easiest)

Use **ezgif.com** (free, no account needed):

1. Go to https://ezgif.com/resize
2. Upload your GIF
3. Set width to **320** (or 240 for vertical)
4. Click **Resize**
5. Download the resized GIF

**Optional - Optimize:**
1. Go to https://ezgif.com/optimize
2. Upload your resized GIF
3. Set compression level to **35-50**
4. Click **Optimize**
5. Download

### Option B: Using FFmpeg (Advanced)

If you have FFmpeg installed:

```bash
# Resize to 320x240
ffmpeg -i input.gif -vf "scale=320:240" -r 15 output.gif

# Reduce colors to 128 (smoother playback)
ffmpeg -i input.gif -vf "scale=320:240,fps=15,split[s0][s1];[s0]palettegen=max_colors=128[p];[s1][p]paletteuse" output.gif
```

### Option C: Photoshop/GIMP

1. Open GIF in Photoshop/GIMP
2. **Image** → **Image Size** → Set to 320x240
3. **Export As** → **GIF**
4. Set colors to 128-256
5. Set frame delay for ~10-15 fps

---

## Step 4: Prepare SD Card

### Format SD Card:

**Windows:**
1. Insert SD card into computer
2. Right-click SD card in File Explorer
3. Select **Format**
4. File System: **FAT32**
5. Click **Start**

**Mac:**
1. Open **Disk Utility**
2. Select SD card
3. Click **Erase**
4. Format: **MS-DOS (FAT)**
5. Click **Erase**

**Linux:**
```bash
# Find SD card device (e.g., /dev/sdb1)
lsblk

# Format to FAT32 (CAREFUL - double check device name!)
sudo mkfs.vfat -F 32 /dev/sdb1
```

### Copy GIF Files:

1. Copy your GIF files to the **root directory** of the SD card
2. Rename one file to **test.gif** (or update the code with your filename)
3. Safely eject the SD card

---

## Step 5: Wire SD Card to ESP32

Your ST7796S display likely has an **SD card slot built-in**. Check the pins:

| SD Pin | ESP32 Pin | Notes |
|--------|-----------|-------|
| SD_CS  | GPIO 5    | Chip Select (may vary) |
| SD_MOSI| GPIO 23   | Shared with display |
| SD_MISO| GPIO 19   | Shared with display |
| SD_SCK | GPIO 18   | Shared with display |

**Note:** The SD card shares SPI pins with the display. The `SD_CS` pin might be different - common values are GPIO 5, GPIO 0, or GPIO 21. Check your display's documentation or try each one!

---

## Step 6: Upload the Code

1. Open **gif_player_test.ino** in Arduino IDE
2. **If needed:** Change the `SD_CS` pin number (line 7) to match your display
3. Make sure TFT_eSPI library is configured (from previous setup)
4. Click **Upload**

---

## Step 7: Insert SD Card and Test

1. Insert the microSD card into the display's SD slot
2. Reset the ESP32 (press the reset button or re-plug USB)
3. Open **Serial Monitor** (115200 baud)
4. You should see:
   ```
   === Animated GIF Player ===
   Initializing SD card...
   SD Card initialized successfully

   Files on SD card:
     test.gif (45678 bytes)

   Starting GIF playback...
   Successfully opened GIF: /test.gif
   Image size: 320 x 240
   ```
5. **The GIF should start playing on the display!**

---

## Troubleshooting

### Problem: "SD Card initialization failed!"

**Solutions:**
- Check SD card is inserted properly
- Try a different SD card (some cheap cards don't work well)
- Verify SD_CS pin number (try 5, 0, or 21)
- Make sure card is FAT32 formatted
- Check wiring if using external SD card reader

### Problem: "No GIF files found"

**Solutions:**
- Make sure files have .gif extension (lowercase)
- Files must be in root directory (not in folders)
- SD card must be FAT32 formatted

### Problem: GIF plays but is slow/stuttery

**Solutions:**
- Reduce GIF resolution (try 240x180 or 160x120)
- Reduce frame rate to 10 fps or lower
- Reduce color count to 128 or fewer
- Optimize/compress the GIF file

### Problem: Display shows garbage/crashes

**Solutions:**
- GIF file is too large - try smaller file
- Reduce colors in GIF
- Make sure GIF is properly formatted

### Problem: GIF doesn't loop

**Update the code:**
```cpp
void loop() {
  playGif("/test.gif");
  delay(100); // Already in the code!
}
```

---

## Finding/Creating GIFs

### Where to Find Small GIFs:

- **Giphy.com** - Search, then download and resize
- **Tenor.com** - Similar to Giphy
- **Pixel art GIFs** - Usually already small!
- **Make your own** - Convert from video or create in Photoshop

### Creating from Video:

```bash
# Using FFmpeg - extract 5 seconds from video
ffmpeg -i video.mp4 -ss 00:00:00 -t 5 -vf "fps=15,scale=320:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" output.gif
```

---

## Playing Multiple GIFs

Edit the `loop()` function:

```cpp
void loop() {
  playGif("/animation1.gif");
  delay(1000);

  playGif("/animation2.gif");
  delay(1000);

  playGif("/animation3.gif");
  delay(1000);
}
```

---

## Advanced: Center Small GIFs

If your GIF is smaller than the display, center it:

```cpp
void playGif(const char *gifPath) {
  if (gif.open(gifPath, GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw)) {
    int x = (480 - gif.getCanvasWidth()) / 2;
    int y = (320 - gif.getCanvasHeight()) / 2;

    // Clear screen with black
    tft.fillScreen(TFT_BLACK);

    // Play centered (modify GIFDraw function to add offset)
    while (gif.playFrame(true, NULL)) {
      // Playing...
    }
    gif.close();
  }
}
```

---

## Next Steps

Once GIFs are working:
1. ✅ Display animated graphics
2. 🔜 Combine with ESP-NOW messenger (show GIF reactions)
3. 🔜 Add button to switch between GIFs
4. 🔜 Create custom animations for your project

**Ready to try it?** Follow these steps and let me know how it goes!
