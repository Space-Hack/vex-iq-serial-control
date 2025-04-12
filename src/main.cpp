/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       lukasdiebold                                              */
/*    Created:      4/13/2025, 1:10:50 AM                                     */
/*    Description:  IQ2 project                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the IQ2 brain screen
vex::brain       Brain;

// define your global instances of motors and other devices here


int main() {
	
    Brain.Screen.printAt( 2, 30, "Hello IQ New" );
   
    // while(1) {
        
    //     // Allow other tasks to run
    //     this_thread::sleep_for(10);
    // }
    int counter = 0;
    while (true) {
        // Print data to the serial console (which goes over USB)
        printf("Hello from VEX IQ! Count: %d\n", counter);
        counter++;

        Brain.Screen.printAt( 2, 60, "hi" );

        // Wait for a short period before sending the next message
        wait(1, seconds);
    }
}