#!/usr/bin/env python3
"""
ESP32 WiFi Audio Receiver
Receives UDP audio stream from ESP32 and plays it in real-time

Requirements:
    pip install pyaudio numpy

Usage:
    python audio_receiver.py
"""

import socket
import pyaudio
import numpy as np
import struct
import time
import sys

# Configuration (must match ESP32 settings)
UDP_IP = "0.0.0.0"  # Listen on all interfaces
UDP_PORT = 5555
SAMPLE_RATE = 16000
CHANNELS = 1
SAMPLE_WIDTH = 2  # 16-bit = 2 bytes

# Audio output settings
CHUNK = 256  # Must match SAMPLES_PER_PACKET on ESP32

class AudioReceiver:
    def __init__(self):
        self.sock = None
        self.audio = None
        self.stream = None
        self.running = False
        self.packets_received = 0
        self.last_stats_time = time.time()

    def start(self):
        """Initialize UDP socket and audio output"""
        print("=== ESP32 WiFi Audio Receiver ===")
        print(f"Listening on {UDP_IP}:{UDP_PORT}")
        print(f"Sample Rate: {SAMPLE_RATE}Hz, Channels: {CHANNELS}, Bit Depth: 16-bit")
        print("\nWaiting for audio stream from ESP32...")
        print("(Make sure your ESP32 is configured with this computer's IP address)\n")

        # Create UDP socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind((UDP_IP, UDP_PORT))
        self.sock.settimeout(1.0)  # 1 second timeout for checking

        # Initialize PyAudio
        self.audio = pyaudio.PyAudio()
        self.stream = self.audio.open(
            format=pyaudio.paInt16,
            channels=CHANNELS,
            rate=SAMPLE_RATE,
            output=True,
            frames_per_buffer=CHUNK
        )

        print("Audio output initialized!")
        print("Press Ctrl+C to stop\n")

        self.running = True
        self.receive_loop()

    def receive_loop(self):
        """Main loop to receive and play audio"""
        try:
            while self.running:
                try:
                    # Receive UDP packet
                    data, addr = self.sock.recvfrom(4096)

                    if len(data) > 0:
                        self.packets_received += 1

                        # Play audio
                        self.stream.write(data)

                        # Print statistics every 2 seconds
                        current_time = time.time()
                        if current_time - self.last_stats_time > 2.0:
                            elapsed = current_time - self.last_stats_time
                            pkt_rate = self.packets_received / elapsed

                            # Calculate audio level
                            samples = np.frombuffer(data, dtype=np.int16)
                            level = np.abs(samples).mean()
                            level_pct = min(100, int((level / 5000) * 100))

                            print(f"Packets/s: {pkt_rate:.1f} | Level: {level_pct:3d}% | ", end="")
                            bars = int(level_pct / 5)
                            print("█" * bars)

                            self.packets_received = 0
                            self.last_stats_time = current_time

                except socket.timeout:
                    # No data received, check if we should continue
                    continue

        except KeyboardInterrupt:
            print("\n\nStopping audio receiver...")
        finally:
            self.cleanup()

    def cleanup(self):
        """Clean up resources"""
        self.running = False

        if self.stream:
            self.stream.stop_stream()
            self.stream.close()

        if self.audio:
            self.audio.terminate()

        if self.sock:
            self.sock.close()

        print("Cleanup complete. Goodbye!")

def get_computer_ip():
    """Get this computer's local IP address"""
    try:
        # Create a dummy socket to find local IP
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        ip = s.getsockname()[0]
        s.close()
        return ip
    except Exception:
        return "Unable to determine"

def main():
    print("\n" + "="*50)
    print("Your computer's IP address:", get_computer_ip())
    print("Configure this IP in the ESP32 code!")
    print("="*50 + "\n")

    receiver = AudioReceiver()
    receiver.start()

if __name__ == "__main__":
    main()
