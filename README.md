# 🎓 Smart Exam Hall Monitoring and Management System

> **Bare-metal embedded firmware for automated examination hall management**  
> Built on ARM7TDMI-S (LPC2148/LPC2129) — fully register-level, zero HAL abstraction

---

## 📌 Overview

An intelligent embedded solution that automates examination timing, environmental monitoring, and status indication in examination halls. The system eliminates manual timing errors and reduces invigilator workload through hardware-driven automation.

Developed as part of the **Vector India Embedded Systems Industry Training Program** (C → ARM Architecture → DSA progression), Hyderabad.

---

## ✨ Features

| Feature | Description |
|---|---|
| ⏱️ RTC-based auto-start | Exam starts automatically when RTC time matches configured start time |
| ⏳ Countdown timer | Minute-by-minute countdown from configured duration (up to 99 min) |
| 🔢 7-Segment display | Remaining exam minutes shown on dual multiplexed 7-seg display |
| 🌡️ Temperature monitoring | Continuous room temperature display via LM35 sensor (ADC) |
| 🔐 Password protection | 4-digit password-protected configuration menu |
| 🕐 RTC editing | Set/update current date and time via keypad |
| ⏸️ Pause/Resume | Emergency pause via external interrupt (EINT1), resumable |
| 🚨 LED status alerts | Green (>3 min) → Yellow (1-3 min) → Red (<1 min) |
| 🔔 Buzzer notification | Audio alert when exam countdown reaches zero |
| 🔑 Password change | Change admin password with old-password verification |
| 📅 Date/time validation | Input validation (hour 0-23, min 0-59, day 1-31, month 1-12) |

---

## 🏗️ System Architecture

### State Machine

```
                    ┌─────────────────────────────────┐
         ┌─────────►│           IDLE_MODE              │◄──────────────┐
         │          │  • LCD: TIME / DATE / TEMP       │               │
         │          │  • 7-seg: 00                     │               │
         │          │  • Monitors exam start time      │               │
         │          └──────────────┬──────────────┬────┘               │
         │                        │              │                      │
         │               EINT0    │              │ RTC matches          │
         │               press    ▼              │ start time           │
         │          ┌─────────────────────┐      │                      │
         │          │    CONFIG_MODE      │      │                      │
         │          │  • Password entry   │      │                      │
         │          │  • RTC edit         │      │                      │
         │          │  • Exam time setup  │      │                      │
         │          │  • Duration config  │      │                      │
         │          │  • Password change  │      │                      │
         │          └─────────────────────┘      │                      │
         │                                       ▼                      │
         │                          ┌────────────────────────┐          │
         │              ┌──────────►│      EXAM_RUNNING      │          │
         │              │           │  • Countdown active    │          │
         │   EINT1      │           │  • LCD: time+temp+rem  │          │
         │   resume     │           │  • 7-seg: min countdown│          │
         │              │           │  • LED alerts active   │          │
         │              │           └────────────┬──────────┬┘          │
         │              │                        │          │            │
         │   ┌──────────┴──────┐      countdown  │  EINT1   │            │
         │   │  EXAM_PAUSED    │      reaches 0  │  press   │            │
         │   │  (via toggle)   │                 │  pause   │            │
         │   └─────────────────┘                 │          │            │
         │                                       ▼          ▼            │
         │                          ┌────────────────────────┐          │
         └──────────────────────────│       EXAM_ENDED       │──────────┘
                     EINT0          │  • Buzzer ON (5 sec)   │
                     press          │  • "EXAM OVER" on LCD  │
                                    │  • RED LED ON          │
                                    └────────────────────────┘
```

### Key Design Decisions

- **No HAL/RTOS** — all peripherals driven at register level (IODIR, IOPIN, IOSET, IOCLR, VIC, EXTMODE, ADCR, CCR, etc.)
- **Timer0 ISR** drives 7-seg multiplexing (5ms refresh, non-blocking) — frees main loop entirely
- **RTC hardware** provides 1-second tick reference for countdown and display updates
- **External interrupts** (EINT0/EINT1) handle config trigger and pause/resume via VIC
- **Blocking CONFIG_MODE** — acceptable since config is invigilator-initiated, not time-critical

