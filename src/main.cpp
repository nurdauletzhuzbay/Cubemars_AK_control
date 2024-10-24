#include <Arduino.h>
#include <mcp2515.h>
#include <SPI.h>
#include <CubemarsAK.h>

CubemarsAK motor1(10); // Instantiate motor1 with chip select pin 10
char receivedChars[32]; // Buffer for input
char currentMode;       // Variable to hold the current mode
float value;           // Variable to hold the parsed value

void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for serial to be ready

    motor1.initializeCAN(); // Initialize CAN in CubemarsAK class
}

void loop() {
    // Unpack the servo readings
    motor1.unpackServo();

    // Start rotating randomly for 10 minutes
    static unsigned long startTime = 0; // Record start time
    static bool running = false; // Flag to keep track of whether we're running

    // Start the motor and set the timer if not already running
    if (!running) {
        Serial.println("Starting random rotation for 10 minutes.");
        startTime = millis(); // Record the start time
        running = true; // Set the running flag
    }

    // Check if 10 minutes have passed
    if (running) {
        if (millis() - startTime < 10 * 60 * 1000) { // 10 minutes in milliseconds
            // Generate random position and speed
            float randomPosition = random(-360, 360); // Random position between 0 and 180 degrees
            int randomSpeed = random(1000, 5000); // Random speed between 100 and 1000 RPM
            int randomAcceleration = random(1000, 2000); // Random acceleration

            // Set position-velocity mode
            Serial.print("Setting Position to: ");
            Serial.println(randomPosition);
            Serial.print("Setting Speed to: ");
            Serial.println(randomSpeed);

            motor1.comm_can_set_pos_spd(105, randomPosition, randomSpeed, randomAcceleration); // Set position-velocity
            
            // Optional: Add a delay to allow the motor to reach the position before changing again
            delay(1000); // Adjust this delay as necessary to allow for movement
        } else {
            Serial.println("10 minutes elapsed. Stopping motor.");
            motor1.comm_can_set_duty(105, 0); // Stop the motor
            running = false; // Update the flag to stop running
        }
    }

    // Optionally print the current position
    Serial.print("Temp: ");
    Serial.println(motor1.getMotorTemp());
}

