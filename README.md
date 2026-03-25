# Car Black Box System

A real-time vehicle event data recorder (EDR) built in **Embedded C** on the **PIC18F4580 microcontroller**, programmed using **MPLAB X IDE**. Inspired by aircraft black boxes, this system continuously monitors and logs critical driving events — gear changes, collisions, speed — to external EEPROM via I2C, with secure UART-based log download and a password-protected menu system displayed on a character LCD.

---

## Project Structure

| File | Description |
|------|-------------|
| `MAIN.C` | Entry point — initialises all peripherals, runs the main event loop and dispatches to dashboard, menu, password, and log screens |
| `black_box.c` | Core logic — dashboard display, EEPROM read/write via I2C (24C02), real-time clock read, event capture, view/download/clear log, password and menu handling |
| `main.h` | Global declarations — flags, shared variables, macros, and all function prototypes |
| `adc.c / adc.h` | ADC driver — reads analogue speed sensor values from the PIC18F4580 ADC module |
| `clcd.c / clcd.h` | Character LCD driver — initialisation, character and string output, cursor positioning |
| `uart.c / uart.h` | UART driver — serial communication for downloading stored event logs to a PC |
| `i2c.c / i2c.h` | I2C driver — start, stop, read, write operations for EEPROM and RTC communication |
| `eeprom.c / eeprom.h` | Internal EEPROM driver — read/write to PIC18F4580 on-chip EEPROM |
| `ds1307.c / ds1307.h` | DS1307 RTC driver — initialises and reads hours, minutes, seconds over I2C |
| `isr.c` | Interrupt Service Routine — handles timer and peripheral interrupts |
| `timer0.c / timer0.h` | Timer0 driver — configures and manages system timing |
| `matrix_keypad.c / matrix_keypad.h` | Matrix keypad driver — detects key presses (state change and level change modes) |
| `Makefile` | MPLAB X build configuration for PIC18F4580 target |

---

## System Features

### Dashboard (default screen)
- Displays current time (from DS1307 RTC), vehicle speed (from ADC), and latest event
- Gear Up / Gear Down keys navigate through gear events: GN, GR, G1–G4
- Collision key logs a collision event instantly

### Real-time event capture
- Events stored in **external 24C02 EEPROM** via I2C
- Every event format: `EVENT TIME | EVENT SIGNATURE | SPEED`
- Capture runs continuously — even while navigating menus

### Password-protected menu access
- 4-key combination entry, each press shown as `*` on CLCD
- Maximum 3 wrong attempts per 15 minutes — locked for 2 minutes after 3 failures
- 3-second inactivity returns to dashboard automatically

### Menu options
| Option | Function |
|--------|----------|
| View Log | Scrolls through stored events on CLCD |
| Set Time | Adjusts DS1307 RTC clock fields |
| Download Log | Transmits all events via UART to PC |
| Clear Log | Erases all stored events from EEPROM |
| Change Password | Updates the 4-key access password |

---

## Technologies used

- **Language**: Embedded C
- **Microcontroller**: PIC18F4580
- **IDE**: MPLAB X IDE
- **Protocols**: I2C (EEPROM + RTC), UART (log download)
- **Peripherals**: ADC, Timer0, Interrupt Service Routine, Matrix Keypad, Character LCD
- **Storage**: External 24C02 EEPROM (event logs), Internal EEPROM (password)
- **RTC**: DS1307 for real-time timestamping

---

## Hardware components

| Component | Purpose |
|-----------|---------|
| PIC18F4580 | Main microcontroller |
| 24C02 EEPROM | External persistent event log storage via I2C |
| DS1307 RTC | Real-time clock for event timestamps via I2C |
| Character LCD (CLCD) | Dashboard and menu display |
| Matrix Keypad | User input — navigation and password entry |
| ADC input | Reads analogue speed sensor |
| UART interface | Downloads event logs to a PC |

---

## Event log format

```
[HH:MM:SS]   [EVENT]   [SPEED]
12:45:03     G3        72
12:45:10     C         68      ← Collision detected
12:46:01     G4        85
```

---

## How to build

Open the project in **MPLAB X IDE**, select the PIC18F4580 target, and build using the included `Makefile`. Flash the generated `.hex` file to the microcontroller using a PIC programmer.

---

## Author

Hassan Sai Siragari — [LinkedIn](https://linkedin.com/in/hassansai-siragari)
