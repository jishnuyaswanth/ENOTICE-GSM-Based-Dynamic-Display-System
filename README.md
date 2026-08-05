# 📟 NextGen E-Notice: Secure GSM Based Dynamic Display System

<div align="center">

![Platform](https://img.shields.io/badge/Platform-LPC2148-blue?style=for-the-badge)
![Language](https://img.shields.io/badge/Language-Embedded%20C-green?style=for-the-badge)
![IDE](https://img.shields.io/badge/IDE-Keil%20µVision-orange?style=for-the-badge)
![Communication](https://img.shields.io/badge/Communication-GSM%20%7C%20I²C%20%7C%20UART-red?style=for-the-badge)

**Remotely update a scrolling LED notice board via SMS — securely, wirelessly, instantly.**

</div>

---

## 📖 Table of Contents

- [🌟 Overview](#-overview)
- [✨ Features](#-features)
- [🔧 Hardware Requirements](#-hardware-requirements)
- [💻 Software Requirements](#-software-requirements)
- [🔌 Hardware Connections](#-hardware-connections)
- [📱 SMS Command Format](#-sms-command-format)
- [🏗️ Block Diagram](#️-block-diagram)
- [📁 Project Structure](#-project-structure)
- [⚙️ Flow Chart](#️-flow-chart)
- [🔐 Security Mechanism](#-security-mechanism)
- [🚀 Getting Started](#-getting-started)
- [📡 AT Commands Reference](#-at-commands-reference)

---

## 🌟 Overview

The **NextGen E-Notice System** replaces traditional static notice boards with a **dynamic, remotely controlled scrolling LED dot-matrix display**. Authorized users can update the displayed message from anywhere by simply sending an SMS — no physical access, no wiring changes, no manual intervention required.

Built on the **LPC2148 ARM7TDMI-S microcontroller**, the system uses a **GSM module** for SMS-based control, an **I²C EEPROM** for persistent storage, and four **8×8 dot-matrix LED panels** for vivid scrolling output.

> 🏫 **Ideal for:** Educational institutions, corporate offices, hospitals, government facilities, railway stations, and any public information display.

---

## ✨ Features

| Feature | Description |
|---------|-------------|
| 📡 **Remote SMS Control** | Update the notice board from any mobile phone |
| 🔐 **Two-Layer Security** | Sender number verification + security code check |
| 💾 **EEPROM Persistence** | Messages survive power cycles and resets |
| 📜 **Smooth Scrolling** | Text scrolls across four 8×8 LED panels |
| 📲 **Auto SMS Feedback** | Instant reply for invalid commands or unauthorized access |
| 🔄 **Dynamic Auth Number** | Authorized phone number is updatable via SMS |
| ⚡ **Interrupt-Driven RX** | UART ISR ensures no SMS is ever missed |
| 🔋 **Low Power Design** | Efficient busy-wait display loop with SMS wake-up |

---

## 🔧 Hardware Requirements

| # | Component | Description |
|---|-----------|-------------|
| 1 | 🧠 **LPC2148** | ARM7TDMI-S microcontroller — main processing unit |
| 2 | 🟥 **4× 8×8 Dot Matrix Display** | LED panels for scrolling message output |
| 3 | 🔀 **74HC164 (×4)** | 8-bit SIPO shift registers — column drivers |
| 4 | 🔒 **74HC573** | Octal D-type latch — row driver |
| 5 | 💾 **AT24C256** | I²C EEPROM — persistent message & number storage |
| 6 | 📶 **M660A GSM Module** | GSM modem for SMS send/receive |
| 7 | 🔁 **MAX232** | RS-232 level converter (optional, for PC interfacing) |
| 8 | 🔌 **USB to UART Converter** | For programming and debugging |

---

## 💻 Software Requirements

| Tool | Purpose |
|------|---------|
| 🖥️ **Keil µVision (ARM)** | IDE and compiler for ARM7 embedded C |
| ⚡ **Flash Magic** | Firmware flashing utility for LPC2148 |
| 🔬 **HyperTerminal / Tera Term** | AT command testing via serial port |

---

## 🔌 Hardware Connections

### 🟦 Dot Matrix — Column Drivers (74HC164)

| Shift Register | 📥 SIN (DSA) Pin | ⏱️ CP Pin | 🖥️ Drives |
|----------------|-----------------|-----------|-----------|
| 74HC164 **#1** | P0.8  | P0.9  | Display 1 columns |
| 74HC164 **#2** | P0.10 | P0.11 | Display 2 columns |
| 74HC164 **#3** | P0.12 | P0.13 | Display 3 columns |
| 74HC164 **#4** | P0.14 | P0.15 | Display 4 columns |

> 💡 Each 74HC164's Q0–Q7 outputs connect to COL1–COL8 of its respective dot-matrix panel. Column drive is **active-low**.

---

### 🟩 Dot Matrix — Row Driver (74HC573)

| 74HC573 Pin | LPC2148 Pin | Dot Matrix Row |
|-------------|-------------|----------------|
| D0 | P0.16 | ROW 8 |
| D1 | P0.17 | ROW 7 |
| D2 | P0.18 | ROW 6 |
| D3 | P0.19 | ROW 5 |
| D4 | P0.20 | ROW 4 |
| D5 | P0.21 | ROW 3 |
| D6 | P0.22 | ROW 2 |
| D7 | P0.23 | ROW 1 |

---

### 🟨 I²C EEPROM (AT24C256)

| Signal | LPC2148 Pin | Notes |
|--------|-------------|-------|
| SCL | P0.2 | I²C clock @ 100 kHz |
| SDA | P0.3 | I²C data |
| VCC | 3.3V | — |
| GND | GND | A0–A2 tied to GND → slave address `0x50` |

---

### 🟧 GSM Module (M660A)

| Signal | LPC2148 Pin | Baud Rate |
|--------|-------------|-----------|
| TXD | P0.0 (TXD0) | 9600 |
| RXD | P0.1 (RXD0) | 9600 |

---

## 📱 SMS Command Format

### 🖊️ Update Display Message

```
5665DYour_Message_Here@
```

| Part | Meaning |
|------|---------|
| `5665` | 🔑 Security code |
| `D` | 📺 **D**isplay update command |
| `Your_Message_` | ✍️ Text to scroll on the board |
| `@` | 🔚 End-of-message terminator |

**✅ Example:**
```
5665DWELCOME TO MY PROFILE@
```

---

### 📞 Update Authorized Mobile Number

```
5665M9898999923@
```

| Part | Meaning |
|------|---------|
| `5665` | 🔑 Security code |
| `M` | 📱 **M**obile number update command |
| `9898999923` | ☎️ New 10-digit authorized number |
| `@` | 🔚 Terminator |

**✅ Example:**
```
5665M9898999923@
```

---

### 🚨 Security Response Table

| Scenario | 🔁 System Response |
|----------|--------------------|
| ✅ Valid `D` command, authorized sender | Display updated + EEPROM saved + SMS deleted |
| ✅ Valid `M` command, authorized sender | Auth number updated + EEPROM saved + SMS deleted |
| ⚠️ Authorized sender, wrong format/code | ❌ Reply SMS: *"Security Incorrect. Correct format is 5665Dmsg@"* |
| ❌ Unauthorized sender number | 🚨 Alert SMS with sender's number sent to owner |

---

## 🏗️ Block Diagram

```
  <img width="1536" height="892" alt="ChatGPT Image Aug 5, 2026, 02_41_39 PM" src="https://github.com/user-attachments/assets/17ae8e06-8734-48cc-8b71-ae0d06c4c79f" />
```

---

## 📁 Project Structure

```
📦 NextGen-ENotice/
│
├── 📄 main.c                  ← 🎯 Entry point & main SMS dispatch loop
├── 📄 gsm.c / gsm.h           ← 📶 GSM module: AT commands, SMS ops
├── 📄 uart.c / uart.h         ← ⚡ UART0: ISR-driven receive, TX functions
├── 📄 uart_def.h              ← ⚙️  UART baud rate & bit definitions
├── 📄 uart0.h                 ← 📋 Alternate minimal UART0 prototypes
│
├── 📄 dot_matrix.c            ← 🖥️  Display: row scan, font lookup, scroll
├── 📄 dotmatrix.h             ← 📋 Dot matrix public API
├── 📄 dot_mat_def.h           ← 🔤 8×8 font bitmap table (96 ASCII chars)
│
├── 📄 sipo.c                  ← 🔀 74HC164 bit-bang shift register driver
├── 📄 sipo_74hc164.h          ← 📋 SIPO driver prototypes
│
├── 📄 i2c.c / i2c.h           ← 🔗 I²C bus: start/stop/write/read
├── 📄 i2c_defines.h           ← ⚙️  I²C speed, pin & register definitions
├── 📄 i2c_eprom.c             ← 💾 AT24C256 byte-write, string read/write
├── 📄 i2c_eeprom.h            ← 📋 EEPROM public API
├── 📄 i2c_eeprom_defines.h    ← 🗺️  EEPROM addresses & memory map
│
├── 📄 fun.c / fun.h           ← 🔐 SMS verification & content extraction
├── 📄 security.h              ← 🔐 Security function prototypes
│
├── 📄 delay.c / delay.h       ← ⏱️  Busy-wait delays (µs / ms / s)
├── 📄 defines.h               ← 🔧 Bit manipulation macros
└── 📄 types.h                 ← 📐 Type aliases (u8, u16, u32, s8, s32…)
```

### 🗺️ EEPROM Memory Map

| Address | Symbol | Content | Flag Value |
|---------|--------|---------|------------|
| `0x00` | `EE_MOBI` | Auth number presence flag | `'2'` = valid |
| `0x01` | `EE_MOB` | Authorized mobile number (10 digits + `@`) | — |
| `0x14` | `EE_MSGI` | Message presence flag | `'1'` = valid |
| `0x16` | `EE_MSG` | Display message string (variable + `@`) | — |

---

## ⚙️ Flow Chart

```
  <img width="1254" height="440" alt="ChatGPT Image Aug 5, 2026, 02_41_28 PM" src="https://github.com/user-attachments/assets/183d3dbd-c586-4430-a39c-38bfc5f66e29" />
```

---

## 🔐 Security Mechanism

The system implements a **two-layer security model** to ensure only authorized users can control the display:

```
🔒 Layer 1 — Sender Number Authentication
┌─────────────────────────────────────────────────────┐
│  Only the number stored in EEPROM can issue commands │
│  Any other number → 🚨 Alert SMS sent to owner       │
│                   → 🗑️  SMS deleted                  │
└─────────────────────────────────────────────────────┘

🔑 Layer 2 — Security Code + Command Validation
┌─────────────────────────────────────────────────────┐
│  Message MUST contain "1212" + "D" or "M" + "#"     │
│  Wrong code or missing terminator →                  │
│       ❌ Error reply SMS to authorized owner          │
│       🗑️  SMS deleted immediately                    │
└─────────────────────────────────────────────────────┘
```

> 🛡️ **Zero tolerance:** No display update or EEPROM write ever occurs on a failed verification. Every unauthorized attempt triggers an immediate alert SMS to the registered owner.

---

## 🚀 Getting Started

### 📦 Step 1 — Clone the Repository

```bash
git clone https://github.com/yourusername/nextgen-enotice.git
cd nextgen-enotice
```

### 🛠️ Step 2 — Hardware Setup

Wire all components per the connection tables above.
> ⚠️ Double-check polarity on dot-matrix panels — columns are **active-low**.

### 🧪 Step 3 — Module Testing (Recommended Order)

Test each subsystem individually before full integration:

- [ ] ✅ Single character on one dot-matrix panel
- [ ] ✅ 4-panel word display (`HELP`)
- [ ] ✅ Full string scrolling (`PROJECT SUCCESSFULLY STARTED`)
- [ ] ✅ EEPROM write → power cycle → read → display
- [ ] ✅ UART0 loopback with interrupt
- [ ] ✅ GSM AT command test via HyperTerminal at 9600 baud
- [ ] ✅ Full SMS send/receive/display loop

### 🔨 Step 4 — Build

Open the project in **Keil µVision**, add all `.c` files to the project, and compile. Ensure `LPC21xx.H` is in your include path.

### ⚡ Step 5 — Flash

Use **Flash Magic** to program the `.hex` file to LPC2148 via UART0:
- Baud: `9600`
- Device: `LPC2148`
- Oscillator: `12 MHz`
- Interface: `None (ISP)`

### 📲 Step 6 — Test

Power on the board and send an SMS from the registered number:

```
1212DHELLO WORLD#
```

🎉 **`HELLO WORLD` should begin scrolling on the display!**

---

## 📡 AT Commands Reference

| Command | 🎯 Purpose | ✅ Expected Response |
|---------|-----------|---------------------|
| `AT` | Check modem presence | `OK` |
| `ATE0` | Disable command echo | `OK` |
| `AT+CMGF=1` | Set SMS text mode | `OK` |
| `AT+CNMI=2,1,0,0,0` | Enable new SMS notification | `OK` |
| `AT+CMGR=1` | Read SMS at index 1 | SMS content + `OK` |
| `AT+CMGD=1` | Delete SMS at index 1 | `OK` |
| `AT+CMGS="number"` | Send SMS (end with `0x1A`) | `+CMGS: n` then `OK` |

---

## 👥 Contributors

> 🎓 Developed as part of an embedded systems academic project on the LPC2148 ARM7 microcontroller platform.

---

## 📜 License

This project is developed for **educational purposes**. Refer to your institution's guidelines before redistribution.

---

<div align="center">

⭐ **If this project helped you, give it a star!** ⭐

<br/>

Made with ❤️ using **Embedded C** on **LPC2148 ARM7**

📟 *NextGen E-Notice — Bringing notice boards into the wireless era*

</div>
