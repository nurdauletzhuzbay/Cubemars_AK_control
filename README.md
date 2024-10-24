# CubeMars AK80-9 Motor Control with Arduino (ESP32) and MCP2515 CAN Module

## Overview

Code to control the CubeMars AK80-9 motor using an Arduino (ESP32) and the MCP2515 CAN module. The motor supports various control modes such as position control, velocity control, and position-velocity hybrid control. The code also includes functions to read motor parameters like position, speed, current, temperature, and error codes via CAN communication.

## Equipment
- **CubeMars AK80-9 motor** (30-45V power input)
- **Arduino or ESP32** (controller)
- **MCP2515 CAN module** (to handle CAN communication)
- **Power supply** (30-45V for motor)
- CAN bus connection cables
- SPI cables for connection between ESP32 and MCP2515 module

## Wiring

### MCP2515 Module to Arduino Nano Pin Connections
| MCP2515 Pin | Arduino Pin |
|-------------|-------------|
| CS          | GPIO 10     |
| SO          | GPIO 12     |
| SI          | GPIO 11     |
| SCK         | GPIO 13     |

### Motor to Power Supply
- Connect the CubeMars AK80-9 motor to the 30-45V power supply.

### CAN Bus Wiring
- Connect the CAN High (CANH) and CAN Low (CANL) lines between the motor and MCP2515 module.

## Software Setup

### Library Requirements
Install the following libraries in your Arduino IDE:
[mcp2515 library](https://github.com/autowp/arduino-mcp2515) - For controlling the MCP2515 CAN module.

### Motor Control Functions

The motor supports several control modes via CAN commands. Below are the main functions used for controlling the motor.

#### 1. **Position Control** (`comm_can_set_pos()`)
This function sets the motor to position control mode. You specify a controller id and target position, and the motor will rotate to that position.

#### 2. **Velocity Control** (`comm_can_set_spd()`)
This function sets the motor to velocity control mode. You specify a controller id and target speed, and the motor will attempt to maintain that speed.

#### 3. **Position-Velocity Control** (`comm_can_set_pos_spd()`)
This function combines position and velocity control, allowing you to set both a target position and a maximum velocity for reaching that position.

#### 4. **Reading Motor Parameters** (`unpack_servo()`)
This function is used to read motor parameters such as:
- Position
- Speed
- Current
- Temperature
- Error codes

The motor sends these parameters back via CAN, and `unpack_servo()` extracts this data for monitoring purposes.
