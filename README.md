# Smart Exam Hall Monitoring and Management System

> Bare-metal embedded firmware on ARM7TDMI-S (LPC2148 / LPC2129)  
> Register-level drivers · No HAL · No RTOS · VIC interrupt architecture

---

## What this does

Automates examination timing and hall management. The system starts and ends the exam automatically based on a pre-configured time, counts down the remaining duration on a 7-segment display, monitors room temperature, and provides LED + buzzer alerts — all without any manual intervention from the invigilator during the exam itself.

---

## Platform

| | |
|---|---|
| MCU | NXP LPC2148 (ARM7TDMI-S, 60 MHz, LQFP64) |
| Dev board | Vector India Advanced Development Board |
| Simulation | LPC2124/LPC2129 on Proteus ISIS |
| IDE | Keil MDK (µVision) |
| Flash tool | Flash Magic (ISP via UART) |
| Language | Embedded C — bare-metal, register-level |

---

## Block Diagram

```
┌─────────────────────────────────────────────────────────────────────┐
│                        INPUTS                                       │
│                                                                     │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────┐              │
│  │  4×4 Keypad │    │EINT0 (Cfg SW)│    │EINT1 (Pause)│             │
│  │ P1.16–P1.23 │    │    P0.1     │    │    P0.3     │              │
│  └──────┬──────┘    └──────┬──────┘    └──────┬──────┘              │
│         │                  │                   │                    │
│  ┌──────┴──────┐    ┌──────┴──────┐           │                     │
│  │  LM35 Sensor│    │ RTC Crystal │           │                     │
│  │ P0.28(AD0.1)│    │  32.768 KHz │           │                     │
│  └──────┬──────┘    └──────┬──────┘           │                     │
└─────────┼─────────────────┼──────────────────┼────────────────────--┘
          │                 │                  │
          ▼                 ▼                  ▼
┌─────────────────────────────────────────────────────────────────────┐
│                                                                     │
│                        LPC2148                                      │
│                    ARM7TDMI-S 60MHz                                 │
│                                                                     │
│   ┌───────────────────┐    ┌────────────────────────────────────┐   │
│   │   Timer0 ISR      │    │         VIC                        │   │
│   │  1ms tick         │    │  Slot0: Timer0 (ch4)               │   │
│   │  refreshSeg() 5ms │    │  Slot1: EINT0  (ch14)              │   │
│   └───────────────────┘    │  Slot2: EINT1  (ch15)              │   │
│                             └────────────────────────────────────┘  │
│   ┌───────────────────────────────────────────────────────────────┐ │
│   │              State Machine                                    │ │
│   │  IDLE → CONFIG → IDLE → EXAM_RUNNING → EXAM_END → IDLE        │ │
│   └───────────────────────────────────────────────────────────────┘ │
│                                                                     │
└──────────────────────────────┬──────────────────────────────────────┘
                               │
          ┌────────────────────┼────────────────────┐
          ▼                    ▼                    ▼
┌─────────────────────────────────────────────────────────────────────┐
│                        OUTPUTS                                      │
│                                                                     │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐             │
│  │ 20×4 LCD │  │  7-Seg   │  │Green LED │  │Yellow LED│             │
│  │P0.8–P0.17│  │P0.18–P1.25│ │  P1.26   │  │  P1.27   │             │
│  └──────────┘  └──────────┘  └──────────┘  └──────────┘             │
│                                                                     │
│  ┌──────────┐  ┌──────────┐                                         │
│  │  Red LED │  │  Buzzer  │                                         │
│  │  P1.28   │  │  P1.29   │                                         │
│  └──────────┘  └──────────┘                                         │
└─────────────────────────────────────────────────────────────────────┘
```

---

## System Flow

