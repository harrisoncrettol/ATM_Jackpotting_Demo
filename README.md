# ATM-Jackpotting-Demo

A physical ATM jackpotting demo for a university Cyber Physical Systems class. Uses an ESP32 and servo motor to simulate malware overriding hardware to dispense cash via a fake web terminal.

### Hardware & Wiring
* **ESP-32S NodeMCU** (Microcontroller)
* **SG90 Micro Servo** (Cash dispenser)
* **Cardboard Box** (ATM chassis)

* Servo Red ➔ ESP32 **VIN** (5V)
* Servo Brown/Black ➔ ESP32 **GND**
* Servo Orange/Yellow ➔ ESP32 **GPIO 13**

### Software
Written in C++ using the Arduino IDE (requires the `ESP32Servo` library). The ESP32 hosts a local Wi-Fi Access Point and serves the HTML/JS terminal interface.

### How to Run
1. Power the ESP32 (the servo automatically zeros out to hold the trapdoor).
2. Connect your smartphone to the `Ploutus_Terminal` Wi-Fi network.
3. Navigate to `192.168.4.1` in your mobile browser.
4. Tap **[ INITIATE JACKPOT ]** to run the exploit sequence, trigger the servo, and dispense the cash.

### Disclaimer
*Educational model only. This project demonstrates cyber-physical hardware vulnerabilities using a cardboard box, not a real ATM.*
