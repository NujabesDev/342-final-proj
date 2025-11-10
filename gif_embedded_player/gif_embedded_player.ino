#include <TFT_eSPI.h>
#include <AnimatedGIF.h>

TFT_eSPI tft = TFT_eSPI();
AnimatedGIF gif;

// Your GIF data will go here (replace this with your converted GIF)
// This is just a placeholder - see instructions for how to convert your GIF
const uint8_t gifData[] PROGMEM = {
  // GIF data bytes will be here after conversion
  // Example: 0x47, 0x49, 0x46, 0x38, 0x39, 0x61, ...
};

const int gifDataSize = sizeof(gifData);

// Current position in the GIF data
int gifPosition = 0;

// GIF draw callback
void GIFDraw(GIFDRAW *pDraw) {
  uint8_t *s;
  uint16_t *d, *usPalette, usTemp[320];
  int x, y, iWidth;

  iWidth = pDraw->iWidth;
  if (iWidth > tft.width())
    iWidth = tft.width();

  usPalette = pDraw->pPalette;
  y = pDraw->iY + pDraw->y;
  s = pDraw->pPixels;

  if (pDraw->ucDisposalMethod == 2) {
    for (x = 0; x < iWidth; x++) {
      if (s[x] == pDraw->ucTransparent)
        s[x] = pDraw->ucBackground;
    }
    pDraw->ucHasTransparency = 0;
  }

  if (pDraw->ucHasTransparency) {
    uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
    int x, iCount;
    pEnd = s + iWidth;
    x = 0;
    iCount = 0;
    while (x < iWidth) {
      c = ucTransparent - 1;
      d = usTemp;
      while (c != ucTransparent && s < pEnd) {
        c = *s++;
        if (c == ucTransparent) {
          s--;
        } else {
          *d++ = usPalette[c];
          iCount++;
        }
      }
      if (iCount) {
        tft.pushImage(pDraw->iX + x, y, iCount, 1, (uint16_t*)usTemp);
        x += iCount;
        iCount = 0;
      }
      c = ucTransparent;
      while (c == ucTransparent && s < pEnd) {
        c = *s++;
        if (c == ucTransparent)
          iCount++;
        else
          s--;
      }
      if (iCount) {
        x += iCount;
        iCount = 0;
      }
    }
  } else {
    s = pDraw->pPixels;
    for (x = 0; x < iWidth; x++)
      usTemp[x] = usPalette[*s++];
    tft.pushImage(pDraw->iX, y, iWidth, 1, (uint16_t*)usTemp);
  }
}

// Memory read functions for embedded GIF
void * GIFOpenFile(const char *fname, int32_t *pSize) {
  *pSize = gifDataSize;
  gifPosition = 0;
  return (void *)1; // Return non-null to indicate success
}

void GIFCloseFile(void *pHandle) {
  // Nothing to close for embedded data
}

int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen) {
  int32_t bytesToRead = iLen;

  if (gifPosition + iLen > gifDataSize) {
    bytesToRead = gifDataSize - gifPosition;
  }

  if (bytesToRead <= 0) {
    return 0;
  }

  memcpy_P(pBuf, gifData + gifPosition, bytesToRead);
  gifPosition += bytesToRead;
  pFile->iPos = gifPosition;

  return bytesToRead;
}

int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition) {
  if (iPosition < 0) iPosition = 0;
  if (iPosition > gifDataSize) iPosition = gifDataSize;

  gifPosition = iPosition;
  pFile->iPos = gifPosition;

  return gifPosition;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== Embedded GIF Player ===");

  // Initialize display
  tft.init();
  tft.setRotation(1); // Landscape
  tft.fillScreen(TFT_BLACK);

  // Check if GIF data exists
  if (gifDataSize < 100) {
    Serial.println("ERROR: No GIF data embedded!");
    Serial.println("Please follow the conversion instructions to add your GIF.");
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 100);
    tft.println("No GIF data!");
    tft.setTextSize(1);
    tft.setCursor(10, 130);
    tft.println("See conversion instructions");
    while (1) delay(1000);
  }

  Serial.printf("GIF data size: %d bytes\n", gifDataSize);

  // Initialize GIF decoder
  gif.begin(LITTLE_ENDIAN_PIXELS);

  Serial.println("Starting GIF playback...");
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  // Play embedded GIF
  if (gif.open("memory", GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw)) {
    Serial.println("Playing embedded GIF...");

    while (gif.playFrame(true, NULL)) {
      // Playing frames
    }

    gif.close();
  } else {
    Serial.println("Failed to play GIF!");
    delay(5000);
  }

  delay(100);
}
