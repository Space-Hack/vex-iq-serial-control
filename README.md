# VEX IQ2 Serial Control Clawbot

This project implements serial control for a VEX IQ2 Clawbot robot. It allows for remote control of the robot's movements and provides real-time sensor data feedback.

## Hardware Configuration

- **Drive Motors:**
  - Left Motor: Port 12
  - Right Motor: Port 7
- **Arm Motor:** Port 4
- **Claw Motor:** Port 9
- **Sensors:**
  - Distance Sensor: Port 1
  - Bumper: Port 11
  - Inertial Sensor: Built-in

## Features

- Serial communication interface for remote control
- Real-time sensor data output in JSON format
- Precise motor control for:
  - Forward/backward movement
  - Turning
  - Arm height adjustment
  - Claw width control
- Sensor monitoring:
  - Distance measurements
  - Bumper state
  - Gyroscopic heading

## Serial Communication

The robot accepts commands in the format `COMMAND:VALUE` where:
- `COMMAND` can be:
  - `FORWARD`: Move forward
  - `BACKWARD`: Move backward
  - `TURN`: Rotate robot
  - `GRABBAR_HEIGHT`: Adjust arm height
  - `GRABBER_WIDTH`: Control claw width
- `VALUE` is a floating-point number specifying the magnitude of the action

## Sensor Data Output

The robot outputs sensor data in JSON format:
```json
{
  "object_detected": boolean,
  "distance": number,
  "bumper_pressed": boolean,
  "heading": number
}
```

## Building and Running

1. Compile the project using the VEX V5 IDE
2. Upload to the VEX IQ2 brain
3. Connect to the robot via serial port
4. Send commands in the specified format

## Dependencies

- VEX IQ2 Brain
- VEX V5 IDE
- Serial communication library 