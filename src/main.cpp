/*----------------------------------------------------------------------------------------*/
/*                                                                                        */      
/*    Project:          VEX IQ2 Serial Control Clawbot                                    */
/*    Module:           main.cpp                                                          */
/*    Author:           VEX                                                               */
/*    Created:          Sat Apr 12 2025                                                   */
/*    Description:      Serial control implementation for VEX IQ2 Clawbot                 */
/*                      Features motor control and sensor data feedback via serial        */
/*                                                                                        */      
/*    Configuration:    Clawbot with Individual Motors                                    */
/*                      Left Motor: Port 12                                               */
/*                      Right Motor: Port 7                                               */
/*                      Claw Motor: Port 9                                                */
/*                      Arm Motor: Port 4                                                 */
/*                      Distance Sensor: Port 1                                           */
/*                      Bumper: Port 11                                                   */
/*                      Inertial Sensor: Built-in                                         */
/*                                                                                        */      
/*----------------------------------------------------------------------------------------*/

// Include the IQ Library
#include "vex.h"


// Allows for easier use of the VEX Library
using namespace vex;

// Brain should be defined by default
brain Brain;

// Robot configuration code.
inertial BrainInertial = inertial();
// controller Controller = controller();
motor ClawMotor = motor(PORT9, false);
motor ArmMotor = motor(PORT4, true);
motor LeftDriveSmart = motor(PORT12, 1, false);
motor RightDriveSmart = motor(PORT7, 1, true);
bumper Bumper = bumper(PORT11);

// proximity sensors
distance DistanceSensor = distance(PORT1);

void calibrateDrivetrain() {
  wait(200, msec);
  Brain.Screen.print("Calibrating");
  Brain.Screen.newLine();
  Brain.Screen.print("Inertial");
  BrainInertial.calibrate();
  while (BrainInertial.isCalibrating()) {
    wait(25, msec);
  }

  // Clears the screen and returns the cursor to row 1, column 1.
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
}

// Helper function to safely get integer part of a float
int getIntPart(float f) {
    // Handle potential NaN or Inf values if necessary, though unlikely for sensors
    if (isnan(f) || isinf(f)) return 0;
    return (int)f;
}

// Helper function to safely get fractional part * scale (e.g., scale=100 for 2 decimal places)
int getFracPart(float f, int scale) {
    if (isnan(f) || isinf(f)) return 0;
    // Use fabsf for float absolute value
    return (int)fabsf((f - getIntPart(f)) * (float)scale);
}

void printSensorDataAsJson() {

    // --- 1. Read Sensor Values ---
    // Assumes sensor objects (DistanceSensor, BumperA, etc.) are accessible globally

    bool dist_found = DistanceSensor.isObjectDetected();
    float dist_mm_float = DistanceSensor.objectDistance(mm);

    bool bumper_pressed = Bumper.pressing();

    float gyro_heading_float = BrainInertial.heading(degrees);

    // --- 2. Prepare Values for JSON (Convert Floats/Bools) ---

    // Distance (2 decimal places)
    int dist_mm_int = getIntPart(dist_mm_float);
    int dist_mm_frac = getFracPart(dist_mm_float, 100);

    // Gyro Heading (1 decimal place)
    int gyro_heading_int = getIntPart(gyro_heading_float);
    int gyro_heading_frac = getFracPart(gyro_heading_float, 10);

    // --- 3. Print JSON Object ---
    printf("{"); 
    printf("\"object_detected\": %s, ", dist_found ? "true" : "false"); 
    if (dist_found) { 
        printf("\"distance\": %d,\".\"%02d,", dist_mm_int, dist_mm_frac); 
    } else { 
        printf("\"distance\": null, "); 
    } 
    printf("\"bumper_pressed\": %s, ", bumper_pressed ? "true" : "false"); 
    printf("\"heading\": %d.%d", gyro_heading_int, gyro_heading_frac); 
    printf("}\n");
}

