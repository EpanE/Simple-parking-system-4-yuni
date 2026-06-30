# Simple Parking System — ESP32-S3 DevKitC-1

Smart parking demo built on an ESP32-S3 DevKitC-1: 3 ultrasonic slot sensors, whitelisted RFID gate entry, an IR-triggered exit, an LCD status display, per-slot LED indicators, a shared servo barrier, and an optional Blynk IoT scaffold.

**Live pinout doc:** https://epane.github.io/Simple-parking-system-4-yuni/

## Hardware

| Device | Qty | Purpose |
|---|---|---|
| HC-SR04 ultrasonic | 3 | Detect occupied/free per slot (Front, Left, Right) |
| RC522 RFID reader | 1 | Authorizes gate entry (hardcoded whitelist) |
| IR obstacle sensor | 1 | Detects a car at the exit lane, opens the gate — no auth needed |
| LCD 16×2 (I²C) | 1 | Shows slot status / scanned UID / access result |
| Red LED | 3 | Lit when matching slot is occupied |
| Green LED | 3 | Lit when matching slot is free |
| SG90 servo | 1 | Shared gate barrier — opens 3s on a whitelisted RFID scan OR exit IR trigger |

## GPIO map

| Function | GPIO |
|---|---|
| HC-SR04 #1 (Front) TRIG / ECHO | 1 / 2 |
| HC-SR04 #2 (Left) TRIG / ECHO | 4 / 5 |
| HC-SR04 #3 (Right) TRIG / ECHO | 6 / 7 |
| LCD SDA / SCL | 8 / 9 |
| RC522 SS / MOSI / SCK / MISO / RST | 10 / 11 / 12 / 13 / 14 |
| Red LED — Front / Left / Right | 15 / 16 / 17 |
| Green LED — Front / Left / Right | 18 / 21 / 38 |
| Servo SG90 (gate) | 39 |
| IR sensor (exit) | 40 |

All pins are SPI2/Wire defaults on the S3 — no HSPI workaround, no I²C remap, no GPIO12 boot conflict (unlike the original ESP32 DEVKIT V1). Full diagram, legend, and free-pin list are in the doc page above.

## Repo layout

```
index.html                     full pinout doc (GitHub Pages site)
s3_devkitc_full_system.html    duplicate of index.html (source copy)
arduino/
  main_system/
    main_system.ino    full integrated sketch — all devices + outputs
    rfid_whitelist.h   hardcoded list of allowed card UIDs
    blynk_config.h     optional Blynk IoT credentials + ENABLE_BLYNK toggle
  tests/                       standalone sketch per device, same GPIOs, no
                                cross-dependencies — bench-test before wiring
                                the full rig
    test_hcsr04/   3x ultrasonic only
    test_rc522/    RFID reader only
    test_lcd/      LCD I2C only
    test_leds/     6x status LED only
    test_servo/    SG90 gate only
    test_ir/       IR exit sensor only
apps-script/
  Code.gs           doGet() entry point
  index.html        HtmlService template (same content as root index.html,
                     stripped of <html>/<head>/<body> wrapper)
```

## Arduino setup

Board: **ESP32S3 Dev Module** (esp32 by Espressif Systems core).

Libraries (Arduino Library Manager):
- `MFRC522`
- `LiquidCrystal_I2C`
- `ESP32Servo`
- `Blynk` (only needed if you enable Blynk — see below)

Flash a test sketch from `arduino/tests/` first to verify each device individually, then flash `arduino/main_system/main_system.ino` for the full system.

### RFID whitelist

`arduino/main_system/rfid_whitelist.h` holds the list of allowed card UIDs — only cards on this list open the gate. To find a card's UID: flash `arduino/tests/test_rc522/test_rc522.ino`, open Serial Monitor at 115200 baud, scan the card, copy the printed UID into `ALLOWED_UIDS[]`.

### Exit IR sensor

The exit lane's IR sensor opens the same shared gate servo on any detected obstacle — no whitelist check. If your module's wiring is inverted (HIGH instead of LOW when blocked), flip `IR_OBSTACLE_STATE` in `main_system.ino` (and `test_ir.ino`).

## Blynk IoT (optional)

Off by default — the sketch compiles and runs fine without it. To enable, in `arduino/main_system/blynk_config.h`:

1. Create a new template at [blynk.cloud](https://blynk.cloud) (or the Blynk IoT app).
2. Copy the Template ID and Template Name from the template's Info tab.
3. Create a device from that template, copy its Auth Token (Device Info).
4. Paste all three into `blynk_config.h`, fill in your WiFi SSID/password.
5. Set `ENABLE_BLYNK` to `1`.
6. Install the **Blynk** library (by Volodymyr Shymanskyy) via Library Manager.
7. In the Blynk dashboard, add a "Labeled Value" widget bound to `V0` (free slot count), optionally a "Button" widget bound to `V1` (manual gate trigger).

## Google Apps Script deployment

1. Create a project at script.google.com.
2. Paste [apps-script/Code.gs](apps-script/Code.gs) into `Code.gs`.
3. Add an HTML file named `index`, paste [apps-script/index.html](apps-script/index.html).
4. Deploy → New deployment → Web app, execute as me, access anyone.
