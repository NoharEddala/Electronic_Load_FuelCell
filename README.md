# ⚡ Electronic Load for Fuel Cell Characterization

**Goal:**  
To design and develop an electronic load with integrated digital control for real-time testing of automotive fuel cell stacks up to 160 A.

---

## 🔧 Overview
This project was developed as part of my Master’s Thesis at **Hochschule Stralsund**.  
It provides a programmable electronic load system capable of evaluating hydrogen fuel cell stacks under various operating conditions.

The setup integrates:
- Analog & digital control using ADC/DAC and PWM modules  
- ESP32 microcontroller for measurement and control  
- Real-time monitoring of stack performance

---

## 🧩 System Architecture
- **Hardware:** Multi-layer PCB designed in *Altium 365*, ensuring EMC compliance and signal integrity  
- **Firmware:** Written in *C/C++* for the ESP32, handling sensor input and digital load control  
- **Communication:** I²C / SPI interfaces for peripheral integration  
- **Measurement:** Voltage, current, and temperature monitoring for fuel cell safety  

![System Diagram](Images/fuelcell_setup.jpg)  
*(Example: Replace with your own photo or diagram)*

---

## ⚙️ Key Features
- Current regulation up to **160 A** with high stability  
- Closed-loop control using PWM and ADC feedback  
- Modular design for easy adaptation to different load ranges  
- Full documentation and test data included  

---

## 🛠 Tools & Technologies
| Category | Tools |
|-----------|--------|
| PCB Design | Altium 365 |
| Simulation | NI Multisim, MATLAB/Simulink |
| Firmware | ESP32 (C/C++) |
| Testing | Oscilloscope, Logic Analyzer |

---

## 📁 Repository Structure

Hardware/ → schematics, PCB exports, BOM
Firmware/ → ESP32 source code and libraries
Docs/ → thesis report, user manual, and test data
Images/ → setup photos, measurement graphs


---

## 👨‍🔬 My Contribution
I was responsible for **end-to-end development**:
- Designed analog and digital hardware circuits  
- Developed firmware for current control and data acquisition  
- Created PCB layout and validated EMC performance  
- Documented and tested the final product  

---

## 📈 Results
- Achieved stable dynamic current response under varied load conditions  
- Verified accuracy of current measurement within ±2%  
- Integrated safety cutoff and thermal monitoring  

---

## 🪪 License
MIT License — feel free to reference or adapt for educational purposes.  
<img width="676" height="343" alt="Picture1" src="https://github.com/user-attachments/assets/5c8ff475-b4be-483b-b618-ed364c6df6b9" />

---

*© 2024 Nohar Chandrakanth Eddala — Developed as part of Master’s Thesis in Renewable Energy & E-Mobility at Hochschule Stralsund.*



