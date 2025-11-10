# Embedded GIF Player - Simple Instructions

## What is Embedded?
The GIF data is converted to code and compiled directly into your program. No SD card, no SPIFFS needed!

**Perfect for:** Very small GIFs (<50KB)

---

## Step 1: Install Library

1. Arduino IDE → **Sketch** → **Include Library** → **Manage Libraries**
2. Search: **AnimatedGIF**
3. Install **AnimatedGIF by Larry Bank**

---

## Step 2: Prepare Your GIF

### IMPORTANT: Must be TINY!
- **Resolution:** 160x120 or smaller (tiny!)
- **File size:** Under 50KB
- **Frames:** 10-20 frames max
- **Colors:** 64 colors or less

### Where to get small GIFs:
- Search "pixel art gif" or "8bit gif"
- Make your own in Photoshop (8-10 frames)
- Use https://ezgif.com to resize/optimize aggressively

### Optimize it:
1. Go to https://ezgif.com/resize
2. Upload your GIF
3. Resize to **160x120** or smaller
4. Download
5. Go to https://ezgif.com/optimize
6. Upload resized GIF
7. Set compression to **100** (maximum)
8. Reduce colors to **64**
9. Download - should be under 50KB!

---

## Step 3: Convert GIF to Code

### Online Tool (EASIEST):

1. Go to: https://notisrac.github.io/FileToCArray/
2. Click **Browse** → Select your tiny GIF
3. **Array name:** `gifData`
4. **Output format:** Choose **Arduino (PROGMEM)**
5. Click **Convert**
6. Copy the generated code

You'll get something like:
```cpp
const uint8_t gifData[] PROGMEM = {
  0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0xA0, 0x00,
  0x78, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x00,
  // ... lots more bytes ...
};
```

---

## Step 4: Paste Into Code

1. Open `gif_embedded_player.ino`
2. Find this line:
   ```cpp
   const uint8_t gifData[] PROGMEM = {
     // GIF data bytes will be here after conversion
   };
   ```
3. **Replace it** with the code you copied from the converter
4. Save the file

---

## Step 5: Upload and Play!

1. Select your ESP32 board and port
2. Click **Upload**
3. Open Serial Monitor (115200 baud)
4. You should see:
   ```
   === Embedded GIF Player ===
   GIF data size: 45678 bytes
   Starting GIF playback...
   Playing embedded GIF...
   ```
5. **Your GIF plays on the display!**

---

## Alternative: Command Line Conversion

If you have Python installed:

```bash
# Install xxd (usually pre-installed on Mac/Linux)
xxd -i animation.gif > animation.h
```

Then copy the array from `animation.h` into your sketch.

---

## Troubleshooting

### "No GIF data embedded!"
- You forgot to paste the converted GIF data
- Make sure you replaced the placeholder array

### Code won't compile - "sketch too big"
- Your GIF is too large!
- Must be under 50KB
- Resize smaller or use SPIFFS instead

### GIF won't play / garbage on screen
- GIF might be corrupted
- Try a different GIF
- Make sure conversion copied completely

### Slow/stuttery playback
- GIF is still too big or complex
- Try 128x96 or even smaller
- Reduce to 32 colors
- Fewer frames

---

## Size Limits

| ESP32 Program Space | ~1.3MB |
|---------------------|--------|
| Your code | ~100KB |
| Libraries | ~200KB |
| **Available for GIF** | **~50-100KB max** |

If your GIF is bigger, use SPIFFS or SD card instead!

---

## Example Tiny GIFs

These work great as embedded GIFs:
- Loading spinners (16x16, 8 frames)
- Small icons animating (32x32, 10 frames)
- Simple pixel art (64x64, 15 frames)
- Logo animations (80x80, 12 frames)

---

## When to Use Embedded vs SPIFFS

| Method | Best For | Size Limit |
|--------|----------|------------|
| **Embedded** | Tiny loading animations, icons | <50KB |
| **SPIFFS** | Medium animations | <1MB |
| **SD Card** | Large/multiple GIFs | Unlimited |

---

## Converting on Mac/Linux

```bash
# Using xxd
xxd -i animation.gif | sed 's/unsigned char/const uint8_t/' | sed 's/\[\]/\[\] PROGMEM/' > gif_data.h
```

Then copy the array into your .ino file.

---

## That's It!

Embedded GIFs are perfect for small loading animations or icons that need to always be available without external storage!
