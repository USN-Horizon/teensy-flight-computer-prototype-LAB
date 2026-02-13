#define SIMULATION_MODE   // Comment this out for hardware mode

#include <Arduino.h>
#include <SD.h>

// ---------------- STATE MACHINE ----------------

enum State { IDLE, ARMED, BOOST, APOGEE, DESCENT, LANDED };

State currentState = IDLE;

float altitude = 0;
float previousAltitude = 0;
float temperature = 0;

unsigned long startTime;

File logFile;

// ---------------- FSM LOGIC ----------------

void updateState() {

    switch (currentState) {

        case IDLE:
            if (altitude > 5)
                currentState = ARMED;
            break;

        case ARMED:
            if (altitude > previousAltitude)
                currentState = BOOST;
            break;

        case BOOST:
            if (altitude < previousAltitude)
                currentState = APOGEE;
            break;

        case APOGEE:
            currentState = DESCENT;
            break;

        case DESCENT:
            if (altitude <= 0)
                currentState = LANDED;
            break;

        case LANDED:
            break;
    }
}

// ---------------- TELEMETRY AND SD LOGGING ----------------

void sendTelemetry() {

    float timeSec = millis() / 1000.0;

    Serial.print(timeSec);
    Serial.print(",");
    Serial.print(altitude);
    Serial.print(",");
    Serial.print(temperature);
    Serial.print(",");
    Serial.println(currentState);

    if (logFile) {
        logFile.print(timeSec);
        logFile.print(",");
        logFile.print(altitude);
        logFile.print(",");
        logFile.print(temperature);
        logFile.print(",");
        logFile.println(currentState);
        logFile.flush();
    }
}

#ifdef SIMULATION_MODE

// ---------------- SIMULATION MODE ----------------

float simulateAltitude() {

    float t = (millis() - startTime) / 1000.0;

    if (t < 3) return t * 20;
    if (t < 6) return 60 - (t - 3) * 20;
    return 0;
}

void setup() {

    Serial.begin(115200);
    startTime = millis();

    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("SD initialization failed!");
    } else {
        logFile = SD.open("flight.csv", FILE_WRITE);
        if (logFile) {
            logFile.println("time,altitude,temperature,state");
            logFile.flush();
        }
    }
}

void loop() {

    previousAltitude = altitude;
    altitude = simulateAltitude();
    temperature = 25.0;

    updateState();
    sendTelemetry();

    delay(100);
}

#else

// ---------------- HARDWARE MODE ----------------

#include <DHT.h>
#include <Servo.h>

#define DHTPIN 8
#define DHTTYPE DHT11

const int trigPin = 2;
const int echoPin = 3;
const int servoPin = 9;

DHT dht(DHTPIN, DHTTYPE);
Servo parachuteServo;

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

    parachuteServo.attach(servoPin);
    parachuteServo.write(0);

    dht.begin();

    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("SD initialization failed!");
    } else {
        logFile = SD.open("flight.csv", FILE_WRITE);
        if (logFile) {
            logFile.println("time,altitude,temperature,state");
            logFile.flush();
        }
    }
}

void loop() {

    previousAltitude = altitude;
    altitude = readUltrasonic();
    temperature = dht.readTemperature();

    updateState();
    sendTelemetry();

    delay(100);
}

#endif