```
Power ON
    │
    ▼
┌─────────────────────────────┐
│         IDLE MODE            │
│  LCD: TIME / DATE / TEMP     │◄─────────────────────────┐
│  7-seg: 00                   │                          │
│  Monitors exam start time    │                          │
└──────────┬───────────────────┘                          │
           │                                              │
     ┌─────┴─────┐                                        │
     │           │                                        │
  EINT0       RTC matches                                 │
  press       start time                                  │
     │           │                                        │
     ▼           ▼                                        │
┌──────────┐  ┌────────────────────────────────────┐      │
│  CONFIG  │  │         EXAM RUNNING                │     │
│  MODE    │  │  LCD: time + temp + remaining       │     │
│          │  │  7-seg: countdown in minutes        │     │
│ Password │  │  Green LED  >3 min remaining        │     │
│ RTC edit │  │  Yellow LED  1-3 min remaining      │     │
│ Exam time│  │  Red LED    <1 min remaining        │     │
│ Duration │  │                                     │     │
│ Password │  │  EINT1 press → PAUSED               │     │
│ change   │  │  EINT1 press → RESUMED              │     │
└────┬─────┘  └──────────────┬─────────────────────┘      │
     │                       │ countdown = 0              │
     │ exit                  ▼                            │
     │              ┌─────────────────┐                   │
     └─────────────►│   EXAM ENDED    │───────────────────┘
                    │  Buzzer 5 sec   │  EINT0 press
                    │  Red LED ON     │  to acknowledge
                    │  EXAM OVER msg  │
                    └─────────────────┘
```

---

## Configuration Menu Tree

```
Press EINT0 → Enter Password (4 digits, masked)
│
├── Wrong password → "WRONG PASSWORD" → back to IDLE
│
└── Correct password → "AUTHENTICATION SUCCESSFUL"
    │
    ├── 1. RTC_EDIT
    │   ├── 1. EDIT TIME
    │   │   ├── Enter HOUR  (0–23, validated)
    │   │   └── Enter MIN   (0–59, validated) → setRtcTime()
    │   ├── 2. EDIT DATE
    │   │   ├── Enter DAY   (1–31, validated)
    │   │   ├── Enter MONTH (1–12, validated)
    │   │   └── Enter YEAR  (4 digits) → setRtcDate()
    │   └── 3. EXIT → main menu
    │
    ├── 2. SET_EXAM_TIME
    │   ├── 1. EXAM START TIME
    │   │   ├── Enter hour  (0–23, validated)
    │   │   └── Enter min   (0–59, validated)
    │   ├── 2. EXAM DURATION
    │   │   └── Enter minutes (0–99)
    │   └── 3. EXIT → main menu
    │
    ├── 3. EDIT PASSWORD
    │   ├── Enter current password
    │   ├── If correct → enter new password → "PASSWORD CHANGED"
    │   └── If wrong × 3 → "SO MANY TIMES" → back to main menu
    │
    └── 4. EXIT → back to IDLE
```

---

## Features

- RTC-based automatic exam start — transitions to exam mode when RTC matches configured start time
- Minute-by-minute countdown on dual multiplexed 7-segment display (00–99 min)
- Live temperature readout via LM35 on 20×4 LCD throughout exam
- Three-level LED alert system: Green (>3 min) → Yellow (1–3 min) → Red (<1 min)
- Buzzer fires for 5 seconds at exam end
- Emergency pause/resume via EINT1 — first press freezes countdown, second resumes
- Password-protected 4-digit keypad menu with masked input (shows `*`)
- Input validation for all time/date fields
- Admin password change with 3-attempt lockout
- Timer0 ISR drives 7-segment refresh every 5 ms — fully non-blocking

---

## Pin Mapping

### LPC2148 (hardware kit)

| Component | Pins | Notes |
|---|---|---|
| LCD data D0–D7 | P0.8 – P0.15 | 8-bit mode |
| LCD RS / EN | P0.16 / P0.17 | |
| Keypad rows | P1.16 – P1.19 | output |
| Keypad cols | P1.20 – P1.23 | input + pullups |
| 7-seg segments | P0.18 – P0.25 | A–G+DP |
| 7-seg DSEL1 / DSEL2 | P1.24 / P1.25 | active high digit select |
| LM35 | P0.28 (AD0.1) | ADC channel 1 |
| Green LED | P1.26 | active low |
| Yellow LED | P1.27 | active low |
| Red LED | P1.28 | active low |
| Buzzer | P1.29 | active high via BC109 |
| EINT0 config switch | P0.16 | hardwired on board |
| EINT1 pause switch | P0.14 | hardwired on board |

### LPC2129 (Proteus simulation)

Same as above except EINT pins: P0.1 (EINT0) and P0.3 (EINT1).  
Handled via `#define CPU LPC2129 / LPC2148` conditional in `mini_pro_defines.h`.

---

## VIC Interrupt Allocation

| Slot | Channel | Source | ISR | Trigger |
|---|---|---|---|---|
| 0 | 4 | Timer0 | `timer0_isr()` | every 1 ms |
| 1 | 14 | EINT0 | `eint0_isr()` | falling edge, P0.1/P0.16 |
| 2 | 15 | EINT1 | `eint1_isr()` | falling edge, P0.3/P0.14 |

