# 🏠 Smart Home Automation System using ESP32 and Blynk

## Overview
An IoT-based Smart Home Automation System built using ESP32, Blynk IoT, DS18B20 temperature sensor, LDR, HC-SR04 ultrasonic sensor, I2C LCD, LEDs, fan, and buzzer. The system supports **Automatic** and **Manual** modes with live monitoring through Blynk.

## Hardware
- ESP32 Dev Module
- DS18B20 Temperature Sensor
- LDR Module
- HC-SR04 Ultrasonic Sensor
- 16x2 I2C LCD
- 2 LEDs
- DC Fan
- Buzzer

## Pin Configuration

| Device | ESP32 Pin |
|---|---|
| DS18B20 | GPIO4 |
| LDR | GPIO34 |
| Ultrasonic Trigger | GPIO5 |
| Ultrasonic Echo | GPIO18 |
| Living Room LED | GPIO2 |
| Bedroom LED | GPIO15 |
| Fan | GPIO23 |
| Buzzer | GPIO19 |
| LCD SDA | GPIO21 |
| LCD SCL | GPIO22 |

## Blynk Virtual Pins

| Pin | Function |
|---|---|
| V0 | Temperature |
| V1 | Distance |
| V2 | Light Status |
| V4 | Bedroom LED |
| V5 | Living Room LED |
| V6 | Auto / Manual Mode |
| V7 | Fan |

## System Workflow

1. ESP32 boots and connects to WiFi.
2. Connects to the Blynk Cloud.
3. Initializes LCD and sensors.
4. Reads temperature, light intensity, and distance.
5. In **AUTO** mode:
   - LDR controls the living room light.
   - Ultrasonic sensor controls the bedroom light.
   - Temperature controls the fan and buzzer.
6. In **MANUAL** mode:
   - Living room light, bedroom light, and fan are controlled from Blynk.
7. Updates the LCD and uploads sensor values to Blynk.
8. Prints diagnostics to the Serial Monitor.
9. Repeats continuously.

## LCD Display

**Line 1:** Scrolling text: `SMART HOME SYSTEM`

**Line 2 (Priority):**
1. Motion Detected
2. Temperature Alert
3. Dark | Light ON
4. Bright | OFF

## Features

- Automatic lighting control
- Motion-based bedroom lighting
- Automatic fan control
- High-temperature buzzer alert
- Blynk dashboard monitoring and control
- Auto / Manual operating modes
- Scrolling LCD interface
- Serial debugging

## Project Architecture

```text
Sensors
│
├── DS18B20 ──► Temperature
├── LDR ──────► Light
└── HC-SR04 ──► Motion
        │
        ▼
      ESP32
        │
 ┌──────┼─────────────┐
 │      │             │
 ▼      ▼             ▼
 LCD   Blynk      Actuators
                  ├── Living LED
                  ├── Bedroom LED
                  ├── Fan
                  └── Buzzer
```
