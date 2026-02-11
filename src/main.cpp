#define SIMULATION_MODE   // Comment this out for hardware mode

#include <Arduino.h>

#ifdef SIMULATION_MODE

// ---------------- SIMULATION CODE ----------------

enum State { IDLE, ARMED, BOOST, APOGEE, DESCENT, LANDED };

State currentState = IDLE;

float altitude = 0;
float previousAltitude = 0;
unsigned long startTime;

float simulateAltitude() {
    float t = (millis() - startTime) / 1000.0;

    if (t < 3) return t * 20;
    if (t < 6) return 60 - (t - 3) * 20;
    return 0;
}

void setup() {
    Serial.begin(115200);
    startTime = millis();
}

void loop() {
    previousAltitude = altitude;
    altitude = simulateAltitude();

    Serial.println(altitude);
    delay(100);
}

#else

// ---------------- HARDWARE CODE ----------------

#include <DHT.h>
#include <Servo.h>

#define DHTPIN 8
#define DHTTYPE DHT11

const int trigPin = 2;
const int echoPin = 3;

DHT dht(DHTPIN, DHTTYPE);

float readUltrasonic() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000);
    return duration * 0.0343 / 2.0;
}

void setup() {
    Serial.begin(115200);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    dht.begin();
}

void loop() {
    float altitude = readUltrasonic();
    float temperature = dht.readTemperature();

    Serial.print(altitude);
    Serial.print(",");
    Serial.println(temperature);

    delay(100);
}

#endif
