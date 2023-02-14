/*
  StepperMotor_h - Library to control a stepper motor.
  Created by ClayXrex, February 14, 2023.
  Released into the public domain.
*/

#include "Arduino.h"
#include "StepperMotor.h"


/*----------------------------------------------------------------------------------------------------------------------------
  Constructor

  The first three parameters represent the pins that are connected to the motor driver:
  -> enable_pin
  -> direction_pin
  -> pulse_pin

  Parameter steps_per_revolution and max_rpm should be self explanatory.

  Parameter hold_position determines whether or not the motor will be powered thus holding its position in between operations.
----------------------------------------------------------------------------------------------------------------------------*/
StepperMotor::StepperMotor(int enable_pin, int direction_pin, int pulse_pin, int steps_per_revolution, int max_rpm, bool hold_position)
{
  m_enable_pin = enable_pin;
  m_direction_pin = direction_pin;
  m_pulse_pin = pulse_pin;
  m_steps_per_revolution = steps_per_revolution;
  m_max_rpm = max_rpm;
  m_hold_position = hold_position;
  
  m_is_home_position_set = false; 

  StepperMotor::set_pin_modes();
}

void StepperMotor::set_pin_modes()
{
  pinMode(m_enable_pin, OUTPUT);
  pinMode(m_direction_pin, OUTPUT);
  pinMode(m_pulse_pin, OUTPUT);

  if (m_hold_position)
  {
    digitalWrite(m_enable_pin, LOW);
  }
  else
  {
    digitalWrite(m_enable_pin, HIGH);
  }
}

 void StepperMotor::update_position(bool turn_clockwise)
{
  // This method should be called after every single step.  
  if (!m_is_home_position_set) // Home position has not been set.
  {
    return;
  }

  if (turn_clockwise)
  {
    ++m_steps_from_home;
    if (m_steps_from_home == m_steps_per_revolution) // The stepper motor has reached its home position again.
    {
      m_steps_from_home = 0;
    }
  }
  else
  {
    if (m_steps_from_home == 0) // The stepper motor is at its home position.
    {
      m_steps_from_home = m_steps_per_revolution;
    }
    --m_steps_from_home;      
  }
}

long StepperMotor::calculate_delay_from_rpm(int rpm)
{
  // time_per_revolution is in milliseconds.
  //long can hold numbers up to 2,147,483,647 -> longest delay is ~2 million seconds.
  long time_per_revolution {60000 / rpm};

  long delay_in_milliseconds {time_per_revolution / (m_steps_per_revolution * 2)};

  return delay_in_milliseconds;
}

/*----------------------------------------------------------------------------------------------------------------------------
  Step
  
  steps -> Number of steps the motor should move.

  rpm -> Determines the speed at which the motor turns.
----------------------------------------------------------------------------------------------------------------------------*/
void StepperMotor::step(int steps, bool turn_clockwise, int rpm)
{
  if (rpm > m_max_rpm)
  {
    Serial.println("Error: Desired rpm is higher than max_rpm");
    return;
  }

  if (turn_clockwise)
  {
    digitalWrite(m_direction_pin, HIGH);
  }
  else {
    digitalWrite(m_direction_pin, LOW);
  }

  // Enable stepper motor.
  digitalWrite(m_enable_pin, LOW);

  long delay_in_milliseconds {calculate_delay_from_rpm(rpm)};

  for (int count{0}; count < steps; ++count)
  {
    digitalWrite(m_pulse_pin, HIGH);
    delay(delay_in_milliseconds);
    digitalWrite(m_pulse_pin, LOW);
    delay(delay_in_milliseconds);     
  }

  update_position(turn_clockwise);

  if (!m_hold_position)
  {
    // Disable stepper motor.
    digitalWrite(m_enable_pin, HIGH);
  }
}

/*----------------------------------------------------------------------------------------------------------------------------
  Do one rotation

  // The same result can be achieved by calling the step() method with the appropriate number of steps directly.
----------------------------------------------------------------------------------------------------------------------------*/
void StepperMotor::do_one_rotation(bool turn_clockwise, int rpm)
{
  step(m_steps_per_revolution, turn_clockwise, rpm);
}

/*----------------------------------------------------------------------------------------------------------------------------
  Rotate

  The same result can be achieved by calling the step() method with the appropriate number of steps directly.
----------------------------------------------------------------------------------------------------------------------------*/
void StepperMotor::rotate(int rotations, bool turn_clockwise, int rpm)
{
  step((rotations * m_steps_per_revolution), turn_clockwise, rpm);
}

/*----------------------------------------------------------------------------------------------------------------------------
  set current position as home

  Sets the current position as the motors home position. ¯\_☺_/¯
  Calling this function is required before the motor can be moved relative to its home position using go_to_position() 
----------------------------------------------------------------------------------------------------------------------------*/
void StepperMotor::set_current_position_as_home()
{
  m_steps_from_home = 0;  
  m_is_home_position_set = true;  
}

/*----------------------------------------------------------------------------------------------------------------------------
  Go to position

  Moves the motor relative to its home position.
  
  steps_relative to home -> Represents the position on the 360° range of motion that the motor will move to.

  The motors home position has to be set before with set_current_position_as_home() before this method can be used.
----------------------------------------------------------------------------------------------------------------------------*/
void StepperMotor::go_to_position(bool turn_clockwise, int rpm, int steps_relative_to_home)
{
  // The value of steps_relative_to_home has to be between 0 and self.steps_per_revolution.

  if (!m_is_home_position_set)
  {
  Serial.println("Cannot go to position if home position has not been set.");
  return;
  }   

  if (steps_relative_to_home == m_steps_from_home) // Motor is already at desired position.
  {
    return;
  }

  if (steps_relative_to_home < 0 || steps_relative_to_home > m_steps_per_revolution)
  {
    Serial.println("Value given for steps_relative_to_home exceeds self.steps_per_revolution.");
    return;
  }

  // Compare m_steps_from_home (current position) to steps_relative_to_home (position to move to).
  // Calculate number of steps needed to reach steps_relative_to_home respecting the desired direction of rotation.
  if (turn_clockwise)
  {
    if (steps_relative_to_home > m_steps_from_home)
    {
      step((steps_relative_to_home - m_steps_from_home), turn_clockwise, rpm);
    }
    else
    {
      step((m_steps_per_revolution - m_steps_from_home + steps_relative_to_home), turn_clockwise, rpm);
    }  
  }
  else{
    if (steps_relative_to_home > m_steps_from_home)
    {
      step((m_steps_from_home + (m_steps_per_revolution - steps_relative_to_home)), turn_clockwise, rpm);
    }
    else
    {
      step((m_steps_from_home - steps_relative_to_home), turn_clockwise, rpm);
    }      
  }
}