---

## 🔧 Hardware

### Primary Platform

| Component | Details |
|---|---|
| MCU | NXP LPC2148 (ARM7TDMI-S, 60MHz, LQFP64) |
| Development Board | Vector India Advanced Development Board |
| Crystal | 12MHz (main) + 32.768KHz (RTC) |
| LCD | JHD 204A — 20×4 character display (HD44780 compatible) |
| Keypad | 4×4 matrix keypad |
| 7-Segment | Dual common-anode 7-segment (multiplexed) |
| Temperature | LM35 analog sensor (10mV/°C) |
| LEDs | 3× (Green/Yellow/Red) — active LOW |
| Buzzer | Active HIGH (driven via BC109 NPN transistor) |

### Simulation Platform

| Component | Details |
|---|---|
| MCU | LPC2124/LPC2129 (Proteus ISIS simulation) |
| Simulator | Proteus 8 Professional |

---

## 📍 Pin Mapping

### LPC2148 (Hardware Kit)

| Component | Pin(s) | Notes |
|---|---|---|
| LCD D0–D7 | P0.8 – P0.15 | 8-bit mode |
| LCD RS | P0.16 | Register select |
| LCD EN | P0.17 | Enable strobe |
| Keypad ROW0–3 | P1.16 – P1.19 | Output |
| Keypad COL0–3 | P1.20 – P1.23 | Input (with pullups) |
| 7-seg segments | P0.18 – P0.25 | A–G + DP |
| 7-seg DSEL1 | P1.24 | Digit 1 select |
| 7-seg DSEL2 | P1.25 | Digit 2 select |
| LM35 (ADC) | P0.28 (AD0.1) | CH1 |
| Green LED | P1.26 | Active LOW |
| Yellow LED | P1.27 | Active LOW |
| Red LED | P1.28 | Active LOW |
| Buzzer | P1.29 | Active HIGH |
| EINT0 (Config SW) | P0.16 | Hardwired on board |
| EINT1 (Pause SW) | P0.14 | Hardwired on board |

### LPC2129 (Proteus Simulation)

| Component | Pin(s) | Notes |
|---|---|---|
| LCD D0–D7 | P0.8 – P0.15 | 8-bit mode |
| LCD RS | P0.16 | |
| LCD EN | P0.17 | |
| Keypad ROW0–3 | P1.16 – P1.19 | |
| Keypad COL0–3 | P1.20 – P1.23 | |
| 7-seg segments | P0.18 – P0.25 | |
| 7-seg DSEL1/2 | P1.24 / P1.25 | |
| LM35 | P0.28 (AD0.1) | |
| LEDs | P1.26 – P1.28 | |
| Buzzer | P1.29 | |
| EINT0 (Config) | P0.1 | EINT0 alt function on LPC2129 |
| EINT1 (Pause) | P0.3 | EINT1 alt function on LPC2129 |

> **Note:** LPC2129 and LPC2148 differ in EINT pin locations — handled via `#define CPU LPC2129 / LPC2148` conditional compilation in `mini_pro_defines.h`

---

## 📁 Project Structure

```
Smart-Exam-Hall-Management-LPC2148/
│
├── README.md
│
├── src/
│   ├── mini_project_main.c      # Main application — state machine
│   ├── mini_pro_defines.h       # All pin definitions, peripheral constants
│   ├── headers.h                # Master include header
│   │
│   ├── lcd.c / lcd.h            # HD44780 LCD driver (8-bit, register-level)
│   ├── kpm.c / kpm.h            # 4×4 matrix keypad driver
│   ├── seg.c / seg.h            # 7-segment multiplexed display driver
│   ├── rtc.c / rtc.h            # RTC driver (CCR, PREINT, PREFRAC)
│   ├── adc.c / adc.h            # ADC driver (10-bit, polling)
│   ├── lm35.c / lm35.h          # LM35 temperature sensor wrapper
│   ├── interrupt.c / interrupt.h # EINT0/EINT1 VIC configuration + ISRs
│   ├── timer_delay.c / timer_delay.h # Timer0 ISR (ms_tick + 7-seg refresh)
│   └── delay.c / delay.h        # Software delay utilities
│
├── docs/
│   ├── Smart_Exam_Hall_System.pdf   # Project specification document
│   └── LPC2148_Dev_Board.pdf        # Vector India board documentation
│
└── simulation/
    └── mini_project.pdsprj          # Proteus simulation file
```

