/*----------------------------------------------------------------------------------------*/
/*                                                                                        */      
/*    Project:          IQ2 Clawbot Project                                               */
/*    Module:           main.cpp                                                          */
/*    Author:           VEX                                                               */
/*    Created:          Fri Aug 05 2022                                                   */
/*    Description:      This is an IQ2 python Clawbot project                             */
/*                                                                                        */      
/*    Configuration:    Clawbot Template (Individual Motors + Controller)                 */
/*                      Controller                                                        */
/*                      Left Motor in Port 1                                              */
/*                      Right Motor in Port 6                                             */
/*                      Claw Motor in Port 4                                              */
/*                      Arm Motor in Port 10                                              */
/*                                                                                        */      
/*----------------------------------------------------------------------------------------*/

// Include the IQ Library
// #include <iostream>
// #include <string>
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

// proximity sensors
sonar Sonar = sonar(PORT1);

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

// // define a task that will handle monitoring inputs from Controller
// int rc_auto_loop_function_Controller() {
//   // process the controller input every 20 milliseconds
//   // update the motors based on the input values
//   while(true) {

//     const int MAX_SPEED = 50;

//     // buttons
//     // Three values, max, 0 and -max.
//     //
//     int control_l1  = (Controller.ButtonLUp.pressing() - Controller.ButtonLDown.pressing()) * MAX_SPEED;
//     int control_r1  = (Controller.ButtonRUp.pressing() - Controller.ButtonRDown.pressing()) * MAX_SPEED;

//     // calculate the drivetrain motor velocities from the controller joystick axies
//     // left = AxisA
//     // right = AxisD
//     int drivetrainLeftSideSpeed = Controller.AxisA.position();
//     int drivetrainRightSideSpeed = Controller.AxisD.position();

//     // threshold the variable channels so the drive does not
//     // move if the joystick axis does not return exactly to 0
//     const int deadband = 15;
//     if(abs(drivetrainLeftSideSpeed) < deadband) {
//       drivetrainLeftSideSpeed = 0;
//     }

//     if(abs(drivetrainRightSideSpeed) < deadband) {
//       drivetrainRightSideSpeed = 0;
//     }

//     // update motor velocities
//     LeftDriveSmart.spin(forward, drivetrainLeftSideSpeed, percent);
//     RightDriveSmart.spin(forward, drivetrainRightSideSpeed, percent);

//     // Claw and Arm motors
//     ClawMotor.spin(forward, control_l1, percent);
//     ArmMotor.spin(forward, control_r1, percent);

//     // wait before repeating the process
//     wait(25, msec);
//   }
//   return 0;
// }

int main() {
    char inputBuffer[50]; // Buffer for received string
    int loopCounter = 0;

    float value = 0; // Variable to hold the float value from scanf

    Brain.Screen.setFont(monoM);
    Brain.Screen.print("Polling Example (scanf blocks)");
    Brain.Screen.newLine();
    printf("Polling Example (scanf still blocks)\n");
    printf("-------------------------------------\n");
    printf("Send 'forward' or 'stop' from Mac (with newline)\n");

    while (true) {
        // --- Part 1: Non-Blocking Robot Tasks ---
        // Code here runs on every loop iteration *before* checking serial.
        // Example: Read sensors, apply basic motor logic, update screen counter.
        loopCounter++;
        Brain.Screen.setCursor(3, 1); // Row 3, Col 1
        Brain.Screen.print("Loop Count: %d", loopCounter);
        Brain.Screen.clearLine(3); // Clear rest of the line

        // Example: Maybe check a bumper sensor
        // if (BumperA.pressing()) {
        //     // React to bumper press
        // }

        printf("Loop %d - About to check for input...\n", loopCounter);

        // --- End Part 1 ---


        // --- Part 2: Attempting to Read Serial Input (BLOCKING) ---
        // THIS IS WHERE THE PROGRAM WILL PAUSE AND WAIT FOR INPUT
        // It's NOT true polling. We are just calling the blocking function.
        // We can't easily check *if* data is available first.

        // *** The program WILL BLOCK HERE until you send a string + newline from Mac ***
        int result_str = scanf("%s", inputBuffer); // Read string and float from input


        // splintn on ":"

        char *command = strtok(inputBuffer, ":");
        char *param = strtok(NULL, ":");

        value = atof(param);

        if (result_str == 1) {
            // Input was received and successfully read as a string
            printf("--> Received string: '%s'\n", inputBuffer);
            Brain.Screen.setCursor(4, 1);
            Brain.Screen.print("Rx: %s", inputBuffer);
            Brain.Screen.clearLine(4);
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

            } else if (strcmp(inputBuffer, "GRABBER_WIDTH") == 0) {

            }
             // Clear buffer for next read attempt by reading until newline (risky, might block more)
             // while(getchar() != '\n' && getchar() != EOF);

        } else {
            printf("scanf result: %d. No valid string input received or stream error.\n", result_str);
        }
        // --- End Part 2 ---


        // --- Part 3: Other tasks (only run AFTER scanf completes/returns) ---
        printf("...Finished input check for loop %d.\n", loopCounter);


        // --- Loop Delay ---
        // Wait a bit before the *next* loop iteration.
        // This delay happens *after* the scanf block resolves.
        wait(50, msec);
    }
}