# ESP32 I2S Microphone WiFi Audio Streamer

Real-time audio streaming from ESP32 + SPH0645LM4H microphone to your computer over WiFi.

## Hardware Setup

### Wiring (SPH0645LM4H → ESP32)
```
Microphone Pin  →  ESP32 Pin
─────────────────────────────
BCLK            →  GPIO26
LRCL (WS)       →  GPIO25
DOUT            →  GPIO22
SEL             →  GND (for Left channel)
GND             →  GND
3V              →  3.3V
```

**Important:** Never connect the microphone to 5V! It's 3.3V only (1.6-3.6V range).

## Software Setup

### 1. ESP32 Setup (Arduino IDE)

#### Install ESP32 Board Support
1. Open Arduino IDE
2. Go to `File` → `Preferences`
3. Add to "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to `Tools` → `Board` → `Boards Manager`
5. Search for "ESP32" and install "ESP32 by Espressif Systems"

#### Configure the Code
Open `microphone_stream.ino` and change:

```cpp
const char* ssid = "YOUR_WIFI_SSID";        // Your WiFi name
const char* password = "YOUR_WIFI_PASSWORD"; // Your WiFi password
const char* udpAddress = "192.168.1.100";    // Your computer's IP (see below)
```

#### Upload to ESP32
1. Connect ESP32 via USB
2. Select `Tools` → `Board` → `ESP32 Dev Module`
3. Select the correct `Port`
4. Click Upload (→ button)

### 2. Computer Setup (Python Receiver)

#### Install Python Dependencies
```bash
pip install pyaudio numpy
```

**Note for Linux users:** You may need to install PortAudio first:
```bash
sudo apt-get install portaudio19-dev python3-pyaudio
```

**Note for macOS users:**
```bash
brew install portaudio
pip install pyaudio
```

#### Find Your Computer's IP Address

**Windows:**
```
ipconfig
```
Look for "IPv4 Address" (e.g., 192.168.1.100)

**Mac/Linux:**
```
ifconfig
```
or
```
ip addr show
```
Look for your local network IP (starts with 192.168 or 10.x)

**OR** - The Python script will show it when you run it!

#### Run the Receiver
```bash
cd tools
python audio_receiver.py
```

## Usage

1. **Start the Python receiver first:**
   ```bash
   python audio_receiver.py
   ```

2. **Power on/reset the ESP32**
   - It will connect to WiFi
   - Start streaming audio automatically

3. **You should hear audio from your microphone!**
   - The receiver will show audio levels and packet rate
   - Try speaking, clapping, playing music near the mic

## Troubleshooting

### No Audio / No Connection

1. **Check WiFi credentials** - ESP32 must connect to same network as computer
2. **Check IP address** - Make sure ESP32 code has your computer's correct IP
3. **Check firewall** - Allow UDP port 5555 through firewall
4. **Check Serial Monitor** - Look for error messages from ESP32

### Audio Quality Issues

- **Choppy/Glitchy:** WiFi signal too weak, move ESP32 closer to router
- **Loud static:** Check wiring, ensure good connections
- **No sound but packets received:** Check computer volume settings
- **Distorted:** Audio level might be too high, move away from sound source

### ESP32 Not Connecting to WiFi

- Double-check SSID and password (case-sensitive!)
- Make sure using 2.4GHz WiFi (ESP32 doesn't support 5GHz)
- Try moving ESP32 closer to router

## Monitoring ESP32

Open Serial Monitor (Tools → Serial Monitor, set to 115200 baud) to see:
- WiFi connection status
- ESP32 IP address
- Audio level indicator
- Streaming status

## Next Steps

Once you confirm the microphone is working:
- ✅ Microphone hardware verified
- ✅ I2S audio capture working
- ✅ WiFi streaming functional

Ready to move on to:
- FFT processing
- ESP32-to-ESP32 communication
- Display visualization

## Technical Details

- **Sample Rate:** 16kHz (good for voice/music analysis)
- **Bit Depth:** 16-bit (converted from 32-bit I2S)
- **Channels:** Mono
- **Protocol:** UDP (low latency, best for real-time)
- **Latency:** ~50-100ms depending on network

## Pin Changes (Optional)

If you need different pins, edit these lines in the code:
```cpp
#define I2S_BCK_PIN 26   // Bit Clock
#define I2S_WS_PIN 25    // Word Select
#define I2S_DATA_PIN 22  // Data Out
```

Avoid GPIO 6-11 (flash), GPIO 0/2/15 (boot modes), GPIO 34-39 (input only).
