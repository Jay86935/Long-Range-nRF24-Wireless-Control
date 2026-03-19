# Long-Range Wireless Control System (~1 km)

This project implements a **long-range wireless transmitter and receiver system** using **nRF24L01 RF modules and Arduino**. The system enables reliable real-time transmission of control signals over long distances and can be used for **robotic vehicles, RC systems, drones, or industrial wireless control applications**.

## Features

* Long-range wireless communication (~1 km line-of-sight)
* 5-channel control system for multiple actuators
* Low-latency packet-based RF communication
* Failsafe mechanism to reset outputs if signal is lost
* Standard **1000–2000 µs PWM output** compatible with servos and ESCs

## Hardware Components

* Arduino (Transmitter & Receiver)
* nRF24L01 RF Modules
* Joystick / Potentiometers (for control inputs)
* Servo motors or ESCs (for output testing)
* Power supply and supporting electronics

## Working Principle

The transmitter reads analog input values from control devices (joysticks/potentiometers) and converts them into digital packets. These packets are transmitted via the **nRF24L01 RF module** using SPI communication.

The receiver module continuously listens for incoming packets and converts the received data into **PWM signals**, which can directly control servos, ESCs, or robotic actuators.

To ensure safety, a **failsafe mechanism** resets the control values if communication is lost for a specific duration.

## Applications

* Wireless robotic vehicle control
* Drone / RC system communication
* Industrial wireless control systems
* Remote actuator control

## Technologies Used

* Arduino (Embedded C/C++)
* nRF24L01 RF Communication
* SPI Communication Protocol
* Servo PWM Control