Both external interrupts use 20 ms software debounce with pin re-verification inside the ISR.

---

## File Structure

```
Smart_Exam_Hall_Monitoring_and_Management_System/
│
├── README.md
│
├── src/
│   ├── mini_project_main.c      ← main application, state machine
│   ├── mini_pro_defines.h       ← all pin/peripheral constants
│   ├── headers.h                ← master include file
│   │
│   ├── lcd.c                    ← HD44780 driver (8-bit, register-level)
│   ├── lcd.h
│   │
│   ├── kpm.c                    ← 4×4 keypad (row scan, backspace, masked)
│   ├── kpm.h
│   │
│   ├── seg.c                    ← dual 7-seg, Timer0 ISR refresh
│   ├── seg.h
│   │
│   ├── rtc.c                    ← RTC driver (CCR, PREINT, PREFRAC)
│   ├── rtc.h
│   │
│   ├── adc.c                    ← 10-bit ADC, polling mode
│   ├── adc.h
│   │
│   ├── lm35.c                   ← LM35 voltage→temperature conversion
│   ├── lm35.h
│   │
│   ├── interrupt.c              ← EINT0/EINT1 VIC setup + ISRs
│   ├── interrupt.h
│   │
│   ├── timer_delay.c            ← Timer0 ISR, ms_tick, refreshSeg()
│   ├── timer_delay.h
│   │
│   ├── delay.c                  ← software delay utilities
│   └── delay.h
│
├── simulation/
│   └── mini_project.pdsprj      ← Proteus simulation file
│
└── docs/
    ├── project_spec.pdf         ← project specification document
    └── board_manual.pdf         ← Vector India LPC2148 board manual
```

---

## How to Use These Files

### Step 1 — Clone the repository

```bash
git clone https://github.com/anilss1812/Smart_Exam_Hall_Monitoring_and_Management_System.git
cd Smart_Exam_Hall_Monitoring_and_Management_System
```

### Step 2 — Open in Keil MDK

1. Open **Keil µVision** (v4 or v5)
2. Go to `Project → Open Project`
3. Select `mini_project.uvproj`
4. In the **Project** panel on the left, confirm all `.c` files are listed under Source Group 1:
   - `mini_project_main.c`
   - `lcd.c`, `kpm.c`, `seg.c`, `rtc.c`, `adc.c`, `lm35.c`
   - `interrupt.c`, `timer_delay.c`, `delay.c`

### Step 3 — Select target device

- Go to `Project → Options for Target`
- Under **Device** tab → select **NXP LPC2148** (or LPC2129 for simulation)
- Under **Target** tab → set Crystal frequency to **12 MHz**
- Under **C/C++** tab → add these defines if not present:
  ```
  CPU=LPC2148
  ```

### Step 4 — Build

Press **F7** or go to `Project → Build Target`.  
Expected output:
```
Build Output:
linking...
Program Size: Code=XXXX RO-data=XX RW-data=XX ZI-data=XXXX
".\Objects\mini_project.axf" - 0 Error(s), 0 Warning(s).
```
The `.hex` file will appear in `Objects/mini_project.hex`.

### Step 5 — Flash to hardware (LPC2148 kit)

**Hardware setup:**
1. Connect USB-UART converter: TX→RXD0 (P0.1), RX→TXD0 (P0.0), GND→GND
2. Set **ISP switch** on the Vector board to **LOAD** position
3. Press the **RESET** button

**Flash Magic setup:**
1. Open Flash Magic
2. Select COM port (check Device Manager for your USB-UART COM port)
3. Baud rate: **115200**
4. Device: **LPC2148**
5. Interface: **None (ISP)**
6. Oscillator frequency: **12 MHz**
7. Browse to `Objects/mini_project.hex`
8. Click **Start**

**After flashing:**
1. Set ISP switch back to **EXE** position
2. Press **RESET**
3. LCD shows `EXAM HALL / MONITORING SYSTEM` splash screen for 1 second
4. Then switches to default screen showing TIME / DATE / TEMP

### Step 6 — Run in Proteus simulation

1. Open `simulation/mini_project.pdsprj` in **Proteus ISIS**
2. Make sure the `.hex` file path in the LPC2124 component properties points to your built `mini_project.hex`
3. Right-click LPC2124 component → **Edit Properties** → set `Program File` to your `.hex`
4. Click **Run** (green play button)