---

## ⚙️ Peripheral Drivers — Technical Details

### LCD Driver (`lcd.c`)
- 8-bit parallel interface (D0–D7 on P0.8–P0.15)
- Full command set: clear, cursor positioning, display on/off
- Custom `F32Lcd()` for floating-point temperature display
- Backspace/cursor movement for keypad input feedback

### Keypad Driver (`kpm.c`)
- Active-low row scan with pullup resistors on columns
- Row drive via `IOSET1`/`IOCLR1` (not IOPIN direct write — avoids mixed I/O port issues)
- `keyInput()`: multi-digit entry with backspace (`-` key) and cancel (`c` key)
- `passInput()`: masked entry (shows `*` instead of digits)
- `menu_sel()`: single-digit menu selection
- Input validation for time/date ranges

### Timer0 ISR (`timer_delay.c`)
- Configured for 1ms interrupt (PCLK=15MHz, PR=14999, MR0=1)
- `ms_tick` counter for general timing reference
- `refreshSeg()` called every 5ms — non-blocking 7-seg multiplexing
- VIC slot 0, channel 4

### Interrupt Controller (`interrupt.c`)
- EINT0: falling-edge, 20ms software debounce, sets `menu_flag`
- EINT1: falling-edge, 20ms software debounce, sets `pause_flag`
- VIC slots 1 and 2 respectively (slot 0 reserved for Timer0)
- Both flags declared `volatile` for compiler-optimization safety

### RTC Driver (`rtc.c`)
- LPC2148: external 32.768KHz crystal (CCR bit4 CLKSRC=1)
- LPC2129: internal PCLK prescaler (PREINT=456, PREFRAC=25024 at 15MHz PCLK)
- Functions: `setRtcTime()`, `setRtcDate()`, `getRtcTime()`, `dispRtcTime()`
- All pointer parameters declared `volatile u32*` to match global variable types

### ADC Driver (`adc.c`)
- 10-bit successive approximation ADC
- LM35 on CH1 (P0.28/AD0.1), PINSEL1 configured accordingly
- Conversion formula: `voltage = (3.3/1024) × digital_value`, `temp_C = voltage × 100`
- Polling-based conversion with DONE_BIT check

---

## 🔌 VIC Interrupt Allocation

| Slot | Channel | Source | ISR |
|---|---|---|---|
| 0 | 4 | Timer0 | `timer0_isr()` — 7-seg refresh every 5ms |
| 1 | 14 | EINT0 | `eint0_isr()` — config mode trigger |
| 2 | 15 | EINT1 | `eint1_isr()` — pause/resume toggle |

---

## 🚀 Build & Flash

### Requirements
- **Keil MDK** (µVision 4 or 5)
- **Flash Magic** (for flashing via ISP/UART)
- **USB-UART converter** or DB-9 cable

### Build Steps
1. Open `mini_project.uvproj` in Keil µVision
2. Select target (LPC2148 or LPC2129)
3. Build → `Project → Build Target` (F7)
4. Output: `Objects/mini_project.hex`

### Flash Steps (LPC2148 Kit)
1. Set ISP switch to LOAD position on Vector board
2. Press RESET
3. Open Flash Magic → select COM port, baud 115200, LPC2148
4. Browse to `.hex` file → Start
5. Set ISP switch back to EXE position
6. Press RESET — system boots

---

