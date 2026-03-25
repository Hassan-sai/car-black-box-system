# Car Black Box System

A vehicle event data recorder (EDR) built using **Embedded C** on the **PIC18F4580 microcontroller**, programmed in **MPLAB X IDE**. Inspired by aircraft black boxes, this system continuously logs critical driving events — speed, gear changes, collisions — to external EEPROM memory in real time, with secure UART-based log download support.

---

## Abstract

The Black Box concept, widely known from aviation, is applied here to automobiles as an Event Data Recorder (EDR) / Accident Data Recorder (ADR). The system monitors vehicle behaviour in real time and logs critical events to help analyse rash driving, track vehicle usage, and improve road safety accountability.

---

## Problem Statement

With increasing road accidents caused by overspeeding and reckless driving, there is a need for a system that silently records driving events. This black box logs every critical event with a timestamp and speed reading — providing an unalterable record that can be downloaded via UART for analysis.

---

## System Features

### Default screen (operation mode)
- Acts as a live dashboard showing current time, vehicle speed, and the latest event
- **Gear Up / Gear Down** keys navigate through gear events (GN, GR, G1–G4)
- **Collision** key triggers and logs a collision event (C)

### Event capture
- Captures and stores events in external EEPROM memory in real time
- Every event is stored in the format: `EVENT TIME | EVENT SIGNATURE | SPEED AT EVENT`
- Events are recorded continuously regardless of which menu screen is active

### Login screen (secure access)
- Accessed via UP or DOWN user keys
- Password: 4-key combination, each press shown as `*`
- Maximum 3 wrong attempts per 15 minutes — user blocked for 2 minutes after 3 failures
- Incomplete entry (3-second pause) returns to default screen

### Main menu
Navigated via UP / DOWN keys with long-press selection:
- **View Log** — displays stored events on CLCD
- **Set Time** — adjust system clock (UP = field change, DOWN = increment)
- **Download Log** — transmits all stored logs via UART; displays confirmation on CLCD
- **Clear Log** — erases all stored events; displays confirmation on CLCD
- **Change Password** — update the 4-key access password with confirmation

---

## Technologies used

- **Language**: Embedded C
- **Microcontroller**: PIC18F4580
- **IDE**: MPLAB X IDE
- **Communication**: UART (for external log download)
- **Storage**: External EEPROM (real-time event logging)
- **Display**: CLCD (Character LCD)
- **Concepts**: Firmware development, interrupt-driven event capture, memory management, real-time systems

---

## Hardware components

| Component | Purpose |
|-----------|---------|
| PIC18F4580 | Main microcontroller — runs all firmware logic |
| External EEPROM | Persistent storage for event logs |
| CLCD | Display for dashboard, menus, and log view |
| UART interface | Secure external access to download logs |
| User keys (UP/DOWN) | Navigation and password entry |

---

## Event log format

Each stored event follows this structure:

```
[EVENT TIME]  [EVENT SIGNATURE]  [SPEED AT EVENT]
Example:
12:45:03      G3                 72 km/h
12:45:10      C                  68 km/h   ← Collision detected
```

---

## Author

Hassan Sai Siragari — [LinkedIn](https://linkedin.com/in/hassansai-siragari)