> **Note:** Proteus simulation runs slower than real hardware. RTC ticks appearing slow is a simulator limitation, not a code bug. Test timing-critical behavior on real hardware.

### Step 7 — Operating the system

**On first power-up:**
- System displays splash screen then shows TIME / DATE / TEMP on the 20×4 LCD
- Default RTC time is set to 10:10:00 in code — update via CONFIG mode

**To configure the exam:**
1. Press **SW1** (EINT0) → enter password (default: **1234**)
2. Select `2. SET_EXAM_TIME`
3. Enter exam start hour and minute
4. Enter exam duration in minutes
5. Select `4. EXIT` to return to idle
6. The system will auto-start the exam when RTC reaches the configured time

**During the exam:**
- LCD row 1: current RTC time
- LCD row 2: remaining minutes (REMAINING: XX)
- LCD row 3: current temperature
- 7-seg: remaining minutes as two-digit countdown
- Press **SW2** (EINT1) to pause — "PAUSED" appears on LCD
- Press **SW2** again to resume — "RESUMED" appears for 1 second

**At exam end:**
- Buzzer sounds for 5 seconds
- LCD shows "EXAM OVER / TIME IS UP"
- Red LED turns ON
- Press **SW1** (EINT0) to acknowledge and return to idle

---

## Keypad Layout

```
┌────┬────┬────┬────┐
│ 1  │ 2  │ 3  │ A  │
├────┼────┼────┼────┤
│ 4  │ 5  │ 6  │ B  │
├────┼────┼────┼────┤
│ 7  │ 8  │ 9  │ C  │  
├────┼────┼────┼────┤
│ *  │ 0  │ #  │ D  │  ← D = backspace , * = cancel/exit, # = confirm
└────┴────┴────┴────┘
```

| Key | Function |
|---|---|
| `0`–`9` | digit input |
| `#` | confirm entry (only accepted after all required digits are entered) |
| `D` | backspace — deletes last entered digit |
| `*` | cancel — exits current input, returns to previous menu |
| `1`–`4` | menu selection in config menus |

---

## Key Bugs Resolved

Worth knowing for anyone working on similar bare-metal ARM7 projects:

| Issue | Root cause | Fix |
|---|---|---|
| 7-seg flickering | Blocking `delay_ms()` in main loop starved refresh | Timer0 ISR — `refreshSeg()` every 5 ms |
| LCD garbage text | LCD writes called every loop iteration | Gated inside `if(sec != last_sec)` |
| Interrupts not re-triggering | Flags missing `volatile` keyword | Added `volatile u8 menu_flag, pause_flag` |
| VIC corruption after first ISR | `VICVectAddr0 = 0` instead of `VICVectAddr = 0` | Corrected — no numeric suffix on the end-of-interrupt register |
| Pause toggling instantly | `pause_flag` set again during `delay_ms(1000)` | Added `pause_flag = 0` after each delay in pause/resume block |
| ADC reading ~350°C | `CH0` (P0.27) selected but LM35 on `CH1` (P0.28) | Changed to `CH1` in `lm35.c` |
| `=` accepted without full input | `=` not gated by digit-count check | Added `if(cnt == nodigit)` guard |
| Switch bounce on hardware | Mechanical contact oscillation | 20 ms ISR debounce + pin re-verify |
| PINSEL clearing other pins | `PINSEL0 = value` overwrote entire register | Changed to `PINSEL0 |= value` |

---

## About

**Tumma Anil Sri Sai**  
B.Tech ECE — Sasi Institute of Technology and Engineering, Tadepalligudem (2026)  
6-month embedded systems industry training — Vector India, Hyderabad  
Training track: C → ARM7 architecture → DSA → C++ → Linux internals → TCP/IP

## 👨‍💻 Author

**Tumma Anil Sri Sai**  
B.Tech Electronics and Communication Engineering  

**Training:** 6-month Embedded Systems Industry Program — Vector India, Hyderabad  

**Connect:**  
[![LinkedIn](https://img.shields.io/badge/LinkedIn-Connect-blue)](https://linkedin.com/in/tumma-anil-sri-sai)
[![GitHub](https://img.shields.io/badge/GitHub-Follow-black)](https://github.com/anilss1812)

---

*Developed during Vector India Embedded Systems Training Program, 2026*