## 📋 Configuration Menu Flow

```
Power ON → "SYSTEM READY" → Default Screen (TIME / DATE / TEMP)
│
├── Press EINT0 → "MENU MODE" → Enter Password (4 digits)
│   │
│   ├── Wrong Password → "WRONG PASSWORD" → Return to idle
│   │
│   └── Correct Password → "AUTHENTICATION SUCCESSFUL"
│       │
│       ├── 1. RTC_EDIT
│       │   ├── 1. EDIT TIME → Enter HOUR (0-23) → MINUTES (0-59)
│       │   ├── 2. EDIT DATE → DAY (1-31) → MONTH (1-12) → YEAR (4 digits)
│       │   └── 3. EXIT → back to main menu
│       │
│       ├── 2. SET_EXAM_TIME
│       │   ├── 1. EXAM_START_TIME → HR (0-23) → MIN (0-59)
│       │   ├── 2. EXAM_DURATION → minutes (0-99)
│       │   └── 3. EXIT → back to main menu
│       │
│       ├── 3. EDIT_PASSWORD
│       │   └── Enter current → Enter new → "PASSWORD CHANGED"
│       │       (3 wrong attempts → locked out)
│       │
│       └── 4. EXIT → Return to idle screen
│
└── Exam auto-starts when RTC time == configured start time
    │
    ├── LCD: current time + temperature + remaining minutes
    ├── 7-seg: countdown in minutes (MM)
    ├── Green LED: > 3 minutes remaining
    ├── Yellow LED: 1–3 minutes remaining
    ├── Red LED: < 1 minute remaining
    │
    ├── Press EINT1 → "PAUSED" (countdown freezes, RTC continues)
    │   └── Press EINT1 again → "RESUMED" (1 sec) → countdown continues
    │
    └── Countdown reaches 0 → "EXAM OVER" + Buzzer (5 sec) → Return to idle
```

---

## 🐛 Key Debugging Lessons

During development, several important embedded programming issues were encountered and resolved:

| Issue | Root Cause | Fix |
|---|---|---|
| RTC appearing slow | Proteus simulation runs slower than real-time | Test on real hardware — RTC was accurate |
| 7-seg flickering | Blocking `delay_ms()` in main loop starving refresh | Moved to Timer0 ISR-driven `refreshSeg()` |
| LCD garbage text | LCD write called every loop pass (thousands/sec) | Moved to `if(sec != last_sec)` — once per second |
| Interrupt not re-triggering | `pause_flag`/`menu_flag` not declared `volatile` | Added `volatile` keyword |
| ISR corrupting VIC | `VICVectAddr0 = 0` instead of `VICVectAddr = 0` | Corrected register name (no suffix) |
| Switch bounce | Mechanical switch oscillates on press | 20ms software debounce in ISR |
| ADC reading wrong | `CH0` selected but LM35 on `CH1` (P0.28) | Changed to `CH1` in `lm35.c` |
| Cursor position wrong | `dispRtcTime()` leaves cursor in row1 | Added `cmdLcd(GOTO_LINE2_POS0)` before next write |
| PINSEL overwriting | `PINSEL0 = value` clearing other pin configs | Changed to `PINSEL0 |= value` |

---

## 👨‍💻 Author

**Tumma Anil Sri Sai**  
B.Tech Electronics and Communication Engineering  
Sasi Institute of Technology and Engineering, Tadepalligudem (2026)

**Training:** 6-month Embedded Systems Industry Program — Vector India, Hyderabad  
**Progression:** C Programming → ARM7 Architecture → DSA → Project

**Connect:**  
[![LinkedIn](https://img.shields.io/badge/LinkedIn-Connect-blue)](https://linkedin.com/in/)
[![GitHub](https://img.shields.io/badge/GitHub-Follow-black)](https://github.com/anilss1812)

---

## 📄 License

This project is open-source under the MIT License. Feel free to use, modify, and distribute with attribution.

---

*Developed during Vector India Embedded Systems Training Program, 2025–2026*
