# Transmitter-Receiver-for-Drone
A DIY project using arduino nano to build a transmitter and receiver to control drones, rc planes etc. 
# 🚁 DIY 5-Channel Arduino RC Transmitter & Receiver

A fully functional **5-channel wireless RC transmitter and receiver** built using **Arduino Nano** and **NRF24L01** modules. This project was designed as a low-cost alternative to commercial RC systems such as the FlySky FS-i6S and is capable of controlling a flight controller through standard PWM outputs.

---

## ✨ Features

* 📡 Reliable NRF24L01 wireless communication
* 🎮 5 independent RC channels
* 🖥️ Live OLED display (SH1106 1.3")
* 📶 Real-time connection status
* 🎯 Smooth joystick input with center dead-zone
* 🔄 Channel reversing support
* 🛡️ Built-in failsafe on signal loss
* ⚡ Standard 1000–2000 µs PWM outputs
* 🚁 Compatible with KK2.1.5 and other PWM flight controllers

---

## 📋 Channels

| Channel | Function         |
| ------- | ---------------- |
| CH1     | Roll (Aileron)   |
| CH2     | Pitch (Elevator) |
| CH3     | Throttle         |
| CH4     | Yaw (Rudder)     |
| CH5     | AUX / Arm Switch |
| CH6     | AUX / Arm Switch |


---

## 🔧 Hardware Used

### Transmitter

* Arduino Nano
* NRF24L01
* 1.3" SH1106 OLED Display
* 2 × Analog Joysticks
* 2 × Toggle Switch (CH5, CH6)
* 3.3V supply for NRF24L01

### Receiver

* Arduino Nano
* NRF24L01
* 6 PWM Outputs
* KK2.1.5 Flight Controller

---

## 🖥️ Transmitter Functions

* Reads all joystick inputs
* Generates standard RC PWM channel values
* Sends all 5 channels wirelessly using NRF24L01
* Displays:

  * CH1
  * CH2
  * CH3
  * CH4
  * CH5
  * CH6
  * Connection Status

---

## 📡 Receiver Functions

* Receives wireless packets
* Validates incoming data
* Generates standard 1000–2000 µs PWM outputs
* Outputs all 5 channels to the flight controller
* Activates failsafe if communication is lost

---

## 🛡️ Failsafe

If the transmitter signal is lost, the receiver automatically switches to safe values:

* Roll → Center (1500 µs)
* Pitch → Center (1500 µs)
* Throttle → Minimum (1000 µs)
* Yaw → Center (1500 µs)
* AUX → OFF (1000 µs)

This helps prevent unexpected motor behavior during signal loss.

---

## 📌 Future Improvements

* OLED menu system
* Channel calibration
* Trim adjustment
* Dual Rates
* Exponential (Expo)
* Endpoint Adjustment (EPA)
* Model Memory
* Battery voltage monitoring
* Telemetry support

---

## 🎯 Project Goal

The objective of this project was to design and build a complete DIY RC radio system capable of replacing a commercial transmitter/receiver for educational purposes while remaining affordable, customizable, and open-source.

---

## 📄 License

This project is released under the MIT License.

Feel free to modify, improve, and use it in your own robotics and UAV projects.
