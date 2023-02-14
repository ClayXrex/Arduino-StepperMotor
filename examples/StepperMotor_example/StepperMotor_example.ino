#include <StepperMotor.h>

void setup() {
  StepperMotor stepper_motor{ 7, 6, 5, 200, 100, true };
  /*
    stepper_motor{int enable_pin, int direction_pin, int pulse_pin, int steps_per_revolution, int max_rpm, bool hold_position}

    Initialise an instance of StepperMotor.
    The first three parameters represent the pins that are connected to the motor driver:
    -> enable_pin
    -> direction_pin
    -> pulse_pin

    Parameter steps_per_revolution and max_rpm should be self explanatory.

    Parameter hold_position determines whether or not the motor will be powered thus holding its position in between operations.
  */

  stepper_motor.step(100, true, 50);
  /*
    step(int steps, bool turn_clockwise, int rpm)

    steps -> Number of steps the motor should move.

    rpm -> Determines the speed at which the motor turns.
  */

  delay(1000);

  stepper_motor.do_one_rotation(false, 100);
  /*
    do_one_rotation(bool turn_clockwise, int rpm)

    The same result can be achieved by calling the step() method with the appropriate number of steps directly.
  */

  delay(1000);

  stepper_motor.rotate(5, true, 100);
  /*
    rotate(int rotations, bool turn_clockwise, int rpm)

    rotations -> Number of full rotations the stepper motor should complete.

    The same result can be achieved by calling the step() method with the appropriate number of steps directly.
  */

  delay(1000);

  stepper_motor.set_current_position_as_home();
  /*
    Sets the current position as the motors home position. ¯\_☺_/¯
    Calling this function is required before the motor can be moved relative to its home position using go_to_position() 
  */

  delay(1000);

  stepper_motor.go_to_position(false, 100, 100);
  /*
    go_to_position(bool turn_clockwise, int rpm, int steps_relative_to_home)

    Moves the motor relative to its home position.
    
    steps_relative to home -> Represents the position on the 360° range of motion that the motor will move to.

    The motors home position has to be set before with set_current_position_as_home() before this method can be used.
  */
}

void loop() {}