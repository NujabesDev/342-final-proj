#include <TFT_eSPI.h>
#include <AnimatedGIF.h>
#include <SPIFFS.h>

TFT_eSPI tft = TFT_eSPI();
AnimatedGIF gif;
File gifFile;

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

void * GIFOpenFile(const char *fname, int32_t *pSize) {
  gifFile = SPIFFS.open(fname);
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

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== SPIFFS GIF Player ===");

  // Initialize display
  tft.init();
  tft.setRotation(1); // Landscape
  tft.fillScreen(TFT_BLACK);

  // Initialize SPIFFS
  Serial.println("Mounting SPIFFS...");
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed!");
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 100);
    tft.println("SPIFFS Error!");
    while (1) delay(1000);
  }

  Serial.println("SPIFFS mounted successfully");

  // List files in SPIFFS
  Serial.println("\nFiles in SPIFFS:");
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  bool foundGif = false;

  while (file) {
    Serial.printf("  %s (%d bytes)\n", file.name(), file.size());
    String filename = String(file.name());
    if (filename.endsWith(".gif") || filename.endsWith(".GIF")) {
      foundGif = true;
    }
    file = root.openNextFile();
  }

  if (!foundGif) {
    Serial.println("\nNo GIF files found!");
    Serial.println("Upload a GIF using 'ESP32 Sketch Data Upload'");
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 80);
    tft.println("No GIF found");
    tft.setTextSize(1);
    tft.setCursor(10, 110);
    tft.println("Upload GIF to SPIFFS:");
    tft.setCursor(10, 130);
    tft.println("Tools > ESP32 Sketch Data Upload");
    while (1) delay(1000);
  }

  // Initialize GIF decoder
  gif.begin(LITTLE_ENDIAN_PIXELS);

  Serial.println("\nStarting GIF playback...");
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  // Play GIF from SPIFFS
  // The file should be in the 'data' folder as 'animation.gif'

  if (gif.open("/animation.gif", GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw)) {
    Serial.println("Playing animation.gif");

    while (gif.playFrame(true, NULL)) {
      // Playing frames
    }

    gif.close();
  } else {
    Serial.println("Failed to open /animation.gif");
    delay(5000);
  }

  delay(100);
}
