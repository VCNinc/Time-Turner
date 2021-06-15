# Time-Turner
Simple Device for Defeating RF Attendance Systems (To Be in Two Places at Once)

![Time-Turner Device](time-turner.jpg)

## Disclaimers
The “Time-Turner” project is provided for entertainment and experimental research purposes only. Research applicable hacking laws and your institution’s own policies before using a “Time-Turner” device. Do not use a “Time-Turner” device with any “base station” you don’t own or have permission to use with a “Time-Turner” device. The contributors of this project are not responsible for improper or nefarious use. Further disclaimers are provided in the [MIT License](https://github.com/VCNinc/Time-Turner/blob/main/LICENSE).

## Introduction
RFID-based attendance and polling devices have become an increasingly popular way to monitor student attendance and participation. “Time-Turner” is an Arduino-based device that can emulate a popular RFID attendance device (thus allowing you to appear to be in two places at once). It is capable of intercepting other poll responses and choosing the most popular answer. This repository contains the “Time-Turner” firmware and instructions for configuring the necessary hardware to build your own “Time-Turner” device.

Unlike other options, the “Time-Turner” device is self-contained, battery-powered, and does not require a physical connection to any other device. It can be built for about $39 using easily-accessible off-the-shelf components - less than the cost of a legitimate RFID attendance system. The assembled device weighs about 20g and lasts for over 20 hours on a single charge. It should take an hour or less to fully assemble and configure.

“Time-Turner” is developed by Vivek Nair and is based on the previous work of Aaron Wisner, Jacob Glueck, and Charles Cao in reverse-engineering the proprietary protocol used by the targeted RFID attendance device.

## Suggested Hardware
* Adafruit Feather M0 RFM69HCW Packet Radio - 868 or 915 MHz - RadioFruit: https://www.adafruit.com/product/3176
* Adafruit FeatherWing OLED - 128x32 OLED Add-on For Feather: https://www.adafruit.com/product/2900
* Soldering Kit (Soldering Iron & Solder), eg. https://www.amazon.com/gp/product/B07GTGGLXN
* USB 2.0 A-Male to Micro B Cable, eg. https://www.amazon.com/dp/B0711PVX6Z
* (Optional) Lithium Ion Polymer Battery Ideal For Feathers - 3.7V 400mAh: https://www.adafruit.com/product/3898
* (Optional) Electrical Tape, eg. https://www.amazon.com/gp/B07ZWC2VL
* (Optional) Mounting Putty, eg. https://www.amazon.com/dp/B01ETX43FG

## Hardware Setup
1. Solder the FeatherWing OLED board to the Feather M0: https://learn.adafruit.com/adafruit-oled-featherwing/assembly
1. (Optional) Connect the lithium-ion polymer battery to the Feather M0
1. (Optional) Use electrical tape to secure components in place
1. (Optional) Use mounting putty to temporarily attach device to surfaces

## Software Setup
1. Install the Arduino IDE: https://www.arduino.cc/en/software
1. Add the Adafruit package manager to the Arduino IDE: https://learn.adafruit.com/adafruit-feather-m0-basic-proto/setup
1. Add the Adafruit M0 libraries to the Arduino IDE: https://learn.adafruit.com/adafruit-feather-m0-basic-proto/using-with-arduino-ide
1. Add the iSkipper library to the Arduino IDE by copying them into your libraries folder (C:\Users\%username%\Documents\Arduino\libraries): https://github.com/wizard97/iSkipper/tree/master/emulator/iSkipper
1. Add the iSkipper library dependencies to the Arduino IDE by copying them into your libraries folder: https://github.com/wizard97/iSkipper/tree/master/emulator/libs
1. Download this repository and open TimeTurner_Arduino_Sketch.ino in the Arduino IDE
1. Go to Tools > Board > Adafruit SAMD Boards and select “Adafruit Feather M0”
1. You may want to change “MY_CLICKER_ID” to something else. All other configuration options are already set optimally for the Feather M0 platform.
1. Plug your device into your computer using a USB 2.0 A-Male to Micro B Cable
1. Go to Tools > Port and select the USB port where you plugged in the device
1. Go to Sketch > Upload - this sometimes takes multiple attempts to work.

## Usage
* Button A: Cycle through modes of operation (see modes of operation below)
* Button B: Cycle through possible votes (A-E)
* Button C: Toggle selected mode on/off; resets all counters
Estimated percent battery life remaining is indicated on the bottom right. Battery use is higher when any mode is toggled “on”. The device remains operational while charging.

## Modes of Operation
* View Votes: Displays the number of votes for each option overheard by device.
* Fake Votes: Floods the system with fake votes from random device IDs (Layer 7).
* Change Votes: Changes (overwrites) all overheard votes to the selected option.
* DoS Attack: Floods the system with random packets causing real votes to fail (Layer 4).
* Copy Votes: Automatically votes for the most popular option based on overheard votes.
* Vote Choice: Automatically votes for the selected option regardless of overheard votes.

## Other Known Attacks
If an instructor is using an attendance device to controll their machine, additional exploits may be possible.
* Switching the instructor's slides
* Starting and stopping polling
* Changing the answer marked as "correct"

## Source Repositories
Time-Turner: https://github.com/VCNinc/Time-Turner
iSkipper Software: https://github.com/charlescao460/iSkipper-Software
iSkipper: https://github.com/wizard97/iSkipper
*Contributors: [Aaron Wisner](https://github.com/wizard97), [Jacob Glueck](https://github.com/orangeturtle739), [Charles Cao](https://github.com/charlescao460), [Ammar Askar](https://github.com/ammaraskar), [Bruce Land](https://people.ece.cornell.edu/land/), [Vivek Nair](https://github.com/VCNinc)*
*License: [MIT](https://github.com/VCNinc/Time-Turner/blob/main/LICENSE)*
