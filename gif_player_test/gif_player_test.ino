#include <TFT_eSPI.h>
#include <AnimatedGIF.h>
#include <SD.h>
#include <SPI.h>

// SD Card CS pin (check your display - usually on the SD card slot)
#define SD_CS 5  // Change this if your SD card uses a different CS pin

TFT_eSPI tft = TFT_eSPI();
AnimatedGIF gif;

// File to play
File gifFile;

// This function will be called during GIF decoding to draw each line
void GIFDraw(GIFDRAW *pDraw) {
  uint8_t *s;
  uint16_t *d, *usPalette, usTemp[320];
  int x, y, iWidth;

  iWidth = pDraw->iWidth;
  if (iWidth > tft.width())
    iWidth = tft.width();

  usPalette = pDraw->pPalette;
  y = pDraw->iY + pDraw->y; // current line

  s = pDraw->pPixels;
  if (pDraw->ucDisposalMethod == 2) { // restore to background color
    for (x = 0; x < iWidth; x++) {
      if (s[x] == pDraw->ucTransparent)
        s[x] = pDraw->ucBackground;
    }
    pDraw->ucHasTransparency = 0;
  }

  // Apply the new pixels to the main image
  if (pDraw->ucHasTransparency) { // if transparency used
    uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
    int x, iCount;
    pEnd = s + iWidth;
    x = 0;
    iCount = 0; // count non-transparent pixels
    while (x < iWidth) {
      c = ucTransparent - 1;
      d = usTemp;
      while (c != ucTransparent && s < pEnd) {
        c = *s++;
        if (c == ucTransparent) { // done, stop
          s--;
        } else { // opaque
          *d++ = usPalette[c];
          iCount++;
        }
      } // while looking for opaque pixels
      if (iCount) { // any opaque pixels?
        tft.pushImage(pDraw->iX + x, y, iCount, 1, (uint16_t*)usTemp);
        x += iCount;
        iCount = 0;
      }
      // no, look for a run of transparent pixels
      c = ucTransparent;
      while (c == ucTransparent && s < pEnd) {
        c = *s++;
        if (c == ucTransparent)
          iCount++;
        else
          s--;
      }
      if (iCount) {
        x += iCount; // skip these
        iCount = 0;
      }
    }
  } else {
    s = pDraw->pPixels;
    // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
    for (x = 0; x < iWidth; x++)
      usTemp[x] = usPalette[*s++];
    tft.pushImage(pDraw->iX, y, iWidth, 1, (uint16_t*)usTemp);
  }
} // GIFDraw()

void * GIFOpenFile(const char *fname, int32_t *pSize) {
  gifFile = SD.open(fname);
  if (gifFile) {
    *pSize = gifFile.size();
    return (void *)&gifFile;
  }
  return NULL;
}

void GIFCloseFile(void *pHandle) {
  File *f = static_cast<File *>(pHandle);
  if (f != NULL)
    f->close();
}

int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen) {
  int32_t iBytesRead;
  iBytesRead = iLen;
  File *f = static_cast<File *>(pFile->fHandle);
  if ((pFile->iSize - pFile->iPos) < iLen)
    iBytesRead = pFile->iSize - pFile->iPos - 1;
  if (iBytesRead <= 0)
    return 0;
  iBytesRead = (int32_t)f->read(pBuf, iBytesRead);
  pFile->iPos = f->position();
  return iBytesRead;
}

int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition) {
  File *f = static_cast<File *>(pFile->fHandle);
  f->seek(iPosition);
  pFile->iPos = (int32_t)f->position();
  return pFile->iPos;
}

void playGif(const char *gifPath) {
  if (gif.open(gifPath, GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw)) {
    Serial.printf("Successfully opened GIF: %s\n", gifPath);
    Serial.printf("Image size: %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
    Serial.printf("Frame count: %d\n", gif.getFrameCount());

    // Play the GIF in a loop
    while (gif.playFrame(true, NULL)) {
      // Continues until manually stopped
    }
    gif.close();
  } else {
    Serial.printf("Failed to open GIF: %s\n", gifPath);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== Animated GIF Player ===");

  // Initialize display
  tft.init();
  tft.setRotation(1); // Landscape
  tft.fillScreen(TFT_BLACK);

  // Initialize SD card
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("SD Card initialization failed!");
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 100);
    tft.println("SD Card Error!");
    tft.setCursor(10, 130);
    tft.println("Check SD card");
    while (1) delay(1000);
  }

  Serial.println("SD Card initialized successfully");

  // List files on SD card
  Serial.println("\nFiles on SD card:");
  File root = SD.open("/");
  File file = root.openNextFile();
  bool foundGif = false;
  while (file) {
    if (!file.isDirectory()) {
      String filename = file.name();
      Serial.printf("  %s (%d bytes)\n", filename.c_str(), file.size());
      if (filename.endsWith(".gif") || filename.endsWith(".GIF")) {
        foundGif = true;
      }
    }
    file = root.openNextFile();
  }

  if (!foundGif) {
    Serial.println("\nNo GIF files found on SD card!");
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 100);
    tft.println("No GIF files found");
    tft.setCursor(10, 130);
    tft.setTextSize(1);
    tft.println("Add .gif files to SD card");
    while (1) delay(1000);
  }

  // Initialize GIF decoder
  gif.begin(LITTLE_ENDIAN_PIXELS);

  Serial.println("\nStarting GIF playback...");
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  // Play GIF file from SD card
  // Change "test.gif" to your actual GIF filename
  playGif("/test.gif");

  // If you have multiple GIFs, you can play them in sequence:
  // playGif("/animation1.gif");
  // playGif("/animation2.gif");

  delay(100); // Small delay before looping
}
