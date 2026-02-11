# teensy-flight-computer-prototype-LAB


This project is a prototype embedded flight computer built using a Teensy microcontroller and common Arduino components.

The goal is to simulate real rocket flight computer behavior including:

Altitude measurement

Flight state detection

Apogee detection

Parachute deployment simulation

Audio and LED feedback

Real-time telemetry to a Qt ground station


 System Architecture

Sensors → Flight Logic → Actuators → Telemetry

Components used:

Teensy (main controller)

HC-SR04 ultrasonic sensor (simulated altitude)

DHT temperature sensor

Servo motor (parachute deployment simulation)

Buzzer (flight event signaling)

LEDs (state indication)

 
  Flight States

The system implements a deterministic state machine:

IDLE

ARMED

BOOST

COAST

APOGEE

DESCENT

LANDED

Apogee is detected when altitude transitions from increasing to decreasing.

 
  Telemetry

Telemetry is transmitted over USB serial in structured format:

time, altitude, temperature, state


This data is received and visualized in a Qt ground station application.