int main() {
    char inputBuffer[50]; // Buffer for received string
    int loopCounter = 0;

    float value = 0; // Variable to hold the float value from scanf

    Brain.Screen.setFont(monoM);
    // Brain.Screen.print("Polling Example (scanf blocks)");
    Brain.Screen.newLine();
    // printf("Polling Example (scanf still blocks)\n");
    // printf("-------------------------------------\n");
    // printf("Send 'forward' or 'stop' from Mac (with newline)\n");

    while (true) {
        // --- Part 1: Non-Blocking Robot Tasks ---
        // Code here runs on every loop iteration *before* checking serial.
        // Example: Read sensors, apply basic motor logic, update screen counter.
        loopCounter++;
        Brain.Screen.setCursor(3, 1); // Row 3, Col 1
        // Brain.Screen.clearLine(3); // Clear rest of the line

        // Example: Maybe check a bumper sensor
        // if (BumperA.pressing()) {
        //     // React to bumper press
        // }

        // printf("Loop %d - About to check for input...\n", loopCounter);

        // --- End Part 1 ---


        // --- Part 2: Attempting to Read Serial Input (BLOCKING) ---
        // THIS IS WHERE THE PROGRAM WILL PAUSE AND WAIT FOR INPUT
        // It's NOT true polling. We are just calling the blocking function.
        // We can't easily check *if* data is available first.

        // Get all the sensor data and send on serial por



        printSensorDataAsJson();

        // *** The program WILL BLOCK HERE until you send a string + newline from Mac ***
        int result_str = scanf("%s", inputBuffer); // Read string and float from input


        // Split on ":"
        char *command = strtok(inputBuffer, ":");
        char *param = strtok(NULL, ":");

        value = atof(param);

        if (result_str == 1) {
            // Input was received and successfully read as a string
            // printf("--> Received string: '%s'\n", inputBuffer);
            LeftDriveSmart.stop();
            RightDriveSmart.stop();
            LeftDriveSmart.setPosition(0, degrees);
            RightDriveSmart.setPosition(0, degrees);
            Brain.Screen.setCursor(5, 1);


            // Act on the received command
            if (strcmp(inputBuffer, "FORWARD") == 0) {
                Brain.Screen.print("Forward %f", value); ;
                LeftDriveSmart.spinToPosition(value * 18, degrees, false);
                RightDriveSmart.spinToPosition(value * 18, degrees, false);
            } else if (strcmp(inputBuffer, "BACKWARD") == 0) {
                Brain.Screen.print("Backward %f", value); ;
                LeftDriveSmart.spinToPosition(value * (-18), degrees, false);
                RightDriveSmart.spinToPosition(value * (-18), degrees, false);
            } else if (strcmp(inputBuffer, "TURN") == 0) {
                // Angle is in degrees from -180 to 180
                Brain.Screen.setCursor(5, 1);
                if (value > 0) {
                    Brain.Screen.print("Turn Right %f", value);
                    LeftDriveSmart.spinToPosition(value * 2.29, degrees, false);
                    RightDriveSmart.spinToPosition(value * (-2.29), degrees, false);
                } else {
                    Brain.Screen.print("Turn Left %f", value);
                    LeftDriveSmart.spinToPosition(value * (-2.29), degrees, false);
                    RightDriveSmart.spinToPosition(value * 2.29, degrees, false);
                }

            } else if (strcmp(inputBuffer, "GRABBAR_HEIGHT") == 0) {
                // Down: -200
                // Up: 100
                // Brain.Screen.print("Height %f", value);
                ArmMotor.spinToPosition(value, degrees, false);
            } else if (strcmp(inputBuffer, "GRABBER_WIDTH") == 0) {
                // Open: 0
                // Close: 30
                Brain.Screen.print("Width %f", value);
                ClawMotor.spinToPosition(value, degrees, false);
            }
             // Clear buffer for next read attempt by reading until newline (risky, might block more)
             // while(getchar() != '\n' && getchar() != EOF);

        } else {

            // printf("scanf result: %d. No valid string input received or stream error.\n", result_str);
        }
        // --- End Part 2 ---


        // --- Part 3: Other tasks (only run AFTER scanf completes/returns) ---
        // printf("...Finished input check for loop %d.\n", loopCounter);


        // --- Loop Delay ---
        // Wait a bit before the *next* loop iteration.
        // This delay happens *after* the scanf block resolves.
        wait(50, msec);
    }
}